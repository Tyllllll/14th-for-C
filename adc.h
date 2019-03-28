#ifndef _ADC_H_
#define _ADC_H_

#ifdef NEWCAR

#define INFRARED_ADCx	ADC0
#define INFRARED_CHx		AD19	//DM0

#define MAGNETIC_MID_ADCx	ADC0
#define MAGNETIC_MID_CH1x	AD20	//DM1
#define MAGNETIC_MID_CH2x	DAD1	//DP1

#define MAGNETIC_ADCx	ADC1
#define MAGNETIC_CH1x	AD19	//DP0
#define MAGNETIC_CH2x	DAD1	//DP1
#define MAGNETIC_CH3x	AD20	//DM1
#define MAGNETIC_CH4x	DAD0	//DM0

#else

#define INFRARED_ADCx	ADC0
#define INFRARED_CHx		AD19	//DM0

#define MAGNETIC_MID_ADCx	ADC0
#define MAGNETIC_MID_CH1x	AD20	//DM1
#define MAGNETIC_MID_CH2x	DAD1	//DP1

#define MAGNETIC_ADCx	ADC1
#define MAGNETIC_CH1x	DAD0	//DP0
#define MAGNETIC_CH2x	DAD1	//DP1
#define MAGNETIC_CH3x	AD20	//DM1
#define MAGNETIC_CH4x	AD19	//DM0

#endif

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
	float value[8];
	float value_zero_drift[6];
	float max_value[6];
	float angle;
	int16 kp;
	int16 kp_default;
	int16 kd;
	float error[2];
	float error_differ;
	float mapping_error[120];
	float correction;
}Magnetic_Class;
extern Magnetic_Class magnetic;

typedef struct
{
	int16 distance;
}Infrared_Class;
extern Infrared_Class infrared;
extern float Position_transit_short[4];
extern uint8 Position;
uint8 Adc_Init(void);
uint8 Adc_Find_Zero_Drift(void);
void Adc_Find_Max_Value(void);
void Adc_Magnetic_Get_Result(void);
float Adc_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn);
void Adc_Magnetic_Solution(void);
uint8 Adc_Magnetic_Lose_Line(void);
void Adc_Measure_Distance(void);
void Data_analyse(void);
#endif