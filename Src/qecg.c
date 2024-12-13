/**
  ******************************************************************************
  * @file    qecg.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2024-12-13T14:23:55+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "qecg.h"
#include "qecg_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_qecg
 
#undef AI_QECG_MODEL_SIGNATURE
#define AI_QECG_MODEL_SIGNATURE     "0x24392b04a0ef4091215b7edef5ef29a3"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2024-12-13T14:23:55+0800"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_QECG_N_BATCHES
#define AI_QECG_N_BATCHES         (1)

static ai_ptr g_qecg_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_qecg_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 186, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2976, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  _relu_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2976, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  _pool_MaxPool_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1488, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2976, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  _relu_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2976, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  _pool_1_MaxPool_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1472, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  _fc1_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  _relu_2_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  _fc2_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 5, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  output_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 5, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1536, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  _fc1_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_LUT4_FLOAT,
  NULL, NULL, 188416, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  _fc1_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  _fc2_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 640, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  _fc2_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 5, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  input_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 186), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &input_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_Conv_output_0_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 186), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_conv1_Conv_output_0_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  _relu_Relu_output_0_output, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 186), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_relu_Relu_output_0_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  _pool_MaxPool_output_0_output, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 93), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_pool_MaxPool_output_0_output_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_Conv_output_0_output, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 93), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_conv2_Conv_output_0_output_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  _relu_1_Relu_output_0_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 93), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_relu_1_Relu_output_0_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  _pool_1_MaxPool_output_0_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 46), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_pool_1_MaxPool_output_0_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  _pool_1_MaxPool_output_0_output0, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 1472, 1, 1), AI_STRIDE_INIT(4, 4, 4, 5888, 5888),
  1, &_pool_1_MaxPool_output_0_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  _fc1_Gemm_output_0_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_fc1_Gemm_output_0_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  _relu_2_Relu_output_0_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_relu_2_Relu_output_0_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  _fc2_Gemm_output_0_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &_fc2_Gemm_output_0_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  output_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &output_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_Conv_output_0_weights, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 3, 16), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_conv1_Conv_output_0_weights_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_Conv_output_0_bias, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_conv1_Conv_output_0_bias_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_Conv_output_0_weights, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 3, 32), AI_STRIDE_INIT(4, 4, 64, 2048, 2048),
  1, &_conv2_Conv_output_0_weights_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_Conv_output_0_bias, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_conv2_Conv_output_0_bias_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  _fc1_Gemm_output_0_weights, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1472, 128, 1, 1), AI_STRIDE_INIT(4, 1, 736, 94208, 94208),
  1, &_fc1_Gemm_output_0_weights_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  _fc1_Gemm_output_0_bias, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_fc1_Gemm_output_0_bias_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  _fc2_Gemm_output_0_weights, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 128, 5, 1, 1), AI_STRIDE_INIT(4, 4, 512, 2560, 2560),
  1, &_fc2_Gemm_output_0_weights_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  _fc2_Gemm_output_0_bias, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &_fc2_Gemm_output_0_bias_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_Conv_output_0_scratch0, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 3), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &_conv1_Conv_output_0_scratch0_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_Conv_output_0_scratch0, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 3), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_conv2_Conv_output_0_scratch0_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  output_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc2_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  output_layer, 17,
  NL_TYPE, 0x0, NULL,
  nl, forward_sm,
  &output_chain,
  NULL, &output_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc2_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_relu_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc2_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_fc2_Gemm_output_0_weights, &_fc2_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc2_Gemm_output_0_layer, 16,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_fc2_Gemm_output_0_chain,
  NULL, &output_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _relu_2_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc1_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_relu_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _relu_2_Relu_output_0_layer, 15,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_relu_2_Relu_output_0_chain,
  NULL, &_fc2_Gemm_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc1_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_1_MaxPool_output_0_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc1_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_fc1_Gemm_output_0_weights, &_fc1_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc1_Gemm_output_0_layer, 14,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_fc1_Gemm_output_0_chain,
  NULL, &_relu_2_Relu_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _pool_1_MaxPool_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_relu_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_1_MaxPool_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _pool_1_MaxPool_output_0_layer, 6,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &_pool_1_MaxPool_output_0_chain,
  NULL, &_fc1_Gemm_output_0_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(1, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _relu_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_relu_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _relu_1_Relu_output_0_layer, 5,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_relu_1_Relu_output_0_chain,
  NULL, &_pool_1_MaxPool_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv2_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_MaxPool_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_conv2_Conv_output_0_weights, &_conv2_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_conv2_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _conv2_Conv_output_0_layer, 4,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_conv2_Conv_output_0_chain,
  NULL, &_relu_1_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 0, 1, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _pool_MaxPool_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_relu_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_MaxPool_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _pool_MaxPool_output_0_layer, 3,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &_pool_MaxPool_output_0_chain,
  NULL, &_conv2_Conv_output_0_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(1, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _relu_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_relu_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _relu_Relu_output_0_layer, 2,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_relu_Relu_output_0_chain,
  NULL, &_pool_MaxPool_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv1_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_conv1_Conv_output_0_weights, &_conv1_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_conv1_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _conv1_Conv_output_0_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_conv1_Conv_output_0_chain,
  NULL, &_relu_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 0, 1, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 103892, 1, 1),
    103892, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 18048, 1, 1),
    18048, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_QECG_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_QECG_OUT_NUM, &output_output),
  &_conv1_Conv_output_0_layer, 0x28efcba5, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 103892, 1, 1),
      103892, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 18048, 1, 1),
      18048, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_QECG_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_QECG_OUT_NUM, &output_output),
  &_conv1_Conv_output_0_layer, 0x28efcba5, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool qecg_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_qecg_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_output_array.data = AI_PTR(g_qecg_activations_map[0] + 5196);
    input_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 5196);
    _conv1_Conv_output_0_scratch0_array.data = AI_PTR(g_qecg_activations_map[0] + 5940);
    _conv1_Conv_output_0_scratch0_array.data_start = AI_PTR(g_qecg_activations_map[0] + 5940);
    _conv1_Conv_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 5952);
    _conv1_Conv_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 5952);
    _relu_Relu_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 5952);
    _relu_Relu_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 5952);
    _pool_MaxPool_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 0);
    _pool_MaxPool_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 0);
    _conv2_Conv_output_0_scratch0_array.data = AI_PTR(g_qecg_activations_map[0] + 5952);
    _conv2_Conv_output_0_scratch0_array.data_start = AI_PTR(g_qecg_activations_map[0] + 5952);
    _conv2_Conv_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 6144);
    _conv2_Conv_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 6144);
    _relu_1_Relu_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 6144);
    _relu_1_Relu_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 6144);
    _pool_1_MaxPool_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 0);
    _pool_1_MaxPool_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 0);
    _fc1_Gemm_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 5888);
    _fc1_Gemm_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 5888);
    _relu_2_Relu_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 0);
    _relu_2_Relu_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 0);
    _fc2_Gemm_output_0_output_array.data = AI_PTR(g_qecg_activations_map[0] + 512);
    _fc2_Gemm_output_0_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 512);
    output_output_array.data = AI_PTR(g_qecg_activations_map[0] + 0);
    output_output_array.data_start = AI_PTR(g_qecg_activations_map[0] + 0);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool qecg_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_qecg_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    _conv1_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _conv1_Conv_output_0_weights_array.data = AI_PTR(g_qecg_weights_map[0] + 0);
    _conv1_Conv_output_0_weights_array.data_start = AI_PTR(g_qecg_weights_map[0] + 0);
    _conv1_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _conv1_Conv_output_0_bias_array.data = AI_PTR(g_qecg_weights_map[0] + 192);
    _conv1_Conv_output_0_bias_array.data_start = AI_PTR(g_qecg_weights_map[0] + 192);
    _conv2_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _conv2_Conv_output_0_weights_array.data = AI_PTR(g_qecg_weights_map[0] + 256);
    _conv2_Conv_output_0_weights_array.data_start = AI_PTR(g_qecg_weights_map[0] + 256);
    _conv2_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _conv2_Conv_output_0_bias_array.data = AI_PTR(g_qecg_weights_map[0] + 6400);
    _conv2_Conv_output_0_bias_array.data_start = AI_PTR(g_qecg_weights_map[0] + 6400);
    _fc1_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _fc1_Gemm_output_0_weights_array.data = AI_PTR(g_qecg_weights_map[0] + 6592);
    _fc1_Gemm_output_0_weights_array.data_start = AI_PTR(g_qecg_weights_map[0] + 6528);
    _fc1_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _fc1_Gemm_output_0_bias_array.data = AI_PTR(g_qecg_weights_map[0] + 100800);
    _fc1_Gemm_output_0_bias_array.data_start = AI_PTR(g_qecg_weights_map[0] + 100800);
    _fc2_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _fc2_Gemm_output_0_weights_array.data = AI_PTR(g_qecg_weights_map[0] + 101312);
    _fc2_Gemm_output_0_weights_array.data_start = AI_PTR(g_qecg_weights_map[0] + 101312);
    _fc2_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _fc2_Gemm_output_0_bias_array.data = AI_PTR(g_qecg_weights_map[0] + 103872);
    _fc2_Gemm_output_0_bias_array.data_start = AI_PTR(g_qecg_weights_map[0] + 103872);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_qecg_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_QECG_MODEL_NAME,
      .model_signature   = AI_QECG_MODEL_SIGNATURE,
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
      
      .n_macc            = 353088,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x28efcba5,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_qecg_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_QECG_MODEL_NAME,
      .model_signature   = AI_QECG_MODEL_SIGNATURE,
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
      
      .n_macc            = 353088,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x28efcba5,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_qecg_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_qecg_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_qecg_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_qecg_create(network, AI_QECG_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_qecg_data_params_get(&params) != true) {
    err = ai_qecg_get_error(*network);
    return err;
  }
#if defined(AI_QECG_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_QECG_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_qecg_init(*network, &params) != true) {
    err = ai_qecg_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_qecg_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_qecg_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_qecg_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_qecg_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= qecg_configure_weights(net_ctx, params);
  ok &= qecg_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_qecg_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_qecg_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_QECG_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

