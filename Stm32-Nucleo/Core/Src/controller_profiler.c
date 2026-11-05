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
#include "memory_monitor.h"

#include <stddef.h>

#define STACK_WATERMARK_PATTERN 0xA55A3CC3UL

static __attribute__((always_inline)) inline uintptr_t ControllerProfiler_GetMsp(void)
{
    uintptr_t stack_pointer;
    __asm volatile ("mrs %0, msp" : "=r" (stack_pointer));
    return stack_pointer;
}

static __attribute__((always_inline)) inline uintptr_t ControllerProfiler_PrepareStackMeasurement(void)
{
    extern uint8_t _sstack;
    uintptr_t stack_floor = (uintptr_t)&_sstack;
    uintptr_t stack_pointer = ControllerProfiler_GetMsp();
    volatile uint32_t *word = (volatile uint32_t *)stack_floor;
    uintptr_t fill_end = stack_pointer & ~(uintptr_t)(sizeof(uint32_t) - 1U);

    if (fill_end <= stack_floor)
    {
        return 0U;
    }

    while ((uintptr_t)word < fill_end)
    {
        *word++ = STACK_WATERMARK_PATTERN;
    }

    return (fill_end >= stack_floor) ? fill_end : 0U;
}

static __attribute__((always_inline)) inline size_t ControllerProfiler_GetStackPeak(
    uintptr_t fill_end,
    int *limit_reached)
{
    extern uint8_t _estack;
    extern uint8_t _sstack;
    uintptr_t stack_floor = (uintptr_t)&_sstack;
    uintptr_t stack_top = (uintptr_t)&_estack;
    volatile const uint32_t *word = (volatile const uint32_t *)stack_floor;

    *limit_reached = (fill_end <= stack_floor);
    if (fill_end == 0U)
    {
        return (size_t)(stack_top - stack_floor);
    }

    while ((uintptr_t)word < fill_end && *word == STACK_WATERMARK_PATTERN)
    {
        word++;
    }

    if ((uintptr_t)word >= fill_end)
    {
        return (size_t)(stack_top - fill_end);
    }

    if ((uintptr_t)word == stack_floor)
    {
        *limit_reached = 1;
    }

    return (size_t)(stack_top - (uintptr_t)word);
}

void ControllerProfiler_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static __attribute__((always_inline)) inline uint32_t ControllerProfiler_GetCycles(void)
{
    return DWT->CYCCNT;
}

ControllerProfilerResult ControllerProfiler_Medir(ControllerDecideFn fn,
                                                   float bateria_pct,
                                                   float taxa_normalizada,
                                                   int index)
{
    ControllerProfilerResult resultado;
    uint32_t inicio, fim;
    size_t heap_before, heap_after;
    uintptr_t stack_fill_end;

    resultado.index = index;
    heap_before = MemoryMonitor_GetHeapUsage();
    stack_fill_end = ControllerProfiler_PrepareStackMeasurement();
    inicio = ControllerProfiler_GetCycles();
    resultado.decisao = fn(bateria_pct, taxa_normalizada);
    fim = ControllerProfiler_GetCycles();
    resultado.stack_peak_bytes = ControllerProfiler_GetStackPeak(
        stack_fill_end,
        &resultado.stack_limit_reached);
    heap_after = MemoryMonitor_GetHeapUsage();
    resultado.heap_growth_bytes = (ptrdiff_t)heap_after - (ptrdiff_t)heap_before;
    resultado.heap_reserved_bytes = heap_after;

    /* Subtração em aritmética unsigned: correta mesmo se DWT->CYCCNT
     * tiver dado overflow entre "inicio" e "fim". */
    resultado.ciclos = fim - inicio;
    resultado.tempo_us = (float)resultado.ciclos / ((float)SystemCoreClock / 1000000.0f);

    return resultado;
}

ControllerProfilerResult ControllerProfiler_MedirEReportar(const char *nome,
                                                             ControllerDecideFn fn,
                                                             float bateria_pct,
                                                             float taxa_normalizada,
                                                             int index)
{
    ControllerProfilerResult resultado = ControllerProfiler_Medir(fn, bateria_pct, taxa_normalizada, index);

    SerialLogger_Printf("[%d], %s, %lu ciclos, %lu us, %d decisao, "
                        "stack peak %lu B%s, heap growth %ld B, heap reserved %lu B\r\n",
                         index,
                         nome,
                         (unsigned long)resultado.ciclos,
                         (unsigned long)resultado.tempo_us,
                         resultado.decisao,
                         (unsigned long)resultado.stack_peak_bytes,
                         resultado.stack_limit_reached ? " (limite atingido)" : "",
                         (long)resultado.heap_growth_bytes,
                         (unsigned long)resultado.heap_reserved_bytes);

    return resultado;
}
