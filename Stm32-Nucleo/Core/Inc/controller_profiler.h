/**
  ******************************************************************************
  * @file           : controller_profiler.h
  * @brief          : Medição de tempo de execução (ciclos de clock, via
  *                    DWT->CYCCNT) dos controladores de decisão do GPRS
  *                    (Fuzzy, Regressão Logística, Árvore de Decisão).
  ******************************************************************************
  */
#ifndef CONTROLLER_PROFILER_H
#define CONTROLLER_PROFILER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Ponteiro de função com a assinatura comum aos wrappers de decisão de
 * cada controlador: FuzModelo_DecideLigarGPRS, RegModelo_DecideLigarGPRS
 * e DecModelo_DecideLigarGPRS. Graças a essa assinatura padronizada, o
 * profiler consegue medir qualquer uma delas com o mesmo código.
 */
typedef int (*ControllerDecideFn)(float bateria_pct, float taxa_normalizada);

/* Resultado de uma medição de execução */
typedef struct {
    uint32_t ciclos;    /* ciclos de clock consumidos pela chamada */
    float    tempo_us;  /* tempo estimado em microssegundos (baseado em SystemCoreClock) */
    int      decisao;   /* valor retornado pela função medida (1 = ligar, 0 = não ligar) */
} ControllerProfilerResult;

/**
 * @brief  Habilita o contador de ciclos DWT->CYCCNT (equivalente ao
 *         DWT_Init original). Deve ser chamada uma única vez, antes de
 *         qualquer medição (ex.: logo após o clock ser configurado, no
 *         início do main).
 */
void ControllerProfiler_Init(void);

/**
 * @brief  Executa `fn(bateria_pct, taxa_normalizada)` medindo o número de
 *         ciclos de clock consumidos, via DWT->CYCCNT (equivalente ao
 *         get_cycles original chamado antes/depois da chamada).
 * @note   DWT->CYCCNT é um contador de 32 bits que dá overflow (~poucos
 *         segundos em clocks de dezenas/centenas de MHz). A subtração
 *         `fim - inicio` em aritmética unsigned trata o overflow
 *         corretamente para uma única chamada, então isso não é problema
 *         aqui — só evite medir intervalos muito longos com esta função.
 */
ControllerProfilerResult ControllerProfiler_Medir(ControllerDecideFn fn,
                                                   float bateria_pct,
                                                   float taxa_normalizada);

/**
 * @brief  Igual a ControllerProfiler_Medir(), mas também imprime o
 *         resultado via SerialLogger_Printf, prefixado por `nome`.
 * @param  nome: rótulo do controlador a ser impresso no log (ex.: "Fuzzy",
 *         "LogisticRegression", "DecisionTree")
 */
ControllerProfilerResult ControllerProfiler_MedirEReportar(const char *nome,
                                                             ControllerDecideFn fn,
                                                             float bateria_pct,
                                                             float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* CONTROLLER_PROFILER_H */
