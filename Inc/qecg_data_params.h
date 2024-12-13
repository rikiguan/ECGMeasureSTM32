/**
  ******************************************************************************
  * @file    qecg_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2024-12-13T14:23:55+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef QECG_DATA_PARAMS_H
#define QECG_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_QECG_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_qecg_data_weights_params[1]))
*/

#define AI_QECG_DATA_CONFIG               (NULL)


#define AI_QECG_DATA_ACTIVATIONS_SIZES \
  { 18048, }
#define AI_QECG_DATA_ACTIVATIONS_SIZE     (18048)
#define AI_QECG_DATA_ACTIVATIONS_COUNT    (1)
#define AI_QECG_DATA_ACTIVATION_1_SIZE    (18048)



#define AI_QECG_DATA_WEIGHTS_SIZES \
  { 103892, }
#define AI_QECG_DATA_WEIGHTS_SIZE         (103892)
#define AI_QECG_DATA_WEIGHTS_COUNT        (1)
#define AI_QECG_DATA_WEIGHT_1_SIZE        (103892)



#define AI_QECG_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_qecg_activations_table[1])

extern ai_handle g_qecg_activations_table[1 + 2];



#define AI_QECG_DATA_WEIGHTS_TABLE_GET() \
  (&g_qecg_weights_table[1])

extern ai_handle g_qecg_weights_table[1 + 2];


#endif    /* QECG_DATA_PARAMS_H */
