#ifndef _AI_H
#define _AI_H

#include "main.h"
#include "qecg_data.h"
#include "qecg.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int ai_init(void);
int ai_run(ai_float *in_data, ai_float *out_data, float *data, int length);
void convert_processed_buf(uint16_t maxval);

extern float in_data[AI_QECG_IN_1_SIZE];
extern float out_data[AI_QECG_OUT_1_SIZE]; 
extern float ConvertedBuf[AI_QECG_IN_1_SIZE];
#endif
