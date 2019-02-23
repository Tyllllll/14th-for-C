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
	LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH3x);
	LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH4x);
}

/***************************************************************
	*	@brief	获取ADC值
	*	@param	无
	*	@note	无
***************************************************************/
void Magnetic_GetAdc(void)
{
	uint8 i = 0;
	float32 data = 0;
	for(i = 0; i < 5; i++)
	{
		data += LPLD_ADC_Get(MAGNETIC_ADCx, MAGNETIC_CH1x);
	}
	magnetic.left1_mag = (data / 5) * 100 / 4095;
	data = 0;
	for(i = 0; i < 5; i++)
	{
		data += LPLD_ADC_Get(MAGNETIC_ADCx, MAGNETIC_CH2x);
	}
	magnetic.right1_mag = (data / 5) * 100 / 4095;
	data = 0;
	for(i = 0; i < 5; i++)
	{
		data += LPLD_ADC_Get(MAGNETIC_ADCx, MAGNETIC_CH3x);
	}
	magnetic.left2_mag = (data / 5) * 100 / 4095;
	data = 0;
	for(i = 0; i < 5; i++)
	{
		data += LPLD_ADC_Get(MAGNETIC_ADCx, MAGNETIC_CH4x);
	}
	magnetic.right2_mag = (data / 5) * 100 / 4095;
}

///***************************************************************
//	*	@brief	获取转换结果
//	*	@param	ADC_Type：ADC类型，AdcChnEnum_Type：ADC通道
//	*	@note	返回结果未归一化
//***************************************************************/
//float32 Magnetic_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn)
//{
//	uint8 n = 5;
//	uint16 window[3], temp;
//	uint16 sum = 0;
//	uint8 i, j;
//	for(i = 0; i < n; i++)
//	{
//		for(j = 0; j < 3; j++)
//		{
//			window[j] = LPLD_ADC_Get(MAGNETIC_ADCx, MAGNETIC_CH1x);
//		}
//		//取中值
//		if(window[0] > window[1])
//		{
//			temp = window[0];
//			window[0] = window[1];
//			window[1] = temp;
//		}
//		if(window[2] > window[1])
//		{
//			temp = window[1];
//		}
//		else if(window[2] > window[0])
//		{
//			temp = window[2];
//		}
//		else
//		{
//			temp = window[0];
//		}
//		sum += temp;
//	}
//	return (float32)sum / n;
//}
//
///***************************************************************
//	*	@brief	获取电磁
//	*	@param	无
//	*	@note	一次归一化
//***************************************************************/
//void Magnetic_Get_Result(void)
//{
//	magnetic.left_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 100 / 4095;
//	magnetic.right_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 100 / 4095;
////	magnetic.left2_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 100 / 4095;
////	magnetic.right2_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 100 / 4095;
//}
