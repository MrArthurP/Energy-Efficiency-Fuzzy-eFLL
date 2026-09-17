/**
  ******************************************************************************
  * @file           : lgb_model.h
  * @brief          : Interface do modelo de Machine Learning - Arvore de Decisao
  *                    - ajustado via Controlador Fuzzy para decidir
  *                    ligar ou nao o GPRS da coleira de monitoramento de
  *                    animais silvestres, com base na carga da bateria e
  *                    na taxa de acerto no envio de dados via
  *                    GPRS/satelite.
  *
  * Portado via m2cgen do modelo treinado em Python (sklearn) para C puro.
  ******************************************************************************
  */
#ifndef LGB_MODEL_H
#define LGB_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Numero de features de entrada do modelo */
#define LGB_MODEL_NUM_INPUTS  2
/* Numero de classes de saida (pertinencia/probabilidade de cada classe) */
#define LGB_MODEL_NUM_OUTPUTS 2

/* Indices das classes no vetor de saida de score() */
#define LGB_MODEL_CLASSE_NAO_LIGA 0
#define LGB_MODEL_CLASSE_LIGA     1

/**
 * @brief  Executa o modelo (portado via m2cgen do modelo treinado em
 *         Python/sklearn) e retorna a pertinencia de cada classe.
 * @param  input:  vetor com LGB_MODEL_NUM_INPUTS valores de entrada, na
 *                 mesma ordem/escala usada no treinamento do modelo.
 * @param  output: vetor de saida com LGB_MODEL_NUM_OUTPUTS valores;
 *                 output[LGB_MODEL_CLASSE_NAO_LIGA] e
 *                 output[LGB_MODEL_CLASSE_LIGA] somam 1.0
 */
void LgbModel_Score(double *input, double *output);

/**
 * @brief  Wrapper de conveniencia: executa o modelo e retorna diretamente
 *         a decisao binaria, comparando a pertinencia de cada classe.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario
 */
int LgbModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* LGB_MODEL_H */
