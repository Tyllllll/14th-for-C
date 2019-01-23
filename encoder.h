#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "common.h"

#define ENCODER1_PTx PTB
#define ENCODER1_Pin1x GPIO_Pin18
#define ENCODER1_Pin2x GPIO_Pin19
#define ENCODER1_FTMx FTM2
#define ENCODER1_PORTPin1x PTB18
#define ENCODER1_PORTPin2x PTB19
#define ENCODER2_PTx PTA
#define ENCODER2_Pin1x GPIO_Pin8
#define ENCODER2_Pin2x GPIO_Pin9
#define ENCODER2_FTMx FTM1
#define ENCODER2_PORTPin1x PTA8
#define ENCODER2_PORTPin2x PTA9

void Encoder_Init(void);

#endif