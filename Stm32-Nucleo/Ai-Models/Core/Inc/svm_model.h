/**
  ******************************************************************************
  * @file           : svm_model.h
  * @brief          : Interface do modelo de Machine Learning - SVM (Support Vector Machine)
  *                    - ajustado via Controlador Fuzzy para decidir
  *                    ligar ou nao o GPRS da coleira de monitoramento de
  *                    animais silvestres, com base na carga da bateria e
  *                    na taxa de acerto no envio de dados via
  *                    GPRS/satelite.
  *
  * Portado via m2cgen do modelo treinado em Python (sklearn) para C puro.
  ******************************************************************************
  */
#ifndef SVM_MODEL_H
#define SVM_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Numero de features de entrada do modelo */
#define SVM_MODEL_NUM_INPUTS  2

/* Limite de decisao: probabilidade a partir da qual o GPRS e ligado */
#define SVM_MODEL_THRESHOLD 0.5

/**
 * @brief  Executa o modelo (portado via m2cgen do modelo treinado em
 *         Python/sklearn) e retorna o valor da funcao de decisao do SVM (distancia com sinal
 *         ate o hiperplano separador).
 * @param  input: vetor com SVM_MODEL_NUM_INPUTS valores de entrada, na
 *                mesma ordem/escala usada no treinamento do modelo.
 * @retval valor bruto devolvido pelo modelo (nao e uma probabilidade)
 */
double SvmModel_Score(double *input);

/**
 * @brief  Converte a margem da funcao de decisao em pseudo-
 *         probabilidade aplicando a sigmoide (Platt scaling
 *         simplificado): 1 / (1 + exp(-decisao)).
 * @param  input: vetor com SVM_MODEL_NUM_INPUTS valores de entrada
 * @retval probabilidade, no intervalo [0, 1], de o GPRS ser ligado
 */
double SvmModel_Probabilidade(double *input);

/**
 * @brief  Wrapper de conveniencia: executa o modelo e retorna diretamente
 *         a decisao binaria, comparando a probabilidade com
 *         SVM_MODEL_THRESHOLD.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario
 */
int SvmModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* SVM_MODEL_H */
