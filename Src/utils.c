#include "utils.h"

float float_int_mod(float a, int b)
{
    float res = a - b * ((int)(a / b));
    if (res < 0)
    {
        res += b;
    }
    return res;
}
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
