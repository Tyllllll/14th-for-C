#ifndef _ENCODER_H_
#define _ENCODER_H_

#define ENCODER1_PTx PTA
#define ENCODER1_Pin1x GPIO_Pin8
#define ENCODER1_Pin2x GPIO_Pin9
#define ENCODER1_FTMx FTM1
#define ENCODER1_PORTPin1x PTA8
#define ENCODER1_PORTPin2x PTA9
#define ENCODER2_PTx PTB
#define ENCODER2_Pin1x GPIO_Pin18
#define ENCODER2_Pin2x GPIO_Pin19
#define ENCODER2_FTMx FTM2
#define ENCODER2_PORTPin1x PTB18
#define ENCODER2_PORTPin2x PTB19

#define ENCODER_NUM_PER_METER_LEFT  5700 //编码器一米累加的脉冲数
#define ENCODER_NUM_PER_METER_RIGHT  5723 //编码器一米累加的脉冲数

typedef struct
{
	int left_num;
	int right_num;
}Encoder_Class;
extern Encoder_Class encoder;

void Encoder_Init(void);
void Encoder_Get(void);

#endif