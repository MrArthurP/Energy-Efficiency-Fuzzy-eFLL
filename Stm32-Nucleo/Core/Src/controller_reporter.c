#include "controller_reporter.h"
#include "serial_logger.h"

#include "fuzzy-controller.h"
#include "reg_modelo.h"
#include "dec_modelo.h"

int ControllerReporter_InitController(ControllerType type)
{
    int ret = -1;
    switch (type)
    {
        case CTRL_TYPE_FUZZY:
            ret = FuzzyController_Init();
            ControllerReporter_ReportCreation(CTRL_TYPE_FUZZY, (ret==0));
            break;
            
        case CTRL_TYPE_LOGISTIC:
        {
            /* reg_modelo é stateless (apenas combinação linear + sigmoide):
             * não há o que alocar/montar como no fuzzy. "Inicializar" aqui
             * significa validar que o modelo responde corretamente a uma
             * entrada de referência antes de liberá-lo para uso. */
            double input[REG_MODELO_NUM_INPUTS] = {50.0, 50.0};
            double prob = RegModelo_Probabilidade(input);

            ret = (prob >= 0.0 && prob <= 1.0) ? 0 : -1;
            ControllerReporter_ReportCreation(CTRL_TYPE_LOGISTIC, (ret == 0));
            break;
        }
        case CTRL_TYPE_DECISION_TREE:
        {
            /* dec_modelo também é stateless: validamos que as duas
             * probabilidades de classe somam ~1.0 para uma entrada de
             * referência. */
            double input[DEC_MODELO_NUM_INPUTS] = {50.0, 50.0};
            double output[DEC_MODELO_NUM_OUTPUTS];
            double soma;

            DecModelo_Score(input, output);
            soma = output[DEC_MODELO_CLASSE_NAO_LIGA] + output[DEC_MODELO_CLASSE_LIGA];

            ret = (soma > 0.99 && soma < 1.01) ? 0 : -1;
            ControllerReporter_ReportCreation(CTRL_TYPE_DECISION_TREE, (ret == 0));
            break;
        }
        default:
            ControllerReporter_ReportCreation(CTRL_TYPE_OTHER, 0);
            ret = -1;
            break;
    }
    return ret;
}

void ControllerReporter_ReportCreation(ControllerType type, int success)
{
    const char *name;
    switch (type)
    {
        case CTRL_TYPE_FUZZY: name = "Fuzzy"; break;
        case CTRL_TYPE_LOGISTIC: name = "LogisticRegression"; break;
        case CTRL_TYPE_DECISION_TREE: name = "DecisionTree"; break;
        default: name = "OtherController"; break;
    }

    if (success)
    {
        SerialLogger_Printf("[Controller] %s created successfully\r\n", name);
    }
    else
    {
        SerialLogger_Printf("[Controller] %s creation FAILED\r\n", name);
    }
}