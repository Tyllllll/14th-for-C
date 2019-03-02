#include "header.h"

Servo_Class servo;

/***************************************************************
	*	@brief	�����ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
void Servo_Gpio_Init(void)
{
	servo.duty = DEG_MID;
	servo.fore_max = 58;
	servo.kp_left = 3.9;
	servo.kp_right = 3.9;
	servo.ki = 0;
	servo.kd = 11.1;
	servo.dead_zone = 2;
	servo.dynamic_zone = 30;
	servo.dif_const_left = 7.4;//7.28;
	servo.dif_const_right = 5.2;//5.6;
    magnetic.s_kp = 10;
    magnetic.s_kd = 20;
	servo.enable = 1;
	
	static GPIO_InitTypeDef GPIO_InitStructure;
	//������ų�ʼ��
	GPIO_InitStructure.GPIO_PTx = SERVO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	GPIO_InitStructure.GPIO_Pins = SERVO_Pinx;
	LPLD_GPIO_Init(GPIO_InitStructure);
	//PIT3��ʱ�жϣ��������͵�ƽ
	static PIT_InitTypeDef PIT_LOW_InitStructure;
	PIT_LOW_InitStructure.PIT_Pitx = SERVO_LOW_PITx;
	PIT_LOW_InitStructure.PIT_PeriodUs = 100000;
	PIT_LOW_InitStructure.PIT_Isr = Servo_PIT_Isr;
	LPLD_PIT_Init(PIT_LOW_InitStructure);
	LPLD_PIT_EnableIrq(PIT_LOW_InitStructure);//ʹ���ж�
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
	//PIT0 20ms��ʱ�жϣ��������ߵ�ƽ
	static PIT_InitTypeDef PIT_HIGH_InitStructure;
	PIT_HIGH_InitStructure.PIT_Pitx = SERVO_HIGH_PITx;
	PIT_HIGH_InitStructure.PIT_PeriodMs = 20;
	PIT_HIGH_InitStructure.PIT_Isr = Servo_Output;
	LPLD_PIT_Init(PIT_HIGH_InitStructure);
	LPLD_PIT_EnableIrq(PIT_HIGH_InitStructure);//ʹ���ж�
}

/***************************************************************
	*	@brief	PIT0��ʱ�ж�
	*	@param	��
	*	@note	���ڵ��ã��������ߵ�ƽ
***************************************************************/
void Servo_Output(void)
{
	if(servo.duty > DEG_MAX)
	{
		servo.duty = DEG_MAX;
	}
	if(servo.duty < DEG_MIN)
	{
		servo.duty = DEG_MIN;
	}
	PIT->CHANNEL[3].LDVAL = (servo.duty) * (g_bus_clock / 1000000) - 1;
	SERVO = 1;
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//��ʼ��ʱ
}

/***************************************************************
	*	@brief	PIT3��ʱ�ж�
	*	@param	��
	*	@note	�������͵�ƽ
***************************************************************/
void Servo_PIT_Isr(void)
{
	SERVO = 0;
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
}
/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	��
***************************************************************/
void Servo_Control(void)
{
	if(servo.which == 0)
	{
		servo.foresight = servo.fore_max;
		if(servo.foresight < feature.top_point)
		{
			servo.foresight = feature.top_point - 6;
		}
		servo.error[0] = Get_Mid_Average(servo.foresight);
	}
    
    else if(servo.which == 1)
	{
//		float32 result = (magnetic.right_equivalent - magnetic.left_equivalent) / (magnetic.right_equivalent + magnetic.left_equivalent);
//		if(result > -0.12)
//		{
//			servo.error[0] = magnetic.a * (result - 0.12) * (result - 0.12);
//		}
//		else
//		{
//			servo.error[0] = -magnetic.a * (result - 0.12) * (result - 0.12);
//		}
	}
	if(servo.error[0] > 60)
	{
		servo.error[0] = 60;
	}
	if(servo.error[0] < -60)
	{
		servo.error[0] = -60;
	}
	if(servo.error[0] - servo.error[1] > 15)
	{
		servo.error[0] = servo.error[1] + 15;
	}
	if(servo.error[0] - servo.error[1] < -15)
	{
		servo.error[0] = servo.error[1] - 15;
	}
	servo.error_differ[0] = servo.error[0] - servo.error[1];
	if(servo.error_differ[0] - servo.error_differ[1] > 13)
	{
		servo.error_differ[0] = servo.error_differ[1] + 13;
	}
	if(servo.error_differ[0] - servo.error_differ[1] < -13)
	{
		servo.error_differ[0] = servo.error_differ[1] - 13;
	}
	for(uint8 i = 4; i > 0; i--)
	{
		servo.error[i] = servo.error[i - 1];
	}
	for(uint8 i = 3; i > 0; i--)
	{
		servo.error_differ[i] = servo.error_differ[i - 1];
	}
	Servo_PID();
}

/***************************************************************
	*	@brief	���pid����
	*	@param	��
	*	@note	Servo_Control����
***************************************************************/
void Servo_PID(void)
{
    if(servo.which ==0 )
	{
        int16 p_value;
        int16 d_value;
        //��
        if(servo.error[0] <= 0)
        {
            if(servo.error[0] > -servo.dynamic_zone)
            {
                servo.kp = servo.kp_left * servo.error[0] * servo.error[0] / servo.dynamic_zone / servo.dynamic_zone;
            }
            else
            {
                servo.kp = servo.kp_left;
            }
        }
        //��
        else
        {
            if(servo.error[0] < servo.dynamic_zone)
            {
                servo.kp = servo.kp_right * servo.error[0] * servo.error[0] / servo.dynamic_zone / servo.dynamic_zone;
            }
            else
            {
                servo.kp = servo.kp_right;
            }
        }
        p_value = (int16)(servo.kp * servo.error[0]);
        d_value = (int16)(servo.kd * servo.error_differ[0]);
        servo.duty = fabs(servo.error[0]) < servo.dead_zone ? (int16)DEG_MID : (int16)(DEG_MID - p_value - d_value);
        if(servo.duty>DEG_MAX)
        {
            servo.duty = DEG_MAX;
        }
        else if(servo.duty<DEG_MIN)
        {
            servo.duty = DEG_MIN;
        }
    }
    else if(servo.which == 1)
    {
        int16 p_valu;
        int16 d_valu;
        magnetic.error = (float32)(magnetic.onceUni[MIDRIGHT] - magnetic.onceUni[MIDLEFT])/(magnetic.onceUni[MIDRIGHT] + magnetic.onceUni[MIDLEFT]);
        magnetic.diff_error = magnetic.error - magnetic.error_pre1;
        magnetic.error_pre1 = magnetic.error;
        p_valu = (int16)(magnetic.s_kp * magnetic.error);
        d_valu = (int16)(magnetic.s_kd * magnetic.diff_error);
        servo.duty = (int16)(DEG_MID - p_valu - d_valu); 
        if(servo.duty>DEG_MAX)
        {
            servo.duty = DEG_MAX;
        }
        else if(servo.duty<DEG_MIN)
        {
            servo.duty = DEG_MIN;
        }
    }
}

int16 Get_Mid_Average(uint8 foresight)
{
    int16 outvalu = 0;
    for(int8 i= -4; i <= 0; i++)
    {
        outvalu += 2 * (line.midline[foresight+i] - 80);   
    }
    for(int8 i = 1; i <= 5; i++)
    {
        outvalu += line.midline[foresight+i] - 80; 
    }
    outvalu = (int16)(outvalu / 15);
    return outvalu;
}

/**********************a little funcitons**********************/
/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	��
***************************************************************/
void servo_up1(void)
{
	servo.duty += 1;
}
void servo_up5(void)
{
	servo.duty += 5;
}
void servo_up10(void)
{
	servo.duty += 10;
}
void servo_down1(void)
{
	servo.duty -= 1;
}
void servo_down5(void)
{
	servo.duty -= 5;
}
void servo_down10(void)
{
	servo.duty -= 10;
}