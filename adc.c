#include "header.h"

Magnetic_Class magnetic;
Infrared_Class infrared;

/***************************************************************
	*	@brief	ADC初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Adc_Init(void)
{
	magnetic.kd = 0;
	static ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Adcx = MAGNETIC_ADCx;
	ADC_InitStructure.ADC_DiffMode = ADC_SE;
	ADC_InitStructure.ADC_BitMode = SE_12BIT;
	ADC_InitStructure.ADC_SampleTimeCfg = SAMTIME_SHORT;
	ADC_InitStructure.ADC_HwAvgSel = HW_4AVG;
	ADC_InitStructure.ADC_CalEnable = TRUE;
	if(!LPLD_ADC_Init(ADC_InitStructure))
		return STATUS_FAILED;
	if(!LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH1x))
		return STATUS_FAILED;
	if(!LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH2x))
		return STATUS_FAILED;
	if(!LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH3x))
		return STATUS_FAILED;
	if(!LPLD_ADC_Chn_Enable(MAGNETIC_ADCx, MAGNETIC_CH4x))
		return STATUS_FAILED;
	
	ADC_InitStructure.ADC_Adcx = MAGNETIC_MID_ADCx;
	if(!LPLD_ADC_Init(ADC_InitStructure))
		return STATUS_FAILED;
	if(!LPLD_ADC_Chn_Enable(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x))
		return STATUS_FAILED;
	if(!LPLD_ADC_Chn_Enable(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x))
		return STATUS_FAILED;
	if(!LPLD_ADC_Chn_Enable(INFRARED_ADCx, INFRARED_CHx))
		return STATUS_FAILED;
	return STATUS_OK;
}

/***************************************************************
	*	@brief	测零漂
	*	@param	无
	*	@note	远离电磁线调用
***************************************************************/
uint8 Adc_Find_Zero_Drift(void)
{
	magnetic.value_zero_drift[MID_L] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x);
	magnetic.value_zero_drift[MID_R] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x);
	magnetic.value_zero_drift[HORIZONTAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x);
	magnetic.value_zero_drift[HORIZONTAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x);
	magnetic.value_zero_drift[VERTICAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x);
	magnetic.value_zero_drift[VERTICAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x);
	for(uint8 i = 0; i < 6; i++)
	{
		if(magnetic.value_zero_drift[i] > 250)
		{
			return STATUS_FAILED;
		}
	}
	return STATUS_OK;
}

/***************************************************************
	*	@brief	测最大值
	*	@param	无
	*	@note	无
***************************************************************/
void Adc_Find_Max_Value(void)
{
	float32 temp[6];
	OLED_Fill(0x00);
	OLED_Put6x8Str(20, 1, "checking");
	OLED_Put6x8Str(20, 2, "magnetic");
	OLED_Put6x8Str(20, 3, "max value");
	OLED_Put6x8Str(20, 5, "press KEY6");
	OLED_Put6x8Str(20, 6, "to continue.");
	while(1)
	{
		temp[MID_L] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x);
		temp[MID_R] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x);
		temp[HORIZONTAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x);
		temp[HORIZONTAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x);
		temp[VERTICAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x);
		temp[VERTICAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x);
		if(magnetic.max_value[MID_L] < temp[MID_L])
		{
			magnetic.max_value[MID_L] = temp[MID_L];
		}
		if(magnetic.max_value[MID_R] < temp[MID_R])
		{
			magnetic.max_value[MID_R] = temp[MID_R];
		}
		if(magnetic.max_value[HORIZONTAL_L] < temp[HORIZONTAL_L])
		{
			magnetic.max_value[HORIZONTAL_L] = temp[HORIZONTAL_L];
		}
		if(magnetic.max_value[HORIZONTAL_R] < temp[HORIZONTAL_R])
		{
			magnetic.max_value[HORIZONTAL_R] = temp[HORIZONTAL_R];
		}
		if(magnetic.max_value[VERTICAL_L] < temp[VERTICAL_L])
		{
			magnetic.max_value[VERTICAL_L] = temp[VERTICAL_L];
		}
		if(magnetic.max_value[VERTICAL_R] < temp[VERTICAL_R])
		{
			magnetic.max_value[VERTICAL_R] = temp[VERTICAL_R];
		}
		if(KEY6 == 0)
		{
			Key_Delay();
			if(KEY6 == 0)
			{
				break;
			}
		}
	}
}

/***************************************************************
	*	@brief	获取电磁
	*	@param	无
	*	@note	一次归一化
***************************************************************/
void Adc_Magnetic_Get_Result(void)
{
	//调电位器用
//	magnetic.value[MID_L] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) * 100 / 4095;
//	magnetic.value[MID_R] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) * 100 / 4095;
//	magnetic.value[HORIZONTAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 100 / 4095;
//	magnetic.value[HORIZONTAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 100 / 4095;
//	magnetic.value[VERTICAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 100 / 4095;
//	magnetic.value[VERTICAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 100 / 4095;
	
	magnetic.value[MID_L] = (Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) - magnetic.value_zero_drift[MID_L]) * 100 / magnetic.max_value[MID_L];
	if(magnetic.value[MID_L] < 0)
	{
		magnetic.value[MID_L] = 0;
	}
	magnetic.value[MID_R] = (Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) - magnetic.value_zero_drift[MID_R]) * 100 / magnetic.max_value[MID_R];
	if(magnetic.value[MID_R] < 0)
	{
		magnetic.value[MID_R] = 0;
	}
	magnetic.value[HORIZONTAL_L] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) - magnetic.value_zero_drift[HORIZONTAL_L]) * 100 / magnetic.max_value[HORIZONTAL_L];
	if(magnetic.value[HORIZONTAL_L] < 0)
	{
		magnetic.value[HORIZONTAL_L] = 0;
	}
	magnetic.value[HORIZONTAL_R] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) - magnetic.value_zero_drift[HORIZONTAL_R]) * 100 / magnetic.max_value[HORIZONTAL_R];
	if(magnetic.value[HORIZONTAL_R] < 0)
	{
		magnetic.value[HORIZONTAL_R] = 0;
	}
	magnetic.value[VERTICAL_L] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) - magnetic.value_zero_drift[VERTICAL_L]) * 100 / magnetic.max_value[VERTICAL_L];
	if(magnetic.value[VERTICAL_L] < 0)
	{
		magnetic.value[VERTICAL_L] = 0;
	}
	magnetic.value[VERTICAL_R] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) - magnetic.value_zero_drift[VERTICAL_R]) * 100 / magnetic.max_value[VERTICAL_R];
	if(magnetic.value[VERTICAL_R] < 0)
	{
		magnetic.value[VERTICAL_R] = 0;
	}
}

/***************************************************************
	*	@brief	获取转换结果
	*	@param	adcx：ADC类型，chn：ADC通道
	*	@note	返回结果未归一化
***************************************************************/
float32 Adc_GetAdc(ADC_Type *adcx, AdcChnEnum_Type chn)
{
	uint8 i, j;
	float32 window[3], temp;
	float32 sum = 0;
	for(i = 0; i < MAGNETIC_SAMPLING_POINTS; i++)
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
	return sum / MAGNETIC_SAMPLING_POINTS;
}

/***************************************************************
	*	@brief	电磁位置解算
	*	@param	无
	*	@note	无
***************************************************************/
void Adc_Magnetic_Solution(void)
{
	magnetic.angle = (atan(magnetic.value[HORIZONTAL_L] / magnetic.value[VERTICAL_L]) + atan(magnetic.value[HORIZONTAL_R] / magnetic.value[VERTICAL_R])) / 2;
	magnetic.value[EQUIVALENT_L] = magnetic.value[HORIZONTAL_L] / sin(magnetic.angle);
	magnetic.value[EQUIVALENT_R] = magnetic.value[HORIZONTAL_R] / sin(magnetic.angle);
	if(feature.straight_state == 1)
	{
		magnetic.correction = magnetic.value[EQUIVALENT_R] - magnetic.value[EQUIVALENT_L];
	}
}

/***************************************************************
	*	@brief	电磁丢线
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Adc_Magnetic_Lose_Line(void)
{
	if((int16)(magnetic.value[MID_L] + magnetic.value[MID_R] + magnetic.value[HORIZONTAL_L] + magnetic.value[HORIZONTAL_R]) < 6)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/***************************************************************
	*	@brief	测距
	*	@param	无
	*	@note	无
***************************************************************/
void Adc_Measure_Distance(void)
{
	infrared.distance = (int16)Adc_GetAdc(INFRARED_ADCx, INFRARED_CHx);
}