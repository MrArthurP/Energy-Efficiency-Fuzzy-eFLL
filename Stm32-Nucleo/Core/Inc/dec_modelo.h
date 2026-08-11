/**
  ******************************************************************************
  * @file           : dec_modelo.h
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
#ifndef DEC_MODELO_H
#define DEC_MODELO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Número de features de entrada do modelo */
#define DEC_MODELO_NUM_INPUTS  2
/* Número de classes de saída (pertinência/probabilidade de cada classe) */
#define DEC_MODELO_NUM_OUTPUTS 2

/* Índices das classes no vetor de saída de score() */
#define DEC_MODELO_CLASSE_NAO_LIGA 0
#define DEC_MODELO_CLASSE_LIGA     1

/**
 * @brief  Executa a árvore de decisão (portada via m2cgen do modelo
 *         treinado em Python/sklearn) e retorna a pertinência de cada
 *         classe.
 * @param  input:  vetor com DEC_MODELO_NUM_INPUTS valores de entrada, na
 *                 mesma ordem/escala usada no treinamento do modelo
 *                 (verifique o script Python de treinamento/exportação
 *                 para confirmar a ordem exata das features).
 * @param  output: vetor de saída com DEC_MODELO_NUM_OUTPUTS valores;
 *                 output[DEC_MODELO_CLASSE_NAO_LIGA] e
 *                 output[DEC_MODELO_CLASSE_LIGA] somam 1.0
 */
void DecModelo_Score(double *input, double *output);

/**
 * @brief  Wrapper de conveniência: executa o modelo e retorna diretamente
 *         a decisão binária, comparando a pertinência de cada classe.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrário
 */
int DecModelo_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* DEC_MODELO_H */