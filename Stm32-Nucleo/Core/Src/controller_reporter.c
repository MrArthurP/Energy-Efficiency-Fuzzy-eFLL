#include "controller_reporter.h"
#include "serial_logger.h"

#include "fuzzy-controller.h"

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
            /* Placeholder: call your logistic init here and report */
            ControllerReporter_ReportCreation(CTRL_TYPE_LOGISTIC, 0);
            ret = -1;
            break;
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
