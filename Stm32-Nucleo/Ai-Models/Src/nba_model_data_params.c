/**
  ******************************************************************************
  * @file    nba_model_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-09-04T15:28:16-0300
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

#include "nba_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_nba_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_nba_model_weights_array_u64[203] = {
  0x100000000U, 0xc0d5fccbc01b29d7U, 0xc02a5a3ac0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc036f120U, 0xc01b29d7c0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc01fd649U,
  0xc0d5fccbc0d5fccbU, 0xc0d5fccbc024e108U, 0xc036f120c0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc01b29d7U, 0xc046a137c0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc03e4e6cc0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0d5fccbc046a137U, 0xc07a877ec0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0692022c0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0692022c0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0d5fccbc09ca6b3U, 0xc07a877ec0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc07a877eU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc09ca6b3U, 0xc07a877ec0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0d5fccbc09ca6b3U, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU, 0xc0d5fccbc0d5fccbU,
  0xc0d5fccbc0d5fccbU, 0xc0d8aa4fc0d5fccbU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc06e7b29U,
  0xc043a973c0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc07fe285U, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc0558e2bU, 0xc02a3c0ec0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc03c4c27U,
  0xc07fe285c0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc035b195c0d8aa4fU, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc035b195U, 0xc02fb540c0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc043a973U,
  0xc043a973c0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc08beec0c0d8aa4fU, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc0253150U, 0xc043a973c0d8aa4fU, 0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc04bfc3dU,
  0xc0d8aa4fc0d8aa4fU, 0xc0d8aa4fc04bfc3dU, 0xc04bfc3dc0d8aa4fU, 0xc0d8aa4fc0d8aa4fU,
  0xc0d8aa4fc035b195U, 0xc03c4c27c0d8aa4fU, 0xc08969fec0d6258dU, 0xc08969fec09ccf75U,
  0xc08969fec08969feU, 0xc08969fec0d6258dU, 0xc07ad902c07ad902U, 0xc08969fec07ad902U,
  0xc08969fec07ad902U, 0xc08969fec09ccf75U, 0xc08969fec07ad902U, 0xc09ccf75c08969feU,
  0xc09ccf75c08969feU, 0xc0d6258dc09ccf75U, 0xc08969fec09ccf75U, 0xc08969fec08969feU,
  0xc09ccf75c08969feU, 0xc0d6258dc09ccf75U, 0xc07ad902c08969feU, 0xc0d6258dc09ccf75U,
  0xc08969fec08969feU, 0xc09ccf75c09ccf75U, 0xc08969fec09ccf75U, 0xc07ad902c08969feU,
  0xc09ccf75c08969feU, 0xc09ccf75c09ccf75U, 0xc08969fec08969feU, 0xc09ccf75c08969feU,
  0xc09ccf75c0d6258dU, 0xc08969fec08969feU, 0xc09ccf75c0d6258dU, 0xc08969fec07ad902U,
  0xc09ccf75c09ccf75U, 0xc08969fec07ad902U, 0xc0d6258dc08969feU, 0xc09ccf75c08969feU,
  0xc0d6258dc0d6258dU, 0xc09ccf75c08969feU, 0xc0d6258dc0d6258dU, 0xc08969fec0d6258dU,
  0xc08969fec0d6258dU, 0xc0d6258dc09ccf75U, 0xc08969fec07ad902U, 0xc08969fec08969feU,
  0xc0d6258dc08969feU, 0xc09ccf75c09ccf75U, 0xc09ccf75c08969feU, 0xc0d6258dc0d6258dU,
  0xc0d6258dc09ccf75U, 0xc07ad902c09ccf75U, 0xc09ccf75c0d6258dU, 0xc08969fec0d6258dU,
  0xc0d8cfcdc09ccf75U, 0xc08c143ec0d8cfcdU, 0xc09f79b5c09f79b5U, 0xc08016c1c09f79b5U,
  0xc0d8cfcdc09f79b5U, 0xc0d8cfcdc0d8cfcdU, 0xc0d8cfcdc09f79b5U, 0xc08c143ec09f79b5U,
  0xc0d8cfcdc09f79b5U, 0xc09f79b5c09f79b5U, 0xc09f79b5c08c143eU, 0xc08c143ec08c143eU,
  0xc08c143ec08016c1U, 0xc09f79b5c09f79b5U, 0xc09f79b5c09f79b5U, 0xc08c143ec08c143eU,
  0xc09f79b5c08016c1U, 0xc08c143ec0d8cfcdU, 0xc09f79b5c08016c1U, 0xc08c143ec09f79b5U,
  0xc08c143ec08c143eU, 0xc09f79b5c09f79b5U, 0xc09f79b5c0d8cfcdU, 0xc08c143ec08c143eU,
  0xc09f79b5c08c143eU, 0xc09f79b5c09f79b5U, 0xc08016c1c08c143eU, 0xc09f79b5c08c143eU,
  0xc08016c1c09f79b5U, 0xc0d8cfcdc08c143eU, 0xc08c143ec09f79b5U, 0xc0d8cfcdc08c143eU,
  0xc09f79b5c09f79b5U, 0xc09f79b5c08016c1U, 0xc08016c1c08c143eU, 0xc09f79b5c08016c1U,
  0xc08016c1c08c143eU, 0xc08016c1c08016c1U, 0xc08016c1c09f79b5U, 0xc08c143ec09f79b5U,
  0xc0d8cfcdc08016c1U, 0xc09f79b5c09f79b5U, 0xc09f79b5c09f79b5U, 0xc08c143ec08016c1U,
  0xc09f79b5c08c143eU, 0xc08016c1c08c143eU, 0xc08c143ec08016c1U, 0xc08c143ec08016c1U,
  0xc08016c1c0d8cfcdU, 0xc08c143ec08c143eU, 0xc0d8cfcdc09f79b5U, 0x1U,
  0x3f8000003f800000U, 0xbf259d7dbf3dd978U, 0x100U,
};


ai_handle g_nba_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_nba_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

