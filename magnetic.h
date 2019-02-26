#ifndef _MAGNETIC_H_
#define _MAGNETIC_H_

#define MAGNETIC_ADCx ADC1
#define MAGNETIC_CH1x AD19	//DM0
#define MAGNETIC_CH2x AD20	//DM1
#define MAGNETIC_CH3x DAD0	//DP0
#define MAGNETIC_CH4x DAD1	//DP1

typedef struct
{
	int16 middle_left_mag;
	int16 middle_right_mag;
	int16 left_horizontal_mag;
	int16 right_horizontal_mag;
	int16 left_vertical_mag;
	int16 right_vertical_mag;
	int16 mapping_error[120];
}Magnetic_Class;
extern Magnetic_Class magnetic;

void Magnetic_Adc_Init(void);
void Magnetic_Get_Result(void);
float32 Magnetic_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn);
void Magnetic_Solution(void);

#endif