/* Minimal serial logger using ITM(SWO) when available. No dependency on HAL UART.
 * SerialLogger_Init is a no-op here; pass NULL. If you want UART output,
 * implement a transmit hook and re-enable UART HAL sources in the project.
 */

#include "serial_logger.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

/* Avoid including the full HAL headers here to prevent heavy dependency
 * and typedef visibility issues during build. We treat the HAL UART handle
 * as an opaque pointer and declare the transmit prototype we need. */
static void *g_huart = NULL;

/* Minimal prototype to call HAL UART transmit without pulling in HAL types. */
int HAL_UART_Transmit(void *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void SerialLogger_Init(void *huart_handle)
{
    g_huart = huart_handle;
}

void SerialLogger_Puts(const char *s)
{
    if (s == NULL) return;
    if (g_huart != NULL)
    {
        HAL_UART_Transmit(g_huart, (uint8_t*)s, (uint16_t)strlen(s), 100);
        return;
    }
#if defined(ITM_SendChar)
    const char *p = s;
    while (*p) { ITM_SendChar((int)*p++); }
#else
    (void)s; /* no-op when ITM not available */
#endif
}

void SerialLogger_Printf(const char *fmt, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n <= 0) return;
    if ((size_t)n >= sizeof(buf)) n = sizeof(buf)-1;
    buf[n] = '\0';
    SerialLogger_Puts(buf);
}
