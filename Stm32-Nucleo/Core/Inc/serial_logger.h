/* Serial logger helper (non-blocking if UART not provided) */
#ifndef SERIAL_LOGGER_H
#define SERIAL_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

/* Opaque initializer: passe ponteiro para UART (NULL aceita). */
void SerialLogger_Init(void *huart_handle);

/* Imprime formatado via UART/ITM se disponível */
void SerialLogger_Printf(const char *fmt, ...);

/* Imprime string simples */
void SerialLogger_Puts(const char *s);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_LOGGER_H */
