/**
  ******************************************************************************
  * @file    nnw_model_data_params.c
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

#include "nnw_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_nnw_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_nnw_model_weights_array_u64[351] = {
  0xbbb1456a3f800000U, 0x3ee3c1febbcbde7dU, 0x3e6a27cd3e952245U, 0x3db3db37bea2bc41U,
  0xbe9ce8b2bc4a38bdU, 0x3c6806b13df6866eU, 0xbeffea7fbbe7eec2U, 0x3eaeb7c83dc7615bU,
  0x3d4ed4f5beb47a93U, 0x3e42bbeebee84d19U, 0xbed87f573ee18630U, 0x3eec69dc3eeff827U,
  0x3ea475623e96797dU, 0xb5dc08d1be3620ffU, 0xbed9ecadb9458f5aU, 0xb70170433e2f1854U,
  0xafe7e2a310769858U, 0x3d6208acb8bae89bU, 0x1325e8683c9cb8eeU, 0xbdc54175ba684bc1U,
  0x3df0f0053edecde4U, 0xbeafb2adbe919120U, 0xbd90b0013dbeaaa6U, 0xbe44ef4dbd85d332U,
  0x3d00fb3c3d9fa48aU, 0x3f959fe2bf870201U, 0x3f98dd863f91b76dU, 0x3e2ce74a3fb2ee33U,
  0xbee61ea2be61a673U, 0xbf8fb745bf10521cU, 0xbf46f5573ed6e6f7U, 0x3f63b580be60f3fdU,
  0xbe5785cbbe0cee14U, 0xbeb0a261bf75a3eeU, 0xbd931472be491121U, 0x3e7d040b3f40269cU,
  0xb84bbf08bf76f59fU, 0x3e8dff1b3ceb2864U, 0x3e00f9e2bd44dd26U, 0x3eb2c2b83ede4d76U,
  0x3e9f185d3eaa121dU, 0xbf72e1fcbe12daf2U, 0xbd9970343e903c6dU, 0x3edb93392d52b2b4U,
  0xb845dd2417f7cc63U, 0x26b5b2003a6c2439U, 0x3ded8537be4a4959U, 0xb5a7902e3e46b456U,
  0x32ded8cf17fbe1adU, 0x3e6458b4bb4365a3U, 0xbe36835ebee8cba1U, 0x3e9cf908bf10fe7dU,
  0x3e0dac963e9ec1eeU, 0x3ec077093ddc93acU, 0x3df0fb4c3e26b0daU, 0xbed501c3323bd41bU,
  0x89c667b9b61b0090U, 0xb3e4698c3ed7e63fU, 0xbc74876c3e8ba2d8U, 0xabf418d83e96bf43U,
  0x2a29a5403eca4972U, 0xa62e1667b805c47eU, 0x253cd048964ed3caU, 0x9dc2f5fa20e3345dU,
  0x28c68e3e369313bfU, 0x33d1edacb7e6cdd2U, 0xb0fcda3095467dfU, 0xa5f08baaab8c982eU,
  0xb74e0941b698fd6aU, 0x367c78e18c3b4da2U, 0x3566d20032ba61e9U, 0xb339228c0748fa03U,
  0xb32e952d0ac11b37U, 0xb8af3ff4b90583c0U, 0xb99001d3b8e18263U, 0x3378083cb8d27ea6U,
  0x29953a34b8015dbaU, 0x289f56f7b99f2777U, 0xb79e4125ba427dfeU, 0xb76e58bfb333fa30U,
  0xb58e93a5295dee48U, 0x3333b3b599b62c6dU, 0xb5f119103863e118U, 0xb680149ab75591d6U,
  0x373ba4e02e782c87U, 0x3f526903beb98b0dU, 0x3e520c3abe039775U, 0x3c72a20a3f3700d4U,
  0x3e06c13fbe2bccccU, 0xbdccd17c3e53d0fdU, 0x3e9b6c95bc09ef37U, 0x3eda97402eb2c885U,
  0xb59749eeb6e91741U, 0x377b9d33be8ce68cU, 0x3edeb2e63eb1835fU, 0xb6a04039bc818c3fU,
  0xb60e2c80bf378f6fU, 0x3cd5bd703d9ead0fU, 0x3e677af43c68902aU, 0xbd9e48de3f0f592eU,
  0xbdc9b4543ea243ecU, 0xbe4d4337bdf83b38U, 0xbdd1d8ddbb35c8cdU, 0x3ebce7b63270fdb6U,
  0x9d621475332bd5bfU, 0x2c6550fb3cc33386U, 0xbe6e78e13deacb1aU, 0x17a957693ee7769fU,
  0x9ceef039bef04a09U, 0x3e380ef0bde156a7U, 0x1d779a573e7276caU, 0xb652534abbdee4cfU,
  0x3e15b065bd2a96edU, 0xb4242c92be6bcb92U, 0x3e8d9f19be4fc8b6U, 0x9aba68bd384ffd97U,
  0x2e518db82a1792a7U, 0x9691ccbfbe833437U, 0x3e002986301c1fecU, 0xb1aed92d0785610dU,
  0xb5a325df2b04266aU, 0x3e4a051a3d577f47U, 0xbed53602be973e79U, 0x3c9ef132befbd66bU,
  0x3e35a2f13e9ed3cfU, 0xbe02c34c3d814708U, 0x3e9652d8bccbfac3U, 0xbe0fac7292252748U,
  0x113a8461b6a9dd8dU, 0xfc467be3d90f46dU, 0x3c2a5c8c3ea4c508U, 0x96fddf60be054f6fU,
  0x35084abb3f2a6e22U, 0xbed162353eab609dU, 0xbd711cfabebd3718U, 0xbd94aaefbeee3847U,
  0x3e5d13733e9e5681U, 0x3d9c05013e714e08U, 0xbe51c322bd51bcd5U, 0xbe8165b519e35618U,
  0x37b9611ab694c4beU, 0x370dae403f0de12aU, 0x3d83e96c3e7dcfaeU, 0xab7643733da5e6cbU,
  0x187bc7503ec9a04cU, 0x8273342aeb4b272U, 0x1698ad1a8818541fU, 0xaf2368a0b698cbd2U,
  0x368a258a348b3f08U, 0x2b22b2dcb11e1a68U, 0xb7039dba16b1de0bU, 0xa3a65360309df4b6U,
  0x17d97e0f383b657eU, 0x3509bd7935d66cc8U, 0xb14a0110aee14718U, 0x27b7b300af76e263U,
  0x37411bc10a697fb2U, 0x3d662ef4bda0c516U, 0x3f037f7a3ef2c649U, 0x3e6fa0953f047da9U,
  0x3ea13db5be0d477fU, 0x3d8bd5533d9e1a16U, 0xbea387b23dd3d625U, 0x3e70894620b72d47U,
  0xab1a97c018a74ac5U, 0xb7937639be91aa9fU, 0x3f2344c7bcc15634U, 0xb816e30cbe92f877U,
  0x35f2de36bf2e74a4U, 0x3dd724bebc488ec1U, 0x3e888ade3e412ed1U, 0xbe400eb23f1748feU,
  0x380021bdbd2cde2eU, 0xbde51d65bc9fd724U, 0xbdc3c77d3c366e11U, 0x3e5c0db937042ba8U,
  0x351c50581a49ddd8U, 0xb8022aa5be988a58U, 0x3e5b95653c30af39U, 0x3209ae133ca527b2U,
  0xa7849a72be5719faU, 0xb587d6a4be24acc7U, 0xbe0b533cbe5efbd6U, 0x3ec09051bf09ab70U,
  0x31fc7e3d347b665dU, 0x3ec707e4be9035e1U, 0xb64bebc3bda6d218U, 0x3e690b4b34b9e183U,
  0xb09dd3f232b90e3eU, 0x986385bd3de03c84U, 0x1d9615d43eb55526U, 0x99da47f7be80d8a5U,
  0xa4a3a68c3ed4b26dU, 0xbddf774dbea6185aU, 0xbe5da4f43d163be6U, 0x3e86b108bf2a4ca9U,
  0x9e07ed2ca66a2db3U, 0x3e4556b03c29551bU, 0x2665b3d93d64bd4dU, 0x3e773bf78fb58c0aU,
  0x37bf61ca37679d25U, 0x329dc672be38efc5U, 0x917375d73d953f3cU, 0xb6a9d7bbbe4a987fU,
  0x37c471c33e409199U, 0x3e7652acbd17cbcbU, 0x3ec9f8a93e97abbcU, 0xbdee32773f149c89U,
  0x3f006f6abd68ec22U, 0xbdcf0bb6bc8baa8aU, 0xbe1d965b3d0168b5U, 0x3e23f16e2eb81a37U,
  0xb841a12f38382a29U, 0x383eaa54bef33e1dU, 0x3f17a8813be38ee5U, 0x35475487bcae935fU,
  0x36fda7fdbefc1bc0U, 0xbdd40ddfbe8ab983U, 0x3ef00f5b3e8e856fU, 0xbdc771d33f17970eU,
  0x3e5e63283c2633ecU, 0x3d0eda883da2cc9cU, 0xbe3b80733e7afe51U, 0x3ddaa0f22f0b13f8U,
  0x37e9638e2ef461f4U, 0xb466bb0cbec01a01U, 0xbccdba293e0e69f6U, 0x370559583de5bc90U,
  0x35bb621fbef929faU, 0xbd3e72703eb285b9U, 0xbeb8e8b1be572ca5U, 0x3e16409cbf0d0a71U,
  0x3e6e897c3e93d6e6U, 0xbc8523943e50272aU, 0x3e8115e33e06dfadU, 0xbed525e2b60e7f87U,
  0xb79d4b2a984a3714U, 0x9381204b3e944d89U, 0x3e54fd9b3dadf277U, 0xb72f7a93be730249U,
  0x370aae1d3d34fcb3U, 0x3e4d2c633e361b66U, 0x3ea148173e4fb153U, 0xbda27efc3efffb57U,
  0xbd953f1e3e22aad0U, 0xbe53b4c43e99573cU, 0xbeaf7abdbe827656U, 0x3e1a4ab13634792fU,
  0x35ded37bb69c918cU, 0x982fd7e0bf042066U, 0xbd0c8ed6bdc410c4U, 0xafd4fd86bdaacaa4U,
  0xb5898e20bef66930U, 0x9a8b971e9ac72c08U, 0x32fb88169ac4c703U, 0xb24ea7f68b070134U,
  0x97a753300f7d2b86U, 0x9d5a76e388df3944U, 0x32d0015db6fc2be4U, 0x8dec4d7b873e2847U,
  0x8a29e8f130a8f8cfU, 0x37e9a1f838050ddcU, 0x6849e9fb5b50c24U, 0x32660363360df885U,
  0x36057bb6b16590e4U, 0x3ec8f7533e5edc75U, 0x3e6ae62f3e100745U, 0x3d8156f03f023821U,
  0xbe7ad662be87f51eU, 0x3e10ea17be2bf529U, 0xbe2680853da52f54U, 0x3ec086553367f394U,
  0x385e0d6c0deb1007U, 0x341704043de3d313U, 0x3e8d781f3df29b57U, 0x2ebcac58bbda857bU,
  0x2f88226fbf01aeb2U, 0x37baef34323e8bc8U, 0x8a0098b5b670abcbU, 0xababa3b29f034893U,
  0x90d95f6414d934afU, 0xb83630ed31c58adcU, 0xb592ecceb801a01eU, 0x1d37db16a5c25b10U,
  0xb6e08ada0fd3553fU, 0x33f1adb428fbed45U, 0x950628f508bf1f70U, 0xb2df1e4722551a5eU,
  0x372aaf66b5887d6aU, 0xbdaf773bbe58bfafU, 0x3e6635b93e95da83U, 0x3de5aefa3e67731bU,
  0xbe900b2b3ecf4e17U, 0xb5a1a603be3bfaf5U, 0x3e383bf83d9d3ebfU, 0x3e50df6a36068d37U,
  0x8cb8452e36529261U, 0x89bb53f33c64d61aU, 0x3e3924f0bdd64c03U, 0xb188affd3e52a6afU,
  0x36ee316f35d8e9afU, 0x3d5e0bc63e7689dcU, 0x3db55450bcdf3460U, 0x3ca2f43f3ef50185U,
  0x3c933faebe6f09b0U, 0xbeab2976bd9fdb1bU, 0x3db1efa03e97cfb3U, 0xbdaaddbe1692a614U,
  0x37e57801b5c91354U, 0x91fa651dbea8d025U, 0x3ea8f3a63cecc366U, 0x2c94afe0be288be5U,
  0x78ba2a1beca92f0U, 0xbe2ae6f93b3adf3eU, 0x3e3494c1be7cdd23U, 0xbb092909b5df0115U,
  0x3b671875bc4eb4e4U, 0xbebeb002bea4f6e0U, 0x3e7749913ce852c5U, 0x3dc8db23b848a1fdU,
  0x996371c0785b342U, 0xac822fa73e496cc7U, 0x3e95bfce3e2d5a14U, 0x121a9894bebc91a5U,
  0x3f31a2b525973688U, 0xbe958d03bf2b85f0U, 0x3f3006553bd03a5bU, 0xbd67f5863f68e37fU,
  0xbf3581b6bf4dc614U, 0x3f43c472bdc6472cU, 0xbf1d526b3f76876dU, 0x3f9660af3e1c27bbU,
  0xbf54a97b3ef09965U, 0xbe37f23e3e9440a6U, 0xbe62f3213e9dce4eU, 0x3e8cac553dda9dcdU,
  0xbef0308e3e816c5eU, 0x9807ca463d9a4de9U, 0xbf118c243abed3fbU, 0x3c85a305be984f99U,
  0x3ed6503f3ed665bdU, 0xbf048fb7b3cb6b61U, 0x3f21154ebf472450U, 0xbf8fda8f3e0b7084U,
  0x3ea9f217bea9772eU, 0xae2a35edbeb4f0c7U, 0x8182c88bdbdfa76U, 0xbeb6d0b1beddd5a6U,
  0xbf3a79093db179c4U, 0x100U, 0x1U,
};


ai_handle g_nnw_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_nnw_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

