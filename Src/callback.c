#include "callback.h"
#include "graphic.h"
#include "utils.h"
#include <string.h>
#include "filter.h"
#include "ai.h"

uint8_t freshNum = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        // 心电采集读取
        freshNum = (freshNum + 1) % 5;
        pos += point_num_each_tick;
        if (freshNum == 0)
        {
            RenderChart1();
        }
        tick++;
    }
    if (htim->Instance == TIM7)
    {
        //printf("begin");
        convert_processed_buf(max_val);
        if (ai_run(in_data, out_data, ConvertedBuf, AI_QECG_IN_1_SIZE) != 0)
        {
            return ;
        }
        updateAIResults(out_data, AI_QECG_OUT_1_SIZE);
        // for (int i = 0; i < AI_QECG_OUT_1_SIZE; i++)
        // {
        //     //printf("%.2f,  ", out_data[i]);
        // }
        //printf("end\n");
    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (ADCState == 0)
    {
        memcpy(DATABuf, ADCBuf + 140, 2 * 94);
        memcpy(ADCBuf, ADCBuf + 188, 2 * 140);
        ADCState = 1;
        Opt_ADC_Value(ADCBuf + 140 - 140, ProcessedBuf + 188 * ADCProcessedBufState, 3, 2, 94, 70, 70);
    }
}
uint8_t initFlag = 0;

// ADC Full 测满返回
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (ADCState == 1)
    {

        memcpy(DATABuf + 94, ADCBuf + 94 + 140, 2 * 94);
        ADCState = 2;
        Opt_ADC_Value(ADCBuf + 140 - 140 + 94, ProcessedBuf + 188 * ADCProcessedBufState + 94, 3, 2, 94, 70, 70);
        ADCProcessedBufState = (ADCProcessedBufState + 1) % 3;

        int addition = 0;
        if (initFlag == 0 & ADCProcessedBufState == 2)
        {
            pos = 0;
            initFlag = 1;
        }
        else if (ADCProcessedBufState == 2)
        {
            pos = pos - 188 * 3;
            addition = (int)pos;
            pos = float_int_mod(pos, 564);
        }
        else if (ADCProcessedBufState == 1)
        {
            pos = float_int_mod(pos, 564);
            addition = (int)pos - 188 * 2;
        }
        else if (ADCProcessedBufState == 0)
        {
            pos = float_int_mod(pos, 564);
            addition = (int)pos - 188;
        }
        if (addition > 10)
        {
            addition = 10;
        }
        if (addition < -10)
        {
            addition = -10;
        }
        point_num_each_tick = (188.0f - addition) / (tick - last_dma_tick);
        last_dma_tick = tick;
    }
}
