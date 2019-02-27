#ifndef _MAGNETIC_H_
#define _MAGNETIC_H_

#define MAGNETIC_MID_ADCx ADC0
#define MAGNETIC_MID_CH1x AD20	//DM1
#define MAGNETIC_MID_CH2x DAD1	//DP1

#define MAGNETIC_ADCx ADC1
#define MAGNETIC_CH1x DAD0	//DP0
#define MAGNETIC_CH2x DAD1	//DP1
#define MAGNETIC_CH3x AD20	//DM1
#define MAGNETIC_CH4x AD19	//DM0

typedef struct
{
//	int16 middle_left_mag;
//	int16 middle_right_mag;
//	int16 left_horizontal_mag;
//	int16 right_horizontal_mag;
//	int16 left_vertical_mag;
//	int16 right_vertical_mag;
//	int16 middle_left_mag_zero_drift;
//	int16 middle_right_mag_zero_drift;
//	int16 left_horizontal_mag_zero_drift;
//	int16 right_horizontal_mag_zero_drift;
//	int16 left_vertical_mag_zero_drift;
//	int16 right_vertical_mag_zero_drift;
//	int16 left_equivalent;
//	int16 right_equivalent;
	
	float32 middle_left_mag;
	float32 middle_right_mag;
	float32 left_horizontal_mag;
	float32 right_horizontal_mag;
	float32 left_vertical_mag;
	float32 right_vertical_mag;
	float32 middle_left_mag_zero_drift;
	float32 middle_right_mag_zero_drift;
	float32 left_horizontal_mag_zero_drift;
	float32 right_horizontal_mag_zero_drift;
	float32 left_vertical_mag_zero_drift;
	float32 right_vertical_mag_zero_drift;
	float32 left_equivalent;
	float32 right_equivalent;
	float32 angle;
	float32 mapping_error[120];
	float32 k;
	float32 b;
}Magnetic_Class;
extern Magnetic_Class magnetic;

void Magnetic_Adc_Init(void);
void Magnetic_Find_Zero_Drift(void);
void Magnetic_Get_Result(void);
float32 Magnetic_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn);
void Magnetic_Solution(void);

void Magnetic_Error_Mapping(void);

#endif