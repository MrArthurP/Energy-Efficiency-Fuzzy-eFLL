/**
  ******************************************************************************
  * @file           : controller_profiler.c
  * @brief          : Medição de tempo de execução (ciclos de clock, via
  *                    DWT->CYCCNT) dos controladores de decisão do GPRS.
  ******************************************************************************
  */

#include "controller_profiler.h"
#include "serial_logger.h"
#include "main.h" /* traz stm32f3xx_hal.h -> CMSIS core (CoreDebug, DWT, SystemCoreClock) */

void ControllerProfiler_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t ControllerProfiler_GetCycles(void)
{
    return DWT->CYCCNT;
}

ControllerProfilerResult ControllerProfiler_Medir(ControllerDecideFn fn,
                                                   float bateria_pct,
                                                   float taxa_normalizada)
{
    ControllerProfilerResult resultado;
    uint32_t inicio, fim;

    inicio = ControllerProfiler_GetCycles();
    resultado.decisao = fn(bateria_pct, taxa_normalizada);
    fim = ControllerProfiler_GetCycles();

    /* Subtração em aritmética unsigned: correta mesmo se DWT->CYCCNT
     * tiver dado overflow entre "inicio" e "fim". */
    resultado.ciclos = fim - inicio;
    resultado.tempo_us = (float)resultado.ciclos / ((float)SystemCoreClock / 1000000.0f);

    return resultado;
}

ControllerProfilerResult ControllerProfiler_MedirEReportar(const char *nome,
                                                             ControllerDecideFn fn,
                                                             float bateria_pct,
                                                             float taxa_normalizada)
{
    ControllerProfilerResult resultado = ControllerProfiler_Medir(fn, bateria_pct, taxa_normalizada);

    SerialLogger_Printf("[Profiler] %s: %lu ciclos (%lu us) -> decisao=%d\r\n",
                         nome,
                         (unsigned long)resultado.ciclos,
                         (unsigned long)resultado.tempo_us,
                         resultado.decisao);

    return resultado;
}
