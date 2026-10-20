#!/usr/bin/env python3
"""
add_wrapper_knn.py
------------------
Adequa os arquivos .c / .h gerados pelo knn_to_c.py ao padrao do projeto,
da mesma forma que o add_wrapper_m2cgen.py faz com os modelos do m2cgen.

O que o script faz (modifica o .c e REGENERA o .h):
    - Renomeia a funcao  {pfx}_predict(const float *input)  ->  {Prefixo}_Score
      (retorna o INDICE da classe prevista, igual a posicao em model.classes_)
    - Renomeia os macros  KNN_N_FEATURES -> {MACRO}_NUM_INPUTS,
                          KNN_N_CLASSES  -> {MACRO}_NUM_CLASSES,
                          KNN_N_SAMPLES  -> {MACRO}_NUM_SAMPLES,
                          KNN_K / KNN_*  -> {MACRO}_K / {MACRO}_*
    - Renomeia os simbolos internos (static) para s_{macro}_train_X etc. e
      knn_class_names -> {Prefixo}_ClassNames, evitando colisao caso o
      projeto tenha mais de um modelo KNN.
    - Descobre, pelos nomes das classes gravados no .c, qual indice e a
      classe "liga" (rotulo "1") e qual e a "nao liga" (rotulo "0") e cria
      {MACRO}_CLASSE_LIGA / {MACRO}_CLASSE_NAO_LIGA.
    - Adiciona  int {Prefixo}_DecideLigarGPRS(float bateria_pct,
                                              float taxa_normalizada)
    - Regenera o .h com o cabecalho padrao do projeto, defines, prototipos,
      guard {MACRO}_H e extern "C".
    - Se existir <pfx>_test.c ao lado, atualiza a chamada para o novo nome.

O nome do arquivo define os prefixos (igual aos outros scripts):
    knn_model.c -> KnnModel_Score / KNN_MODEL_NUM_INPUTS

Uso:
    python add_wrapper_knn.py Core/Src/knn_model.c
    python add_wrapper_knn.py knn_model.c --rotulo-liga 1 --rotulo-nao-liga 0
    python add_wrapper_knn.py knn_model.c --sem-backup

Pode ser executado de novo sobre um arquivo ja processado (idempotente):
o wrapper e o header sao regenerados, sem duplicar nada.
"""

import argparse
import json
import re
import sys
from pathlib import Path

ROTULO_NAO_LIGA_PADRAO = "0"
ROTULO_LIGA_PADRAO = "1"

# nomes especiais dos macros (os demais so trocam o prefixo)
MAPA_MACROS = {
    "N_FEATURES": "NUM_INPUTS",
    "N_CLASSES": "NUM_CLASSES",
    "N_SAMPLES": "NUM_SAMPLES",
}


# ----------------------------------------------------------------------
# prefixos (mesma convencao dos outros scripts do projeto)
# ----------------------------------------------------------------------
def derive_prefixes(nome_arquivo):
    """knn_model.c -> ('KnnModel', 'KNN_MODEL')"""
    stem = Path(nome_arquivo).stem
    partes = [p for p in re.split(r"[_\-]+", stem) if p]
    pascal = "".join(p[:1].upper() + p[1:] for p in partes)
    macro = "_".join(p.upper() for p in partes)
    return pascal, macro


# ----------------------------------------------------------------------
# leitura de informacoes do modelo gerado pelo knn_to_c.py
# ----------------------------------------------------------------------
def ler_define(texto, alternativas):
    """Le '#define <QUALQUER>_<alternativa> <inteiro>' (aceita nomes antigos e novos)."""
    m = re.search(rf"#define\s+\w*_(?:{alternativas})\s+(\d+)", texto)
    return int(m.group(1)) if m else None


def ler_info_header(texto_h):
    info = {
        "samples": ler_define(texto_h, "N_SAMPLES|NUM_SAMPLES"),
        "features": ler_define(texto_h, "N_FEATURES|NUM_INPUTS"),
        "k": ler_define(texto_h, "K"),
        "classes": ler_define(texto_h, "N_CLASSES|NUM_CLASSES"),
        "weights_distance": ler_define(texto_h, "WEIGHTS_DISTANCE"),
        "p_norm": ler_define(texto_h, "P_NORM"),
        "has_pre": ler_define(texto_h, "HAS_PREPROCESS"),
    }
    faltando = [k for k, v in info.items() if v is None]
    if faltando:
        raise ValueError(
            "o .h ao lado do .c nao parece ter sido gerado pelo knn_to_c.py "
            f"(defines nao encontrados: {', '.join(faltando)})."
        )
    return info


def ler_nomes_classes(conteudo_c):
    m = re.search(
        r"(?:class_names|ClassNames)\s*\[[^\]]*\]\s*=\s*\{(.*?)\}\s*;",
        conteudo_c,
        re.DOTALL,
    )
    if not m:
        raise ValueError("nao encontrei o array de nomes de classes no .c.")
    return [json.loads(f'"{s}"') for s in re.findall(r'"((?:[^"\\]|\\.)*)"', m.group(1))]


# ----------------------------------------------------------------------
# edicao do .c
# ----------------------------------------------------------------------
def garantir_include(conteudo, linha_include, no_topo=False):
    if linha_include in conteudo:
        return conteudo, False
    if no_topo:
        return linha_include + "\n" + conteudo, True
    ultimo = None
    for m in re.finditer(r"^#include\s+.*$", conteudo, re.MULTILINE):
        ultimo = m
    if ultimo:
        pos = ultimo.end()
        return conteudo[:pos] + "\n" + linha_include + conteudo[pos:], True
    return linha_include + "\n" + conteudo, True


def localizar_funcao(conteudo, nome):
    m = re.search(rf"^[^\n;]*\b{re.escape(nome)}\s*\([^;{{}}]*\)\s*\n?\{{",
                  conteudo, re.MULTILINE)
    if not m:
        return None
    inicio = conteudo.rfind("\n", 0, m.start()) + 1
    profundidade = 0
    for i in range(conteudo.index("{", m.start()), len(conteudo)):
        if conteudo[i] == "{":
            profundidade += 1
        elif conteudo[i] == "}":
            profundidade -= 1
            if profundidade == 0:
                return inicio, i + 1
    return None


def inserir_ou_substituir_funcao(conteudo, nome, codigo):
    intervalo = localizar_funcao(conteudo, nome)
    if intervalo:
        inicio, fim = intervalo
        return conteudo[:inicio] + codigo.rstrip("\n") + conteudo[fim:], "substituida"
    return conteudo.rstrip("\n") + "\n\n" + codigo, "adicionada"


def renomear_simbolos(conteudo, pfx, pascal, macro):
    """Troca os nomes gerados pelo knn_to_c.py pelos nomes do projeto."""
    PFX = pfx.upper()
    ml = macro.lower()

    conteudo = re.sub(rf"\b{re.escape(pfx)}_predict\b", f"{pascal}_Score", conteudo)
    conteudo = re.sub(rf"\b{re.escape(pfx)}_class_names\b", f"{pascal}_ClassNames", conteudo)
    conteudo = re.sub(
        rf"\b{re.escape(pfx)}_(train_X|train_y|pre_a|pre_b)\b",
        lambda m: f"s_{ml}_{m.group(1)}",
        conteudo,
    )
    conteudo = re.sub(
        rf"\b{re.escape(PFX)}_([A-Z0-9_]+)\b",
        lambda m: f"{macro}_{MAPA_MACROS.get(m.group(1), m.group(1))}",
        conteudo,
    )
    # o aviso "nao edite" deixa de valer: o arquivo agora e adequado ao projeto
    conteudo = re.sub(
        r"/\* Gerado automaticamente por knn_to_c\.py[^*]*\*/",
        "/* Gerado por knn_to_c.py e adequado ao projeto por add_wrapper_knn.py.\n"
        "   Ao gerar o modelo de novo, rode os dois scripts em sequencia. */",
        conteudo,
        count=1,
    )
    return conteudo


# ----------------------------------------------------------------------
# geracao do .h
# ----------------------------------------------------------------------
def gerar_header(nome_h, pascal, macro, info, idx_nao_liga, idx_liga):
    pre = "sim (aplicado dentro de Score)" if info["has_pre"] else "nao"
    metrica = "euclidiana" if info["p_norm"] == 2 else "manhattan"
    pesos = "1/distancia" if info["weights_distance"] else "uniformes"
    return f"""/**
  ******************************************************************************
  * @file           : {nome_h}
  * @brief          : Interface do modelo de Machine Learning - KNN
  *                    (K-Nearest Neighbors) - ajustado via Controlador Fuzzy
  *                    para decidir ligar ou nao o GPRS da coleira de
  *                    monitoramento de animais silvestres, com base na carga
  *                    da bateria e na taxa de acerto no envio de dados via
  *                    GPRS/satelite.
  *
  * Portado via knn_to_c.py do modelo treinado em Python (sklearn) para C puro.
  * Os dados de treino ficam em memoria somente leitura (flash); nao ha
  * alocacao dinamica.
  *
  * Parametros do modelo: {info['samples']} amostras, K = {info['k']}, metrica {metrica},
  * pesos {pesos}, pre-processamento embutido: {pre}.
  ******************************************************************************
  */
#ifndef {macro}_H
#define {macro}_H

#ifdef __cplusplus
extern "C" {{
#endif

/* Numero de features de entrada do modelo */
#define {macro}_NUM_INPUTS  {info['features']}
/* Numero de amostras de treino armazenadas no modelo */
#define {macro}_NUM_SAMPLES {info['samples']}
/* Numero de vizinhos considerados na votacao */
#define {macro}_K           {info['k']}
/* Numero de classes de saida */
#define {macro}_NUM_CLASSES {info['classes']}

/* Configuracao do modelo (informativa) */
#define {macro}_WEIGHTS_DISTANCE {info['weights_distance']}
#define {macro}_P_NORM           {info['p_norm']}   /* 2 = euclidiana, 1 = manhattan */
#define {macro}_HAS_PREPROCESS   {info['has_pre']}

/* Indices das classes devolvidos por {pascal}_Score() */
#define {macro}_CLASSE_NAO_LIGA {idx_nao_liga}
#define {macro}_CLASSE_LIGA     {idx_liga}

/* Rotulos originais das classes, indexados pelo valor devolvido por Score */
extern const char *const {pascal}_ClassNames[{macro}_NUM_CLASSES];

/**
 * @brief  Executa o modelo (portado via knn_to_c.py do modelo treinado em
 *         Python/sklearn) e retorna o indice da classe prevista.
 * @param  input: vetor com {macro}_NUM_INPUTS valores de entrada BRUTOS
 *                (mesma ordem e unidade do treinamento); a normalizacao,
 *                quando existe, e feita dentro da funcao.
 * @retval indice da classe prevista (0 .. {macro}_NUM_CLASSES-1), igual a
 *         posicao em model.classes_ do sklearn
 */
int {pascal}_Score(const float *input);

/**
 * @brief  Wrapper de conveniencia: executa o modelo e retorna diretamente
 *         a decisao binaria.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario
 */
int {pascal}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}}
#endif

#endif /* {macro}_H */
"""


def gerar_wrapper_c(pascal, macro):
    return f"""int {pascal}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{{
    float input[{macro}_NUM_INPUTS];

    input[0] = bateria_pct;
    input[1] = taxa_normalizada;

    return ({pascal}_Score(input) == {macro}_CLASSE_LIGA) ? 1 : 0;
}}
"""


# ----------------------------------------------------------------------
# processamento
# ----------------------------------------------------------------------
def ler_texto(caminho):
    """Le como UTF-8; se falhar, tenta cp1252 (arquivos gerados no Windows)."""
    dados = Path(caminho).read_bytes()
    try:
        return dados.decode("utf-8")
    except UnicodeDecodeError:
        return dados.decode("cp1252")


def salvar_backup(caminho, texto_original):
    destino = Path(str(caminho) + ".bak")
    if destino.exists():
        print(f"  - backup '{destino.name}' ja existe; mantido (guarda o original)")
        return
    destino.write_text(texto_original, encoding="utf-8", newline="")
    print(f"  - backup em '{destino.name}'")


def processar(caminho_c, rotulo_liga, rotulo_nao_liga, backup=True):
    caminho_h = caminho_c.with_suffix(".h")
    if not caminho_h.is_file():
        print(f"[{caminho_c}] ERRO: '{caminho_h.name}' nao encontrado ao lado do .c.")
        return False

    original_c = ler_texto(caminho_c)
    original_h = ler_texto(caminho_h)
    conteudo = original_c.replace("\r\n", "\n")

    pascal, macro = derive_prefixes(caminho_c.name)

    # 1) identifica o estado do arquivo: recem-gerado ou ja processado
    m = re.search(r"\bint\s+(\w+)_predict\s*\(\s*const\s+float\s*\*\s*\w+\s*\)", conteudo)
    ja_processado = re.search(rf"\bint\s+{pascal}_Score\s*\(", conteudo) is not None
    if not m and not ja_processado:
        print(f"[{caminho_c}] ERRO: nao encontrei 'int <pfx>_predict(const float *)' "
              f"nem '{pascal}_Score'. O .c foi gerado pelo knn_to_c.py?")
        return False
    pfx = m.group(1) if m else None

    try:
        info = ler_info_header(original_h)
        nomes = ler_nomes_classes(conteudo)
    except ValueError as e:
        print(f"[{caminho_c}] ERRO: {e}")
        return False

    # 2) indices das classes liga / nao liga
    for rot, desc in ((rotulo_liga, "liga"), (rotulo_nao_liga, "nao liga")):
        if rot not in nomes:
            print(f"[{caminho_c}] ERRO: rotulo '{rot}' (classe '{desc}') nao existe entre as "
                  f"classes do modelo {nomes}. Use --rotulo-liga / --rotulo-nao-liga.")
            return False
    idx_liga = nomes.index(rotulo_liga)
    idx_nao_liga = nomes.index(rotulo_nao_liga)

    print(f"[{caminho_c}]")
    print(f"  - modelo KNN: {info['samples']} amostras x {info['features']} features, "
          f"k={info['k']}, {info['classes']} classes {nomes}")
    print(f"  - prefixos: funcao '{pascal}_*' / macro '{macro}_*'")
    print(f"  - classe liga = indice {idx_liga} (rotulo '{rotulo_liga}'), "
          f"nao liga = indice {idx_nao_liga} (rotulo '{rotulo_nao_liga}')")

    if info["features"] != 2:
        print(f"  AVISO: o modelo tem {info['features']} entradas; o wrapper preenche somente "
              f"input[0]=bateria_pct e input[1]=taxa_normalizada. Ajuste manualmente.")
    if info["classes"] != 2:
        print(f"  AVISO: o modelo tem {info['classes']} classes; o wrapper decide so pela "
              f"classe 'liga' (todas as demais contam como 'nao liga').")

    # 3) renomeia simbolos do .c
    if pfx:
        # tira o include do header antigo (sera recolocado no topo)
        conteudo = re.sub(rf'^#include\s+"{re.escape(caminho_h.name)}"\s*\n', "", conteudo,
                          flags=re.MULTILINE)
        conteudo = renomear_simbolos(conteudo, pfx, pascal, macro)
        print(f"  - '{pfx}_predict' renomeada para '{pascal}_Score'; macros e simbolos "
              f"internos renomeados")
    else:
        print("  - arquivo ja processado antes: renomeacao ignorada")

    # 4) includes
    conteudo, add = garantir_include(conteudo, f'#include "{caminho_h.name}"', no_topo=True)
    if add or pfx:
        print(f"  - '#include \"{caminho_h.name}\"' no topo")
    conteudo, add = garantir_include(conteudo, "#include <math.h>")
    if add:
        print("  - '#include <math.h>' adicionado")

    # 5) wrapper de decisao binaria
    nome_wrapper = f"{pascal}_DecideLigarGPRS"
    conteudo, acao = inserir_ou_substituir_funcao(conteudo, nome_wrapper,
                                                  gerar_wrapper_c(pascal, macro))
    print(f"  - '{nome_wrapper}' {acao}")

    # 6) header novo
    header = gerar_header(caminho_h.name, pascal, macro, info, idx_nao_liga, idx_liga)

    # 7) arquivo de teste gerado pelo knn_to_c.py (se existir)
    caminho_teste = caminho_c.with_name(f"{pfx or 'knn'}_test.c")
    texto_teste = None
    if pfx and caminho_teste.is_file():
        original_teste = ler_texto(caminho_teste)
        texto_teste = re.sub(rf"\b{re.escape(pfx)}_predict\b", f"{pascal}_Score",
                             original_teste.replace("\r\n", "\n"))
        if backup:
            salvar_backup(caminho_teste, original_teste)

    # 8) grava
    if backup:
        salvar_backup(caminho_c, original_c)
        salvar_backup(caminho_h, original_h)
    caminho_h.write_text(header, encoding="utf-8", newline="\n")
    print(f"  - '{caminho_h.name}' regenerado")
    caminho_c.write_text(conteudo.rstrip("\n") + "\n", encoding="utf-8", newline="\n")
    print(f"  - '{caminho_c.name}' modificado")
    if texto_teste is not None:
        caminho_teste.write_text(texto_teste, encoding="utf-8", newline="\n")
        print(f"  - '{caminho_teste.name}' atualizado para chamar {pascal}_Score")
    return True


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("arquivos", nargs="+", help="arquivo(s) .c gerado(s) pelo knn_to_c.py")
    ap.add_argument("--rotulo-liga", default=ROTULO_LIGA_PADRAO,
                    help=f"rotulo (em model.classes_) da classe 'liga GPRS' (padrao '{ROTULO_LIGA_PADRAO}')")
    ap.add_argument("--rotulo-nao-liga", default=ROTULO_NAO_LIGA_PADRAO,
                    help=f"rotulo da classe 'nao liga' (padrao '{ROTULO_NAO_LIGA_PADRAO}')")
    ap.add_argument("--sem-backup", action="store_true", help="nao gerar arquivos .bak")
    args = ap.parse_args()

    ok = True
    for nome in args.arquivos:
        caminho = Path(nome)
        if not caminho.is_file():
            print(f"[{caminho}] ERRO: arquivo nao encontrado.")
            ok = False
            continue
        if not processar(caminho, args.rotulo_liga, args.rotulo_nao_liga,
                         backup=not args.sem_backup):
            ok = False
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()