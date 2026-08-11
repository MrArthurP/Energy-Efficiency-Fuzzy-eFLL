/**
  ******************************************************************************
  * @file           : reg_modelo.c
  * @brief          : Modelo de Machine Learning - Regressão Logística -
  *                    usado para decidir ligar ou não o GPRS da coleira de
  *                    monitoramento de animais silvestres, com base na
  *                    carga da bateria e na taxa de acerto no envio de
  *                    dados via GPRS/satélite.
  *
  * Portado via m2cgen do modelo treinado em Python (sklearn) para C puro,
  * através do wrapper "extern C" em reg_modelo.h.
  ******************************************************************************
  */
 
#include "reg_modelo.h"
#include <math.h>
 
/* Código original gerado pelo m2cgen (NÃO EDITAR manualmente: se o
 * modelo for retreinado, regenere este arquivo). Retorna o logit -
 * a combinação linear das features com os coeficientes aprendidos. */

double RegModelo_Score(double * input) {
    return -21.00895032876039 + input[0] * 0.13537046791506138 + input[1] * 0.3337593276078033;
}


double RegModelo_Probabilidade(double *input)
{
    double logit = RegModelo_Score(input);
    return 1.0 / (1.0 + exp(-logit));
}
 
int RegModelo_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{
    double input[REG_MODELO_NUM_INPUTS];
 
    input[0] = (double)bateria_pct;
    input[1] = (double)taxa_normalizada;
 
    return (RegModelo_Probabilidade(input) >= REG_MODELO_THRESHOLD) ? 1 : 0;
}
