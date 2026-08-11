/**
  ******************************************************************************
  * @file           : reg_modelo.h
  * @brief          : Interface do modelo de Machine Learning - Regressão
  *                    Logística - usado para decidir ligar ou não o GPRS da
  *                    coleira de monitoramento de animais silvestres, com
  *                    base na carga da bateria e na taxa de acerto no envio
  *                    de dados via GPRS/satélite.
  *
  * Portado via m2cgen do modelo treinado em Python (sklearn) para C puro.
  ******************************************************************************
  */
#ifndef REG_MODELO_H
#define REG_MODELO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Número de features de entrada do modelo */
#define REG_MODELO_NUM_INPUTS 2

/* Limiar de decisão sobre a probabilidade (0.0 - 1.0) retornada pelo
 * modelo para decidir se o GPRS deve ser ligado */
#define REG_MODELO_THRESHOLD  0.5

/**
 * @brief  Calcula o valor da combinação linear (logit) do modelo de
 *         Regressão Logística treinado em Python (sklearn) e portado via
 *         m2cgen para C puro. Esta é a função original gerada pelo m2cgen
 *         -- NÃO aplica a sigmoide.
 * @param  input: vetor com REG_MODELO_NUM_INPUTS valores de entrada, na
 *                mesma ordem/escala usada no treinamento do modelo
 *                (verifique o script Python de treinamento/exportação
 *                para confirmar a ordem exata das features).
 * @retval Valor do logit (qualquer número real). Para obter a
 *         probabilidade da classe positiva, use RegModelo_Probabilidade().
 */
double RegModelo_Score(double *input);

/**
 * @brief  Converte o logit retornado por score() em probabilidade (0-1)
 *         através da função sigmoide: p = 1 / (1 + e^(-logit))
 * @param  input: vetor com REG_MODELO_NUM_INPUTS valores de entrada
 * @retval Probabilidade estimada da classe positiva (ligar o GPRS)
 */
double RegModelo_Probabilidade(double *input);

/**
 * @brief  Wrapper de conveniência: executa o modelo e retorna diretamente
 *         a decisão binária, comparando a probabilidade da classe
 *         positiva com REG_MODELO_THRESHOLD.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrário
 */
int RegModelo_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* REG_MODELO_H */