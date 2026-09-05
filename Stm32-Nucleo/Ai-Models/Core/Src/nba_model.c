/**
  ******************************************************************************
  * @file    nba_model.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-09-04T21:44:29-0300
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "nba_model.h"
#include "nba_model_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_nba_model
 
#undef AI_NBA_MODEL_MODEL_SIGNATURE
#define AI_NBA_MODEL_MODEL_SIGNATURE     "0x7b479d544f16553162c540e3c9306bfa"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-09-04T21:44:29-0300"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NBA_MODEL_N_BATCHES
#define AI_NBA_MODEL_N_BATCHES         (1)

static ai_ptr g_nba_model_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_nba_model_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  float_input_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 2, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  classes_0_cast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  indices1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  cast_indices1_output_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  jll1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  indices_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  cast_indices_output_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  jll_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  summation_jll_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  sum_result_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  reduce_log_sum_exp_result_exp_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  reduce_log_sum_exp_result_reduce_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  reduce_log_sum_exp_result_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  log_prob_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  probabilities_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  output_probability_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  argmax_output_output_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  array_feature_extractor_result_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  label_output_array, AI_ARRAY_FORMAT_S32|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  classes_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 2, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  feature_log_proba1_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 194, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  feature_log_proba_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 202, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  feature_index1_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  feature_index_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  sum_result_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  sum_result_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  output_probability_classlabels_ints_array, AI_ARRAY_FORMAT_U8,
  NULL, NULL, 2, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  argmax_output_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &argmax_output_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  array_feature_extractor_result_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &array_feature_extractor_result_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  cast_indices1_output, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &cast_indices1_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  cast_indices_output, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &cast_indices_output_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  classes, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &classes_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  classes_0_cast_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &classes_0_cast_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  feature_index, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &feature_index_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  feature_index1, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &feature_index1_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  feature_log_proba, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 101, 1, 2), AI_STRIDE_INIT(4, 4, 4, 404, 404),
  1, &feature_log_proba_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  feature_log_proba1, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 97, 1, 2), AI_STRIDE_INIT(4, 4, 4, 388, 388),
  1, &feature_log_proba1_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  float_input_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &float_input_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  indices1_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &indices1_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  indices_output, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &indices_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  jll1_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 2), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &jll1_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  jll1_output0, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &jll1_output_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  jll_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 2), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &jll_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  jll_output0, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &jll_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  label_output, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &label_output_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  log_prob_output, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &log_prob_output_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  output_probability_classlabels_ints, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &output_probability_classlabels_ints_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  output_probability_output, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 2, 1), AI_STRIDE_INIT(4, 4, 4, 8, 16),
  1, &output_probability_output_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  probabilities_output, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &probabilities_output_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  reduce_log_sum_exp_result_exp_output, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &reduce_log_sum_exp_result_exp_output_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  reduce_log_sum_exp_result_output, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &reduce_log_sum_exp_result_output_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  reduce_log_sum_exp_result_reduce_output, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &reduce_log_sum_exp_result_reduce_output_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  sum_result_bias, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &sum_result_bias_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  sum_result_output, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &sum_result_output_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  sum_result_scale, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &sum_result_scale_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  summation_jll_output, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &summation_jll_output_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  label_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &array_feature_extractor_result_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &label_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  label_layer, 19,
  CAST_TYPE, 0x0, NULL,
  cast, forward_cast,
  &label_chain,
  NULL, &label_layer, AI_STATIC, 
  .to_format = AI_ARRAY_FORMAT_S32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  array_feature_extractor_result_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &classes_0_cast_output, &argmax_output_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &array_feature_extractor_result_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  array_feature_extractor_result_layer, 13,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &array_feature_extractor_result_chain,
  NULL, &label_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  argmax_output_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &sum_result_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &argmax_output_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  argmax_output_layer, 11,
  ARGMINMAX_TYPE, 0x0, NULL,
  argminmax, forward_argmax,
  &argmax_output_chain,
  NULL, &array_feature_extractor_result_layer, AI_STATIC, 
  .axis = AI_SHAPE_CHANNEL, 
  .select_last_index = 0, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  output_probability_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &probabilities_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_probability_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_probability_classlabels_ints),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  output_probability_layer, 20,
  ZIPMAP_TYPE, 0x0, NULL,
  zipmap, forward_zipmap,
  &output_probability_chain,
  NULL, &argmax_output_layer, AI_STATIC, 
  .has_classlabels_int = 1, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  probabilities_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &log_prob_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &probabilities_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  probabilities_layer, 17,
  NL_TYPE, 0x0, NULL,
  nl, forward_exp,
  &probabilities_chain,
  NULL, &output_probability_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  log_prob_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &sum_result_output, &reduce_log_sum_exp_result_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &log_prob_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  log_prob_layer, 16,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &log_prob_chain,
  NULL, &probabilities_layer, AI_STATIC, 
  .operation = ai_sub_f32, 
  .buffer_operation = ai_sub_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  reduce_log_sum_exp_result_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &reduce_log_sum_exp_result_reduce_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &reduce_log_sum_exp_result_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  reduce_log_sum_exp_result_layer, 14,
  NL_TYPE, 0x0, NULL,
  nl, forward_log,
  &reduce_log_sum_exp_result_chain,
  NULL, &log_prob_layer, AI_STATIC, 
  .nl_params = NULL, 
)


AI_STATIC_CONST ai_float reduce_log_sum_exp_result_reduce_neutral_value_data[] = { 0.0f };
AI_ARRAY_OBJ_DECLARE(
    reduce_log_sum_exp_result_reduce_neutral_value, AI_ARRAY_FORMAT_FLOAT,
    reduce_log_sum_exp_result_reduce_neutral_value_data, reduce_log_sum_exp_result_reduce_neutral_value_data, 1, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  reduce_log_sum_exp_result_reduce_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &reduce_log_sum_exp_result_exp_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &reduce_log_sum_exp_result_reduce_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  reduce_log_sum_exp_result_reduce_layer, 13,
  REDUCE_TYPE, 0x0, NULL,
  reduce, forward_reduce,
  &reduce_log_sum_exp_result_reduce_chain,
  NULL, &reduce_log_sum_exp_result_layer, AI_STATIC, 
  .operation = ai_sum, 
  .neutral_value = &reduce_log_sum_exp_result_reduce_neutral_value, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  reduce_log_sum_exp_result_exp_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &sum_result_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &reduce_log_sum_exp_result_exp_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  reduce_log_sum_exp_result_exp_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_exp,
  &reduce_log_sum_exp_result_exp_chain,
  NULL, &reduce_log_sum_exp_result_reduce_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  sum_result_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &summation_jll_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &sum_result_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &sum_result_scale, &sum_result_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  sum_result_layer, 10,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &sum_result_chain,
  NULL, &reduce_log_sum_exp_result_exp_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  summation_jll_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &jll_output0, &jll1_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &summation_jll_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  summation_jll_layer, 9,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &summation_jll_chain,
  NULL, &sum_result_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  jll_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &feature_log_proba, &cast_indices_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &jll_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  jll_layer, 5,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &jll_chain,
  NULL, &summation_jll_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  cast_indices_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &indices_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &cast_indices_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  cast_indices_layer, 3,
  CAST_TYPE, 0x0, NULL,
  cast, forward_cast,
  &cast_indices_chain,
  NULL, &jll_layer, AI_STATIC, 
  .to_format = AI_ARRAY_FORMAT_S32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  indices_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &float_input_output, &feature_index),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &indices_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  indices_layer, 1,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &indices_chain,
  NULL, &cast_indices_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  jll1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &feature_log_proba1, &cast_indices1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &jll1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  jll1_layer, 6,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &jll1_chain,
  NULL, &indices_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  cast_indices1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &indices1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &cast_indices1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  cast_indices1_layer, 4,
  CAST_TYPE, 0x0, NULL,
  cast, forward_cast,
  &cast_indices1_chain,
  NULL, &jll1_layer, AI_STATIC, 
  .to_format = AI_ARRAY_FORMAT_S32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  indices1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &float_input_output, &feature_index1),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &indices1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  indices1_layer, 2,
  ARRAYFEATUREEXTRACTOR_TYPE, 0x0, NULL,
  arrayfeatureextractor, forward_arrayfeatureextractor,
  &indices1_chain,
  NULL, &cast_indices1_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  classes_0_cast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &classes),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &classes_0_cast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  classes_0_cast_layer, 13,
  CAST_TYPE, 0x0, NULL,
  cast, forward_cast,
  &classes_0_cast_chain,
  NULL, &indices1_layer, AI_STATIC, 
  .to_format = AI_ARRAY_FORMAT_FLOAT, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1620, 1, 1),
    1620, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 40, 1, 1),
    40, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NBA_MODEL_IN_NUM, &float_input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NBA_MODEL_OUT_NUM, &label_output, &output_probability_output),
  &classes_0_cast_layer, 0xce8c9b84, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1620, 1, 1),
      1620, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 40, 1, 1),
      40, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NBA_MODEL_IN_NUM, &float_input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NBA_MODEL_OUT_NUM, &label_output, &output_probability_output),
  &classes_0_cast_layer, 0xce8c9b84, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool nba_model_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_nba_model_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    float_input_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    float_input_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    classes_0_cast_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 8);
    classes_0_cast_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 8);
    indices1_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 16);
    indices1_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 16);
    cast_indices1_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 16);
    cast_indices1_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 16);
    jll1_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 20);
    jll1_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 20);
    indices_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 16);
    indices_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 16);
    cast_indices_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    cast_indices_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    jll_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 28);
    jll_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 28);
    summation_jll_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    summation_jll_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    sum_result_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 16);
    sum_result_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 16);
    reduce_log_sum_exp_result_exp_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    reduce_log_sum_exp_result_exp_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    reduce_log_sum_exp_result_reduce_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 24);
    reduce_log_sum_exp_result_reduce_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 24);
    reduce_log_sum_exp_result_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    reduce_log_sum_exp_result_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    log_prob_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 24);
    log_prob_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 24);
    probabilities_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    probabilities_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    output_probability_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 24);
    output_probability_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 24);
    argmax_output_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    argmax_output_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    array_feature_extractor_result_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 4);
    array_feature_extractor_result_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 4);
    label_output_array.data = AI_PTR(g_nba_model_activations_map[0] + 0);
    label_output_array.data_start = AI_PTR(g_nba_model_activations_map[0] + 0);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool nba_model_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_nba_model_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    classes_array.format |= AI_FMT_FLAG_CONST;
    classes_array.data = AI_PTR(g_nba_model_weights_map[0] + 0);
    classes_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 0);
    feature_log_proba1_array.format |= AI_FMT_FLAG_CONST;
    feature_log_proba1_array.data = AI_PTR(g_nba_model_weights_map[0] + 8);
    feature_log_proba1_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 8);
    feature_log_proba_array.format |= AI_FMT_FLAG_CONST;
    feature_log_proba_array.data = AI_PTR(g_nba_model_weights_map[0] + 784);
    feature_log_proba_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 784);
    feature_index1_array.format |= AI_FMT_FLAG_CONST;
    feature_index1_array.data = AI_PTR(g_nba_model_weights_map[0] + 1592);
    feature_index1_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 1592);
    feature_index_array.format |= AI_FMT_FLAG_CONST;
    feature_index_array.data = AI_PTR(g_nba_model_weights_map[0] + 1596);
    feature_index_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 1596);
    sum_result_scale_array.format |= AI_FMT_FLAG_CONST;
    sum_result_scale_array.data = AI_PTR(g_nba_model_weights_map[0] + 1600);
    sum_result_scale_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 1600);
    sum_result_bias_array.format |= AI_FMT_FLAG_CONST;
    sum_result_bias_array.data = AI_PTR(g_nba_model_weights_map[0] + 1608);
    sum_result_bias_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 1608);
    output_probability_classlabels_ints_array.format |= AI_FMT_FLAG_CONST;
    output_probability_classlabels_ints_array.data = AI_PTR(g_nba_model_weights_map[0] + 1616);
    output_probability_classlabels_ints_array.data_start = AI_PTR(g_nba_model_weights_map[0] + 1616);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_nba_model_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NBA_MODEL_MODEL_NAME,
      .model_signature   = AI_NBA_MODEL_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 70,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xce8c9b84,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_nba_model_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NBA_MODEL_MODEL_NAME,
      .model_signature   = AI_NBA_MODEL_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 70,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xce8c9b84,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_nba_model_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_nba_model_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_nba_model_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_nba_model_create(network, AI_NBA_MODEL_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_nba_model_data_params_get(&params) != true) {
    err = ai_nba_model_get_error(*network);
    return err;
  }
#if defined(AI_NBA_MODEL_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_NBA_MODEL_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_nba_model_init(*network, &params) != true) {
    err = ai_nba_model_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_nba_model_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_nba_model_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_nba_model_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_nba_model_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= nba_model_configure_weights(net_ctx, params);
  ok &= nba_model_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_nba_model_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_nba_model_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NBA_MODEL_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

