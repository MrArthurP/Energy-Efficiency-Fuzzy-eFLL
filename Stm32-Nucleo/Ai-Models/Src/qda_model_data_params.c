/**
  ******************************************************************************
  * @file    qda_model_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-09-04T15:27:00-0300
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

#include "qda_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_qda_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_qda_model_weights_array_u64[105] = {
  0xbe291a2b3f800000U, 0x3f1c113b3edf47f1U, 0x3eb00761be92f8c1U, 0x3e249d6ebeb6ed21U,
  0xbc6de467bed505ecU, 0xbe831248bca81316U, 0xbd6b81a13dd9a8f8U, 0x3ef0a377bc1d6f60U,
  0x3e080687be84633cU, 0x3e9afc853e235519U, 0xbe3e1e03bec20eb2U, 0x3f1e5fd63d599664U,
  0x3f7dfc1bbe55e40eU, 0xbe6f29e83f56526fU, 0x3da5e12c3f27e094U, 0x3e4e59d6bfbd3a47U,
  0xbff9d5c1bf9a8c6aU, 0x3fe54b633fac0017U, 0xa7c2303f3f0b7f9aU, 0xe01f740000435ddU,
  0xa6ad3840b8bee0b6U, 0xb01d280a800000a4U, 0xb5bbc4923282f62eU, 0x233ed18aaa9e4200U,
  0xb68237bfb80714bcU, 0xac38036fb2ec4278U, 0xa3defe0d32f1e0c7U, 0xb717622f8364ce43U,
  0xa0b36e4936460f9eU, 0x2f5e222ab818d140U, 0x3e3a7c49092f6f6eU, 0x3ea55b853f00fcbeU,
  0xbad445093e912b4aU, 0x3c909f6abf72d481U, 0x3ea12dcabe50cb88U, 0x3f2b51713e31c802U,
  0x3e1b0fff3e4b0b22U, 0xbe4448c33f07630fU, 0xbc0db2553f1925a1U, 0x3cd93618bf458f6fU,
  0xbe9dd89bbe97c04dU, 0x3f4de88a3e376868U, 0xbecb02e2be2ad585U, 0xbe6fd0a73ed8d1c4U,
  0x3603db293e85b863U, 0x3742f000b8350124U, 0x3b117df8be937b8eU, 0x966456cebeed25e9U,
  0xbd9198adbb8bce87U, 0x3db6338b3eb6027cU, 0x3c1f567abec835a5U, 0xba2d7f6d3f0ac3acU,
  0x3e882330bd5cd975U, 0xbe7ec445be98a408U, 0xbf01657ebe9a3b24U, 0xbeb6c3ef3da9068bU,
  0xb5be46bebe5a6242U, 0x16a7f3a8a613e41eU, 0xbd6d26a63eab8d5bU, 0xbf1038bb3eb2c500U,
  0x3ed1c8863e059d15U, 0x3e9e4f0a3ec618f6U, 0x3c0458dbbeeab3d3U, 0x3b0595763ef92c1bU,
  0x3f1a50eb3ec3f131U, 0xbf362f9fbea5f4fcU, 0xbe28972dbeb20002U, 0x3e96d50fbeb52bb8U,
  0x3c0867bf3cd214b3U, 0xba6850e33f20222fU, 0x3f4b8354be76b2cfU, 0xbee52ed23d0285f6U,
  0x3e3f46ac3e68d53aU, 0x389ad1dbbeaa4f93U, 0xbbdbcd1d936a40a0U, 0x3cb94b1ebf3c3ed1U,
  0xbea5d985bd18bf09U, 0x3f4f234c3f17054eU, 0xbe3cc9ddbec05bb2U, 0x3e8ad253bedb2356U,
  0x38947bc8bdbe914cU, 0x3d16b63288c5a3c6U, 0xbd7b1f0dbdbb22dfU, 0x3f1d4fd73f0bcb1eU,
  0xbc8eab783eeb2c78U, 0xb21f8e5ebe627d77U, 0x8000000bb77971f9U, 0x8a952edb9f68U,
  0xbe5df076b09ae3f7U, 0xb2d09096af11a367U, 0xbe22bf81bee2cfd5U, 0x3f8c4fe9bec5e6ccU,
  0xbe84ce053febd6cdU, 0xbe46c86bbeb3295cU, 0xbfade380bf21ccd3U, 0x3e12c4293e88e3d5U,
  0x399f6fca3ea5cf14U, 0xbe8bb41525236114U, 0x3e958a9dbfd24542U, 0x3f027e243f58199aU,
  0x3f8aedff3ef2cbc6U, 0xbef87f7dbf5ca3e3U, 0xbefd490b3eefc6e0U, 0x100U,
  0x1U,
};


ai_handle g_qda_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_qda_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

