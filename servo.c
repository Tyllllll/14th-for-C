#include "header.h"

int16 duty = DEG_MID;
uint8 fore_max = 42;
uint8 fore_min = 37;
float32 servo_kp_left = 4;
float32 servo_kp_right = 4;
float32 servo_ki = 0;
float32 servo_kd = 11.1;
uint8 dead_zone = 7;
uint8 dynamic_zone = 25;

Servo_Class servo;

/***************************************************************
	*	@brief	�����ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
void Servo_Gpio_Init(void)
{
	servo.duty = duty;
	servo.kp = servo_kp_left;
	servo.ki = servo_ki;
	servo.kd = servo_kd;
	servo.dead_zone = dead_zone;
	servo.dynamic_zone = dynamic_zone;
	
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
	PIT_InitStructure.PIT_Isr = Sevor_Output;
	LPLD_PIT_Init(PIT_InitStructure);
	LPLD_PIT_EnableIrq(PIT_InitStructure);//ʹ���ж�
}

/***************************************************************
	*	@brief	PIT0��ʱ�ж�
	*	@param	��
	*	@note	���ڵ��ã��������ߵ�ƽ
***************************************************************/
void Sevor_Output(void)
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
	SERVO_O = 1;
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//��ʼ��ʱ 
}

/***************************************************************
	*	@brief	PIT3��ʱ�ж�
	*	@param	��
	*	@note	�������͵�ƽ
***************************************************************/
void Servo_PIT_Isr(void)
{
	SERVO_O = 0;
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
}

/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	��
***************************************************************/
void Sevor_Control(void)
{
	if(motor.speed_ave > 400)
	{
		servo.foresight = fore_min;
	}
	else if(motor.speed_ave < 250)
	{
		servo.foresight = fore_max;
	}
	else
	{
		servo.foresight = (uint8)(fore_min + (float32)(fore_max - fore_min) * (400 - motor.speed_ave) * (400 - motor.speed_ave) / (150 * 150));
	}
	servo.error[0] = (uint8)((line.midline[servo.foresight] - 80) / 3.0) + (uint8)((line.midline[servo.foresight + 1] - 80) / 3.0) + 
		(uint8)((line.midline[servo.foresight + 2] - 80) / 6.0) + (uint8)((line.midline[servo.foresight - 1] - 80) / 6.0);
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
	
}

/***************************************************************
	*	@brief	���pid����
	*	@param	��
	*	@note	Sevor_Control����
***************************************************************/
void Servo_PID(void)
{
	int16 p_value;
	int16 d_value;
	//��
	if(servo.error[0] < 0)
	{
		if(servo.error[0] > -servo.dynamic_zone)
		{
			servo.kp = servo_kp_left * servo.error[0] * servo.error[0] / servo.dynamic_zone / servo.dynamic_zone;
		}
		else
		{
			servo.kp = servo_kp_left;
		}
	}
	//��
	else
	{
		if(servo.error[0] < servo.dynamic_zone)
		{
			servo.kp = servo_kp_right * servo.error[0] * servo.error[0] / servo.dynamic_zone / servo.dynamic_zone;
		}
		else
		{
			servo.kp = servo_kp_right;
		}
	}
	p_value = (int16)(servo.kp * servo.error[0]);
	d_value = (int16)(servo.kd * servo.error_differ[0]);
	servo.duty = fabs(servo.error[0]) < servo.dead_zone ? (int16)DEG_MID : (int16)(DEG_MID - p_value - d_value);
	//Speed_Set();
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