#include "filter.h"

#include <stdlib.h>
#include <string.h>
#include "stdint.h"
#include "firProcess.h"
#define ADC_VALUE_NUM 300
uint16_t temp_value[ADC_VALUE_NUM];
uint16_t temp_value1[ADC_VALUE_NUM];
float ftemp_value[ADC_VALUE_NUM];
float ftemp_value1[ADC_VALUE_NUM];
void extract_values(uint16_t * input,uint16_t *output, int pre, int num) {
    memcpy(output, &input[pre], num * sizeof(int16_t));
}
void convert_adc_to_ftemp(uint16_t *adc_value, float *ftemp_value, int size) {
    for (int i = 0; i < size; i++) {
        ftemp_value[i] = (adc_value[i] / 4095.0f) * 10.0f;
    }
}
void convert_ftemp_to_filtered(float *ftemp_value, uint16_t *filteredValues, int size,int pre) {
    for (int i = 0; i < size; i++) {
        filteredValues[i] = (uint16_t)(ftemp_value[pre+i] / 10.0f * 4095.0f);
    }
}

void overSamplingAvg(uint16_t *input, uint16_t *output, int trueLength, int windowSize) {
    uint32_t *window = (uint32_t *)malloc(windowSize * sizeof(uint32_t));
    uint8_t count = 0;
    uint32_t result = 500;
    for (int i = 0; i < trueLength; i++) {
        for (int w = 0; w < windowSize; w++) {
            avgFilterLazy((uint32_t)input[i*windowSize + w], window, &count, &result, windowSize);
        }
        output[i] = (uint16_t)result;
    }
    free(window);
}

void Opt_ADC_Value(uint16_t* adc_value, uint16_t* filteredValues, uint8_t MedWindowSize, uint8_t AvgWindowSize,uint16_t num,uint16_t pre,uint16_t behind)
{
    //1
    // medianFilter(adc_value, temp_value, num+pre+behind, MedWindowSize,0,0);
    // avgFilterArray(temp_value, filteredValues, num, AvgWindowSize,pre,behind);

    //2
    // firProcessQ15((int16_t *)adc_value, (int16_t *)temp_value1,num+pre+behind);
    // extract_values(temp_value1,filteredValues,pre,num);
    
    //3
    //extract_values(adc_value,filteredValues,pre,num);

    //4
    // convert_adc_to_ftemp(adc_value, ftemp_value, num+pre+behind);
    // firProcessFT(ftemp_value, ftemp_value1);
    // convert_ftemp_to_filtered(ftemp_value1, filteredValues, num,70);

    // //5
    // convert_adc_to_ftemp(adc_value, ftemp_value, num+pre+behind);
    // firProcessFT(ftemp_value, ftemp_value1,num+pre+behind);
    // convert_ftemp_to_filtered(ftemp_value1, temp_value1, num+behind+pre-70,70);
    // medianFilter(temp_value1, temp_value, num+pre+behind-70, MedWindowSize,0,0);
    // avgFilterArray(temp_value, filteredValues, num, AvgWindowSize,pre-35,behind-35);

    //6 with 10 pre&behind 
    // convert_adc_to_ftemp(adc_value, ftemp_value, num+pre+behind);
    // firProcessFT(ftemp_value, ftemp_value1,num+pre+behind);
    // convert_ftemp_to_filtered(ftemp_value1, temp_value1, num+10,130);
    // medianFilter(temp_value1, temp_value, num+10, MedWindowSize,0,0);
    // avgFilterArray(temp_value, filteredValues, num, AvgWindowSize,5,5);

    //only fir
    convert_adc_to_ftemp(adc_value, ftemp_value, num+pre+behind);
    firProcessFT(ftemp_value, ftemp_value1,num+pre+behind);
    convert_ftemp_to_filtered(ftemp_value1, filteredValues, num,140);
}

int compare(const void *a, const void *b)
{
    return (*(uint16_t *)a - *(uint16_t *)b);
}

void medianFilter(uint16_t *input, uint16_t *output, int length, int windowSize,uint16_t pre,uint16_t behind)
{
 

    int halfWindow = windowSize / 2;
    // uint16_t window[medianFilter_WindowSize];
    uint16_t *window = (uint16_t *)malloc(windowSize * sizeof(uint16_t));

    for (int i = 0; i < length+pre+behind; i++)
    {

        for (int j = 0; j < windowSize; j++)
        {
            int index = i + j - halfWindow;
            if (index < 0)
                index = 0;
            if (index >= length+pre+behind)
                index = length+pre+behind - 1;
            window[j] = input[index];
        }
        qsort(window, windowSize, sizeof(uint16_t), compare);
        if(i>=pre&&i<length+pre){
          output[i-pre] = window[halfWindow];
        }
    }
    free(window);
}

void avgFilterLazy_FLOAT(float input, float *window, uint8_t *count, float *result, uint8_t windowSize)
{
    (window)[(*count)] = input;
    (*count) = ((*count) + 1) % windowSize;
    if ((*count) == 0)
    {
        float totalDiff = 0;
        for (int i = 0; i < windowSize; i++)
        {
            totalDiff += (window)[i];
        }
        (*result) = totalDiff / (windowSize + 0.0f);
    }
}

void avgFilterLazy(uint32_t input, uint32_t *window, uint8_t *count, uint32_t *result, uint8_t windowSize)
{
    (window)[(*count)] = input;
    (*count) = ((*count) + 1) % windowSize;
    if ((*count) == 0)
    {
        uint32_t totalDiff = 0;
        for (int i = 0; i < windowSize; i++)
        {
            totalDiff += (window)[i];
        }
        (*result) = totalDiff / windowSize;
    }
}

void avgFilter_FLOAT(float input, float *window, uint8_t *count, float *result, uint8_t windowSize)
{

    window[*count] = input;
    *count = (*count + 1) % windowSize;
    float total = 0;
    uint8_t validCount = (*count == 0) ? windowSize : *count;
    for (int i = 0; i < validCount; i++)
    {
        total += window[i];
    }
    *result = total / validCount;
}

void avgFilter(uint32_t input, uint32_t *window, uint8_t *count, uint32_t *result, uint8_t windowSize)
{

    window[*count] = input;
    *count = (*count + 1) % windowSize;
    uint32_t total = 0;
    uint8_t validCount = (*count == 0) ? windowSize : *count;
    for (int i = 0; i < validCount; i++)
    {
        total += window[i];
    }
    *result = total / validCount;
}

void avgFilterArray(uint16_t *input, uint16_t *output, int length, int windowSize,uint16_t pre,uint16_t behind)
{

    uint32_t *window = (uint32_t *)malloc(windowSize * sizeof(uint32_t));
    // memset(window, 0, windowSize * sizeof(uint32_t));
    uint8_t count = 0;
    uint32_t result = 500;
    for (int i = 0; i < length+pre+behind; i++)
    {
        for (int w = 0; w < windowSize; w++)
        {
            if (i + w >= length+pre+behind)
                break;
            avgFilterLazy((uint32_t)input[i + w], window, &count, &result, (uint8_t)windowSize);
            
        }
        if(i>=pre&&i<length+pre){
            output[i-pre] = (uint16_t)result;
        }
    }
    free(window);
}
