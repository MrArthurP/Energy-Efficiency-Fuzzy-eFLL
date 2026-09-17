/**
  ******************************************************************************
  * @file           : regmodel.h
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
#ifndef REG_MODEL_H
#define REG_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Número de features de entrada do modelo */
#define REGMODEL_NUM_INPUTS  2
/* Número de classes de saída (pertinência/probabilidade de cada classe) */
#define REGMODEL_NUM_OUTPUTS 2

/* Índices das classes no vetor de saída de score() */
#define REGMODEL_CLASSE_NAO_LIGA 0
#define REGMODEL_CLASSE_LIGA     1

#define REG_MODEL_THRESHOLD  0.5

/**
 * @brief  Executa a árvore de decisão (portada via m2cgen do modelo
 *         treinado em Python/sklearn) e retorna a pertinência de cada
 *         classe.
 * @param  input:  vetor com REGMODEL_NUM_INPUTS valores de entrada, na
 *                 mesma ordem/escala usada no treinamento do modelo
 *                 (verifique o script Python de treinamento/exportação
 *                 para confirmar a ordem exata das features).
 */
double RegModel_Score(double *input);

/**
 * @brief  Wrapper de conveniência: executa o modelo e retorna diretamente
 *         a decisão binária, comparando a pertinência de cada classe.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrário
 */

double RegModel_Probabilidade(double *input);

/**
 * @brief  Wrapper de conveniência: executa o modelo e retorna diretamente
 *         a decisão binária, comparando a probabilidade da classe
 *         positiva com REG_MODELO_THRESHOLD.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrário
**/

int RegModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* REG_MODEL_H */
