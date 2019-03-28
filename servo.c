#include "header.h"

Servo_Class servo;
int16 Fore270 = 46;
int16 Fore280 = 45;
int16 Fore290 = 44;
int16 Fore300 = 44;
int16 Fore310 = 43;
int16 Fore320 = 43;
int16 Fore330 = 42;
int16 Fore340 = 42;
int16 Fore350 = 41;
int16 Fore360 = 40;
int16 Fore370 = 40;

/***************************************************************
	*	@brief	舵机初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Servo_Init(void)
{
	servo.duty = DEG_MID;
	servo.kp = 3;
	servo.kd = 6;
	servo.dead_zone = 1;
	servo.enable = 1;
	
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
float error;
void Servo_Control(void)
{
	int16 p_value;
	int16 d_value;
	if(servo.which == 0)
	{
		servo.foresight = Servo_Get_Foresight();
		if(servo.foresight < feature.top_point + 5)
		{
			servo.foresight = feature.top_point + 5;
		}
		servo.error[0] = Get_Mid_Average(servo.foresight);
		servo.error[0] = (int16)(0.88 * servo.error[0] + 0.12 * servo.error[1]);
		servo.error_differ = servo.error[0] - servo.error[1];
		for(uint8 i = 4; i > 0; i--)
		{
			servo.error[i] = servo.error[i - 1];
		}
		Servo_Fuzzy();
		p_value = (int16)(servo.kp * servo.error[0]);
		d_value = (int16)(servo.kd * servo.error_differ);
		servo.duty = fabs(servo.error[0]) < servo.dead_zone ? (int16)DEG_MID : (int16)(DEG_MID - p_value - d_value);
	}
	else if(servo.which == 1)
	{
        if(Position == 0)
        {
            magnetic.error[0] = -1;
        }
        else if(Position == 1)
        {
            magnetic.error[0] = (magnetic.value[MID_R] - 6 * magnetic.value[HORIZONTAL_L]) / (magnetic.value[MID_R] + 6 * magnetic.value[HORIZONTAL_L]);
            magnetic.kp = (int16)fabs(magnetic.kp_default * magnetic.error[0]);
        }
        else if(Position == 2)
        {
            magnetic.error[0] = 0.7 * (magnetic.value[HORIZONTAL_R] - magnetic.value[HORIZONTAL_L]) / (magnetic.value[HORIZONTAL_R] + magnetic.value[HORIZONTAL_L]);
            magnetic.kp = (int16)fabs(magnetic.kp_default * magnetic.error[0] * magnetic.error[0]);
        }
        else if(Position == 3)
        {
            magnetic.error[0] = (6 * magnetic.value[HORIZONTAL_R] - magnetic.value[MID_L]) / (6 * magnetic.value[HORIZONTAL_R] + magnetic.value[MID_L]);
            magnetic.kp = (int16)fabs(magnetic.kp_default * magnetic.error[0]);
        }
        else if(Position == 4)
        {
            magnetic.error[0] = 1;
        }
       // magnetic.error[0] = (magnetic.value[EQUIVALENT_R] - magnetic.value[EQUIVALENT_L]) / (magnetic.value[EQUIVALENT_R] + magnetic.value[EQUIVALENT_L]);
        magnetic.error_differ = magnetic.error[0] - magnetic.error[1];
		magnetic.kp = (int16)fabs(magnetic.kp_default * magnetic.error[0] * magnetic.error[0]);
		p_value = (int16)(magnetic.kp * magnetic.error[0]);
		d_value = (int16)(magnetic.kd * magnetic.error_differ);
		magnetic.error[1] = magnetic.error[0];
		servo.duty = (int16)(DEG_MID - p_value - d_value);//偏大左偏，偏小右偏
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
	*	@brief	模糊计算
	*	@param	无
	*	@note	无
***************************************************************/
static int8 fuzzy0_e[7] = {-71, -50, -25, 0, 22, 49, 70};
static int8 fuzzy0_ec[5] = {-15, -8, 0, 8, 15};
//static float32 fuzzy0_u[7] = {0.2, 0.9, 1.7, 2.4, 3.2, 4.1, 5};
static float32 fuzzy0_u[7] = {0.6, 1.6, 2.7, 3.6, 4.4, 4.9, 5.5};
static uint8 fuzzy_rule_kp[7][5] = {
//	-7,-4, 0, 4, 7,	ec	e
	{6, 5, 5, 3, 3},//	-50
	{5, 4, 3, 2, 2},//	-35
	{4, 2, 1, 1, 1},//	-20
	{2, 1, 0, 1, 2},//	0
	{1, 1, 1, 2, 4},//	20
	{2, 2, 3, 4, 5},//	35
	{3, 3, 5, 5, 6}	//	50
};
void Servo_Fuzzy(void)
{
	uint8 e_probability[2];
	uint8 ec_probability[2];
	uint8 u_probability[4];
	int8 en;
	int8 ecn;
	int8 un[4];
	if(servo.error[0] > fuzzy0_e[0] && servo.error[0] < fuzzy0_e[6])
	{
		//偏差模糊
		if(servo.error[0] <= fuzzy0_e[1])
		{
			en = 0;
			e_probability[0] = (uint8)(100 * (float32)(fuzzy0_e[1] - servo.error[0]) / (fuzzy0_e[1] - fuzzy0_e[0]));
		}
		else if(servo.error[0] <= fuzzy0_e[2])
		{
			en = 1;
			e_probability[0] = (uint8)(100 * (float32)(fuzzy0_e[2] - servo.error[0]) / (fuzzy0_e[2] - fuzzy0_e[1]));
		}
		else if(servo.error[0] <= fuzzy0_e[3])
		{
			en = 2;
			e_probability[0] = (uint8)(100 * (float32)(fuzzy0_e[3] - servo.error[0]) / (fuzzy0_e[3] - fuzzy0_e[2]));
		}
		else if(servo.error[0] <= fuzzy0_e[4])
		{
			en = 3;
			e_probability[0] = (uint8)(100 * (float32)(fuzzy0_e[4] - servo.error[0]) / (fuzzy0_e[4] - fuzzy0_e[3]));
		}
		else if(servo.error[0] <= fuzzy0_e[5])
		{
			en = 4;
			e_probability[0] = (uint8)(100 * (float32)(fuzzy0_e[5] - servo.error[0]) / (fuzzy0_e[5] - fuzzy0_e[4]));
		}
		else if(servo.error[0] <= fuzzy0_e[6])
		{
			en = 5;
			e_probability[0] = (uint8)(100 * (float32)(fuzzy0_e[6] - servo.error[0]) / (fuzzy0_e[6] - fuzzy0_e[5]));
		}
	}
	else if(servo.error[0] <= fuzzy0_e[0])
	{
		en = 0;
		e_probability[0] = 100;
	}
	else if(servo.error[0] >= fuzzy0_e[6])
	{
		en = 5;
		e_probability[0] = 0;
	}
	e_probability[1] = 100 - e_probability[0];
	//偏差变化率模糊
	if(servo.error_differ > fuzzy0_ec[0] && servo.error_differ < fuzzy0_ec[4])
	{
		if(servo.error_differ <= fuzzy0_ec[1])
		{
			ecn = 0;
			ec_probability[0] = (uint8)(100 * (float32)(fuzzy0_ec[1] - servo.error_differ) / (fuzzy0_ec[1] - fuzzy0_ec[0]));
		}
		else if(servo.error_differ <= fuzzy0_ec[2])
		{
			ecn = 1;
			ec_probability[0] = (uint8)(100 * (float32)(fuzzy0_ec[2] - servo.error_differ) / (fuzzy0_ec[2] - fuzzy0_ec[1]));
		}
		else if(servo.error_differ <= fuzzy0_ec[3])
		{
			ecn = 2;
			ec_probability[0] = (uint8)(100 * (float32)(fuzzy0_ec[3] - servo.error_differ) / (fuzzy0_ec[3] - fuzzy0_ec[2]));
		}
		else if(servo.error_differ <= fuzzy0_ec[4])
		{
			ecn = 3;
			ec_probability[0] = (uint8)(100 * (float32)(fuzzy0_ec[4] - servo.error_differ) / (fuzzy0_ec[4] - fuzzy0_ec[3]));
		}
	}
	else if(servo.error_differ <= fuzzy0_ec[0])
	{
		ecn = 0;
		ec_probability[0] = 100;
	}
	else if(servo.error_differ >= fuzzy0_ec[4])
	{
		ecn = 3;
		ec_probability[0] = 0;
	}
	ec_probability[1] = 100 - ec_probability[0];
	un[0] = fuzzy_rule_kp[en][ecn];
	un[1] = fuzzy_rule_kp[en + 1][ecn];
	un[2] = fuzzy_rule_kp[en][ecn + 1];
	un[3] = fuzzy_rule_kp[en + 1][ecn + 1];
	//取隶属度最小值
	if(e_probability[0] < ec_probability[0])
	{
		u_probability[0] = e_probability[0];
	}
	else
	{
		u_probability[0] = ec_probability[0];
	}
	if(e_probability[1] < ec_probability[0])
	{
		u_probability[1] = e_probability[1];
	}
	else
	{
		u_probability[1] = ec_probability[0];
	}
	if(e_probability[1] < ec_probability[0])
	{
		u_probability[2] = e_probability[0];
	}
	else
	{
		u_probability[2] = ec_probability[1];
	}
	if(e_probability[1] < ec_probability[1])
	{
		u_probability[3] = e_probability[1];
	}
	else
	{
		u_probability[3] = ec_probability[1];
	}
	//同隶属取最大，小概率清零
	if(un[0] == un[1])
	{
		if(u_probability[0] > u_probability[1])
		{
			u_probability[1] = 0;
		}
		else
		{
			u_probability[0] = 0;
		}
	}
	if(un[0] == un[2])
	{
		if(u_probability[0] > u_probability[2])
		{
			u_probability[2] = 0;
		}
		else
		{
			u_probability[0] = 0;
		}
	}
	if(un[0] == un[3])
	{
		if(u_probability[0] > u_probability[3])
		{
			u_probability[3] = 0;
		}
		else
		{
			u_probability[0] = 0;
		}
	}
	if(un[1] == un[2])
	{
		if(u_probability[1] > u_probability[2])
		{
			u_probability[2] = 0;
		}
		else
		{
			u_probability[1] = 0;
		}
	}
	if(un[1] == un[3])
	{
		if(u_probability[1] > u_probability[3])
		{
			u_probability[3] = 0;
		}
		else
		{
			u_probability[1] = 0;
		}
	}
	if(un[2] == un[3])
	{
		if(u_probability[2] > u_probability[3])
		{
			u_probability[3] = 0;
		}
		else
		{
			u_probability[2] = 0;
		}
	}
	//解模糊
	servo.kp = (u_probability[0] * fuzzy0_u[un[0]] + u_probability[1] * fuzzy0_u[un[1]] + u_probability[2] * fuzzy0_u[un[2]] 
		+ u_probability[3] * fuzzy0_u[un[3]]) / (u_probability[0] + u_probability[1] + u_probability[2] + u_probability[3]);
}

/***************************************************************
	*	@brief	分段动态前瞻默认值
	*	@param	无
	*	@note	无
***************************************************************/
int16 Servo_Get_Foresight(void)
{//分段前瞻
    int16 foresight1 = 0;
    if(motor.speed_ave<275)
    {
        foresight1 = Fore270;
    }
    else if(motor.speed_ave>=275 && motor.speed_ave<285)
    {
        foresight1=Fore280;
    }
    else if(motor.speed_ave>=285 && motor.speed_ave<295)
    {
        foresight1=Fore290;
    }
    else if(motor.speed_ave>=295 && motor.speed_ave<305)
    {
        foresight1=Fore300;
    }
    else if(motor.speed_ave>=305 && motor.speed_ave<315)
    {
        foresight1=Fore310;
    }
    else if(motor.speed_ave>=315 && motor.speed_ave<325)
    {
        foresight1=Fore320;
    }
    else if(motor.speed_ave>=325 && motor.speed_ave<335)
    {
        foresight1=Fore330;
    }
    else if(motor.speed_ave>=335 && motor.speed_ave<345)
    {
        foresight1=Fore340;
    }
    else if(motor.speed_ave>=345 && motor.speed_ave<355)
    {
        foresight1=Fore350;
    }
    else if(motor.speed_ave>=355 && motor.speed_ave<365)
    {
        foresight1=Fore360;
    }
    else if(motor.speed_ave>=365)
    {
        foresight1=Fore370;
    }
    return foresight1;
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
