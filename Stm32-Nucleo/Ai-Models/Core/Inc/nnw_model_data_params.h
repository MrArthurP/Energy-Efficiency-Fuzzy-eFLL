/**
  ******************************************************************************
  * @file    nnw_model_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-09-04T21:52:59-0300
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef NNW_MODEL_DATA_PARAMS_H
#define NNW_MODEL_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_NNW_MODEL_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_nnw_model_data_weights_params[1]))
*/

#define AI_NNW_MODEL_DATA_CONFIG               (NULL)


#define AI_NNW_MODEL_DATA_ACTIVATIONS_SIZES \
  { 192, }
#define AI_NNW_MODEL_DATA_ACTIVATIONS_SIZE     (192)
#define AI_NNW_MODEL_DATA_ACTIVATIONS_COUNT    (1)
#define AI_NNW_MODEL_DATA_ACTIVATION_1_SIZE    (192)



#define AI_NNW_MODEL_DATA_WEIGHTS_SIZES \
  { 2804, }
#define AI_NNW_MODEL_DATA_WEIGHTS_SIZE         (2804)
#define AI_NNW_MODEL_DATA_WEIGHTS_COUNT        (1)
#define AI_NNW_MODEL_DATA_WEIGHT_1_SIZE        (2804)



#define AI_NNW_MODEL_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_nnw_model_activations_table[1])

extern ai_handle g_nnw_model_activations_table[1 + 2];



#define AI_NNW_MODEL_DATA_WEIGHTS_TABLE_GET() \
  (&g_nnw_model_weights_table[1])

extern ai_handle g_nnw_model_weights_table[1 + 2];


#endif    /* NNW_MODEL_DATA_PARAMS_H */
