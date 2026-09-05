#!/usr/bin/env python3
"""
Script para gerar o wrapper e o header a partir de um arquivo .c produzido
pelo m2cgen (árvore de decisão com 2 features e 2 classes).

Uso:
    python gerar_wrapper_m2cgen.py arquivo_modelo.c

O script:
    - Extrai o nome da função de score (ex.: "DecModel_Score");
    - Se encontrar função "predict_{nome}", renomeia para "{Prefixo}_Score";
    - Conta o número de entradas e saídas usados no código;
    - Cria o arquivo .h com defines, comentários e protótipos;
    - Modifica o .c para incluir o .h e adiciona a função de decisão binária.
"""

import os
import re
import sys
from pathlib import Path

# ----------------------------------------------------------------------
# 1. Parâmetros fixos (caso queira forçar nomes, descomente e ajuste)
# ----------------------------------------------------------------------
FORCAR_PREFIXO = None       # ex.: "MeuModelo"
# FORCAR_INPUTS = None       # ex.: 2
# FORCAR_OUTPUTS = None      # ex.: 2
# CLASSE_NAO_LIGA = 0
# CLASSE_LIGA = 1

# ----------------------------------------------------------------------
# 2. Funções auxiliares
# ----------------------------------------------------------------------
def extrair_nome_funcao_score(conteudo):
    """
    Procura pela definição da função score no formato:
        void Nome_Score(double *input, double *output)
    Retorna o nome completo ou None se não encontrado.
    """
    # Padrão para função predict_{nome}
    padrao_predict = re.compile(r'void\s+(predict_\w+)\s*\([^)]*double\s*\*[^,]*,[^)]*double\s*\*[^)]*\)')
    match = padrao_predict.search(conteudo)
    if match:
        return match.group(1)
    
    # Padrão mais flexível: aceita espaços extras e diferentes nomes de parâmetros
    padrao = re.compile(r'void\s+(\w+_Score)\s*\([^)]*double\s*\*[^,]*,[^)]*double\s*\*[^)]*\)')
    match = padrao.search(conteudo)
    if match:
        return match.group(1)
    
    # Tenta um padrão alternativo: função com qualquer nome que termine com _Score
    padrao2 = re.compile(r'void\s+(\w+_Score)\s*\([^)]*\)')
    match2 = padrao2.search(conteudo)
    if match2:
        return match2.group(1)
    
    # Última tentativa: qualquer função que contenha "Score" no nome e tenha parâmetros double
    padrao3 = re.compile(r'void\s+(\w*Score\w*)\s*\([^)]*double\s*\*[^)]*\)')
    match3 = padrao3.search(conteudo)
    if match3:
        return match3.group(1)
    
    return None

def extrair_prefixo_do_nome_arquivo(nome_arquivo):
    """
    Extrai um prefixo do nome do arquivo (ex.: dec_modelo.c -> DecModelo)
    """
    nome_base = Path(nome_arquivo).stem
    # Remove extensões comuns
    nome_base = re.sub(r'\.(c|h|cpp|hpp)$', '', nome_base)
    # Converte snake_case para CamelCase
    partes = nome_base.split('_')
    prefixo = ''.join([p.capitalize() for p in partes])
    return prefixo

def extrair_nome_predict(conteudo):
    """
    Procura por função predict_{nome} no código
    """
    padrao = re.compile(r'void\s+(predict_\w+)\s*\(')
    match = padrao.search(conteudo)
    if match:
        return match.group(1)
    return None

def renomear_funcao_score(conteudo, nome_antigo, nome_novo):
    """
    Renomeia a função de score no código
    """
    # Substitui a definição da função
    padrao_def = re.compile(rf'void\s+{re.escape(nome_antigo)}\s*\(')
    conteudo = padrao_def.sub(f'void {nome_novo}(', conteudo)
    
    # Substitui chamadas da função dentro do código
    padrao_call = re.compile(rf'\b{re.escape(nome_antigo)}\s*\(')
    conteudo = padrao_call.sub(f'{nome_novo}(', conteudo)
    
    return conteudo

def contar_indices_maximos(conteudo, nome_array):
    """
    Retorna o maior índice usado para o array 'nome_array' (ex.: 'input').
    Se nenhum for encontrado, retorna 0.
    """
    padrao = re.compile(rf'{nome_array}\[(\d+)\]')
    indices = [int(m) for m in padrao.findall(conteudo)]
    if indices:
        return max(indices) + 1   # quantidade = maior índice + 1
    return 0

def extrair_informacoes_modelo(conteudo):
    """
    Extrai informações sobre o modelo: número de entradas, saídas e se tem
    a estrutura de árvore de decisão.
    """
    num_inputs = contar_indices_maximos(conteudo, 'input')
    num_outputs = contar_indices_maximos(conteudo, 'output')
    
    # Verifica se tem a estrutura de memcpy da árvore
    tem_arvore = 'memcpy' in conteudo and 'if' in conteudo
    
    # Se não encontrou índices, assume valores padrão
    if num_inputs == 0:
        # Tenta contar quantas variáveis são usadas nos ifs
        padrao_if = re.compile(r'input\[\s*(\d+)\s*\]')
        indices_if = [int(m) for m in padrao_if.findall(conteudo)]
        if indices_if:
            num_inputs = max(indices_if) + 1
        else:
            num_inputs = 2  # Valor padrão para o exemplo
    
    if num_outputs == 0:
        # Verifica se tem arrays com tamanho explícito
        padrao_array = re.compile(r'double\s+\w+\[(\d+)\]')
        tamanhos = [int(m) for m in padrao_array.findall(conteudo)]
        if tamanhos:
            # Pega o tamanho do array que parece ser de saída
            for tam in tamanhos:
                if tam >= 2:
                    num_outputs = tam
                    break
        if num_outputs == 0:
            num_outputs = 2  # Valor padrão para o exemplo
    
    return num_inputs, num_outputs, tem_arvore

def gerar_header(prefixo, num_inputs, num_outputs, classe_nao_liga=0, classe_liga=1):
    """
    Gera o conteúdo do arquivo .h com defines, comentários e protótipos.
    """
    header = f"""/**
  ******************************************************************************
  * @file           : {prefixo.lower()}.h
  * @brief          : Interface do modelo de Machine Learning - Árvore de
  *                    Decisão - ajustado via Controlador Fuzzy para decidir
  *                    ligar ou não o GPRS da coleira de monitoramento de
  *                    animais silvestres, com base na carga da bateria e
  *                    na taxa de acerto no envio de dados via
  *                    GPRS/satélite.
  *
  * Portado via m2cgen do modelo treinado em Python (sklearn) para C puro.
  ******************************************************************************
  */
#ifndef {prefixo.upper()}_H
#define {prefixo.upper()}_H

#ifdef __cplusplus
extern "C" {{
#endif

/* Número de features de entrada do modelo */
#define {prefixo.upper()}_NUM_INPUTS  {num_inputs}
/* Número de classes de saída (pertinência/probabilidade de cada classe) */
#define {prefixo.upper()}_NUM_OUTPUTS {num_outputs}

/* Índices das classes no vetor de saída de score() */
#define {prefixo.upper()}_CLASSE_NAO_LIGA {classe_nao_liga}
#define {prefixo.upper()}_CLASSE_LIGA     {classe_liga}

/**
 * @brief  Executa a árvore de decisão (portada via m2cgen do modelo
 *         treinado em Python/sklearn) e retorna a pertinência de cada
 *         classe.
 * @param  input:  vetor com {prefixo.upper()}_NUM_INPUTS valores de entrada, na
 *                 mesma ordem/escala usada no treinamento do modelo
 *                 (verifique o script Python de treinamento/exportação
 *                 para confirmar a ordem exata das features).
 * @param  output: vetor de saída com {prefixo.upper()}_NUM_OUTPUTS valores;
 *                 output[{prefixo.upper()}_CLASSE_NAO_LIGA] e
 *                 output[{prefixo.upper()}_CLASSE_LIGA] somam 1.0
 */
void {prefixo}_Score(double *input, double *output);

/**
 * @brief  Wrapper de conveniência: executa o modelo e retorna diretamente
 *         a decisão binária, comparando a pertinência de cada classe.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrário
 */
int {prefixo}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}}
#endif

#endif /* {prefixo.upper()}_H */
"""
    return header

def gerar_wrapper_c(prefixo, num_inputs):
    """
    Gera o código da função wrapper a ser inserida no .c.
    O wrapper assume que o modelo tem exatamente 2 entradas (bateria e taxa).
    Caso num_inputs != 2, gera um wrapper genérico com array.
    """
    if num_inputs == 2:
        return f"""
int {prefixo}_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{{
    double input[{prefixo.upper()}_NUM_INPUTS];
    double output[{prefixo.upper()}_NUM_OUTPUTS];

    input[0] = (double)bateria_pct;
    input[1] = (double)taxa_normalizada;

    {prefixo}_Score(input, output);

    return (output[{prefixo.upper()}_CLASSE_LIGA] > output[{prefixo.upper()}_CLASSE_NAO_LIGA]) ? 1 : 0;
}}
"""
    else:
        # Wrapper genérico para número arbitrário de entradas (mas ainda com 2 saídas)
        return f"""
int {prefixo}_DecideLigarGPRS(double *input)
{{
    double output[{prefixo.upper()}_NUM_OUTPUTS];
    {prefixo}_Score(input, output);
    return (output[{prefixo.upper()}_CLASSE_LIGA] > output[{prefixo.upper()}_CLASSE_NAO_LIGA]) ? 1 : 0;
}}
"""

# ----------------------------------------------------------------------
# 3. Função principal
# ----------------------------------------------------------------------
def main():
    if len(sys.argv) < 2:
        print("Uso: python gerar_wrapper_m2cgen.py arquivo_modelo.c")
        sys.exit(1)

    caminho_c = Path(sys.argv[1])
    if not caminho_c.is_file():
        print(f"Erro: arquivo '{caminho_c}' não encontrado.")
        sys.exit(1)

    # Lê o conteúdo do .c
    with open(caminho_c, 'r', encoding='utf-8') as f:
        conteudo = f.read()

    # 1. Verifica se existe função predict_{nome}
    nome_predict = extrair_nome_predict(conteudo)
    
    # 2. Extrai ou define o prefixo
    if FORCAR_PREFIXO:
        prefixo = FORCAR_PREFIXO
        print(f"Prefixo forçado: {prefixo}")
    else:
        # Tenta extrair do nome da função score
        nome_score = extrair_nome_funcao_score(conteudo)
        if nome_score:
            if nome_score.startswith('predict_'):
                # Extrai prefixo do nome do arquivo
                prefixo = extrair_prefixo_do_nome_arquivo(caminho_c.name)
                print(f"Função predict encontrada. Prefixo inferido do nome do arquivo: {prefixo}")
            else:
                prefixo = nome_score.replace('_Score', '')
                print(f"Prefixo identificado da função _Score: {prefixo}")
        else:
            # Se não encontrou nenhuma função, usa o nome do arquivo
            prefixo = extrair_prefixo_do_nome_arquivo(caminho_c.name)
            print(f"Nenhuma função _Score encontrada. Prefixo inferido do nome do arquivo: {prefixo}")

    # 3. Renomeia a função predict para {prefixo}_Score se necessário
    if nome_predict and nome_predict != f"{prefixo}_Score":
        print(f"Renomeando função '{nome_predict}' para '{prefixo}_Score'")
        conteudo = renomear_funcao_score(conteudo, nome_predict, f"{prefixo}_Score")
        
        # Atualiza nome_score para o novo nome
        nome_score = f"{prefixo}_Score"
    else:
        # Verifica se já existe {prefixo}_Score
        if f"{prefixo}_Score" not in conteudo:
            print(f"Aviso: Função '{prefixo}_Score' não encontrada no arquivo.")
            # Tenta encontrar qualquer função com Score
            nome_score = extrair_nome_funcao_score(conteudo)
            if nome_score:
                print(f"Encontrada função alternativa: {nome_score}")
                # Renomeia para o padrão
                if nome_score != f"{prefixo}_Score":
                    conteudo = renomear_funcao_score(conteudo, nome_score, f"{prefixo}_Score")
                    nome_score = f"{prefixo}_Score"
        else:
            nome_score = f"{prefixo}_Score"

    # 4. Extrai informações do modelo
    num_inputs, num_outputs, tem_arvore = extrair_informacoes_modelo(conteudo)
    
    # Se não encontrou a estrutura de árvore, usa valores padrão
    if num_inputs == 0:
        num_inputs = 2
    if num_outputs == 0:
        num_outputs = 2
        
    print(f"Entradas: {num_inputs}, Saídas: {num_outputs}")
    print(f"Estrutura de árvore encontrada: {tem_arvore}")

    # 5. Cria o conteúdo do .h
    header_content = gerar_header(prefixo, num_inputs, num_outputs)

    # 6. Cria o arquivo .h
    nome_h = caminho_c.with_suffix('.h')
    with open(nome_h, 'w', encoding='utf-8') as f:
        f.write(header_content)
    print(f"Arquivo '{nome_h}' gerado.")

    # 7. Modifica o .c
    # 7a. Verifica se o include do .h já existe
    include_str = f'#include "{nome_h.name}"'
    if include_str not in conteudo:
        # Insere após o último include existente
        linhas = conteudo.splitlines()
        novo_conteudo = []
        include_adicionado = False
        for linha in linhas:
            novo_conteudo.append(linha)
            if not include_adicionado and linha.startswith('#include'):
                # Insere após este include
                novo_conteudo.append(include_str)
                include_adicionado = True
        if not include_adicionado:
            # Se não houver nenhum include, coloca no início
            novo_conteudo = [include_str, ''] + linhas
        conteudo = '\n'.join(novo_conteudo)

    # 7b. Adiciona a função wrapper no final (se já não existir)
    wrapper_func_name = f"{prefixo}_DecideLigarGPRS"
    if wrapper_func_name not in conteudo:
        wrapper_code = gerar_wrapper_c(prefixo, num_inputs)
        conteudo += '\n' + wrapper_code
        print(f"Função '{wrapper_func_name}' adicionada ao arquivo.")
    else:
        print(f"Aviso: a função '{wrapper_func_name}' já existe no arquivo. Não será adicionada novamente.")

    # 7c. Salva o .c modificado (faz backup)
    backup = caminho_c.with_suffix('.c.bak')
    caminho_c.rename(backup)
    with open(caminho_c, 'w', encoding='utf-8') as f:
        f.write(conteudo)
    print(f"Arquivo '{caminho_c}' modificado. Backup salvo em '{backup}'.")

if __name__ == '__main__':
    main()