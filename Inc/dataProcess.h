#ifndef _DPROCESS_H
#define _DPROCESS_H

#include "stdint.h"
#include "main.h"
#include "LCD/lcd_init.h"
#include "LCD/lcd.h"
#include "LCD/pic.h"


uint8_t get_prev_buf_idx(uint8_t current_idx);
uint8_t get_next_buf_idx(uint8_t current_idx);
uint16_t get_current_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i);
uint16_t get_prev_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i);
uint16_t get_next_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i);
void find_min_max(void);
void detect_r_peaks(uint16_t current_buf_idx);


#endif
