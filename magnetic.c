#include "header.h"

Magnetic_Class magnetic;

/***************************************************************
	*	@brief	ADC��ʼ��
	*	@param	��
	*	@note	��
   5                  6      
   |                  |                  
1---------2-3-----------4          
***************************************************************/
void Magnetic_Adc_Init(void)
{
	static ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Adcx = MAGNETIC_ADCx;
	ADC_InitStructure.ADC_DiffMode = ADC_SE;
	ADC_InitStructure.ADC_BitMode = SE_12BIT;
	ADC_InitStructure.ADC_SampleTimeCfg = SAMTIME_SHORT;
	ADC_InitStructure.ADC_HwAvgSel = HW_4AVG;
	ADC_InitStructure.ADC_CalEnable = TRUE;
	LPLD_ADC_Init(ADC_InitStructure);
	LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH1x);
	LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH2x);
	LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH3x);
	LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH4x);
	
	ADC_InitStructure.ADC_Adcx = MAGNETIC_MID_ADCx;
	LPLD_ADC_Init(ADC_InitStructure);
	LPLD_ADC_Chn_Enable(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x);
	LPLD_ADC_Chn_Enable(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x);
	
	Magnetic_findOffset();
    magnetic.a = 55;
}

/***************************************************************
	*	@brief	����Ư
	*	@param	��
	*	@note	��ʼ�����ã�Զ������ʹ��
***************************************************************/
void Magnetic_findOffset(void)
{
    uint8 i = 0, j = 0;
    //������
    for(i = 1; i <= 6; i++)
    {
        magnetic.valu_offset[i] = 0;
    }
    for(i = 0; i < 3; i++)
    {
        Magnetic_GetAllAdc();
        for(j = 1; j <= 6; j++)
        {
            magnetic.valu_offset[j] += magnetic.valu_avr[j];
        }
    }
    for(i = 1; i <= 6; i++)
    {
        magnetic.valu_offset[i] /= 3;
    }
//	magnetic.angle_zero_drift = magnetic.angle;
//	magnetic.middle_left_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) * 0.95;
//	magnetic.middle_right_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) * 0.95;
//	magnetic.left_horizontal_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 0.95;
//	magnetic.right_horizontal_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 0.95;
//	magnetic.left_vertical_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 0.95;
//	magnetic.right_vertical_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 0.95;
}

/***************************************************************
	*	@brief	��ȡ���
	*	@param	��
	*	@note	δ��һ��
   5                  6      
   |                  |                  
5---------2-3-----------1          
***************************************************************/
void Magnetic_GetAllAdc(void)
{
    uint8 i = 0;
    //�ȶ���ͽ�������
    for(i = 1; i <= 6; i++)
    {
        magnetic.valu_temp[i] = 0;
    }
    for(i = 0; i < 3; i++)
    {
        magnetic.valu_temp[1] += Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x);
        magnetic.valu_temp[2] += Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x);
        magnetic.valu_temp[3] += Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x);
        magnetic.valu_temp[4] += Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x);
        magnetic.valu_temp[5] += Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x);
        magnetic.valu_temp[6] += Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x);
    }
    for(i = 1; i <= 6; i++)
    {
        magnetic.valu_avr[i] = magnetic.valu_temp[i] / 3;
    }
//	magnetic.middle_left_mag = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) * 100 / 4095;
//	magnetic.middle_right_mag = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) * 100 / 4095;
//	magnetic.left_horizontal_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 100 / 4095;
//	magnetic.right_horizontal_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 100 / 4095;
//	magnetic.left_vertical_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 100 / 4095;
//	magnetic.right_vertical_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 100 / 4095;

//	magnetic.middle_left_mag = (Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) - magnetic.middle_left_mag_zero_drift) * 100 / 4095;
//	magnetic.middle_right_mag = (Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) - magnetic.middle_right_mag_zero_drift) * 100 / 4095;
//	magnetic.left_horizontal_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) - magnetic.left_horizontal_mag_zero_drift) * 100 / 4095;
//	magnetic.right_horizontal_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) - magnetic.right_horizontal_mag_zero_drift) * 100 / 4095;
//	magnetic.left_vertical_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) - magnetic.left_vertical_mag_zero_drift) * 100 / 4095;
//	magnetic.right_vertical_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) - magnetic.right_vertical_mag_zero_drift) * 100 / 4095;

}

/***************************************************************
	*	@brief	��ȡת�����
	*	@param	adcx��ADC���ͣ�chn��ADCͨ��
	*	@note	���ؽ��δ��һ��
***************************************************************/
float32 Magnetic_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn)
{
	uint8 n = 5;
	uint8 i, j;
	float32 window[3], temp;
	float32 sum = 0;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < 3; j++)
		{
			window[j] = LPLD_ADC_Get(adcx, chn);
		}
		//ȡ��ֵ
		if(window[0] > window[1])
		{
			temp = window[0];
			window[0] = window[1];
			window[1] = temp;
		}
		if(window[2] > window[1])
		{
			temp = window[1];
		}
		else if(window[2] > window[0])
		{
			temp = window[2];
		}
		else
		{
			temp = window[0];
		}
		sum += temp;
	}
	return sum / n;
}

/***************************************************************
	*	@brief	���λ�ý���
	*	@param	��
	*	@note	��
***************************************************************/
void Magnetic_Solution(void)
{
	Magnetic_OnceUnion();
	magnetic.angle = (atan(magnetic.onceUni[HLEFT] / magnetic.onceUni[VLEFT]) + atan(magnetic.onceUni[HRIGHT] / magnetic.onceUni[VRIGHT])) / 2;
	magnetic.left_equivalent = (int16)(magnetic.onceUni[HLEFT] / sin(magnetic.angle));
	magnetic.right_equivalent = (int16)(magnetic.onceUni[HRIGHT] / sin(magnetic.angle));
    magnetic.angle = 90 - 57.3*magnetic.angle;
}

/***************************************************************
	*	@brief	ƫ��ӳ��
	*	@param	��
	*	@note	��
***************************************************************/
void Magnetic_Error_Mapping(void)
{
	magnetic.mapping_error[servo.error[0] + 60] = (magnetic.right_equivalent - magnetic.left_equivalent) / (magnetic.right_equivalent + magnetic.left_equivalent) * 100;
}

/***************************************************************
	*	@brief	�ҵ���������ֵ
	*	@param	��
	*	@note	��
***************************************************************/
void Magnetic_findMax(void)
{
    uint8 i = 0;
    Magnetic_GetAllAdc();
    for(i = 1; i <= 6; i++)
    {
        if(magnetic.valu_avr[i]-magnetic.valu_offset[i] > magnetic.valu_max[i])
        {
            magnetic.valu_max[i] = magnetic.valu_avr[i] - magnetic.valu_offset[i];
        }
    }
}
/***************************************************************
	*	@brief	���һ�ι�һ��
	*	@param	��
	*	@note	��
***************************************************************/
void Magnetic_OnceUnion(void)
{
    uint8 i = 0;
    Magnetic_GetAllAdc();   //�˲���ADֵ
    //����Ư
    for(i = 1; i <= 6; i++)
    {
        if(magnetic.valu_avr[i] > magnetic.valu_offset[i])
        {
            magnetic.valu_avr[i] -= magnetic.valu_offset[i];
        }
        else
        {
            magnetic.valu_avr[i] = 0;
        }
    }
    //һ�ι�һ������
    for(i = 1; i <= 6; i++)
    {
        magnetic.onceUni[i] = (float32)((magnetic.valu_avr[i] * 100.0f) / magnetic.valu_max[i]);
    }
    
}