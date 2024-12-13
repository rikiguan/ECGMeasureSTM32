#include "dataProcess.h"
#include "filter.h"
#include <stdio.h>
uint8_t get_prev_buf_idx(uint8_t current_idx)
{
    return (current_idx + 2) % 3;
}

uint8_t get_next_buf_idx(uint8_t current_idx)
{
    return (current_idx + 1) % 3;
}

uint16_t get_current_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i)
{
    if (i < 188)
    {
        return buffer1[i];
    }
    else if (i < 376)
    {
        return buffer2[i - 188];
    }
    else
    {
        return buffer3[i - 376];
    }
}

uint16_t get_prev_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i)
{
    if (i < 188)
    {
        return (i > 0) ? buffer1[i - 1] : buffer1[0];
    }
    else if (i < 376)
    {
        return (i > 188) ? buffer2[i - 189] : buffer1[187];
    }
    else
    {
        return (i > 376) ? buffer3[i - 377] : buffer2[187];
    }
}

uint16_t get_next_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i)
{
    if (i < 188)
    {
        return (i < 187) ? buffer1[i + 1] : buffer2[0];
    }
    else if (i < 376)
    {
        return (i < 375) ? buffer2[i - 187] : buffer3[0];
    }
    else
    {
        return (i < 562) ? buffer3[i - 375] : buffer3[187];
    }
}

void find_min_max(void)
{
    // uint16_t max_pos = 0;
    // uint16_t min_pos = 0;
    max_val = 0;
    min_val = UINT16_MAX;
    // 直接遍历整个ProcessedBuf
    for (uint16_t i = 0; i < 564; i++)
    {
        if (ProcessedBuf[i] > max_val)
        {
            max_val = ProcessedBuf[i];
            // max_pos = i;
        }
        if (ProcessedBuf[i] < min_val)
        {
            min_val = ProcessedBuf[i];
            // min_pos = i;
        }
    }
}

#define WINDOW_SIZE_MAIN 3
volatile float captureValues11[WINDOW_SIZE_MAIN];
uint8_t captureIndex11 = 0;

void detect_r_peaks(uint16_t current_buf_idx)
{
    current_buf_idx = (current_buf_idx + 1) % 3;

    uint16_t trigCount = 0; 

    uint16_t *buffer1 = &ProcessedBuf[188 * get_prev_buf_idx(current_buf_idx)]; // 获取前一组开头地�???????????
    uint16_t *buffer2 = &ProcessedBuf[188 * current_buf_idx];                   // 获取当前组开头地�???????????
    uint16_t *buffer3 = &ProcessedBuf[188 * get_next_buf_idx(current_buf_idx)]; // 获取下一组开头地�???????????
    static uint16_t samples_since_last_r = 0;

    for (uint16_t i = 0 + 3; i < 564 - 3; i++)
    {
        samples_since_last_r++; 

        // 获取当前点和相邻点的坐标
        uint16_t current_value = 2000; 
        uint16_t prev_value = get_prev_value(buffer1, buffer2, buffer3, i);
        uint16_t next_value = get_next_value(buffer1, buffer2, buffer3, i);

        if (
            current_value > get_prev_value(buffer1, buffer2, buffer3, i - 1) &&
            current_value < get_next_value(buffer1, buffer2, buffer3, i + 1) &&
            samples_since_last_r > 5 // 至少5个点，找到一个R波上升沿
        )
        {
            trigCount++;
            if (trigCount > 1)
            {
                avgFilter_FLOAT(125 * 1.0f / samples_since_last_r, (float *)captureValues11, &captureIndex11, (float *)&(feq), WINDOW_SIZE_MAIN);
                samples_since_last_r = 0;
                LCD_ShowFloatNum1(40, 220, feq, 3, RED, WHITE, 16);
                heart_rate = (int)(feq * 60);
            }
        }
    }
}
