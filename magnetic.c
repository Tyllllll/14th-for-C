#include "header.h"

Magnetic_Class magnetic;

/***************************************************************
	*	@brief	ADC��ʼ��
	*	@param	��
	*	@note	��
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
}

/***************************************************************
	*	@brief	��ȡ���
	*	@param	��
	*	@note	һ�ι�һ��
***************************************************************/
void Magnetic_Get_Result(void)
{
	magnetic.middle_left_mag = (int16)Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 100 / 4095;
	magnetic.middle_right_mag = (int16)Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 100 / 4095;
	magnetic.left_horizontal_mag = (int16)Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 100 / 4095;
	magnetic.right_horizontal_mag = (int16)Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 100 / 4095;
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
	return (float32)sum / n;
}

/***************************************************************
	*	@brief	���λ�ý���
	*	@param	��
	*	@note	��
***************************************************************/
void Magnetic_Solution(void)
{
	Magnetic_Get_Result();
	
}