#ifndef _MAGNETIC_H_
#define _MAGNETIC_H_

#define MAGNETIC_ADCx ADC1
#define MAGNETIC_CH1x AD19	//DM0
#define MAGNETIC_CH2x DAD0	//DP0
#define MAGNETIC_CH3x DAD1	//DP1
#define MAGNETIC_CH4x AD20	//DM1

typedef struct
{
	int16 left1_mag;
	int16 left2_mag;
	int16 right1_mag;
	int16 right2_mag;
}Magnetic_Class;
extern Magnetic_Class magnetic;

void Magnetic_Adc_Init(void);
void Magnetic_Get_Result(void);
float32 Magnetic_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn);
//void Magnetic_GetAdc(void);

#endif