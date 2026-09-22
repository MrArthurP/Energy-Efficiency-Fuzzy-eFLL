#!/usr/bin/env python3
"""
Adicionar_wrapper_xcubeai.py
------------------------------
Gera um wrapper padronizado ({Prefixo}_Init / {Prefixo}_DecideLigarGPRS)
para modelos gerados pelo X-CUBE-AI da ST (bem mais "pesados" que os do
m2cgen: usam handle de rede, buffers de ativacao, ai_error etc., em vez de
uma unica funcao "double Score(input)").

DIFERENCA para o Adicionar_wrapper_m2cgen.py: aquele script MODIFICA o .c
do modelo (renomeia funcao, injeta wrapper dentro dele). Este script NUNCA
toca em <nome>.h / <nome>.c / <nome>_data.* - esses arquivos sao gerados
automaticamente pelo X-CUBE-AI e sao sobrescritos a cada geracao; qualquer
edicao manual neles seria perdida. Em vez disso, cria dois arquivos NOVOS:

    <nome>_wrapper.h   -> int {Prefixo}_Init(void);
                          int {Prefixo}_DecideLigarGPRS(float bateria_pct,
                                                         float taxa_normalizada);
    <nome>_wrapper.c   -> implementacao, usando a API publica gerada pelo
                          X-CUBE-AI (ai_<nome>_create_and_init, _inputs_get,
                          _outputs_get, _run) para esconder toda a
                          complexidade de handle/buffers do resto do
                          projeto.

O que o script LE do <nome>.h (nao modifica, so leitura):
    - AI_<MACRO>_IN_NUM, AI_<MACRO>_OUT_NUM
    - AI_<MACRO>_IN_1_SIZE            (validado - o projeto usa 2 entradas)
    - AI_<MACRO>_OUT_{i}_FORMAT / _SIZE de cada saida, para identificar
      qual delas e o rotulo da classe (formato S32, tamanho 1) - essa e a
      que o wrapper usa como decisao, pois a propria rede X-CUBE-AI ja
      calcula o argmax internamente.
    - o prefixo real das funcoes publicas (ex.: "ai_nba_model_create"),
      lido diretamente das assinaturas do header - normalmente bate com o
      nome do arquivo, mas o script confirma em vez de presumir.

O nome do arquivo <nome>.h (ex.: "nba_model.h") tambem define os prefixos
de funcao/macro do WRAPPER (NbaModel_Init, NBA_MODEL_...), no mesmo padrao
usado pelo Adicionar_wrapper_m2cgen.py e pelo Adicionar_modelo_reporter.py.

Uso:
    python Adicionar_wrapper_xcubeai.py Core/Inc/nba_model.h
    python Adicionar_wrapper_xcubeai.py Core/Inc/nba_model.h --out-dir Core
    python Adicionar_wrapper_xcubeai.py Core/Inc/nba_model.h --inc-dir Core/Inc --src-dir Core/Src
"""

import argparse
import re
import sys
from pathlib import Path


# ----------------------------------------------------------------------
# prefixos (mesma convencao dos outros scripts do projeto)
# ----------------------------------------------------------------------
def derive_prefixes(nome_arquivo):
    """
    nba_model.h -> ('NbaModel', 'NBA_MODEL')
    """
    stem = Path(nome_arquivo).stem
    stem = re.sub(r"_data(_params)?$", "", stem)  # seguranca, caso apontem para o arquivo errado
    partes = [p for p in re.split(r"[_\-]+", stem) if p]
    pascal = "".join(p[:1].upper() + p[1:] for p in partes)
    macro = "_".join(p.upper() for p in partes)
    return pascal, macro


# ----------------------------------------------------------------------
# leitura do <nome>.h gerado pelo X-CUBE-AI
# ----------------------------------------------------------------------
def ler(caminho):
    return Path(caminho).read_text(encoding="utf-8").replace("\r\n", "\n")


def extrair_macro_int(texto, nome):
    m = re.search(rf"#define\s+{re.escape(nome)}\s+\((\d+)\)", texto)
    return int(m.group(1)) if m else None


def extrair_macro_txt(texto, nome):
    m = re.search(rf"#define\s+{re.escape(nome)}\s+\((\w+)\)", texto)
    return m.group(1) if m else None


def extrair_api_real(texto):
    """
    Le o prefixo real das funcoes publicas a partir de uma assinatura
    conhecida (ex.: 'ai_error ai_nba_model_create(' -> 'nba_model').
    """
    m = re.search(r"\bai_error\s+ai_(\w+)_create\s*\(", texto)
    if m:
        return m.group(1)
    m = re.search(r"\bai_i32\s+ai_(\w+)_run\s*\(", texto)
    if m:
        return m.group(1)
    raise ValueError("nao encontrei 'ai_<nome>_create(' nem 'ai_<nome>_run(' no header.")


def ler_info_rede(caminho_h):
    texto = ler(caminho_h)
    api = extrair_api_real(texto)

    macro_h = f"AI_{api.upper()}"  # prefixo das macros AI_NBA_MODEL_*

    in_num = extrair_macro_int(texto, f"{macro_h}_IN_NUM") or 1
    out_num = extrair_macro_int(texto, f"{macro_h}_OUT_NUM") or 1
    in1_size = extrair_macro_int(texto, f"{macro_h}_IN_1_SIZE")

    saidas = []
    for i in range(1, out_num + 1):
        fmt = extrair_macro_txt(texto, f"{macro_h}_OUT_{i}_FORMAT")
        tam = extrair_macro_int(texto, f"{macro_h}_OUT_{i}_SIZE")
        saidas.append({"indice": i, "formato": fmt, "tamanho": tam})

    # a saida "rotulo da classe" e a de formato S32 e tamanho 1; se nao
    # achar nenhuma assim, usa a primeira saida como decisao mesmo assim
    idx_rotulo = next(
        (s["indice"] for s in saidas if s["formato"] == "AI_BUFFER_FORMAT_S32" and s["tamanho"] == 1),
        saidas[0]["indice"] if saidas else 1,
    )

    return {
        "api": api,                # ex.: "nba_model"  (usado em ai_<api>_run etc.)
        "in_num": in_num,
        "out_num": out_num,
        "in1_size": in1_size,
        "saidas": saidas,
        "idx_rotulo": idx_rotulo,
    }


# ----------------------------------------------------------------------
# geracao do <nome>_wrapper.h
# ----------------------------------------------------------------------
def gerar_wrapper_h(nome_h_original, pascal, macro, guard):
    return f"""/**
  ******************************************************************************
  * @file           : {macro.lower()}_wrapper.h
  * @brief          : Wrapper de conveniencia para o modelo "{pascal}", gerado
  *                    pelo X-CUBE-AI a partir de {nome_h_original}. Esconde a
  *                    API nativa (ai_handle, ai_buffer, ai_error) atras da
  *                    mesma interface usada pelos demais modelos do projeto.
  *
  * ATENCAO: este arquivo NAO e gerado pelo X-CUBE-AI (ao contrario de
  * {nome_h_original}) - pode ser editado manualmente sem risco de ser
  * sobrescrito numa nova geracao do modelo.
  ******************************************************************************
  */
#ifndef {guard}
#define {guard}

#ifdef __cplusplus
extern "C" {{
#endif

/**
 * @brief  Aloca os buffers de ativacao e inicializa a rede "{pascal}".
 *         Deve ser chamada uma unica vez antes do primeiro uso de
 *         {pascal}_DecideLigarGPRS. Diferente dos modelos do m2cgen
 *         (stateless), este modelo e stateful: mantem um handle de rede
 *         e buffers estaticos entre chamadas.
 * @retval 0 em sucesso, -1 em falha
 */
int {pascal}_Init(void);

/**
 * @brief  Executa a rede "{pascal}" sobre (bateria_pct, taxa_normalizada)
 *         e retorna a decisao binaria ja calculada pelo X-CUBE-AI
 *         (a rede devolve a classe prevista diretamente).
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario, -1 em erro de
 *         execucao (rede nao inicializada ou falha no ai_run)
 */
int {pascal}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}}
#endif

#endif /* {guard} */
"""


# ----------------------------------------------------------------------
# geracao do <nome>_wrapper.c
# ----------------------------------------------------------------------
def gerar_wrapper_c(nome_h_original, nome_wrapper_h, pascal, macro, info):
    api = info["api"]
    idx_rotulo = info["idx_rotulo"]

    # nomes dos buffers de saida no formato ai_<api>_out_data (um por saida)
    linhas_saida = []
    for s in info["saidas"]:
        c_type = "int32_t" if s["formato"] == "AI_BUFFER_FORMAT_S32" else "float"
        tam = s["tamanho"] or 1
        linhas_saida.append(f"static {c_type} s_{macro.lower()}_out{s['indice']}[{tam}];")
    buffers_saida = "\n".join(linhas_saida)

    sets_saida = "\n".join(
        f"    ai_output[{s['indice']-1}].data = AI_HANDLE_PTR(s_{macro.lower()}_out{s['indice']});"
        for s in info["saidas"]
    )

    expressao_decisao = (
        f"(int)s_{macro.lower()}_out{idx_rotulo}[0]"
    )

    return f"""/**
  ******************************************************************************
  * @file           : {macro.lower()}_wrapper.c
  * @brief          : Implementacao do wrapper de "{pascal}" (ver
  *                    {nome_wrapper_h}). Usa a API publica gerada pelo
  *                    X-CUBE-AI para {nome_h_original}.
  *
  * ATENCAO: este arquivo NAO e gerado pelo X-CUBE-AI - pode ser editado
  * manualmente sem risco de ser sobrescrito numa nova geracao do modelo.
  ******************************************************************************
  */
#include "{nome_wrapper_h}"
#include "{nome_h_original}"
#include "{api}_data.h"

#include <stddef.h>
#include <stdint.h>

static ai_handle s_{macro.lower()}_network = AI_HANDLE_NULL;
static ai_error  s_{macro.lower()}_ultimo_erro;

/* Buffer de ativacao da rede - tamanho definido em {api}_data.h.
 * Os pesos NAO precisam de buffer proprio aqui: o endereco deles ja vem
 * embutido no descritor gerado em {api}_data.c. */
AI_ALIGNED(AI_{macro}_ACTIVATIONS_ALIGNMENT)
static ai_u8 s_{macro.lower()}_activations[AI_{macro}_DATA_ACTIVATIONS_SIZE];

/* Buffers de entrada/saida reutilizados a cada chamada */
static ai_float s_{macro.lower()}_in[2];
{buffers_saida}


int {pascal}_Init(void)
{{
    const ai_handle ativacoes[] = {{ s_{macro.lower()}_activations }};

    s_{macro.lower()}_ultimo_erro = ai_{api}_create_and_init(
        &s_{macro.lower()}_network, ativacoes, NULL);

    return (s_{macro.lower()}_ultimo_erro.type == AI_ERROR_NONE) ? 0 : -1;
}}


int {pascal}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{{
    ai_buffer *ai_input;
    ai_buffer *ai_output;
    ai_i32 lotes_processados;

    if (s_{macro.lower()}_network == AI_HANDLE_NULL)
    {{
        return -1; /* {pascal}_Init nao foi chamado (ou falhou) */
    }}

    s_{macro.lower()}_in[0] = (ai_float)bateria_pct;
    s_{macro.lower()}_in[1] = (ai_float)taxa_normalizada;

    ai_input  = ai_{api}_inputs_get(s_{macro.lower()}_network, NULL);
    ai_output = ai_{api}_outputs_get(s_{macro.lower()}_network, NULL);

    ai_input[0].data = AI_HANDLE_PTR(s_{macro.lower()}_in);
{sets_saida}

    lotes_processados = ai_{api}_run(s_{macro.lower()}_network, ai_input, ai_output);
    if (lotes_processados != 1)
    {{
        s_{macro.lower()}_ultimo_erro = ai_{api}_get_error(s_{macro.lower()}_network);
        return -1;
    }}

    /* A rede ja calcula a classe prevista (saida {idx_rotulo}, formato S32);
     * assume-se a mesma convencao 0 = nao liga / 1 = liga usada no
     * treinamento do modelo - confira se bate com o seu dataset. */
    return {expressao_decisao};
}}
"""


# ----------------------------------------------------------------------
# main
# ----------------------------------------------------------------------
def localizar_pastas(caminho_h, args):
    """Descobre onde gravar o .h (perto do original) e o .c (Src/ irmao de Inc/, se existir)."""
    pasta_h_original = Path(caminho_h).resolve().parent

    if args.out_dir:
        return Path(args.out_dir), Path(args.out_dir)
    if args.inc_dir or args.src_dir:
        return Path(args.inc_dir or pasta_h_original), Path(args.src_dir or pasta_h_original)

    # padrao do projeto: <Core>/Inc/nba_model.h -> wrapper.h tambem em Inc/,
    # wrapper.c no Src/ irmao, se existir
    if pasta_h_original.name.lower() == "inc":
        candidato_src = pasta_h_original.parent / "Src"
        if candidato_src.is_dir():
            return pasta_h_original, candidato_src
    return pasta_h_original, pasta_h_original


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("modelo_h", help="caminho do <nome>.h gerado pelo X-CUBE-AI (ex.: nba_model.h)")
    ap.add_argument("--out-dir", help="pasta unica para gravar .h e .c do wrapper")
    ap.add_argument("--inc-dir", help="pasta para o <nome>_wrapper.h (padrao: junto do .h original)")
    ap.add_argument("--src-dir", help="pasta para o <nome>_wrapper.c (padrao: Src/ irmao de Inc/, se existir)")
    args = ap.parse_args()

    caminho_h = Path(args.modelo_h)
    if not caminho_h.is_file():
        print(f"ERRO: '{caminho_h}' nao encontrado.")
        sys.exit(1)

    pascal, macro = derive_prefixes(caminho_h.name)

    try:
        info = ler_info_rede(caminho_h)
    except ValueError as e:
        print(f"ERRO: {e}")
        sys.exit(1)

    if info["in1_size"] is not None and info["in1_size"] != 2:
        print(f"AVISO: {caminho_h.name} declara AI_{macro}_IN_1_SIZE = "
              f"{info['in1_size']} (esperado 2 - bateria_pct/taxa_normalizada). "
              f"O wrapper gerado assume 2 entradas; ajuste manualmente se necessario.")

    print(f"[{caminho_h.name}]")
    print(f"  - API real do X-CUBE-AI: ai_{info['api']}_*")
    print(f"  - prefixos do wrapper: {pascal}_* / {macro}_*")
    print(f"  - entradas: {info['in_num']} (IN_1_SIZE={info['in1_size']})   saidas: {info['out_num']}")
    for s in info["saidas"]:
        marca = "  <- usada como decisao" if s["indice"] == info["idx_rotulo"] else ""
        print(f"    saida {s['indice']}: formato={s['formato']} tamanho={s['tamanho']}{marca}")

    pasta_inc, pasta_src = localizar_pastas(caminho_h, args)
    caminho_wrapper_h = pasta_inc / f"{caminho_h.stem}_wrapper.h"
    caminho_wrapper_c = pasta_src / f"{caminho_h.stem}_wrapper.c"

    guard = re.sub(r"[^0-9A-Za-z]", "_", caminho_wrapper_h.name).upper()
    texto_h = gerar_wrapper_h(caminho_h.name, pascal, macro, guard)
    texto_c = gerar_wrapper_c(caminho_h.name, caminho_wrapper_h.name, pascal, macro, info)

    pasta_inc.mkdir(parents=True, exist_ok=True)
    pasta_src.mkdir(parents=True, exist_ok=True)
    caminho_wrapper_h.write_text(texto_h, encoding="utf-8", newline="\n")
    caminho_wrapper_c.write_text(texto_c, encoding="utf-8", newline="\n")

    print(f"  - '{caminho_wrapper_h}' gerado")
    print(f"  - '{caminho_wrapper_c}' gerado")


if __name__ == "__main__":
    main()