#ifndef _GRAPHIC_H
#define _GRAPHIC_H


#include "LCD/lcd_init.h"
#include "LCD/lcd.h"
#include "LCD/pic.h"
#include "main.h"

void RenderChart1(void);
void DrawLineArray(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t *array, uint16_t width);
void draw_data(void);
#endif
