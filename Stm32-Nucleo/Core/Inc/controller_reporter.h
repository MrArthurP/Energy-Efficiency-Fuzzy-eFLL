#ifndef CONTROLLER_REPORTER_H
#define CONTROLLER_REPORTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    CTRL_TYPE_FUZZY = 0,
    CTRL_TYPE_LOGISTIC,
    CTRL_TYPE_DECISION_TREE,
    CTRL_TYPE_OTHER,
} ControllerType;

/* Inicializa e reporta (chamada de alto nível). Retorna 0 em sucesso, -1 em falha. */
int ControllerReporter_InitController(ControllerType type);

/* Relata criação sem inicializar (útil se a criação já foi executada) */
void ControllerReporter_ReportCreation(ControllerType type, int success);

#ifdef __cplusplus
}
#endif

#endif /* CONTROLLER_REPORTER_H */
