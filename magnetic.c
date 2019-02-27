#include "header.h"

Magnetic_Class magnetic;

/***************************************************************
	*	@brief	ADC初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Magnetic_Adc_Init(void)
{
	magnetic.k = 0.8876;
	magnetic.b = 10.89;
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
	
	Magnetic_Find_Zero_Drift();
}

/***************************************************************
	*	@brief	测零漂
	*	@param	无
	*	@note	远离电磁线调用
***************************************************************/
void Magnetic_Find_Zero_Drift(void)
{
//	magnetic.angle_zero_drift = magnetic.angle;
	magnetic.middle_left_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) * 0.95;
	magnetic.middle_right_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) * 0.95;
	magnetic.left_horizontal_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 0.95;
	magnetic.right_horizontal_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 0.95;
	magnetic.left_vertical_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 0.95;
	magnetic.right_vertical_mag_zero_drift = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 0.95;
}

/***************************************************************
	*	@brief	获取电磁
	*	@param	无
	*	@note	一次归一化
***************************************************************/
void Magnetic_Get_Result(void)
{
//	magnetic.middle_left_mag = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) * 100 / 4095;
//	magnetic.middle_right_mag = Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) * 100 / 4095;
//	magnetic.left_horizontal_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 100 / 4095;
//	magnetic.right_horizontal_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 100 / 4095;
//	magnetic.left_vertical_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 100 / 4095;
//	magnetic.right_vertical_mag = Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 100 / 4095;
	magnetic.middle_left_mag = (Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) - magnetic.middle_left_mag_zero_drift) * 100 / 4095;
	if(magnetic.middle_left_mag < 0)
	{
		magnetic.middle_left_mag = 0;
	}
	magnetic.middle_right_mag = (Magnetic_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) - magnetic.middle_right_mag_zero_drift) * 100 / 4095;
	if(magnetic.middle_right_mag < 0)
	{
		magnetic.middle_right_mag = 0;
	}
	magnetic.left_horizontal_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) - magnetic.left_horizontal_mag_zero_drift) * 100 / 4095;
	if(magnetic.left_horizontal_mag < 0)
	{
		magnetic.left_horizontal_mag = 0;
	}
	magnetic.right_horizontal_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) - magnetic.right_horizontal_mag_zero_drift) * 100 / 4095;
	if(magnetic.right_horizontal_mag < 0)
	{
		magnetic.right_horizontal_mag = 0;
	}
	magnetic.left_vertical_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) - magnetic.left_vertical_mag_zero_drift) * 100 / 4095;
	if(magnetic.left_vertical_mag < 0)
	{
		magnetic.left_vertical_mag = 0;
	}
	magnetic.right_vertical_mag = (Magnetic_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) - magnetic.right_vertical_mag_zero_drift) * 100 / 4095;
	if(magnetic.right_vertical_mag < 0)
	{
		magnetic.right_vertical_mag = 0;
	}
}

/***************************************************************
	*	@brief	获取转换结果
	*	@param	adcx：ADC类型，chn：ADC通道
	*	@note	返回结果未归一化
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
		//取中值
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
	*	@brief	电磁位置解算
	*	@param	无
	*	@note	无
***************************************************************/
void Magnetic_Solution(void)
{
	Magnetic_Get_Result();
	magnetic.angle = (atan(magnetic.left_horizontal_mag / magnetic.left_vertical_mag) + atan(magnetic.right_horizontal_mag / magnetic.right_vertical_mag)) / 2;
	magnetic.left_equivalent = magnetic.left_horizontal_mag / sin(magnetic.angle);
	magnetic.right_equivalent = magnetic.right_horizontal_mag / sin(magnetic.angle);
	magnetic.angle = 90 - 57.3 * magnetic.angle;
}

/***************************************************************
	*	@brief	偏差映射
	*	@param	无
	*	@note	无
***************************************************************/
void Magnetic_Error_Mapping(void)
{
	magnetic.mapping_error[servo.error[0] + 60] = (magnetic.right_equivalent - magnetic.left_equivalent) / (magnetic.right_equivalent + magnetic.left_equivalent) * 100;
}