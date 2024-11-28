#include "filter.h"

#include <stdlib.h>
#include <string.h>
#include "stdint.h"
#define ADC_VALUE_NUM 200
uint16_t temp_value[ADC_VALUE_NUM];

void Opt_ADC_Value(uint16_t* adc_value, uint16_t* filteredValues, uint8_t MedWindowSize, uint8_t AvgWindowSize,uint16_t num,uint16_t pre,uint16_t behind)
{
    medianFilter(adc_value, temp_value, num+pre+behind, MedWindowSize,0,0);
    avgFilterArray(temp_value, filteredValues, num, AvgWindowSize,pre,behind);
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
