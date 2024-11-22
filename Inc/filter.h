#ifndef _FIL_H
#define _FIL_H

#include "stdint.h"
void Opt_ADC_Value(uint16_t* adc_value, uint16_t* filteredValues, uint8_t MedWindowSize, uint8_t AvgWindowSize,uint16_t num,uint16_t pre,uint16_t behind);
void medianFilter(uint16_t *input, uint16_t *output, int length, int windowSize,uint16_t pre,uint16_t behind);
void avgFilter(uint32_t input, uint32_t *window, uint8_t *count, uint32_t *result, uint8_t windowSize);
void avgFilterLazy(uint32_t input, uint32_t *window, uint8_t *count, uint32_t *result, uint8_t windowSize);
void avgFilterArray(uint16_t *input, uint16_t *output, int length, int windowSize,uint16_t pre,uint16_t behind);
void avgFilterLazy_FLOAT(float input, float *window, uint8_t *count, float *result, uint8_t windowSize);

#endif
