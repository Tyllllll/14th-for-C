#include "header.h"

Magnetic_Class magnetic;
Infrared_Class laser;
float Position_transit_short[4];
uint8 Position;
/***************************************************************
	*	@brief	ADC��ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
uint8 Adc_Init(void)
{
	magnetic.kp_default = 215;
	magnetic.kd = 260;
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
	*	@brief	����Ư
	*	@param	��
	*	@note	Զ�����ߵ���
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
	*	@brief	�����ֵ
	*	@param	��
	*	@note	�����ŷ������ٽ�ֵ
    *   @time   2019��3��25��10:56:09
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
            Position_transit_short[1] = temp[MID_R];
		}
		if(magnetic.max_value[MID_R] < temp[MID_R])
		{
			magnetic.max_value[MID_R] = temp[MID_R];
            Position_transit_short[2] = temp[MID_L];
		}
		if(magnetic.max_value[HORIZONTAL_L] < temp[HORIZONTAL_L])
		{
			magnetic.max_value[HORIZONTAL_L] = temp[HORIZONTAL_L];
            Position_transit_short[0] = temp[MID_L];
		}
		if(magnetic.max_value[HORIZONTAL_R] < temp[HORIZONTAL_R])
		{
			magnetic.max_value[HORIZONTAL_R] = temp[HORIZONTAL_R];
            Position_transit_short[3] = temp[MID_R];
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
                Position_transit_short[0] = (Position_transit_short[0] - magnetic.value_zero_drift[MID_L]) * 100 / (magnetic.max_value[MID_L] - magnetic.value_zero_drift[MID_L]);
                Position_transit_short[1] = (Position_transit_short[1] - magnetic.value_zero_drift[MID_R]) * 100 / (magnetic.max_value[MID_R] - magnetic.value_zero_drift[MID_R]);
                Position_transit_short[2] = (Position_transit_short[2] - magnetic.value_zero_drift[MID_L]) * 100 / (magnetic.max_value[MID_L] - magnetic.value_zero_drift[MID_L]);
                Position_transit_short[3] = (Position_transit_short[3] - magnetic.value_zero_drift[MID_R]) * 100 / (magnetic.max_value[MID_R] - magnetic.value_zero_drift[MID_R]);
				break;
			}
		}
	}
}

/***************************************************************
	*	@brief	��ȡ���
	*	@param	��
	*	@note	һ�ι�һ��
    *   @time   2019��3��25��11:03:21 ��һ����ʽ�е�bug  
***************************************************************/
void Adc_Magnetic_Get_Result(void)
{
	//����λ����
//	magnetic.value[MID_L] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) * 100 / 4095;
//	magnetic.value[MID_R] = Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) * 100 / 4095;
//	magnetic.value[HORIZONTAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) * 100 / 4095;
//	magnetic.value[HORIZONTAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) * 100 / 4095;
//	magnetic.value[VERTICAL_L] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) * 100 / 4095;
//	magnetic.value[VERTICAL_R] = Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) * 100 / 4095;
	
	magnetic.value[MID_L] = (Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH1x) - magnetic.value_zero_drift[MID_L]) * 100 / (magnetic.max_value[MID_L] - magnetic.value_zero_drift[MID_L]);
	if(magnetic.value[MID_L] < 0)
	{
		magnetic.value[MID_L] = 0;
	}
	magnetic.value[MID_R] = (Adc_GetAdc(MAGNETIC_MID_ADCx, MAGNETIC_MID_CH2x) - magnetic.value_zero_drift[MID_R]) * 100 / (magnetic.max_value[MID_R] - magnetic.value_zero_drift[MID_R]);
	if(magnetic.value[MID_R] < 0)
	{
		magnetic.value[MID_R] = 0;
	}
	magnetic.value[HORIZONTAL_L] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH1x) - magnetic.value_zero_drift[HORIZONTAL_L]) * 100 / (magnetic.max_value[HORIZONTAL_L] - magnetic.value_zero_drift[HORIZONTAL_L]);
	if(magnetic.value[HORIZONTAL_L] < 0)
	{
		magnetic.value[HORIZONTAL_L] = 0;
	}
	magnetic.value[HORIZONTAL_R] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH2x) - magnetic.value_zero_drift[HORIZONTAL_R]) * 100 / (magnetic.max_value[HORIZONTAL_R] - magnetic.value_zero_drift[HORIZONTAL_R]);
	if(magnetic.value[HORIZONTAL_R] < 0)
	{
		magnetic.value[HORIZONTAL_R] = 0;
	}
	magnetic.value[VERTICAL_L] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH3x) - magnetic.value_zero_drift[VERTICAL_L]) * 100 / (magnetic.max_value[VERTICAL_L] - magnetic.value_zero_drift[VERTICAL_L]);
	if(magnetic.value[VERTICAL_L] < 0)
	{
		magnetic.value[VERTICAL_L] = 0;
	}
	magnetic.value[VERTICAL_R] = (Adc_GetAdc(MAGNETIC_ADCx, MAGNETIC_CH4x) - magnetic.value_zero_drift[VERTICAL_R]) * 100 / (magnetic.max_value[VERTICAL_R] - magnetic.value_zero_drift[VERTICAL_R]);
	if(magnetic.value[VERTICAL_R] < 0)
	{
		magnetic.value[VERTICAL_R] = 0.1; //����ֱ�Ӹ�ֵ0 �ǶȽ��㲻�ȶ�
	}
}

/***************************************************************
	*	@brief	��ȡת�����
	*	@param	adcx��ADC���ͣ�chn��ADCͨ��
	*	@note	���ؽ��δ��һ��
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
	return sum / MAGNETIC_SAMPLING_POINTS;
}

/***************************************************************
	*	@brief	���λ�ý���
	*	@param	��
	*	@note	��
***************************************************************/
void Adc_Magnetic_Solution(void)
{
	magnetic.angle = (atan(magnetic.value[HORIZONTAL_L] / magnetic.value[VERTICAL_L]) + atan(magnetic.value[HORIZONTAL_R] / magnetic.value[VERTICAL_R])) / 2;
	magnetic.value[EQUIVALENT_L] = magnetic.value[HORIZONTAL_L] / sin(magnetic.angle);
	magnetic.value[EQUIVALENT_R] = magnetic.value[HORIZONTAL_R] / sin(magnetic.angle);
	if(servo.which == 0)
	{
		magnetic.correction = magnetic.value[EQUIVALENT_R] - magnetic.value[EQUIVALENT_L];
	}
}

/***************************************************************
	*	@brief	��Ŷ���
	*	@param	��
	*	@note	��
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
	*	@brief	��ŷ���
	*	@param	��
	*	@note	��
    *   @time   2019��3��25��10:57:30
***************************************************************/
uint8 max_front, max_front_old, Position, last_Position, errcnt;
void Mag_data_analyse(void)
{
    uint8 i;
    for(i = 0; i < 4; i++)
    {
        if(magnetic.value[max_front] < magnetic.value[i] - 2)
        {
            max_front = i;  //Ѱ��ˮƽ������ǿ���
        }
    }
    if(magnetic.value[max_front] < 40) // ���ĵ��ֵ��Сʱ ����ȡ��ֵ
    {
        max_front = max_front_old;
    }
    else
    {
        max_front_old = max_front;
    }
    /******************** λ�ý��� **********************/
    if(max_front == HORIZONTAL_L)
    {
        if(magnetic.value[MID_L] <= Position_transit_short[0] - 1)
        {
            Position = 0;   //HORIZONTAL_L���
        }
        else
        {
            Position = 1;   //HORIZONTAL_L �� MID_L ֮��
        }
    }
    if(max_front == MID_L)
    {
        if(magnetic.value[MID_R] <= Position_transit_short[1] - 1)
        {
            Position = 1;
        }
        else
        {
            Position = 2;   //MID_L �� MID_R ֮��
        }
    }
    if(max_front == MID_R)
    {
        if(magnetic.value[MID_L] <= Position_transit_short[2] - 1)
        {
            Position = 3;   //HORIZONTAL_R �� MID_R ֮��
        }
        else
        {
            Position = 2;
        }
    }
    if(max_front == HORIZONTAL_R)
    {
        if(magnetic.value[MID_R] <= Position_transit_short[3] - 1)
        {
            Position = 4;   //HORIZONTAL_R�Ҳ�
        }
        else
        {
            Position = 3;
        }
    }    
    if(fabs(Position - last_Position >= 2))  //��ֹλ������
    {
        errcnt ++;
        if(errcnt > 3)
        {
            last_Position = Position;
            errcnt = 0;
        }
        else
        {
            Position = last_Position;
        }
    }
    else
    {
        last_Position = Position;
        errcnt = 0;
    }
}

/***************************************************************
	*	@brief	���
	*	@param	��
	*	@note	��
***************************************************************/
void Adc_Measure_Distance(void)
{
    static float temp = 0;
	temp = Adc_GetAdc(INFRARED_ADCx, INFRARED_CHx);
    temp = 174070 / (temp + 570.7) - 28.5;
    LPF_1_db(50,0.005, temp, &laser.distance);
}