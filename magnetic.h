#ifndef _MAGNETIC_H_
#define _MAGNETIC_H_

#define MAGNETIC_ADCx ADC1
#define MAGNETIC_CH1x DAD0	//DM0
#define MAGNETIC_CH2x AD19	//DP0

typedef struct
{
	float32 left_mag;
	float32 mid_mag;
	float32 right_mag;
}Magnetic_Class;
extern Magnetic_Class magnetic;

void Magnetic_Adc_Init(void);
void Magnetic_GetAdc(void);

#endif