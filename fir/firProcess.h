#ifndef FIRPROCESS_H
#define FIRPROCESS_H


#include "stdint.h" 
void firProcess(int16_t *fir_inputbuf, int16_t *fir_outputbuf);
void firProcessFT(float *fir_inputbuf, float *fir_outputbuf);
#endif
