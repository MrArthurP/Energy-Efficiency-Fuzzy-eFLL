#!/usr/bin/env python3
"""
Script para gerar o wrapper e o header a partir de um arquivo .c produzido
pelo m2cgen.

Suporta as duas familias de saida que o m2cgen gera:

  A) MODELOS COM VETOR DE SAIDA  (arvore de decisao, random forest, etc.)
         void predict_x(double * input, double * output)
     -> renomeada para "{Prefixo}_Score"
     -> wrapper compara output[CLASSE_LIGA] com output[CLASSE_NAO_LIGA]

  B) MODELOS DE VALOR UNICO  (regressao logistica, regressao linear, SVM...)
         double predict_x(double * input)
     -> renomeada para "{Prefixo}_Score"
     -> e criada uma funcao "{Prefixo}_Probabilidade" com o calculo de
        probabilidade ADEQUADO AO TIPO do modelo:
            logistica : sigmoide do logit           1/(1+exp(-z))
            svm       : sigmoide da margem (Platt)  1/(1+exp(-d))
            linear    : valor previsto saturado em [0, 1]
     -> wrapper compara a probabilidade com "{PREFIXO}_THRESHOLD"

Uso:
    python Adicionar_wrapper_m2cgen.py arquivo_modelo.c
    python Adicionar_wrapper_m2cgen.py reg_model.c  --tipo logistica
    python Adicionar_wrapper_m2cgen.py svm_model.c  --tipo svm
    python Adicionar_wrapper_m2cgen.py lin_model.c  --tipo linear
    python Adicionar_wrapper_m2cgen.py dec_model.c  --tipo arvore
    python Adicionar_wrapper_m2cgen.py *.c          --threshold 0.6

Sem "--tipo" o script tenta deduzir o tipo pela assinatura da funcao, pelo
codigo (presenca de exp()/pow() indica kernel de SVM) e pelo nome do
arquivo (reg/log -> logistica, lin -> linear, svm -> svm, dec/tree/rf ->
arvore). Como regressao linear e logistica geram codigo IDENTICO (soma de
input[i]*coef), nesses casos vale sempre passar "--tipo" explicitamente.

O script:
    - Detecta a assinatura e o tipo do modelo;
    - Renomeia "predict_{nome}" para "{Prefixo}_Score";
    - Conta o numero de entradas (e saidas, quando houver);
    - Cria o arquivo .h com defines, comentarios e prototipos;
    - Modifica o .c: inclui o .h, garante <math.h> quando necessario, e
      adiciona "{Prefixo}_Probabilidade" (modelos de valor unico) e
      "{Prefixo}_DecideLigarGPRS".
"""

import argparse
import re
import sys
from pathlib import Path

# ----------------------------------------------------------------------
# 1. Parametros fixos (caso queira forcar valores, ajuste aqui)
# ----------------------------------------------------------------------
FORCAR_PREFIXO = None       # ex.: "MeuModelo"
CLASSE_NAO_LIGA = 0
CLASSE_LIGA = 1
THRESHOLD_PADRAO = "0.5"    # limite de decisao dos modelos de valor unico

TIPOS_VALOR_UNICO = ("logistica", "linear", "svm")
TIPOS_SUPORTADOS = ("arvore",) + TIPOS_VALOR_UNICO

DESCRICAO_TIPO = {
    "arvore": "Arvore de Decisao",
    "logistica": "Regressao Logistica",
    "linear": "Regressao Linear",
    "svm": "SVM (Support Vector Machine)",
}


# ----------------------------------------------------------------------
# 2. Nomes / prefixos
# ----------------------------------------------------------------------
def extrair_prefixo_do_nome_arquivo(nome_arquivo):
    """
    dec_modelo.c -> DecModelo ; reg_model.c -> RegModel
    """
    nome_base = Path(nome_arquivo).stem
    nome_base = re.sub(r'\.(c|h|cpp|hpp)$', '', nome_base)
    partes = [p for p in re.split(r'[_\-]+', nome_base) if p]
    return ''.join(p.capitalize() for p in partes)


def extrair_prefixo_macro(nome_arquivo):
    """
    dec_modelo.c -> DEC_MODELO ; reg_model.c -> REG_MODEL
    (mantem os underscores, igual ao padrao de dec_modelo.h)
    """
    nome_base = Path(nome_arquivo).stem
    nome_base = re.sub(r'\.(c|h|cpp|hpp)$', '', nome_base)
    partes = [p for p in re.split(r'[_\-]+', nome_base) if p]
    return '_'.join(p.upper() for p in partes)


# ----------------------------------------------------------------------
# 3. Deteccao da assinatura e do tipo do modelo
# ----------------------------------------------------------------------
def detectar_assinatura(conteudo):
    """
    Descobre qual funcao gerada pelo m2cgen existe no arquivo.

    Retorna dict:
        {"forma": "vetor_saida"|"valor_unico", "nome": <nome da funcao>,
         "retorno": "void"|"double"}
    ou None se nada for encontrado.
    """
    # A) void nome(double * input, double * output)
    padrao_vetor = re.compile(
        r'\b(void)\s+(\w+)\s*\(\s*double\s*\*\s*\w+\s*,\s*double\s*\*\s*\w+\s*\)'
    )
    m = padrao_vetor.search(conteudo)
    if m:
        return {"forma": "vetor_saida", "nome": m.group(2), "retorno": "void"}

    # B) double nome(double * input)
    padrao_unico = re.compile(
        r'\b(double)\s+(\w+)\s*\(\s*double\s*\*\s*\w+\s*\)'
    )
    m = padrao_unico.search(conteudo)
    if m:
        return {"forma": "valor_unico", "nome": m.group(2), "retorno": "double"}

    return None


def detectar_tipo_modelo(conteudo, nome_arquivo, assinatura):
    """
    Deduz o tipo do modelo. Ordem de prioridade:
      1. forma da assinatura (vetor de saida -> arvore);
      2. codigo: exp()+pow() juntos indicam kernel RBF de SVM;
      3. nome do arquivo.
    """
    nome = Path(nome_arquivo).stem.lower()

    if assinatura and assinatura["forma"] == "vetor_saida":
        return "arvore"

    if 'svm' in nome:
        return "svm"
    if re.search(r'\bexp\s*\(', conteudo) and re.search(r'\bpow\s*\(', conteudo):
        return "svm"

    if 'log' in nome:
        return "logistica"
    if 'lin' in nome:
        return "linear"
    if 'reg' in nome:
        # "reg_model" no projeto e a regressao logistica
        return "logistica"
    if any(t in nome for t in ('dec', 'tree', 'arvore', 'rf', 'forest')):
        return "arvore"

    # padrao mais seguro para modelos de valor unico
    return "logistica"


def renomear_funcao(conteudo, nome_antigo, nome_novo):
    """
    Renomeia a funcao (definicao e chamadas) preservando o tipo de retorno.
    """
    return re.sub(rf'\b{re.escape(nome_antigo)}\b', nome_novo, conteudo)


# ----------------------------------------------------------------------
# 4. Contagem de entradas / saidas
# ----------------------------------------------------------------------
def contar_indices_maximos(conteudo, nome_array):
    padrao = re.compile(rf'{nome_array}\[\s*(\d+)\s*\]')
    indices = [int(m) for m in padrao.findall(conteudo)]
    return max(indices) + 1 if indices else 0


def extrair_informacoes_modelo(conteudo, forma):
    """
    num_inputs: maior indice usado em input[] + 1
    num_outputs: so faz sentido para modelos com vetor de saida
    """
    num_inputs = contar_indices_maximos(conteudo, 'input') or 2

    if forma == "vetor_saida":
        num_outputs = contar_indices_maximos(conteudo, 'output')
        if num_outputs == 0:
            tamanhos = [int(m) for m in re.findall(r'double\s+\w+\[\s*(\d+)\s*\]', conteudo)]
            num_outputs = next((t for t in tamanhos if t >= 2), 2)
    else:
        num_outputs = 1   # a funcao devolve um unico valor (return)

    return num_inputs, num_outputs


# ----------------------------------------------------------------------
# 5. Geracao do .h
# ----------------------------------------------------------------------
def _cabecalho_doc(nome_h, tipo):
    return f"""/**
  ******************************************************************************
  * @file           : {nome_h}
  * @brief          : Interface do modelo de Machine Learning - {DESCRICAO_TIPO[tipo]}
  *                    - ajustado via Controlador Fuzzy para decidir
  *                    ligar ou nao o GPRS da coleira de monitoramento de
  *                    animais silvestres, com base na carga da bateria e
  *                    na taxa de acerto no envio de dados via
  *                    GPRS/satelite.
  *
  * Portado via m2cgen do modelo treinado em Python (sklearn) para C puro.
  ******************************************************************************
  */"""


def gerar_header_vetor_saida(nome_h, prefixo, macro, num_inputs, num_outputs, tipo):
    """
    Header dos modelos que devolvem vetor de pertinencia por classe.
    """
    return f"""{_cabecalho_doc(nome_h, tipo)}
#ifndef {macro}_H
#define {macro}_H

#ifdef __cplusplus
extern "C" {{
#endif

/* Numero de features de entrada do modelo */
#define {macro}_NUM_INPUTS  {num_inputs}
/* Numero de classes de saida (pertinencia/probabilidade de cada classe) */
#define {macro}_NUM_OUTPUTS {num_outputs}

/* Indices das classes no vetor de saida de score() */
#define {macro}_CLASSE_NAO_LIGA {CLASSE_NAO_LIGA}
#define {macro}_CLASSE_LIGA     {CLASSE_LIGA}

/**
 * @brief  Executa o modelo (portado via m2cgen do modelo treinado em
 *         Python/sklearn) e retorna a pertinencia de cada classe.
 * @param  input:  vetor com {macro}_NUM_INPUTS valores de entrada, na
 *                 mesma ordem/escala usada no treinamento do modelo.
 * @param  output: vetor de saida com {macro}_NUM_OUTPUTS valores;
 *                 output[{macro}_CLASSE_NAO_LIGA] e
 *                 output[{macro}_CLASSE_LIGA] somam 1.0
 */
void {prefixo}_Score(double *input, double *output);

/**
 * @brief  Wrapper de conveniencia: executa o modelo e retorna diretamente
 *         a decisao binaria, comparando a pertinencia de cada classe.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario
 */
int {prefixo}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}}
#endif

#endif /* {macro}_H */
"""


DOC_SCORE_VALOR_UNICO = {
    "logistica": ("logit (soma linear antes da sigmoide) calculado pelo\n"
                  " *         modelo de regressao logistica"),
    "linear": "valor previsto pelo modelo de regressao linear",
    "svm": ("valor da funcao de decisao do SVM (distancia com sinal\n"
            " *         ate o hiperplano separador)"),
}

DOC_PROB_VALOR_UNICO = {
    "logistica": ("Converte o logit em probabilidade aplicando a funcao\n"
                  " *         sigmoide: 1 / (1 + exp(-logit))."),
    "linear": ("Converte o valor previsto em probabilidade saturando-o\n"
               " *         no intervalo [0, 1] (a regressao linear ja estima\n"
               " *         diretamente o alvo, mas pode extrapolar)."),
    "svm": ("Converte a margem da funcao de decisao em pseudo-\n"
            " *         probabilidade aplicando a sigmoide (Platt scaling\n"
            " *         simplificado): 1 / (1 + exp(-decisao))."),
}


def gerar_header_valor_unico(nome_h, prefixo, macro, num_inputs, tipo, threshold):
    """
    Header dos modelos que devolvem um unico valor (logistica, linear, SVM).
    """
    return f"""{_cabecalho_doc(nome_h, tipo)}
#ifndef {macro}_H
#define {macro}_H

#ifdef __cplusplus
extern "C" {{
#endif

/* Numero de features de entrada do modelo */
#define {macro}_NUM_INPUTS  {num_inputs}

/* Limite de decisao: probabilidade a partir da qual o GPRS e ligado */
#define {macro}_THRESHOLD {threshold}

/**
 * @brief  Executa o modelo (portado via m2cgen do modelo treinado em
 *         Python/sklearn) e retorna o {DOC_SCORE_VALOR_UNICO[tipo]}.
 * @param  input: vetor com {macro}_NUM_INPUTS valores de entrada, na
 *                mesma ordem/escala usada no treinamento do modelo.
 * @retval valor bruto devolvido pelo modelo (nao e uma probabilidade)
 */
double {prefixo}_Score(double *input);

/**
 * @brief  {DOC_PROB_VALOR_UNICO[tipo]}
 * @param  input: vetor com {macro}_NUM_INPUTS valores de entrada
 * @retval probabilidade, no intervalo [0, 1], de o GPRS ser ligado
 */
double {prefixo}_Probabilidade(double *input);

/**
 * @brief  Wrapper de conveniencia: executa o modelo e retorna diretamente
 *         a decisao binaria, comparando a probabilidade com
 *         {macro}_THRESHOLD.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario
 */
int {prefixo}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}}
#endif

#endif /* {macro}_H */
"""


# ----------------------------------------------------------------------
# 6. Geracao do codigo C (Probabilidade + wrapper)
# ----------------------------------------------------------------------
def gerar_probabilidade_c(prefixo, tipo):
    """
    Funcao de probabilidade ADEQUADA AO TIPO do modelo de valor unico.
    """
    if tipo == "logistica":
        return f"""double {prefixo}_Probabilidade(double *input)
{{
    double logit = {prefixo}_Score(input);
    return 1.0 / (1.0 + exp(-logit));
}}
"""

    if tipo == "svm":
        return f"""double {prefixo}_Probabilidade(double *input)
{{
    /* A funcao de decisao do SVM devolve a margem (distancia com sinal ate
       o hiperplano). A sigmoide converte essa margem em pseudo-
       probabilidade (Platt scaling simplificado, A = -1 e B = 0). */
    double decisao = {prefixo}_Score(input);
    return 1.0 / (1.0 + exp(-decisao));
}}
"""

    if tipo == "linear":
        return f"""double {prefixo}_Probabilidade(double *input)
{{
    /* A regressao linear ja estima diretamente o alvo (0 a 1), mas pode
       extrapolar; por isso o valor e saturado no intervalo [0, 1]. */
    double valor = {prefixo}_Score(input);

    if (valor < 0.0)
    {{
        valor = 0.0;
    }}
    if (valor > 1.0)
    {{
        valor = 1.0;
    }}

    return valor;
}}
"""

    raise ValueError(f"tipo sem funcao de probabilidade definida: {tipo}")


def gerar_wrapper_vetor_saida(prefixo, macro):
    return f"""int {prefixo}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{{
    double input[{macro}_NUM_INPUTS];
    double output[{macro}_NUM_OUTPUTS];

    input[0] = (double)bateria_pct;
    input[1] = (double)taxa_normalizada;

    {prefixo}_Score(input, output);

    return (output[{macro}_CLASSE_LIGA] > output[{macro}_CLASSE_NAO_LIGA]) ? 1 : 0;
}}
"""


def gerar_wrapper_valor_unico(prefixo, macro):
    return f"""int {prefixo}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{{
    double input[{macro}_NUM_INPUTS];

    input[0] = (double)bateria_pct;
    input[1] = (double)taxa_normalizada;

    return ({prefixo}_Probabilidade(input) >= {macro}_THRESHOLD) ? 1 : 0;
}}
"""


# ----------------------------------------------------------------------
# 7. Edicao do .c
# ----------------------------------------------------------------------
def garantir_include(conteudo, linha_include, no_topo=False):
    """
    Garante que 'linha_include' exista no arquivo. Com no_topo=True coloca
    antes de qualquer outro include (padrao usado para o proprio .h).
    """
    if linha_include in conteudo:
        return conteudo, False

    if no_topo:
        return linha_include + "\n" + conteudo, True

    ultimo = None
    for m in re.finditer(r'^#include\s+.*$', conteudo, re.MULTILINE):
        ultimo = m
    if ultimo:
        pos = ultimo.end()
        return conteudo[:pos] + "\n" + linha_include + conteudo[pos:], True
    return linha_include + "\n" + conteudo, True


def localizar_funcao(conteudo, nome):
    """
    Devolve (inicio, fim) da definicao completa de 'nome' (assinatura +
    corpo, via contagem de chaves) ou None se nao existir.
    """
    m = re.search(rf'^[^\n;]*\b{re.escape(nome)}\s*\([^;{{}}]*\)\s*\n?\{{',
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
    """
    Substitui a definicao existente da funcao (se houver) ou acrescenta o
    codigo ao final do arquivo. Nunca duplica.
    """
    intervalo = localizar_funcao(conteudo, nome)
    if intervalo:
        inicio, fim = intervalo
        return conteudo[:inicio] + codigo.rstrip("\n") + conteudo[fim:], "substituida"
    return conteudo.rstrip("\n") + "\n\n" + codigo, "adicionada"


# ----------------------------------------------------------------------
# 8. Processamento de um arquivo
# ----------------------------------------------------------------------
def processar(caminho_c, tipo_forcado=None, threshold=THRESHOLD_PADRAO, backup=True):
    original = caminho_c.read_text(encoding='utf-8')
    conteudo = original.replace('\r\n', '\n')

    assinatura = detectar_assinatura(conteudo)
    if assinatura is None:
        print(f"[{caminho_c}] ERRO: nenhuma funcao do m2cgen encontrada "
              f"(esperado 'void f(double*, double*)' ou 'double f(double*)').")
        return False

    prefixo = FORCAR_PREFIXO or extrair_prefixo_do_nome_arquivo(caminho_c.name)
    macro = extrair_prefixo_macro(caminho_c.name)

    tipo = tipo_forcado or detectar_tipo_modelo(conteudo, caminho_c.name, assinatura)
    if tipo not in TIPOS_SUPORTADOS:
        print(f"[{caminho_c}] ERRO: tipo '{tipo}' desconhecido.")
        return False

    # coerencia entre tipo pedido e assinatura encontrada
    if tipo == "arvore" and assinatura["forma"] != "vetor_saida":
        print(f"[{caminho_c}] AVISO: tipo 'arvore' pedido, mas a funcao devolve um "
              f"unico valor; tratando como 'logistica'.")
        tipo = "logistica"
    if tipo in TIPOS_VALOR_UNICO and assinatura["forma"] == "vetor_saida":
        print(f"[{caminho_c}] AVISO: tipo '{tipo}' pedido, mas a funcao tem vetor de "
              f"saida; tratando como 'arvore'.")
        tipo = "arvore"

    print(f"[{caminho_c}]")
    print(f"  - funcao do m2cgen: {assinatura['retorno']} {assinatura['nome']}(...) "
          f"({'vetor de saida' if assinatura['forma'] == 'vetor_saida' else 'valor unico'})")
    print(f"  - tipo do modelo: {tipo} ({DESCRICAO_TIPO[tipo]})")
    print(f"  - prefixos: funcao '{prefixo}_*' / macro '{macro}_*'")

    # 1) renomeia a funcao do m2cgen para {Prefixo}_Score
    nome_score = f"{prefixo}_Score"
    if assinatura["nome"] != nome_score:
        conteudo = renomear_funcao(conteudo, assinatura["nome"], nome_score)
        print(f"  - '{assinatura['nome']}' renomeada para '{nome_score}'")

    # 2) conta entradas/saidas
    num_inputs, num_outputs = extrair_informacoes_modelo(conteudo, assinatura["forma"])
    print(f"  - entradas: {num_inputs}" +
          (f", saidas: {num_outputs}" if assinatura["forma"] == "vetor_saida" else ""))

    # 3) gera o .h
    caminho_h = caminho_c.with_suffix('.h')
    if assinatura["forma"] == "vetor_saida":
        header = gerar_header_vetor_saida(caminho_h.name, prefixo, macro,
                                          num_inputs, num_outputs, tipo)
    else:
        header = gerar_header_valor_unico(caminho_h.name, prefixo, macro,
                                          num_inputs, tipo, threshold)
    with open(caminho_h, 'w', encoding='utf-8', newline='\n') as f:
        f.write(header)
    print(f"  - '{caminho_h.name}' gerado")

    # 4) includes: o proprio .h no topo; <math.h> quando a matematica exige
    conteudo, add = garantir_include(conteudo, f'#include "{caminho_h.name}"', no_topo=True)
    if add:
        print(f"  - '#include \"{caminho_h.name}\"' adicionado")

    precisa_math = (tipo in ("logistica", "svm")
                    or re.search(r'\b(exp|pow|sqrt|log)\s*\(', conteudo) is not None)
    if precisa_math:
        conteudo, add = garantir_include(conteudo, '#include <math.h>')
        if add:
            print("  - '#include <math.h>' adicionado")

    # 5) funcao Probabilidade (somente modelos de valor unico)
    if assinatura["forma"] == "valor_unico":
        nome_prob = f"{prefixo}_Probabilidade"
        conteudo, acao = inserir_ou_substituir_funcao(
            conteudo, nome_prob, gerar_probabilidade_c(prefixo, tipo))
        print(f"  - '{nome_prob}' {acao} (calculo do tipo '{tipo}')")

    # 6) wrapper de decisao binaria
    nome_wrapper = f"{prefixo}_DecideLigarGPRS"
    codigo_wrapper = (gerar_wrapper_vetor_saida(prefixo, macro)
                      if assinatura["forma"] == "vetor_saida"
                      else gerar_wrapper_valor_unico(prefixo, macro))
    conteudo, acao = inserir_ou_substituir_funcao(conteudo, nome_wrapper, codigo_wrapper)
    print(f"  - '{nome_wrapper}' {acao}")

    # 7) salva (com backup opcional)
    if backup:
        destino_bak = Path(str(caminho_c) + '.bak')
        with open(destino_bak, 'w', encoding='utf-8', newline='') as f:
            f.write(original)
        print(f"  - backup em '{destino_bak.name}'")
    with open(caminho_c, 'w', encoding='utf-8', newline='\n') as f:
        f.write(conteudo.rstrip('\n') + '\n')
    print(f"  - '{caminho_c.name}' modificado")
    return True


# ----------------------------------------------------------------------
# 9. Main
# ----------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("arquivos", nargs="+", help="arquivo(s) .c gerado(s) pelo m2cgen")
    ap.add_argument("--tipo", choices=TIPOS_SUPORTADOS, default=None,
                    help="forca o tipo do modelo (recomendado para distinguir "
                         "regressao linear de logistica)")
    ap.add_argument("--threshold", default=THRESHOLD_PADRAO,
                    help=f"limite de decisao dos modelos de valor unico "
                         f"(padrao {THRESHOLD_PADRAO})")
    ap.add_argument("--sem-backup", action="store_true", help="nao gerar arquivo .c.bak")
    args = ap.parse_args()

    ok = True
    for nome in args.arquivos:
        caminho = Path(nome)
        if not caminho.is_file():
            print(f"[{caminho}] ERRO: arquivo nao encontrado.")
            ok = False
            continue
        if not processar(caminho, args.tipo, args.threshold, backup=not args.sem_backup):
            ok = False

    sys.exit(0 if ok else 1)


if __name__ == '__main__':
    main()