#include "controller_reporter.h"
#include "serial_logger.h"
#include "memory_monitor.h"

#include "fuzzy-controller.h"
#include "reg_model.h"
#include "dec_model.h"
#include "svm_model.h"
#include "lgb_model.h"
#include "rfr_model.h"
#include "qda_model_wrapper.h"
#include "nnw_model_wrapper.h"
#include "nba_model_wrapper.h"

static const char *ControllerReporter_GetName(ControllerType type)
{
    switch (type)
    {
        case CTRL_TYPE_FUZZY: return "Fuzzy";
        case CTRL_TYPE_LOGISTIC: return "LogisticRegression";
        case CTRL_TYPE_DECISION_TREE: return "DecisionTree";
        case CTRL_TYPE_SVM: return "SVM";
        case CTRL_TYPE_LGB: return "Lgb";
        case CTRL_TYPE_RFR: return "Rfr";
        case CTRL_TYPE_QDA: return "Qda";
        case CTRL_TYPE_NNW: return "Nnw";
        case CTRL_TYPE_NBA: return "Nba";
        default: return "OtherController";
    }
}

int ControllerReporter_InitController(ControllerType type)
{
    int ret = -1;
    ControllerType report_type = type;
    size_t heap_before = MemoryMonitor_GetHeapUsage();

    switch (type)
    {
        case CTRL_TYPE_FUZZY:
            ret = FuzzyController_Init();
            break;
            
        case CTRL_TYPE_LOGISTIC:
        {
            /* reg_modelo é stateless (apenas combinação linear + sigmoide):
             * não há o que alocar/montar como no fuzzy. "Inicializar" aqui
             * significa validar que o modelo responde corretamente a uma
             * entrada de referência antes de liberá-lo para uso. */
            double input[REGMODEL_NUM_INPUTS] = {50.0, 50.0};
            double prob = RegModel_Probabilidade(input);

            ret = (prob >= 0.0 && prob <= 1.0) ? 0 : -1;
            break;
        }

        case CTRL_TYPE_SVM:
        {
            /* svm_modelo é stateless (apenas combinação de funções exponenciais):
             * não há o que alocar/montar como no fuzzy. "Inicializar" aqui
             * significa validar que o modelo responde corretamente a uma
             * entrada de referência antes de liberá-lo para uso. */
            double input[SVM_MODEL_NUM_INPUTS] = {50.0, 50.0};
            double prob = SvmModel_Probabilidade(input);

            ret = (prob >= 0.0 && prob <= 1.0) ? 0 : -1;
            break;
        }

        case CTRL_TYPE_DECISION_TREE:
        {
            /* dec_modelo também é stateless: validamos que as duas
             * probabilidades de classe somam ~1.0 para uma entrada de
             * referência. */
            double input[DECMODEL_NUM_INPUTS] = {50.0, 50.0};
            double output[DECMODEL_NUM_OUTPUTS];
            double soma;

            DecModel_Score(input, output);
            soma = output[DECMODEL_CLASSE_NAO_LIGA] + output[DECMODEL_CLASSE_LIGA];

            ret = (soma > 0.99 && soma < 1.01) ? 0 : -1;
            break;
        }
        case CTRL_TYPE_LGB:
        {
            /* lgb_model tambem e stateless: validamos que as
             * probabilidades de classe somam ~1.0 para uma entrada de
             * referencia. */
            double input[LGB_MODEL_NUM_INPUTS] = {50.0, 50.0};
            double output[LGB_MODEL_NUM_OUTPUTS];
            double soma;

            LgbModel_Score(input, output);
            soma = output[LGB_MODEL_CLASSE_NAO_LIGA] + output[LGB_MODEL_CLASSE_LIGA];

            ret = (soma > 0.99 && soma < 1.01) ? 0 : -1;
            break;
        }
        case CTRL_TYPE_RFR:
        {
            /* rfr_model tambem e stateless: validamos que as
             * probabilidades de classe somam ~1.0 para uma entrada de
             * referencia. */
            double input[RFR_MODEL_NUM_INPUTS] = {50.0, 50.0};
            double output[RFR_MODEL_NUM_OUTPUTS];
            double soma;

            RfrModel_Score(input, output);
            soma = output[RFR_MODEL_CLASSE_NAO_LIGA] + output[RFR_MODEL_CLASSE_LIGA];

            ret = (soma > 0.99 && soma < 1.01) ? 0 : -1;
            break;
        }
        case CTRL_TYPE_QDA:
        {
            /* qda_model_wrapper.h e uma rede X-CUBE-AI (bem mais pesada que os
             * modelos do m2cgen): e stateful, entao "Inicializar" aqui aloca
             * os buffers de ativacao e monta a rede antes do primeiro uso. */
            ret = QdaModel_Init();
            break;
        }
        case CTRL_TYPE_NNW:
        {
            /* nnw_model_wrapper.h e uma rede X-CUBE-AI (bem mais pesada que os
             * modelos do m2cgen): e stateful, entao "Inicializar" aqui aloca
             * os buffers de ativacao e monta a rede antes do primeiro uso. */
            ret = NnwModel_Init();
            break;
        }
        case CTRL_TYPE_NBA:
        {
            /* nba_model_wrapper.h e uma rede X-CUBE-AI (bem mais pesada que os
             * modelos do m2cgen): e stateful, entao "Inicializar" aqui aloca
             * os buffers de ativacao e monta a rede antes do primeiro uso. */
            ret = NbaModel_Init();
            break;
        }
        default:
            report_type = CTRL_TYPE_OTHER;
            ret = -1;
            break;
    }

    {
        size_t heap_after = MemoryMonitor_GetHeapUsage();
        size_t heap_growth = (heap_after >= heap_before) ? (heap_after - heap_before) : 0U;

        SerialLogger_Printf("[Memory] %s: heap growth %lu B, heap reserved %lu B\r\n",
                            ControllerReporter_GetName(report_type),
                            (unsigned long)heap_growth,
                            (unsigned long)heap_after);
    }
    ControllerReporter_ReportCreation(report_type, (ret == 0));

    return ret;
}

void ControllerReporter_ReportCreation(ControllerType type, int success)
{
    const char *name = ControllerReporter_GetName(type);

    if (success)
    {
        SerialLogger_Printf("[Controller] %s created successfully\r\n", name);
    }
    else
    {
        SerialLogger_Printf("[Controller] %s creation FAILED\r\n", name);
    }
}