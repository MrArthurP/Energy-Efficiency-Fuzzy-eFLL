/**
  ******************************************************************************
  * @file           : fuzzy-controller.h
  * @brief          : Interface do controlador fuzzy (decisão de ligar o GPRS
  *                    com base na carga da bateria e na taxa de acerto do sinal)
  ******************************************************************************
  */
#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Identificadores das entradas/saída do sistema fuzzy (usados internamente,
 * expostos aqui apenas para referência/depuração) */
#define FUZZY_INPUT_BATERIA   1
#define FUZZY_INPUT_TAXA      2
#define FUZZY_OUTPUT_GPRS     1

/* Limiar (threshold) usado para converter a saída defuzzificada (0-100)
 * em uma decisão binária de ligar ou não o GPRS */
#define FUZZY_GPRS_THRESHOLD  50.0f

/**
 * @brief  Cria os conjuntos fuzzy, as entradas/saída e todas as 20 regras
 *         do controlador. Deve ser chamada uma única vez, antes de qualquer
 *         chamada a FuzzyController_Run().
 * @retval 0 em caso de sucesso, -1 em caso de falha de alocação
 */
int FuzzyController_Init(void);

/**
 * @brief  Normaliza um valor de RSSI (0-31) para a escala 0-100 usada
 *         pelos conjuntos fuzzy de taxa de acerto do GPRS.
 */
float FuzzyController_NormalizaTaxa(float rssi);

/**
 * @brief  Executa uma iteração do controlador fuzzy.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto já normalizada (0-100),
 *         ver FuzzyController_NormalizaTaxa()
 * @retval Valor defuzzificado (0-100), onde valores mais altos indicam
 *         maior pertinência à decisão de ligar o GPRS
 */
float FuzzyController_Run(float bateria_pct, float taxa_normalizada);

/**
 * @brief  Converte a saída defuzzificada em decisão binária, usando
 *         FUZZY_GPRS_THRESHOLD como limiar.
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrário
 */
int FuzzyController_DecideLigarGPRS(float saida_defuzzificada);

/**
 * @brief  Libera toda a memória alocada pelo controlador (conjuntos,
 *         entradas, saída, regras e a instância Fuzzy).
 *         Normalmente não é necessária em um firmware embarcado que roda
 *         para sempre, mas é útil para testes/reinicialização.
 */
void FuzzyController_Deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* FUZZY_CONTROLLER_H */