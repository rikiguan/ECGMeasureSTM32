#include "graphic.h"
#include <string.h>
#include "stdio.h"
// 将波形转化为二维图表进行刷屏
#define CHART_X_START 10
#define CHART_Y_START 25
#define Y_TOTAL 190
#define X_TOTAL 230

int posC = 0;

uint16_t DrawBuff1[Y_TOTAL][10];

void RenderChart1()
{
    int posS = pos;
    for (posC = 0; posC < X_TOTAL; posC += 10)
    {
        memset(DrawBuff1, BLACK, Y_TOTAL * 10 * 2);

        for (int i = 0; i < 5; i++)
        {//TODO 有bug，暂行解决方案(Y_TOTAL-10)
            DrawLineArray(i * 2, Y_TOTAL-5 - (int)(((ProcessedBuf[(i + posS) % 564]) * (Y_TOTAL-10) / 4095.0f)) - 1, i * 2 + 2, Y_TOTAL-5 - (int)(((ProcessedBuf[(i + posS + 1) % 564]) * (Y_TOTAL-10) / 4095.0f)) - 1, YELLOW, (uint16_t *)DrawBuff1, 10);
				}
        for (int y = Y_TOTAL - 1; y >= 0; y -= 20)
        {
            for (int x = 0; x < 10; x++)
            {
                if ((posC + x) % 10 < 5)
                {
                    DrawBuff1[y][x] = RED;
                    if (posS + x - 188 < 5 && posS + x - 188 > 0)
                    {
                        DrawBuff1[y][x] = GREEN;
                    }
                    if (posS + x - 188 * 2 < 5 && posS + x - 188 * 2 > 0)
                    {
                        DrawBuff1[y][x] = BLUE;
                    }
                    if ((posS + x) % 564 < 5 && (posS + x) % 564 > 0)
                    {
                        DrawBuff1[y][x] = YELLOW;
                    }
                }
            }
        }
        for (int x = 0; x < 10; x++)
        {
            DrawBuff1[Cursor][x] = GREEN;
        }

        LCD_FillArray(CHART_X_START + posC, CHART_Y_START, CHART_X_START + posC + 10 - 1, CHART_Y_START + Y_TOTAL - 1, (uint8_t *)DrawBuff1);
        posS += 5;
    }
    draw_data();
}

void DrawLineArray(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t *array, uint16_t width)
{

    if(x1 >= width){
        x1 = width-1;
    }
    if(x2 >= width){
        x2 = width-1;
    }
    if(y1 >= Y_TOTAL){
        y1 = Y_TOTAL-1;
    }
    if(y2 >= Y_TOTAL){
        y2 = Y_TOTAL-1;
    }

    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1; // 画线起点坐标
    uCol = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直恒定�???????????
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平恒定�???????????
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
			
        array[uCol * width + uRow] = color; //
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void draw_ui(void)
{
    LCD_ShowString(80, 220, "Vmax:", BLUE, WHITE, 16, 0);
    LCD_ShowString(160, 220, "Vmin:", BLUE, WHITE, 16, 0);
    LCD_ShowString(0, 220, "Freq:", BLUE, WHITE, 16, 0);
    LCD_ShowString(170, 0, "bpm:", BLUE, WHITE, 16, 0);
    LCD_DrawLine(9, 24, 9, 215, BLACK);
    LCD_DrawLine(0, 215, 240, 215, BLACK);
    LCD_DrawLine(0, 24, 240, 24, BLACK);
    for (int i = 0; i <= 3; i++) // �???????????4个标记：0V�???????????1V�???????????2V�???????????3V
    {
        LCD_DrawLine(7, 215 - 55 * i, 10, 215 - 55 * i, BLACK);
        LCD_ShowIntNum(0, 198 - 55 * i, i, 1, BLACK, WHITE, 16);
    }
    LCD_ShowString(0, 0, "V", BLUE, WHITE, 16, 0);
}


void draw_data(){
    LCD_ShowIntNum(210, 0, heart_rate, 3, RED, WHITE, 16);
    LCD_ShowFloatNum1(120, 220, max_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
    LCD_ShowFloatNum1(200, 220, min_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
    LCD_ShowFloatNum1(40, 220, feq, 3, RED, WHITE, 16);
    displayAIResults();
}
void displayAIResults() {
    char *classStr;
    switch (aiClass) {
        case 0:
            classStr = "Normal           ";
            break;
        case 1:
            classStr = "Supraventricular";
            break;
        case 2:
            classStr = "Ventricular      ";
            break;
        case 3:
            classStr = "Fusion beat      ";
            break;
        case 4:
            classStr = "Unclassifiable   ";
            break;
        default:
            classStr = "Unknown          ";
            break;
    }

    char displayStr[50];
    // LCD_ShowString(0, 0, "                 ", BLUE, WHITE, 16, 0);
    snprintf(displayStr, sizeof(displayStr), "%s", classStr);
    LCD_ShowString(0, 0, (const uint8_t *)displayStr, BLUE, WHITE, 16, 0);
}
