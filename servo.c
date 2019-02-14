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
	servo.fore_max = 42;
	servo.fore_min = 37;
	servo.kp_left = 4;
	servo.kp_right = 4;
	servo.ki = 0;
	servo.kd = 11.1;
	servo.dead_zone = 7;
	servo.dynamic_zone = 25;
	servo.dif_const_left = 7.48;
	servo.dif_const_right = 5.4;
	
	static GPIO_InitTypeDef GPIO_InitStructure;
	//������ų�ʼ��
	GPIO_InitStructure.GPIO_PTx = SERVO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	GPIO_InitStructure.GPIO_Pins = SERVO_Pinx;
	LPLD_GPIO_Init(GPIO_InitStructure);
	//PIT3��ʱ�жϣ��������͵�ƽ
	static PIT_InitTypeDef PIT_InitStructure;
	PIT_InitStructure.PIT_Pitx = SERVO_LOW_PITx;
	PIT_InitStructure.PIT_PeriodUs = 100000;
	PIT_InitStructure.PIT_Isr = Servo_PIT_Isr;
	LPLD_PIT_Init(PIT_InitStructure);
	LPLD_PIT_EnableIrq(PIT_InitStructure);//ʹ���ж�
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
	//PIT0 20ms��ʱ�жϣ��������ߵ�ƽ
	PIT_InitStructure.PIT_Pitx = SERVO_HIGH_PITx;
	PIT_InitStructure.PIT_PeriodMs = 20;
	PIT_InitStructure.PIT_Isr = Servo_Output;
	LPLD_PIT_Init(PIT_InitStructure);
	LPLD_PIT_EnableIrq(PIT_InitStructure);//ʹ���ж�
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
	int16 mid_error[4];
	int16 speed = (int16)(0.6 * motor.speed_current[0] + 0.2 * motor.speed_current[1] + 0.2 * motor.speed_current[2]);//��������ֵ���и�Ƶ����
	if(speed > 400)
	{
		servo.foresight = servo.fore_min;
	}
	else if(speed < 250)
	{
		servo.foresight = servo.fore_max;
	}
	else
	{
		servo.foresight = (uint8)(servo.fore_min + (float)(servo.fore_max - servo.fore_min) * (400 - motor.speed_ave) * (400 - motor.speed_ave) / (150 * 150));
	}
	mid_error[0]=2*(line.midline[servo.foresight]-80);
	mid_error[1]=2*(line.midline[servo.foresight+1]-80);
	mid_error[2]=(line.midline[servo.foresight+2]-80);
	mid_error[3]=(line.midline[servo.foresight-1]-80);
	servo.error[0]=(int16)((mid_error[0]+mid_error[1]+mid_error[2]+mid_error[3])/6);
//	servo.error[0] = (int8)((line.midline[servo.foresight] - 80) / 3.0) + (int8)((line.midline[servo.foresight + 1] - 80) / 3.0) + 
//		(int8)((line.midline[servo.foresight + 2] - 80) / 6.0) + (int8)((line.midline[servo.foresight - 1] - 80) / 6.0);
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
	servo.duty = abs(servo.error[0]) < servo.dead_zone ? (int16)DEG_MID : (int16)(DEG_MID - p_value - d_value);
	Speed_Set();
}



/**********************a little funcitons**********************/
int abs(int a)
{ 
	if(a >= 0)
	{
		a = a;
	}
	if(a < 0)
	{
		a = -a;
	}
	return a;
}
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