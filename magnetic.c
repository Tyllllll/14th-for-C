#include "header.h"
Magnetic_Class magnetic;

/***************************************************************
	*	@brief	ADC初始化
	*	@param	无
	*	@note	无
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
}

/***************************************************************
	*	@brief	获取转换结果
	*	@param	无
	*	@note	无
***************************************************************/
void Magnetic_GetAdc(void)
{
	float data1 = 0;
	float data2 = 0;
	for(int i = 0; i < 5; i++)
	{
		data1 += LPLD_ADC_Get(MAGNETIC_ADCx, MAGNETIC_CH1x);
	}
	for(int i = 0; i < 5; i++)
	{
		data2 += LPLD_ADC_Get(MAGNETIC_ADCx, MAGNETIC_CH2x);
	}
	magnetic.right_mag = (data1 / 5) * 100 / 4095;
	magnetic.left_mag = (data2 / 5) * 100 / 4095;
}	