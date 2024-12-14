#ifndef FIRPROCESS_H
#define FIRPROCESS_H


#include "stdint.h" 
void firProcessQ15(int16_t *fir_inputbuf, int16_t *fir_outputbuf,uint32_t blockSize);
void firProcessFT(float *fir_inputbuf, float *fir_outputbuf,uint32_t blockSize);
#endif
