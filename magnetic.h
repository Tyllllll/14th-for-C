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
#define HLEFT       5
#define MIDLEFT     2
#define MIDRIGHT    3
#define HRIGHT      4
#define VLEFT       5
#define VRIGHT      6
typedef struct
{
    int16   a;
    float32 valu_temp[7];
	float32 valu_avr[7];    
    float32 valu_max[7];    //电感最大值
    float32 onceUni[7];     //一次归一化
    float32 valu_offset[7]; //电感零漂
	int16 left_equivalent;
	int16 right_equivalent;
	float32 angle;
	float32 mapping_error[120];
    
    float32 error;
    float32 diff_error;
    float32 s_kp;
    float32 s_kd;
    float32 error_pre1;
}Magnetic_Class;
extern Magnetic_Class magnetic;

void Magnetic_Adc_Init(void);
void Magnetic_findOffset(void);
void Magnetic_findMax(void);
void Magnetic_GetAllAdc(void);
float32 Magnetic_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn);
void Magnetic_Solution(void);
void Magnetic_OnceUnion(void);
void Magnetic_Error_Mapping(void);

#endif