#include "header.h"

Servo_Class servo;

/***************************************************************
	*	@brief	舵机初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Servo_Gpio_Init(void)
{
	servo.duty = DEG_MID;
	servo.fore_default = 48;
	servo.kp_default = 2.6;//3.9;
	servo.kd = 0;//11.1;
	servo.dead_zone = 2;
	servo.enable = 1;
	servo.ramp_change = 20;
//	servo.which = 1;
	
	static GPIO_InitTypeDef GPIO_InitStructure;
	//舵机引脚初始化
	GPIO_InitStructure.GPIO_PTx = SERVO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	GPIO_InitStructure.GPIO_Pins = SERVO_Pinx;
	LPLD_GPIO_Init(GPIO_InitStructure);
	//PIT3定时中断，负责拉低电平
	static PIT_InitTypeDef PIT_LOW_InitStructure;
	PIT_LOW_InitStructure.PIT_Pitx = SERVO_LOW_PITx;
	PIT_LOW_InitStructure.PIT_PeriodUs = 100000;
	PIT_LOW_InitStructure.PIT_Isr = Servo_PIT_Isr;
	LPLD_PIT_Init(PIT_LOW_InitStructure);
	LPLD_PIT_EnableIrq(PIT_LOW_InitStructure);//使能中断
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
	//PIT0 20ms定时中断，负责拉高电平
	static PIT_InitTypeDef PIT_HIGH_InitStructure;
	PIT_HIGH_InitStructure.PIT_Pitx = SERVO_HIGH_PITx;
	PIT_HIGH_InitStructure.PIT_PeriodMs = 20;
	PIT_HIGH_InitStructure.PIT_Isr = Servo_Output;
	LPLD_PIT_Init(PIT_HIGH_InitStructure);
	LPLD_PIT_EnableIrq(PIT_HIGH_InitStructure);//使能中断
}

/***************************************************************
	*	@brief	PIT0定时中断
	*	@param	无
	*	@note	周期调用，负责拉高电平
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
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//开始计时
}

/***************************************************************
	*	@brief	PIT3定时中断
	*	@param	无
	*	@note	负责拉低电平
***************************************************************/
void Servo_PIT_Isr(void)
{
	SERVO = 0;
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
}

/***************************************************************
	*	@brief	舵机控制PID
	*	@param	无
	*	@note	无
***************************************************************/
void Servo_Control(void)
{
	int16 p_value;
	int16 d_value;
	if(servo.which == 0)
	{
		servo.foresight = servo.fore_default;
		Servo_Foresight_Change();
		if(servo.foresight < feature.top_point + 5)
		{
			servo.foresight = feature.top_point + 5;
		}
		servo.error[0] = Get_Mid_Average(servo.foresight);
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
		servo.kp = servo.kp_default * servo.error[0] * servo.error[0] / 60 / 60;
		p_value = (int16)(servo.kp * servo.error[0]);
		d_value = (int16)(servo.kd * servo.error_differ[0]);
		servo.duty = fabs(servo.error[0]) < servo.dead_zone ? (int16)DEG_MID : (int16)(DEG_MID - p_value - d_value);
	}
	else if(servo.which == 1)
	{
		magnetic.error[0] = (magnetic.value[EQUIVALENT_R] - magnetic.value[EQUIVALENT_L] - magnetic.correction) / (magnetic.value[EQUIVALENT_R] + magnetic.value[EQUIVALENT_L]);
		magnetic.error_differ = magnetic.error[0] - magnetic.error[1];
		magnetic.kp = (int16)(magnetic.kp_default * magnetic.error[0] * magnetic.error[0]);
		p_value = (int16)(magnetic.kp * magnetic.error[0]);
		d_value = (int16)(magnetic.kd * magnetic.error_differ);
		magnetic.error[1] = magnetic.error[0];
		servo.duty = (int16)(DEG_MID - p_value - d_value);
	}
	if(servo.duty > DEG_MAX)
	{
		servo.duty = DEG_MAX;
	}
	if(servo.duty < DEG_MIN)
	{
		servo.duty = DEG_MIN;
	}
}

/***************************************************************
	*	@brief	改变前瞻
	*	@param	无
	*	@note	无
***************************************************************/
void Servo_Foresight_Change(void)
{
	if(feature.ramp_state == 1)
	{
		servo.foresight = servo.fore_default + servo.ramp_change;
	}
	if(feature.ramp_state == 2)
	{
		servo.foresight = servo.fore_default - servo.ramp_change;
	}
}


/**********************a little funcitons**********************/
/***************************************************************
	*	@brief	舵机调试
	*	@param	无
	*	@note	无
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
