#include <string.h>
void predict_dec_model(double * input, double * output) {
    double var0[2];
    if (input[1] <= 33.5) {
        memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
    } else {
        if (input[0] <= 18.559999465942383) {
            if (input[1] <= 67.0) {
                memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[1] <= 36.5) {
                if (input[0] <= 54.01499938964844) {
                    memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[0] <= 28.260000228881836) {
                    if (input[1] <= 44.5) {
                        memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[0] <= 20.0649995803833) {
                            if (input[0] <= 19.5649995803833) {
                                memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[1] <= 46.5) {
                        if (input[0] <= 49.66499900817871) {
                            memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    }
    memcpy(output, var0, 2 * sizeof(double));
}
