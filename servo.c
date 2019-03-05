#include "header.h"

Servo_Class servo;

/***************************************************************
	*	@brief	舵机初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Servo_Init(void)
{
	servo.duty = DEG_MID;
	servo.fore_default = 48;
	servo.kp_default = 3.9;
	servo.kd = 2.7;
	servo.dead_zone = 3;
	servo.enable = 1;
	servo.ramp_change = 25;
//	servo.which = 1;
	
	static GPIO_InitTypeDef GPIO_InitStructure;
	//舵机引脚初始化
	GPIO_InitStructure.GPIO_PTx = SERVO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	GPIO_InitStructure.GPIO_Pins = SERVO_Pinx;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	//PIT3定时中断，负责拉低电平
	static PIT_InitTypeDef PIT_LOW_InitStructure;
	PIT_LOW_InitStructure.PIT_Pitx = SERVO_LOW_PITx;
	PIT_LOW_InitStructure.PIT_PeriodUs = 100000;
	PIT_LOW_InitStructure.PIT_Isr = Servo_PIT_Isr;
	if(!LPLD_PIT_Init(PIT_LOW_InitStructure))
		return STATUS_FAILED;
	LPLD_PIT_EnableIrq(PIT_LOW_InitStructure);//使能中断
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
	//PIT0 20ms定时中断，负责拉高电平
	static PIT_InitTypeDef PIT_HIGH_InitStructure;
	PIT_HIGH_InitStructure.PIT_Pitx = SERVO_HIGH_PITx;
	PIT_HIGH_InitStructure.PIT_PeriodMs = 20;
	PIT_HIGH_InitStructure.PIT_Isr = Servo_Output;
	if(!LPLD_PIT_Init(PIT_HIGH_InitStructure))
		return STATUS_FAILED;
	LPLD_PIT_EnableIrq(PIT_HIGH_InitStructure);//使能中断
	return STATUS_OK;
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
//		servo.kp = servo.kp_default * servo.error[0] * servo.error[0] / 60 / 60;
		Servo_Fuzzy();
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

/***************************************************************
	*	@brief	模糊计算
	*	@param	无
	*	@note	无
***************************************************************/
static int8 fuzzy0_e[7] = {-45, -30, -15, 0, 15, 30, 45};
static int8 fuzzy0_ec[5] = {-8, -4, 0, 4, 8};
static float fuzzy0_rule_kp[7][5] = {
//	-8, -4, 0, 4, 8 ec	e
	{4, 4, 3, 2, 1},//	-45
	{3.7, 3.5, 3, 2, 1},//	-30	
	{3, 2, 1, 1, 1},//	-15
	{2, 1, 0, 1, 2},//	0
	{1, 1, 1, 2, 3},//	15
	{1, 2, 3, 3.5, 3.7},//	30
	{1, 2, 3, 4, 4}//	45
};
static uint8 fuzzy0_rule_kd[7][5] = {
//	-8, -4, 0, 4, 8 ec	e
	{9, 8, 7, 5, 3},//	-45
	{8, 6, 4, 2, 1},//	-30	
	{3, 2, 2, 1, 1},//	-15
	{2, 1, 0, 1, 1},//	0
	{1, 1, 2, 2, 3},//	15
	{1, 2, 4, 6, 8},//	30
	{3, 5, 7, 8, 9}//	45
};
//r=50cm kd=8.7
//大圆kd=2.5
void Servo_Fuzzy(void)
{
	float32 e_probability[2];
	float32 ec_probability[2];
	int8 en;
	int8 ecn;
	float32 u[4];
	if(servo.which == 0)
	{
		if(servo.error[0] > fuzzy0_e[0] && servo.error[0] < fuzzy0_e[6])
		{
			//偏差模糊
			if(servo.error[0] <= fuzzy0_e[1])
			{
				en = 0;
				e_probability[0] = (float32)(fuzzy0_e[1] - servo.error[0]) / (fuzzy0_e[1] - fuzzy0_e[0]);
			}
			else if(servo.error[0] <= fuzzy0_e[2])
			{
				en = 1;
				e_probability[0] = (float32)(fuzzy0_e[2] - servo.error[0]) / (fuzzy0_e[2] - fuzzy0_e[1]);
			}
			else if(servo.error[0] <= fuzzy0_e[3])
			{
				en = 2;
				e_probability[0] = (float32)(fuzzy0_e[3] - servo.error[0]) / (fuzzy0_e[3] - fuzzy0_e[2]);
			}
			else if(servo.error[0] <= fuzzy0_e[4])
			{
				en = 3;
				e_probability[0] = (float32)(fuzzy0_e[4] - servo.error[0]) / (fuzzy0_e[4] - fuzzy0_e[3]);
			}
			else if(servo.error[0] <= fuzzy0_e[5])
			{
				en = 4;
				e_probability[0] = (float32)(fuzzy0_e[5] - servo.error[0]) / (fuzzy0_e[5] - fuzzy0_e[4]);
			}
			else if(servo.error[0] <= fuzzy0_e[6])
			{
				en = 5;
				e_probability[0] = (float32)(fuzzy0_e[6] - servo.error[0]) / (fuzzy0_e[6] - fuzzy0_e[5]);
			}
		}
		else if(servo.error[0] <= fuzzy0_e[0])
		{
			en = 0;
			e_probability[0] = 1;
		}
		else if(servo.error[0] >= fuzzy0_e[6])
		{
			en = 5;
			e_probability[0] = 0;
		}
		e_probability[1] = 1 - e_probability[0];
		//偏差变化率模糊
		if(servo.error_differ[0] > fuzzy0_ec[0] && servo.error_differ[0] < fuzzy0_ec[4])
		{
			if(servo.error_differ[0] <= fuzzy0_ec[1])
			{
				ecn = 0;
				ec_probability[0] = (float32)(fuzzy0_ec[1] - servo.error_differ[0]) / (fuzzy0_ec[1] - fuzzy0_ec[0]);
			}
			else if(servo.error_differ[0] <= fuzzy0_ec[2])
			{
				ecn = 1;
				ec_probability[0] = (float32)(fuzzy0_ec[2] - servo.error_differ[0]) / (fuzzy0_ec[2] - fuzzy0_ec[1]);
			}
			else if(servo.error_differ[0] <= fuzzy0_ec[3])
			{
				ecn = 2;
				ec_probability[0] = (float32)(fuzzy0_ec[3] - servo.error_differ[0]) / (fuzzy0_ec[3] - fuzzy0_ec[2]);
			}
			else if(servo.error_differ[0] <= fuzzy0_ec[4])
			{
				ecn = 3;
				ec_probability[0] = (float32)(fuzzy0_ec[4] - servo.error_differ[0]) / (fuzzy0_ec[4] - fuzzy0_ec[3]);
			}
		}
		else if(servo.error_differ[0] <= fuzzy0_ec[0])
		{
			ecn = 0;
			ec_probability[0] = 1;
		}
		else if(servo.error_differ[0] >= fuzzy0_ec[4])
		{
			ecn = 3;
			ec_probability[0] = 0;
		}
		ec_probability[1] = 1 - ec_probability[0];
		//解模糊
		u[0] = fuzzy0_rule_kp[en][ecn];
		u[1] = fuzzy0_rule_kp[en][ecn  + 1];
		u[2] = fuzzy0_rule_kp[en + 1][ecn];
		u[3] = fuzzy0_rule_kp[en + 1][ecn + 1];
		servo.kp = u[0] * e_probability[0] * ec_probability[0]
			+ u[1] * e_probability[0] * ec_probability[1]
				+ u[2] * e_probability[1] * ec_probability[0]
					+ u[3] * e_probability[1] * ec_probability[1];
		u[0] = fuzzy0_rule_kd[en][ecn];
		u[1] = fuzzy0_rule_kd[en][ecn  + 1];
		u[2] = fuzzy0_rule_kd[en + 1][ecn];
		u[3] = fuzzy0_rule_kd[en + 1][ecn + 1];
		servo.kd = u[0] * e_probability[0] * ec_probability[0]
			+ u[1] * e_probability[0] * ec_probability[1]
				+ u[2] * e_probability[1] * ec_probability[0]
					+ u[3] * e_probability[1] * ec_probability[1];
	}
	else if(servo.which == 1)
	{
		
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
