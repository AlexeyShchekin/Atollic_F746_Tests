/**
  ******************************************************************************
  * @file    network_1.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri Oct 18 00:00:54 2019
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */



#include "network_1.h"

#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "layers.h"

#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#define AI_TOOLS_VERSION_MAJOR 3
#define AI_TOOLS_VERSION_MINOR 3
#define AI_TOOLS_VERSION_MICRO 0

#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#define AI_TOOLS_API_VERSION_MAJOR 1
#define AI_TOOLS_API_VERSION_MINOR 1
#define AI_TOOLS_API_VERSION_MICRO 0

#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network_1
 
#undef AI_NETWORK_1_MODEL_SIGNATURE
#define AI_NETWORK_1_MODEL_SIGNATURE     "06a182f1a11ce82c562878ee030141d8"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     "(rev-)"
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Fri Oct 18 00:00:54 2019"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_1_N_BATCHES
#define AI_NETWORK_1_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network arrays declarations  *************************************/
AI_STATIC ai_array main_input_output_array;   /* Array #0 */
AI_STATIC ai_array conv2d_1_output_array;   /* Array #1 */
AI_STATIC ai_array conv2d_2_output_in_array;   /* Array #2 */
AI_STATIC ai_array conv2d_2_output_out_array;   /* Array #3 */
AI_STATIC ai_array dense_1_output_array;   /* Array #4 */
AI_STATIC ai_array activation_3_output_array;   /* Array #5 */
AI_STATIC ai_array main_output_output_array;   /* Array #6 */
AI_STATIC ai_array main_output_nl_output_array;   /* Array #7 */


/**  Forward network tensors declarations  ************************************/
AI_STATIC ai_tensor main_input_output;   /* Tensor #0 */
AI_STATIC ai_tensor conv2d_1_output;   /* Tensor #1 */
AI_STATIC ai_tensor conv2d_2_output_in;   /* Tensor #2 */
AI_STATIC ai_tensor conv2d_2_output_out;   /* Tensor #3 */
AI_STATIC ai_tensor dense_1_output;   /* Tensor #4 */
AI_STATIC ai_tensor activation_3_output;   /* Tensor #5 */
AI_STATIC ai_tensor main_output_output;   /* Tensor #6 */
AI_STATIC ai_tensor main_output_nl_output;   /* Tensor #7 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain conv2d_1_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain conv2d_2_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain dense_1_chain;   /* Chain #2 */
AI_STATIC_CONST ai_tensor_chain activation_3_chain;   /* Chain #3 */
AI_STATIC_CONST ai_tensor_chain main_output_chain;   /* Chain #4 */
AI_STATIC_CONST ai_tensor_chain main_output_nl_chain;   /* Chain #5 */


/**  Subgraph network operators tensor chain declarations  *********************************/


/**  Subgraph network operators declarations  *********************************/


/**  Forward network layers declarations  *************************************/
AI_STATIC ai_layer_conv2d_nl_pool conv2d_1_layer; /* Layer #0 */
AI_STATIC ai_layer_conv2d_nl_pool conv2d_2_layer; /* Layer #1 */
AI_STATIC ai_layer_dense dense_1_layer; /* Layer #2 */
AI_STATIC ai_layer_nl activation_3_layer; /* Layer #3 */
AI_STATIC ai_layer_dense main_output_layer; /* Layer #4 */
AI_STATIC ai_layer_sm main_output_nl_layer; /* Layer #5 */


/**  Arrays declarations section  *********************************************/
AI_ARRAY_OBJ_DECLARE(
  main_input_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 784,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 2304,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_output_in_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 512,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_output_out_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 512,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  dense_1_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  activation_3_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  main_output_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 10,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  main_output_nl_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 10,
  AI_STATIC)


/**  Activations tensors declaration section  *********************************/
AI_TENSOR_OBJ_DECLARE(
  main_input_output,
  AI_SHAPE_INIT(28, 28, 1, 1),
  AI_STRIDE_INIT(112, 4, 4, 4),
  &main_input_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_output,
  AI_SHAPE_INIT(12, 12, 16, 1),
  AI_STRIDE_INIT(768, 64, 4, 4),
  &conv2d_1_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_output_in,
  AI_SHAPE_INIT(4, 4, 32, 1),
  AI_STRIDE_INIT(512, 128, 4, 4),
  &conv2d_2_output_in_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_output_out,
  AI_SHAPE_INIT(1, 1, 512, 1),
  AI_STRIDE_INIT(2048, 2048, 4, 4),
  &conv2d_2_output_out_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  dense_1_output,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &dense_1_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  activation_3_output,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &activation_3_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  main_output_output,
  AI_SHAPE_INIT(1, 1, 10, 1),
  AI_STRIDE_INIT(40, 40, 4, 4),
  &main_output_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  main_output_nl_output,
  AI_SHAPE_INIT(1, 1, 10, 1),
  AI_STRIDE_INIT(40, 40, 4, 4),
  &main_output_nl_output_array,
  AI_STATIC)





/* Layer #0: "conv2d_1" (Conv2D) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 400,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_1_weights,
  AI_SHAPE_INIT(16, 5, 5, 1),
  AI_STRIDE_INIT(100, 20, 4, 4),
  &conv2d_1_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 16,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_1_bias,
  AI_SHAPE_INIT(1, 1, 16, 1),
  AI_STRIDE_INIT(64, 64, 4, 4),
  &conv2d_1_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_1_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&main_input_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_1_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_1_weights, &conv2d_1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_1_layer, 1,
  OPTIMIZED_CONV2D_TYPE,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &AI_NET_OBJ_INSTANCE, &conv2d_2_layer, AI_STATIC,
  .tensors = &conv2d_1_chain, 
  .groups = 1, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_2D_INIT(0, 0), 
  .filter_pad_r = AI_SHAPE_2D_INIT(0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_2D_INIT(0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

/* Layer #1: "conv2d_2" (Conv2D) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 12800,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_2_weights,
  AI_SHAPE_INIT(32, 5, 5, 16),
  AI_STRIDE_INIT(1600, 320, 64, 4),
  &conv2d_2_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 32,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_2_bias,
  AI_SHAPE_INIT(1, 1, 32, 1),
  AI_STRIDE_INIT(128, 128, 4, 4),
  &conv2d_2_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_2_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&conv2d_1_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_2_output_in),
  AI_TENSOR_LIST_ENTRY(&conv2d_2_weights, &conv2d_2_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_2_layer, 5,
  OPTIMIZED_CONV2D_TYPE,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &AI_NET_OBJ_INSTANCE, &dense_1_layer, AI_STATIC,
  .tensors = &conv2d_2_chain, 
  .groups = 1, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_2D_INIT(0, 0), 
  .filter_pad_r = AI_SHAPE_2D_INIT(0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_2D_INIT(0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

/* Layer #2: "dense_1" (Dense) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 65536,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  dense_1_weights,
  AI_SHAPE_INIT(1, 1, 128, 512),
  AI_STRIDE_INIT(262144, 262144, 2048, 4),
  &dense_1_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 128,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  dense_1_bias,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &dense_1_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_1_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&conv2d_2_output_out),
  AI_TENSOR_LIST_ENTRY(&dense_1_output),
  AI_TENSOR_LIST_ENTRY(&dense_1_weights, &dense_1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_1_layer, 10,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &activation_3_layer, AI_STATIC,
  .tensors = &dense_1_chain, 
)

/* Layer #3: "activation_3" (Nonlinearity) */
  


AI_TENSOR_CHAIN_OBJ_DECLARE(
  activation_3_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&dense_1_output),
  AI_TENSOR_LIST_ENTRY(&activation_3_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  activation_3_layer, 11,
  NL_TYPE,
  nl, forward_relu,
  &AI_NET_OBJ_INSTANCE, &main_output_layer, AI_STATIC,
  .tensors = &activation_3_chain, 
)

/* Layer #4: "main_output" (Dense) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  main_output_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 1280,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  main_output_weights,
  AI_SHAPE_INIT(1, 1, 10, 128),
  AI_STRIDE_INIT(5120, 5120, 512, 4),
  &main_output_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  main_output_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 10,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  main_output_bias,
  AI_SHAPE_INIT(1, 1, 10, 1),
  AI_STRIDE_INIT(40, 40, 4, 4),
  &main_output_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  main_output_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&activation_3_output),
  AI_TENSOR_LIST_ENTRY(&main_output_output),
  AI_TENSOR_LIST_ENTRY(&main_output_weights, &main_output_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  main_output_layer, 12,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &main_output_nl_layer, AI_STATIC,
  .tensors = &main_output_chain, 
)

/* Layer #5: "main_output_nl" (Nonlinearity) */
  


AI_TENSOR_CHAIN_OBJ_DECLARE(
  main_output_nl_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&main_output_output),
  AI_TENSOR_LIST_ENTRY(&main_output_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  main_output_nl_layer, 12,
  SM_TYPE,
  sm, forward_sm,
  &AI_NET_OBJ_INSTANCE, &main_output_nl_layer, AI_STATIC,
  .tensors = &main_output_nl_chain, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 320808, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 13380, 1,
                     NULL),
  &main_input_output, &main_output_nl_output,
  &conv2d_1_layer, 0)


AI_DECLARE_STATIC
ai_bool network_1_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT( activations )
  AI_FLAG_SET(net_ctx->flags, AI_NETWORK_FLAG_OUT_COPY);

  {
    /* Updating activations (byte) offsets */
    main_input_output_array.data = NULL;
  main_input_output_array.data_start = NULL;
  conv2d_1_output_array.data = activations + 0;
  conv2d_1_output_array.data_start = activations + 0;
  conv2d_2_output_in_array.data = activations + 9280;
  conv2d_2_output_in_array.data_start = activations + 9280;
  conv2d_2_output_out_array.data = activations + 9280;
  conv2d_2_output_out_array.data_start = activations + 9280;
  dense_1_output_array.data = activations + 0;
  dense_1_output_array.data_start = activations + 0;
  activation_3_output_array.data = activations + 0;
  activation_3_output_array.data_start = activations + 0;
  main_output_output_array.data = activations + 512;
  main_output_output_array.data_start = activations + 512;
  main_output_nl_output_array.data = activations + 512;
  main_output_nl_output_array.data_start = activations + 512;
  
  }
  return true;
}

AI_DECLARE_STATIC
ai_bool network_1_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT( weights )

  {
    /* Updating weights (byte) offsets */
    conv2d_1_weights_array.format |= AI_FMT_FLAG_CONST;
  conv2d_1_weights_array.data = weights + 0;
  conv2d_1_weights_array.data_start = weights + 0;
  conv2d_1_bias_array.format |= AI_FMT_FLAG_CONST;
  conv2d_1_bias_array.data = weights + 1600;
  conv2d_1_bias_array.data_start = weights + 1600;
  conv2d_2_weights_array.format |= AI_FMT_FLAG_CONST;
  conv2d_2_weights_array.data = weights + 1664;
  conv2d_2_weights_array.data_start = weights + 1664;
  conv2d_2_bias_array.format |= AI_FMT_FLAG_CONST;
  conv2d_2_bias_array.data = weights + 52864;
  conv2d_2_bias_array.data_start = weights + 52864;
  dense_1_weights_array.format |= AI_FMT_FLAG_CONST;
  dense_1_weights_array.data = weights + 52992;
  dense_1_weights_array.data_start = weights + 52992;
  dense_1_bias_array.format |= AI_FMT_FLAG_CONST;
  dense_1_bias_array.data = weights + 315136;
  dense_1_bias_array.data_start = weights + 315136;
  main_output_weights_array.format |= AI_FMT_FLAG_CONST;
  main_output_weights_array.data = weights + 315648;
  main_output_weights_array.data_start = weights + 315648;
  main_output_bias_array.format |= AI_FMT_FLAG_CONST;
  main_output_bias_array.data = weights + 320768;
  main_output_bias_array.data_start = weights + 320768;
  
  }

  return true;
}

/**  PUBLIC APIs SECTION  *****************************************************/

AI_API_ENTRY
ai_bool ai_network_1_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if ( report && net_ctx )
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_1_MODEL_NAME,
      .model_signature   = AI_NETWORK_1_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = {AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR,
                            AI_TOOLS_API_VERSION_MICRO, 0x0},

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 1139270,
      .n_inputs          = AI_NETWORK_1_IN_NUM,
      .inputs            = AI_BUFFER_OBJ_INIT(
                              AI_BUFFER_FORMAT_FLOAT,
                              28,
                              28,
                              1,
                              1, NULL),
      .n_outputs         = AI_NETWORK_1_OUT_NUM,
      .outputs           = AI_BUFFER_OBJ_INIT(
                              AI_BUFFER_FORMAT_FLOAT,
                              1,
                              1,
                              10,
                              1, NULL),
      .activations       = net_ctx->activations,
      .weights           = net_ctx->params,
      .n_nodes           = 0,
      .signature         = net_ctx->signature,
    };

    AI_FOR_EACH_NODE_DO(node, net_ctx->input_node)
    {
      r.n_nodes++;
    }

    *report = r;

    return ( r.n_nodes>0 ) ? true : false;
  }
  
  return false;
}

AI_API_ENTRY
ai_error ai_network_1_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_network_1_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_handle ai_network_1_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_network_1_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if ( !net_ctx ) return false;

  ai_bool ok = true;
  ok &= network_1_configure_weights(net_ctx, &params->params);
  ok &= network_1_configure_activations(net_ctx, &params->activations);
  
  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_1_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_network_1_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}

#undef AI_NETWORK_1_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

