#ifndef _MAGNETIC_H_
#define _MAGNETIC_H_

#define MAGNETIC_MID_ADCx	ADC0
#define MAGNETIC_MID_CH1x	AD20	//DM1
#define MAGNETIC_MID_CH2x	DAD1	//DP1
#define HONGWAICEJU_CHx		AD19	//DM0

#define MAGNETIC_ADCx	ADC1
#define MAGNETIC_CH1x	DAD0	//DP0
#define MAGNETIC_CH2x	DAD1	//DP1
#define MAGNETIC_CH3x	AD20	//DM1
#define MAGNETIC_CH4x	AD19	//DM0

#define MAGNETIC_SAMPLING_POINTS 5

#define MID_L 0
#define MID_R 1
#define HORIZONTAL_L 2
#define HORIZONTAL_R 3
#define VERTICAL_L 4
#define VERTICAL_R 5
#define EQUIVALENT_L 6
#define EQUIVALENT_R 7

typedef struct
{
	float32 value[8];
	float32 value_zero_drift[6];
	float32 max_value[6];
	float32 angle;
	int16 kp_default;
	int16 kp;
	int16 kd;
	float32 error[2];
	float32 error_differ;
	float32 mapping_error[120];
	float32 correction;
	float32 hongwaiceju;
}Magnetic_Class;
extern Magnetic_Class magnetic;

void Magnetic_Adc_Init(void);
void Magnetic_Find_Zero_Drift(void);
void Magnetic_Find_Max_Value(void);
void Magnetic_Get_Result(void);
float32 Magnetic_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn);
void Magnetic_Solution(void);
uint8 Magnetic_Lose_Line(void);

#endif