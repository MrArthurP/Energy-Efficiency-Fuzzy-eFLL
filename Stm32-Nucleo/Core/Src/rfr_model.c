#include <string.h>
void add_vectors(double *v1, double *v2, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}
void mul_vector_number(double *v1, double num, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}
void predict_rfr_model(double * input, double * output) {
    double var0[2];
    double var1[2];
    double var2[2];
    double var3[2];
    double var4[2];
    double var5[2];
    double var6[2];
    double var7[2];
    double var8[2];
    double var9[2];
    double var10[2];
    double var11[2];
    double var12[2];
    double var13[2];
    double var14[2];
    double var15[2];
    double var16[2];
    double var17[2];
    double var18[2];
    double var19[2];
    double var20[2];
    double var21[2];
    double var22[2];
    double var23[2];
    double var24[2];
    if (input[1] <= 33.5) {
        memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
    } else {
        if (input[0] <= 28.260000228881836) {
            if (input[1] <= 62.5) {
                if (input[1] <= 46.5) {
                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 49.5) {
                        memcpy(var24, (double[]){0.3333333333333333, 0.6666666666666666}, 2 * sizeof(double));
                    } else {
                        memcpy(var24, (double[]){0.8235294117647058, 0.17647058823529413}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 12.880000114440918) {
                    if (input[0] <= 11.539999961853027) {
                        memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[1] <= 36.5) {
                if (input[0] <= 54.01499938964844) {
                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    double var25[2];
    if (input[0] <= 20.734999656677246) {
        if (input[0] <= 19.5649995803833) {
            if (input[0] <= 18.559999465942383) {
                if (input[0] <= 15.884999752044678) {
                    if (input[1] <= 69.0) {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 67.0) {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
        }
    } else {
        if (input[0] <= 95.1500015258789) {
            if (input[1] <= 36.5) {
                if (input[0] <= 66.88999938964844) {
                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 32.0) {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 28.260000228881836) {
                    if (input[0] <= 27.59000015258789) {
                        memcpy(var25, (double[]){0.05263157894736842, 0.9473684210526315}, 2 * sizeof(double));
                    } else {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 47.32500076293945) {
                        memcpy(var25, (double[]){0.037037037037037035, 0.9629629629629629}, 2 * sizeof(double));
                    } else {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 96.48999786376953) {
                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[1] <= 33.5) {
                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var24, var25, 2, var23);
    double var26[2];
    if (input[0] <= 63.71500015258789) {
        if (input[1] <= 46.5) {
            if (input[1] <= 36.5) {
                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[1] <= 39.5) {
                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[0] <= 38.624999046325684) {
                        memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[1] <= 65.5) {
                if (input[0] <= 19.229999542236328) {
                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[0] <= 98.83000183105469) {
            if (input[0] <= 89.46500015258789) {
                if (input[0] <= 77.09000015258789) {
                    if (input[1] <= 27.0) {
                        memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 30.0) {
                        memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[1] <= 27.0) {
                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
        }
    }
    add_vectors(var23, var26, 2, var22);
    double var27[2];
    if (input[0] <= 50.834999084472656) {
        if (input[0] <= 35.78499984741211) {
            if (input[1] <= 52.5) {
                if (input[1] <= 46.5) {
                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[0] <= 21.40499973297119) {
                        memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 18.059999465942383) {
                    if (input[1] <= 70.5) {
                        memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[1] <= 41.5) {
                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[0] <= 54.35000038146973) {
            memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
        } else {
            if (input[1] <= 33.5) {
                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var22, var27, 2, var21);
    double var28[2];
    if (input[1] <= 36.5) {
        if (input[1] <= 33.5) {
            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[0] <= 54.01499938964844) {
                memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[1] <= 62.5) {
            if (input[1] <= 52.5) {
                if (input[1] <= 46.5) {
                    if (input[0] <= 40.1299991607666) {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 56.0) {
                    if (input[0] <= 23.40999984741211) {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 21.90499973297119) {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
        }
    }
    add_vectors(var21, var28, 2, var20);
    double var29[2];
    if (input[1] <= 33.5) {
        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
    } else {
        if (input[0] <= 28.260000228881836) {
            if (input[1] <= 62.5) {
                if (input[1] <= 59.5) {
                    if (input[1] <= 49.5) {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var29, (double[]){0.9230769230769231, 0.07692307692307693}, 2 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 13.544999957084656) {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[0] <= 47.4950008392334) {
                if (input[0] <= 46.989999771118164) {
                    memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var20, var29, 2, var19);
    double var30[2];
    if (input[0] <= 17.559999465942383) {
        if (input[0] <= 6.855000019073486) {
            if (input[0] <= 1.8399999737739563) {
                memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[1] <= 64.0) {
                    memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var30, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[0] <= 13.375) {
                if (input[0] <= 10.869999885559082) {
                    memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[0] <= 11.704999923706055) {
                        memcpy(var30, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[1] <= 67.0) {
                    memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var30, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[1] <= 33.5) {
            memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[1] <= 36.5) {
                if (input[0] <= 47.48999881744385) {
                    memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var30, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 62.5) {
                    if (input[0] <= 21.90499973297119) {
                        memcpy(var30, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var30, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var30, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var19, var30, 2, var18);
    double var31[2];
    if (input[0] <= 63.71500015258789) {
        if (input[1] <= 56.0) {
            if (input[0] <= 29.9350004196167) {
                memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[1] <= 40.0) {
                    memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 46.5) {
                        memcpy(var31, (double[]){0.5, 0.5}, 2 * sizeof(double));
                    } else {
                        memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 4.849999904632568) {
                if (input[0] <= 2.8450000286102295) {
                    memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 62.5) {
                    if (input[0] <= 21.90499973297119) {
                        memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[0] <= 77.09000015258789) {
            if (input[1] <= 33.5) {
                memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[0] <= 84.95000076293945) {
                if (input[0] <= 80.0999984741211) {
                    if (input[0] <= 77.7599983215332) {
                        memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var31, (double[]){0.14285714285714285, 0.8571428571428571}, 2 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 56.0) {
                        memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 98.49499893188477) {
                    if (input[1] <= 30.0) {
                        memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 99.5) {
                        memcpy(var31, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var31, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var18, var31, 2, var17);
    double var32[2];
    if (input[0] <= 50.834999084472656) {
        if (input[0] <= 4.009999871253967) {
            memcpy(var32, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[0] <= 41.30500030517578) {
                if (input[1] <= 46.5) {
                    memcpy(var32, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[0] <= 20.0649995803833) {
                        memcpy(var32, (double[]){0.16666666666666666, 0.8333333333333334}, 2 * sizeof(double));
                    } else {
                        memcpy(var32, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[1] <= 51.5) {
                    memcpy(var32, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var32, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[0] <= 54.35000038146973) {
            if (input[1] <= 22.5) {
                memcpy(var32, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var32, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[1] <= 33.5) {
                memcpy(var32, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var32, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var17, var32, 2, var16);
    double var33[2];
    if (input[1] <= 36.5) {
        if (input[0] <= 67.72999954223633) {
            memcpy(var33, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[1] <= 32.0) {
                memcpy(var33, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var33, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[1] <= 56.0) {
            if (input[1] <= 39.5) {
                memcpy(var33, (double[]){0.0, 1.0}, 2 * sizeof(double));
            } else {
                if (input[0] <= 28.760000228881836) {
                    if (input[0] <= 25.585000038146973) {
                        memcpy(var33, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var33, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 46.5) {
                        memcpy(var33, (double[]){0.16666666666666666, 0.8333333333333334}, 2 * sizeof(double));
                    } else {
                        memcpy(var33, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[1] <= 62.5) {
                if (input[0] <= 13.544999957084656) {
                    memcpy(var33, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var33, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var33, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var16, var33, 2, var15);
    double var34[2];
    if (input[0] <= 18.559999465942383) {
        if (input[1] <= 65.5) {
            memcpy(var34, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            memcpy(var34, (double[]){0.0, 1.0}, 2 * sizeof(double));
        }
    } else {
        if (input[1] <= 33.5) {
            memcpy(var34, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[1] <= 36.5) {
                if (input[0] <= 54.01499938964844) {
                    memcpy(var34, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var34, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[0] <= 28.260000228881836) {
                    if (input[1] <= 62.5) {
                        memcpy(var34, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var34, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var34, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var15, var34, 2, var14);
    double var35[2];
    if (input[1] <= 33.5) {
        memcpy(var35, (double[]){1.0, 0.0}, 2 * sizeof(double));
    } else {
        if (input[0] <= 18.22499942779541) {
            if (input[1] <= 67.0) {
                memcpy(var35, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var35, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[0] <= 47.32500076293945) {
                if (input[0] <= 46.82500076293945) {
                    if (input[1] <= 44.5) {
                        memcpy(var35, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var35, (double[]){0.024390243902439025, 0.975609756097561}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var35, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var35, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var14, var35, 2, var13);
    double var36[2];
    if (input[0] <= 68.06000137329102) {
        if (input[1] <= 46.5) {
            if (input[1] <= 36.5) {
                memcpy(var36, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[1] <= 39.5) {
                    memcpy(var36, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 43.0) {
                        memcpy(var36, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var36, (double[]){0.625, 0.375}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 18.559999465942383) {
                if (input[1] <= 67.0) {
                    memcpy(var36, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var36, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 62.5) {
                    if (input[1] <= 59.5) {
                        memcpy(var36, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var36, (double[]){0.5, 0.5}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var36, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[1] <= 28.5) {
            memcpy(var36, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            memcpy(var36, (double[]){0.0, 1.0}, 2 * sizeof(double));
        }
    }
    add_vectors(var13, var36, 2, var12);
    double var37[2];
    if (input[0] <= 64.05000114440918) {
        if (input[1] <= 46.5) {
            if (input[1] <= 40.0) {
                memcpy(var37, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[0] <= 49.66499900817871) {
                    memcpy(var37, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var37, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[1] <= 56.0) {
                if (input[0] <= 19.899999618530273) {
                    memcpy(var37, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var37, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[0] <= 3.84499990940094) {
                    if (input[1] <= 70.5) {
                        memcpy(var37, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var37, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var37, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[0] <= 69.9000015258789) {
            if (input[0] <= 67.05999755859375) {
                if (input[1] <= 40.0) {
                    memcpy(var37, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var37, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var37, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[1] <= 28.5) {
                memcpy(var37, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var37, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var12, var37, 2, var11);
    double var38[2];
    if (input[1] <= 33.5) {
        memcpy(var38, (double[]){1.0, 0.0}, 2 * sizeof(double));
    } else {
        if (input[1] <= 56.0) {
            if (input[1] <= 39.5) {
                if (input[1] <= 36.5) {
                    if (input[0] <= 47.48999881744385) {
                        memcpy(var38, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var38, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 46.5) {
                    if (input[0] <= 49.66499900817871) {
                        memcpy(var38, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 20.399999618530273) {
                        memcpy(var38, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 20.0649995803833) {
                if (input[1] <= 67.0) {
                    memcpy(var38, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var38, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var38, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var11, var38, 2, var10);
    double var39[2];
    if (input[0] <= 17.22499942779541) {
        if (input[0] <= 6.855000019073486) {
            if (input[0] <= 2.3450000286102295) {
                memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[1] <= 67.5) {
                    memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var39, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[0] <= 13.545000076293945) {
                if (input[1] <= 67.0) {
                    memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var39, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 64.0) {
                    memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var39, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[0] <= 98.83000183105469) {
            if (input[1] <= 36.5) {
                if (input[0] <= 97.15500259399414) {
                    if (input[0] <= 68.39500045776367) {
                        memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){0.9696969696969697, 0.030303030303030304}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var39, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[0] <= 20.0649995803833) {
                    if (input[0] <= 19.229999542236328) {
                        memcpy(var39, (double[]){0.14285714285714285, 0.8571428571428571}, 2 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 26.755000114440918) {
                        memcpy(var39, (double[]){0.08333333333333333, 0.9166666666666666}, 2 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 99.5) {
                memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[1] <= 48.0) {
                    memcpy(var39, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var39, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var10, var39, 2, var9);
    double var40[2];
    if (input[1] <= 33.5) {
        memcpy(var40, (double[]){1.0, 0.0}, 2 * sizeof(double));
    } else {
        if (input[1] <= 62.5) {
            if (input[1] <= 36.5) {
                if (input[0] <= 54.01499938964844) {
                    memcpy(var40, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var40, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 39.5) {
                    memcpy(var40, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[0] <= 20.399999618530273) {
                        memcpy(var40, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var40, (double[]){0.06896551724137931, 0.9310344827586207}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 12.710000038146973) {
                if (input[0] <= 11.539999961853027) {
                    memcpy(var40, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    memcpy(var40, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var40, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var9, var40, 2, var8);
    double var41[2];
    if (input[0] <= 13.545000076293945) {
        if (input[0] <= 6.855000019073486) {
            if (input[0] <= 5.184999942779541) {
                if (input[1] <= 70.5) {
                    memcpy(var41, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[1] <= 70.5) {
                memcpy(var41, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[0] <= 66.88999938964844) {
            if (input[1] <= 36.5) {
                memcpy(var41, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[0] <= 20.0649995803833) {
                    if (input[1] <= 69.0) {
                        memcpy(var41, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[0] <= 80.0999984741211) {
                if (input[0] <= 70.4000015258789) {
                    memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 27.0) {
                        memcpy(var41, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[1] <= 28.5) {
                    memcpy(var41, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var41, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var8, var41, 2, var7);
    double var42[2];
    if (input[0] <= 71.73999786376953) {
        if (input[0] <= 13.545000076293945) {
            if (input[1] <= 65.5) {
                memcpy(var42, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var42, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[0] <= 14.550000190734863) {
                memcpy(var42, (double[]){0.0, 1.0}, 2 * sizeof(double));
            } else {
                if (input[0] <= 18.394999504089355) {
                    if (input[0] <= 16.389999866485596) {
                        memcpy(var42, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                    } else {
                        memcpy(var42, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 46.5) {
                        memcpy(var42, (double[]){0.9518072289156626, 0.04819277108433735}, 2 * sizeof(double));
                    } else {
                        memcpy(var42, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[1] <= 33.5) {
            memcpy(var42, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            memcpy(var42, (double[]){0.0, 1.0}, 2 * sizeof(double));
        }
    }
    add_vectors(var7, var42, 2, var6);
    double var43[2];
    if (input[0] <= 20.234999656677246) {
        if (input[0] <= 7.359999895095825) {
            if (input[1] <= 67.5) {
                memcpy(var43, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var43, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[0] <= 13.545000076293945) {
                if (input[1] <= 69.0) {
                    memcpy(var43, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var43, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[0] <= 14.71500015258789) {
                    memcpy(var43, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 67.0) {
                        memcpy(var43, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[0] <= 65.04999923706055) {
            if (input[0] <= 24.244999885559082) {
                if (input[1] <= 32.0) {
                    memcpy(var43, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var43, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 46.5) {
                    if (input[0] <= 51.5049991607666) {
                        memcpy(var43, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.8888888888888888, 0.1111111111111111}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var43, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[1] <= 33.5) {
                memcpy(var43, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                memcpy(var43, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var6, var43, 2, var5);
    double var44[2];
    if (input[1] <= 36.5) {
        if (input[0] <= 66.88999938964844) {
            memcpy(var44, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[0] <= 68.89500045776367) {
                memcpy(var44, (double[]){0.0, 1.0}, 2 * sizeof(double));
            } else {
                memcpy(var44, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[0] <= 15.550000190734863) {
            if (input[0] <= 8.52999997138977) {
                if (input[0] <= 4.849999904632568) {
                    if (input[0] <= 2.8450000286102295) {
                        memcpy(var44, (double[]){0.2, 0.8}, 2 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){0.8, 0.2}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var44, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[0] <= 13.210000038146973) {
                    memcpy(var44, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[0] <= 14.71500015258789) {
                        memcpy(var44, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 28.760000228881836) {
                if (input[1] <= 44.5) {
                    memcpy(var44, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 62.5) {
                        memcpy(var44, (double[]){0.25, 0.75}, 2 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 47.4950008392334) {
                    if (input[0] <= 46.82500076293945) {
                        memcpy(var44, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var44, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var5, var44, 2, var4);
    double var45[2];
    if (input[1] <= 36.5) {
        if (input[0] <= 67.72999954223633) {
            memcpy(var45, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[0] <= 69.56500244140625) {
                memcpy(var45, (double[]){0.0, 1.0}, 2 * sizeof(double));
            } else {
                memcpy(var45, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[1] <= 62.5) {
            if (input[1] <= 49.5) {
                if (input[1] <= 46.5) {
                    if (input[1] <= 39.5) {
                        memcpy(var45, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){0.2857142857142857, 0.7142857142857143}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var45, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[1] <= 56.0) {
                    if (input[0] <= 19.899999618530273) {
                        memcpy(var45, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 59.5) {
                        memcpy(var45, (double[]){0.18181818181818182, 0.8181818181818182}, 2 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){0.14285714285714285, 0.8571428571428571}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 12.710000038146973) {
                if (input[0] <= 9.535000085830688) {
                    memcpy(var45, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    memcpy(var45, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var45, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var4, var45, 2, var3);
    double var46[2];
    if (input[0] <= 13.545000076293945) {
        if (input[1] <= 67.0) {
            memcpy(var46, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            memcpy(var46, (double[]){0.0, 1.0}, 2 * sizeof(double));
        }
    } else {
        if (input[0] <= 14.71500015258789) {
            memcpy(var46, (double[]){0.0, 1.0}, 2 * sizeof(double));
        } else {
            if (input[0] <= 18.72499942779541) {
                if (input[1] <= 67.0) {
                    memcpy(var46, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var46, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                if (input[0] <= 20.06999969482422) {
                    memcpy(var46, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 36.5) {
                        memcpy(var46, (double[]){0.9629629629629629, 0.037037037037037035}, 2 * sizeof(double));
                    } else {
                        memcpy(var46, (double[]){0.008264462809917356, 0.9917355371900827}, 2 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var3, var46, 2, var2);
    double var47[2];
    if (input[1] <= 33.5) {
        memcpy(var47, (double[]){1.0, 0.0}, 2 * sizeof(double));
    } else {
        if (input[1] <= 56.0) {
            if (input[1] <= 52.5) {
                if (input[0] <= 29.09500026702881) {
                    if (input[0] <= 18.730000019073486) {
                        memcpy(var47, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var47, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 36.5) {
                        memcpy(var47, (double[]){0.3333333333333333, 0.6666666666666666}, 2 * sizeof(double));
                    } else {
                        memcpy(var47, (double[]){0.029411764705882353, 0.9705882352941176}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 23.90999984741211) {
                    memcpy(var47, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var47, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[0] <= 20.0649995803833) {
                if (input[1] <= 67.0) {
                    memcpy(var47, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    memcpy(var47, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var47, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var2, var47, 2, var1);
    mul_vector_number(var1, 0.041666666666666664, 2, var0);
    memcpy(output, var0, 2 * sizeof(double));
}
