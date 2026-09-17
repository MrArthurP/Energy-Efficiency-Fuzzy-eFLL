#include "reg_model.h"
#include <math.h>

double RegModel_Score(double * input) {
    return -21.00895032876039 + input[0] * 0.13537046791506138 + input[1] * 0.3337593276078033;
}

double RegModel_Probabilidade(double *input)
{
    double logit = RegModel_Score(input);
    return 1.0 / (1.0 + exp(-logit));
}

int RegModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{
    double input[REGMODEL_NUM_INPUTS];

    input[0] = (double)bateria_pct;
    input[1] = (double)taxa_normalizada;


    return (RegModel_Probabilidade(input) >= REG_MODEL_THRESHOLD) ? 1 : 0;
}
