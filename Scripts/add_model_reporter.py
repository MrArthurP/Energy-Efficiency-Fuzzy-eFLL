#!/usr/bin/env python3
"""
Adicionar_modelo_reporter.py
-----------------------------
Registra um modelo ja processado pelo Adicionar_wrapper_m2cgen.py dentro do
ControllerReporter, editando os DOIS arquivos, que ficam em pastas
diferentes do projeto STM32:

    <Core>/Inc/controller_reporter.h   -> constante no enum ControllerType
    <Core>/Src/controller_reporter.c   -> #include, case no InitController
                                          e case no ReportCreation

O script le o .h do modelo (gerado pelo wrapper) para descobrir:
    - prefixo de funcao   (ex.: LinModel)    -> do prototipo _DecideLigarGPRS
    - prefixo de macro    (ex.: LIN_MODEL)   -> dos #define do proprio header
    - familia do modelo:
        * valor unico  -> tem _THRESHOLD e _Probabilidade
                          (regressao logistica, linear, SVM)
        * vetor saida  -> tem _NUM_OUTPUTS e _CLASSE_*
                          (arvore de decisao, random forest)
    - tipo do modelo -> deduzido do NOME DO ARQUIVO .h (ou via --tipo, so
      como anotacao nos comentarios gerados)
    - enum/rotulo do log -> deduzidos do nome do arquivo .h, removendo um
      sufixo final "_model"/"_modelo" quando presente:
          lgb_model.h -> CTRL_TYPE_LGB   (rotulo "Lgb")
          dec_model.h -> CTRL_TYPE_DEC   (rotulo "Dec")

Uso:
    # caminho do .h do modelo; o Core e deduzido (../ da pasta Inc)
    python Adicionar_modelo_reporter.py Core/Inc/lin_model.h

    # varios modelos de uma vez
    python Adicionar_modelo_reporter.py Core/Inc/*.h

    # informando o Core explicitamente (ou os arquivos um a um)
    python Adicionar_modelo_reporter.py lin_model.h --core Stm32-Nucleo/Ai-Models/Core
    python Adicionar_modelo_reporter.py lin_model.h \
        --reporter-h Core/Inc/controller_reporter.h \
        --reporter-c Core/Src/controller_reporter.c

    # sobrescrever nome do enum / rotulo do log
    python Adicionar_modelo_reporter.py Core/Inc/lin_model.h \
        --enum CTRL_TYPE_LINEAR --nome LinearRegression

    # so mostrar o que seria feito
    python Adicionar_modelo_reporter.py Core/Inc/lin_model.h --dry-run

Por padrao o script tambem corrige macros inconsistentes ja presentes no
controller_reporter.c (ex.: REGMODEL_NUM_INPUTS -> REG_MODEL_NUM_INPUTS),
comparando com as macros realmente definidas nos headers incluidos.
Use --sem-correcao para desativar.
"""

import argparse
import re
import sys
from pathlib import Path

NOME_REPORTER_H = "controller_reporter.h"
NOME_REPORTER_C = "controller_reporter.c"

SUFIXOS_MACRO = ("NUM_INPUTS", "NUM_OUTPUTS", "CLASSE_LIGA",
                 "CLASSE_NAO_LIGA", "THRESHOLD")

# sufixo comum que e removido do nome do arquivo ao formar o enum/rotulo
# (ex.: "lgb_model.h" -> "lgb" -> CTRL_TYPE_LGB, rotulo "Lgb")
SUFIXOS_ARQUIVO_IGNORADOS = ("model", "modelo")


def derivar_enum_e_rotulo(nome_arquivo_h):
    """
    Deduz o nome do enum e o rotulo do log a partir do NOME DO ARQUIVO .h,
    removendo um sufixo final "_model"/"_modelo" quando presente:

        lgb_model.h  -> enum CTRL_TYPE_LGB       rotulo "Lgb"
        dec_model.h  -> enum CTRL_TYPE_DEC       rotulo "Dec"
        reg_model.h  -> enum CTRL_TYPE_REG       rotulo "Reg"
        svm_model.h  -> enum CTRL_TYPE_SVM       rotulo "Svm"
        outro.h      -> enum CTRL_TYPE_OUTRO     rotulo "Outro"
    """
    stem = Path(nome_arquivo_h).stem
    partes = [p for p in re.split(r"[_\-]+", stem) if p]
    if len(partes) > 1 and partes[-1].lower() in SUFIXOS_ARQUIVO_IGNORADOS:
        partes = partes[:-1]

    enum = "CTRL_TYPE_" + "_".join(p.upper() for p in partes)
    rotulo = "".join(p.capitalize() for p in partes)
    return enum, rotulo


# ----------------------------------------------------------------------
# leitura / escrita
# ----------------------------------------------------------------------
def ler(caminho):
    return Path(caminho).read_text(encoding="utf-8").replace("\r\n", "\n")


def escrever(caminho, texto):
    with open(caminho, "w", encoding="utf-8", newline="\n") as f:
        f.write(texto)


# ----------------------------------------------------------------------
# descoberta dos arquivos do reporter
# ----------------------------------------------------------------------
def localizar_reporter(modelo_h, core=None, rep_h=None, rep_c=None):
    """
    Descobre <Core>/Inc/controller_reporter.h e <Core>/Src/controller_reporter.c.
    Prioridade: caminhos explicitos > --core > deduzido do caminho do modelo.
    """
    if rep_h and rep_c:
        return Path(rep_h), Path(rep_c)

    candidatos = []
    if core:
        candidatos.append(Path(core))
    else:
        p = Path(modelo_h).resolve()
        # .../Core/Inc/lin_model.h -> .../Core ; tambem tenta a propria pasta
        candidatos.append(p.parent.parent)
        candidatos.append(p.parent)
        candidatos.append(Path.cwd())
        candidatos.append(Path.cwd() / "Core")

    for base in candidatos:
        h = base / "Inc" / NOME_REPORTER_H
        c = base / "Src" / NOME_REPORTER_C
        if h.is_file() and c.is_file():
            return h, c
        # projeto sem separacao Inc/Src
        h2, c2 = base / NOME_REPORTER_H, base / NOME_REPORTER_C
        if h2.is_file() and c2.is_file():
            return h2, c2

    return None, None


# ----------------------------------------------------------------------
# leitura das informacoes do modelo (a partir do .h gerado pelo wrapper)
# ----------------------------------------------------------------------
def ler_info_modelo(caminho_h, tipo_forcado=None):
    texto = ler(caminho_h)
    info = {"header": Path(caminho_h).name}

    m = re.search(r"\bint\s+(\w+)_DecideLigarGPRS\s*\(", texto)
    if not m:
        raise ValueError(
            f"'{caminho_h}' nao parece ter passado pelo wrapper "
            f"(prototipo '_DecideLigarGPRS' nao encontrado)."
        )
    info["prefixo"] = m.group(1)

    m = re.search(r"#define\s+([A-Z][A-Z0-9_]*)_NUM_INPUTS\s+(\d+)", texto)
    if not m:
        raise ValueError(f"'{caminho_h}' nao define <PREFIXO>_NUM_INPUTS.")
    info["macro"] = m.group(1)
    info["num_inputs"] = int(m.group(2))

    tem_threshold = re.search(rf"#define\s+{info['macro']}_THRESHOLD\b", texto)
    tem_outputs = re.search(rf"#define\s+{info['macro']}_NUM_OUTPUTS\b", texto)
    if tem_threshold and re.search(rf"\b{info['prefixo']}_Probabilidade\s*\(", texto):
        info["familia"] = "valor_unico"
    elif tem_outputs:
        info["familia"] = "vetor_saida"
    else:
        raise ValueError(
            f"'{caminho_h}': nao consegui identificar a familia do modelo "
            f"(sem _THRESHOLD/_Probabilidade nem _NUM_OUTPUTS)."
        )

    # tipo: usado so como anotacao/comentario; se nao informado, um rotulo
    # generico e usado (a familia real - valor_unico/vetor_saida - ja foi
    # determinada acima e e o que importa para o corpo do case).
    info["tipo"] = tipo_forcado or (
        "arvore" if info["familia"] == "vetor_saida" else "valor_unico")

    # enum e rotulo do log: deduzidos do NOME DO ARQUIVO .h do modelo
    info["enum"], info["rotulo"] = derivar_enum_e_rotulo(info["header"])
    return info


# ----------------------------------------------------------------------
# edicao do controller_reporter.h (enum)
# ----------------------------------------------------------------------
def adicionar_no_enum(texto_h, constante):
    if re.search(rf"\b{constante}\b", texto_h):
        return texto_h, False

    m = re.search(r"^([ \t]*)CTRL_TYPE_OTHER\b.*$", texto_h, re.MULTILINE)
    if m:
        indent = m.group(1)
        return (texto_h[:m.start()] + f"{indent}{constante},\n"
                + texto_h[m.start():]), True

    # sem CTRL_TYPE_OTHER: insere antes do fechamento do enum
    m = re.search(r"\}\s*ControllerType\s*;", texto_h)
    if m:
        return texto_h[:m.start()] + f"    {constante},\n" + texto_h[m.start():], True

    raise ValueError("nao encontrei o enum ControllerType no header do reporter.")


# ----------------------------------------------------------------------
# edicao do controller_reporter.c
# ----------------------------------------------------------------------
def adicionar_include(texto_c, nome_h):
    linha = f'#include "{nome_h}"'
    if linha in texto_c:
        return texto_c, False
    ultimo = None
    for m in re.finditer(r'^#include\s+".*?"[ \t]*$', texto_c, re.MULTILINE):
        ultimo = m
    if not ultimo:
        return linha + "\n" + texto_c, True
    pos = ultimo.end()
    return texto_c[:pos] + "\n" + linha + texto_c[pos:], True


def corpo_case(info):
    """Case do switch do InitController, conforme a familia do modelo."""
    enum, pref, macro = info["enum"], info["prefixo"], info["macro"]
    init = ", ".join(["50.0"] * info["num_inputs"])

    if info["familia"] == "valor_unico":
        return f"""        case {enum}:
        {{
            /* {info['header'][:-2]} e stateless: nao ha o que alocar/montar como no
             * fuzzy. "Inicializar" aqui significa validar que o modelo responde
             * corretamente a uma entrada de referencia antes de liberar o uso. */
            double input[{macro}_NUM_INPUTS] = {{{init}}};
            double prob = {pref}_Probabilidade(input);

            ret = (prob >= 0.0 && prob <= 1.0) ? 0 : -1;
            ControllerReporter_ReportCreation({enum}, (ret == 0));
            break;
        }}
"""

    return f"""        case {enum}:
        {{
            /* {info['header'][:-2]} tambem e stateless: validamos que as
             * probabilidades de classe somam ~1.0 para uma entrada de
             * referencia. */
            double input[{macro}_NUM_INPUTS] = {{{init}}};
            double output[{macro}_NUM_OUTPUTS];
            double soma;

            {pref}_Score(input, output);
            soma = output[{macro}_CLASSE_NAO_LIGA] + output[{macro}_CLASSE_LIGA];

            ret = (soma > 0.99 && soma < 1.01) ? 0 : -1;
            ControllerReporter_ReportCreation({enum}, (ret == 0));
            break;
        }}
"""


def _intervalo_case(texto, enum, ini_func, fim_func):
    """
    Se 'case <enum>:' existir dentro de [ini_func, fim_func), devolve
    (inicio, fim) cobrindo o case inteiro (ate o proximo 'case'/'default'
    no mesmo nivel).
    """
    janela = texto[ini_func:fim_func]
    m = re.search(rf"^[ \t]*case\s+{enum}\s*:", janela, re.MULTILINE)
    if not m:
        return None
    prox = re.search(r"^[ \t]*(case\s+\w+\s*:|default\s*:)",
                     janela[m.end():], re.MULTILINE)
    fim = ini_func + m.end() + prox.start() if prox else fim_func
    return ini_func + m.start(), fim


def _fim_da_funcao(texto, nome_funcao):
    m = re.search(rf"\b{nome_funcao}\s*\([^;{{}}]*\)\s*\n?\{{", texto)
    if not m:
        raise ValueError(f"funcao '{nome_funcao}' nao encontrada no reporter.c")
    prof = 0
    for i in range(texto.index("{", m.start()), len(texto)):
        if texto[i] == "{":
            prof += 1
        elif texto[i] == "}":
            prof -= 1
            if prof == 0:
                return m.start(), i + 1
    raise ValueError(f"nao consegui delimitar a funcao '{nome_funcao}'.")


def adicionar_case_init(texto_c, info):
    ini_f, fim_f = _fim_da_funcao(texto_c, "ControllerReporter_InitController")
    codigo = corpo_case(info)

    existente = _intervalo_case(texto_c, info["enum"], ini_f, fim_f)
    if existente:
        a, b = existente
        return texto_c[:a] + codigo + texto_c[b:], "substituido"

    m = re.search(r"^[ \t]*default\s*:", texto_c[ini_f:fim_f], re.MULTILINE)
    if not m:
        raise ValueError("nao encontrei o 'default:' do switch do InitController.")
    pos = ini_f + m.start()
    return texto_c[:pos] + codigo + texto_c[pos:], "adicionado"


def adicionar_case_report(texto_c, info):
    ini_f, fim_f = _fim_da_funcao(texto_c, "ControllerReporter_ReportCreation")
    linha = f'        case {info["enum"]}: name = "{info["rotulo"]}"; break;\n'

    existente = _intervalo_case(texto_c, info["enum"], ini_f, fim_f)
    if existente:
        a, b = existente
        return texto_c[:a] + linha + texto_c[b:], "substituido"

    # aproveita uma linha comentada (ex.: '// case CTRL_TYPE_LOGISTIC: ...')
    comentado = re.search(
        rf'^[ \t]*(?://|/\*)\s*case\s+{info["enum"]}\s*:.*$\n?',
        texto_c[ini_f:fim_f], re.MULTILINE)
    if comentado:
        a = ini_f + comentado.start()
        b = ini_f + comentado.end()
        return texto_c[:a] + linha + texto_c[b:], "descomentado"

    m = re.search(r"^[ \t]*default\s*:\s*name\s*=", texto_c[ini_f:fim_f], re.MULTILINE)
    if not m:
        raise ValueError("nao encontrei o 'default:' do switch do ReportCreation.")
    pos = ini_f + m.start()
    return texto_c[:pos] + linha + texto_c[pos:], "adicionado"


# ----------------------------------------------------------------------
# correcao de macros inconsistentes
# ----------------------------------------------------------------------
def macros_dos_headers_incluidos(texto_c, pasta_inc):
    """Coleta as macros <PREFIXO>_<SUFIXO> realmente definidas nos headers."""
    definidas = set()
    for m in re.finditer(r'#include\s+"(\w+\.h)"', texto_c):
        caminho = Path(pasta_inc) / m.group(1)
        if caminho.is_file():
            for d in re.finditer(r"#define\s+([A-Z][A-Z0-9_]*)", ler(caminho)):
                definidas.add(d.group(1))
    return definidas


def corrigir_macros(texto_c, definidas):
    """
    Troca macros escritas com underscores errados (REGMODEL_NUM_INPUTS)
    pela versao realmente definida (REG_MODEL_NUM_INPUTS), comparando os
    nomes sem underscore.
    """
    por_forma = {}
    for nome in definidas:
        por_forma.setdefault(nome.replace("_", ""), nome)

    trocas = {}
    padrao = re.compile(r"\b([A-Z][A-Z0-9_]*_(?:" + "|".join(SUFIXOS_MACRO) + r"))\b")
    for m in padrao.finditer(texto_c):
        usada = m.group(1)
        if usada in definidas:
            continue
        correta = por_forma.get(usada.replace("_", ""))
        if correta and correta != usada:
            trocas[usada] = correta

    for errada, certa in trocas.items():
        texto_c = re.sub(rf"\b{errada}\b", certa, texto_c)
    return texto_c, trocas


# ----------------------------------------------------------------------
# processamento
# ----------------------------------------------------------------------
def processar(modelo_h, args):
    info = ler_info_modelo(modelo_h, args.tipo)
    if args.enum:
        info["enum"] = args.enum
    if args.nome:
        info["rotulo"] = args.nome

    rep_h, rep_c = localizar_reporter(modelo_h, args.core, args.reporter_h, args.reporter_c)
    if not rep_h or not rep_c:
        print(f"[{modelo_h}] ERRO: nao encontrei {NOME_REPORTER_H} (Inc/) e "
              f"{NOME_REPORTER_C} (Src/). Use --core ou --reporter-h/--reporter-c.")
        return False

    print(f"[{Path(modelo_h).name}]")
    print(f"  - modelo: {info['prefixo']}_* / {info['macro']}_* "
          f"({info['tipo']}, {info['familia'].replace('_', ' ')}, "
          f"{info['num_inputs']} entradas)")
    print(f"  - enum: {info['enum']}   rotulo do log: \"{info['rotulo']}\"")
    print(f"  - reporter .h: {rep_h}")
    print(f"  - reporter .c: {rep_c}")

    texto_h = ler(rep_h)
    texto_c = ler(rep_c)

    texto_h, add = adicionar_no_enum(texto_h, info["enum"])
    print(f"  - enum {info['enum']}: {'adicionado' if add else 'ja existia'}")

    texto_c, add = adicionar_include(texto_c, info["header"])
    print(f"  - #include \"{info['header']}\": {'adicionado' if add else 'ja existia'}")

    texto_c, acao = adicionar_case_init(texto_c, info)
    print(f"  - case no InitController: {acao}")

    texto_c, acao = adicionar_case_report(texto_c, info)
    print(f"  - case no ReportCreation: {acao}")

    if not args.sem_correcao:
        definidas = macros_dos_headers_incluidos(texto_c, rep_h.parent)
        texto_c, trocas = corrigir_macros(texto_c, definidas)
        for errada, certa in trocas.items():
            print(f"  - macro corrigida: {errada} -> {certa}")

    if args.dry_run:
        print("  - (dry-run) nenhum arquivo gravado")
        return True

    escrever(rep_h, texto_h)
    escrever(rep_c, texto_c)
    print("  - arquivos gravados")
    return True


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("modelos", nargs="+",
                    help="arquivo(s) .h de modelo ja processado(s) pelo wrapper")
    ap.add_argument("--core", help="pasta Core do projeto (que contem Inc/ e Src/)")
    ap.add_argument("--reporter-h", help="caminho explicito do controller_reporter.h")
    ap.add_argument("--reporter-c", help="caminho explicito do controller_reporter.c")
    ap.add_argument("--tipo", default=None,
                    help="anotacao livre do tipo do modelo, usada so nos "
                         "comentarios gerados (nao afeta o enum, que vem do "
                         "nome do arquivo)")
    ap.add_argument("--enum", help="forca o nome da constante do enum")
    ap.add_argument("--nome", help="forca o rotulo usado no log")
    ap.add_argument("--sem-correcao", action="store_true",
                    help="nao corrigir macros inconsistentes no reporter.c")
    ap.add_argument("--dry-run", action="store_true", help="apenas mostra o que faria")
    args = ap.parse_args()

    ok = True
    for nome in args.modelos:
        caminho = Path(nome)
        if not caminho.is_file():
            print(f"[{caminho}] ERRO: arquivo nao encontrado.")
            ok = False
            continue
        if caminho.name == NOME_REPORTER_H:
            continue
        try:
            if not processar(caminho, args):
                ok = False
        except ValueError as e:
            print(f"[{caminho}] ERRO: {e}")
            ok = False

    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()