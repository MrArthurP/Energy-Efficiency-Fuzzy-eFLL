#include "controller_reporter.h"
#include "serial_logger.h"

#include "fuzzy-controller.h"
#include "reg_model.h"
#include "dec_model.h"
#include "svm_model.h"
#include "lgb_model.h"
#include "rfr_model.h"
#include "qda_model_wrapper.h"
#include "nnw_model_wrapper.h"
#include "nba_model_wrapper.h"

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
            double input[REGMODEL_NUM_INPUTS] = {50.0, 50.0};
            double prob = RegModel_Probabilidade(input);

            ret = (prob >= 0.0 && prob <= 1.0) ? 0 : -1;
            ControllerReporter_ReportCreation(CTRL_TYPE_LOGISTIC, (ret == 0));
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
            ControllerReporter_ReportCreation(CTRL_TYPE_SVM, (ret == 0));
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
            ControllerReporter_ReportCreation(CTRL_TYPE_DECISION_TREE, (ret == 0));
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
            ControllerReporter_ReportCreation(CTRL_TYPE_LGB, (ret == 0));
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
            ControllerReporter_ReportCreation(CTRL_TYPE_RFR, (ret == 0));
            break;
        }
        case CTRL_TYPE_QDA:
        {
            /* qda_model_wrapper.h e uma rede X-CUBE-AI (bem mais pesada que os
             * modelos do m2cgen): e stateful, entao "Inicializar" aqui aloca
             * os buffers de ativacao e monta a rede antes do primeiro uso. */
            ret = QdaModel_Init();
            ControllerReporter_ReportCreation(CTRL_TYPE_QDA, (ret == 0));
            break;
        }
        case CTRL_TYPE_NNW:
        {
            /* nnw_model_wrapper.h e uma rede X-CUBE-AI (bem mais pesada que os
             * modelos do m2cgen): e stateful, entao "Inicializar" aqui aloca
             * os buffers de ativacao e monta a rede antes do primeiro uso. */
            ret = NnwModel_Init();
            ControllerReporter_ReportCreation(CTRL_TYPE_NNW, (ret == 0));
            break;
        }
        case CTRL_TYPE_NBA:
        {
            /* nba_model_wrapper.h e uma rede X-CUBE-AI (bem mais pesada que os
             * modelos do m2cgen): e stateful, entao "Inicializar" aqui aloca
             * os buffers de ativacao e monta a rede antes do primeiro uso. */
            ret = NbaModel_Init();
            ControllerReporter_ReportCreation(CTRL_TYPE_NBA, (ret == 0));
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
        case CTRL_TYPE_LGB: name = "Lgb"; break;
        case CTRL_TYPE_RFR: name = "Rfr"; break;
        case CTRL_TYPE_QDA: name = "Qda"; break;
        case CTRL_TYPE_NNW: name = "Nnw"; break;
        case CTRL_TYPE_NBA: name = "Nba"; break;
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