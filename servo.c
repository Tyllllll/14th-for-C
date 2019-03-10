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
	servo.fore_max = 58;
	servo.ki = 0;
	servo.dead_zone = 2;
	servo.dif_const_left = 7.4;//7.28;
	servo.dif_const_right = 5.2;//5.6;
    magnetic.s_kp = 130;
    magnetic.s_kd = 0;
	servo.enable = 1;
	
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
	*	@brief	舵机控制
	*	@param	无
	*	@note	无
***************************************************************/
void Servo_Control(void)
{
	if(servo.which == 0)
	{
		servo.foresight = servo.fore_max;
		if(servo.foresight < feature.top_point + 5)
		{
			servo.foresight = feature.top_point + 6;
		}
        if(feature.ramp_state == 1)
        {
            servo.foresight -= 20;
        }
		servo.error[0] = (int16)(0.85 * Get_Mid_Average(servo.foresight) + 0.15 * servo.error[1]);
	}
    
    else if(servo.which == 1)
	{
		float32 result = (magnetic.right_equivalent - magnetic.left_equivalent) / (magnetic.right_equivalent + magnetic.left_equivalent);
		if(result > -0.12)
		{
			servo.error[0] = magnetic.a * (result - 0.12) * (result - 0.12);
		}
		else
		{
			servo.error[0] = -magnetic.a * (result - 0.12) * (result - 0.12);
		}
	}
	constrain_16(&servo.error[0],60,-60);
    constrain_16(&servo.error[0],servo.error[1]+15,servo.error[1]-15);
	servo.error_differ[0] = (int16)(0.85 * (servo.error[0] - servo.error[1]) + 0.15 * (servo.error[2] - servo.error[3]));
	constrain_16(&servo.error_differ[0],servo.error_differ[1]+13,servo.error_differ[1]-13);
	for(uint8 i = 9; i > 0; i--)
	{
		servo.error[i] = servo.error[i - 1];
        servo.error_differ[i] = servo.error_differ[i - 1];
	}
	Servo_PID();
}

/***************************************************************
	*	@brief	舵机pid计算
	*	@param	无
	*	@note	Servo_Control调用
***************************************************************/
//int16 sum14, sum23;
void Servo_PID(void)
{
    if(servo.which ==0 )
	{
		int16 p_value;
        int16 d_value;
        Servo_Fuzzy();
        p_value = (int16)(servo.kp * servo.error[0]);
        d_value = (int16)(servo.kd * servo.error_differ[0]);
        servo.duty = fabs(servo.error[0]) < servo.dead_zone ? (int16)DEG_MID : (int16)(DEG_MID - p_value - d_value);
        constrain_16(&servo.duty,DEG_MAX,DEG_MIN);
    }
    else if(servo.which == 1)
    {
        int16 p_valu;
        int16 d_valu;
        //电磁 差比和
        magnetic.error = (float32)(magnetic.right_equivalent - magnetic.left_equivalent)/(magnetic.right_equivalent + magnetic.left_equivalent);
        magnetic.diff_error = magnetic.error - magnetic.error_pre1;
        magnetic.error_pre1 = magnetic.error;
        p_valu = (int16)(magnetic.s_kp * magnetic.error);
        d_valu = (int16)(magnetic.s_kd * magnetic.diff_error);
        servo.duty = (int16)(DEG_MID - p_valu - d_valu); 
//        if(magnetic.onceUni[MIDRIGHT]<8 && magnetic.onceUni[MIDLEFT]<8)
//        {
//            if(magnetic.onceUni[MIDRIGHT]<magnetic.onceUni[MIDLEFT])
//            {
//                servo.duty = DEG_MAX ;
//            }
//            else
//            {
//                servo.duty = DEG_MIN ;
//            }
//            
//        }
        
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


//#define FMAX    100
//int PFF[7] = { -50,-35,-20,0,20,35,50};
//int DFF[5] = { -9, -4, 0, 4, 9};
//int UFF[7] = {  0,15,25,35,55,75,95};//
//int    U;           /*偏差,偏差微分以及输出值的精确量*/
//unsigned int   PF[2]={0}, DF[2]={0}, UF[4]={0};   /*偏差,偏差微分以及输出值的隶属度*/
//int    Pn, Dn, Un[4];
//float   temp1, temp2;
//float  a1=0,a2=0,a3=0,a4=0;
//
//int rule[7][5]={
//  //  ec小于-8为-2  ec大于-8小于0为-1  ec大于0小于8为0    ec>8为1 
////  -2  -1   0   1  2  ec    e 
//  { 6,  5,  5,  4,  4}, //  -2 e<-45
//  { 5,  4,  4,  3,  3}, //  -1   -45<e<-20
//  { 3,  2,  2,  1,  1}, //  0  -20<e<-0
//  { 1,  0,  0,  0,  1}, //   1    0<e<20
//  { 1,  1,  2,  2,  3}, //   2    20<e<45
//  { 3,  3,  4,  4,  5}, //   3    45<e
//  { 4,  4,  5,  5,  6}  //   
//};
//int   Fuzzy(int P, int D)   /*模糊运算引擎*/
//{
//	/*隶属度的确定*/
//	/*根据PD的指定语言值获得有效隶属度*/
//	if (P>PFF[0] && P<PFF[6])
//	{
//		if (P <= PFF[1])
//		{
//			Pn = -2;
//			PF[0] = (int)(FMAX*((float)(PFF[1] - P) / (PFF[1] - PFF[0])));//求p值占该论域的比例即为在该论域的概率
//		}
//		else if (P <=PFF[2])
//		{
//			Pn = -1;
//			PF[0] = (int)(FMAX*((float)(PFF[2] - P) / (PFF[2] - PFF[1])));
//		}
//		else if (P <= PFF[3])
//		{
//			Pn = 0;
//			PF[0] = (int)(FMAX*((float)(PFF[3] - P) / (PFF[3] - PFF[2])));
//		}
//		else if (P <= PFF[4])
//		{
//			Pn = 1; 
//			PF[0] = (int)(FMAX*((float)(PFF[4] - P) / (PFF[4] - PFF[3])));
//		}
//		else if (P <= PFF[5])
//		{
//			Pn = 2; 
//			PF[0] = (int)(FMAX*((float)(PFF[5] - P) / (PFF[5] - PFF[4])));
//		}
//		else if (P <= PFF[6])
//		{
//			Pn = 3; 
//			PF[0] =(int)( FMAX*((float)(PFF[6] - P) / (PFF[6] - PFF[5])));
//		}
//	}
//	else if (P <= PFF[0])
//	{
//	  Pn = -2;  
//	  PF[0] = FMAX;
//	}
//	else if (P >= PFF[6])
//	{
//		Pn = 3;   PF[0] = 0;
//	}
//	PF[1] = FMAX - PF[0];//求p值不在所在论域的概率
//	
//	
//	if (D > DFF[0] && D < DFF[4])//-10~10
//	{
//	  if (D <=DFF[1])//-10~-5
//	  {
//	    Dn = -2; 
//	    DF[0] = (int)(FMAX*((float)(DFF[1] - D) / (DFF[1] - DFF[0])));
//	  }
//	  else if (D <= DFF[2])//-5~-0
//	  {
//	    Dn = -1;
//	    DF[0] =(int)( FMAX*((float)(DFF[2] - D) / (DFF[2] - DFF[1])));
//	  }
//	  else if (D <= DFF[3])//0~5
//	  {
//	    Dn = 0;
//	    DF[0] =(int)( FMAX*((float)(DFF[3] - D) / (DFF[3] - DFF[2])));
//	  }
//	  else if (D <= DFF[4])//5~10
//	  {
//	    Dn = 1;
//	    DF[0] =(int)( FMAX*((float)(DFF[4] - D) / (DFF[4] - DFF[3])));
//	  }
//	}
//	else if (D <= DFF[0])
//	{
//	  Dn = -2;
//	  DF[0] = FMAX;
//	}
//	else if (D >= DFF[4])
//	{
//	  Dn = 1;
//	  DF[0] = 0;
//	}
//	DF[1] = FMAX - DF[0];
//	/*使用误差范围优化后的规则表rule[7][7]*/
//	/*输出值使用13个隶属函数,中心值由UFF[7]指定*/
//	/*一般都是四个规则有效*/
//	Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
//	Un[1] = rule[Pn + 3][Dn - 1 + 3];
//	Un[2] = rule[Pn - 1 + 3][Dn + 3];
//	Un[3] = rule[Pn + 3][Dn + 3];
//	if (PF[0] <= DF[0])
//		UF[0] = PF[0];//Un[0]取值的概率应该为p所在论域的概率和d所在的论域概率的最小值
//	else
//		UF[0] = DF[0];
//	
//	if (PF[1] <= DF[0])
//		UF[1] = PF[1];//Un[1]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d所在的论域概率的最小值
//	else
//		UF[1] = DF[0];
//	
//	if (PF[0] <= DF[1])
//		UF[2] = PF[0];//Un[2]取值的概率应该为p所在论域的概率和d不在所在的论域（在相邻右边论域）概率的最小值
//	else
//		UF[2] = DF[1];
//	
//	if (PF[1] <= DF[1])  //Un[3]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d不在所在的论域（在相邻右边论域）概率的最小值
//		UF[3] = PF[1];
//	else
//		UF[3] = DF[1];
//	/*同隶属函数输出语言值求大*/
//
//	if (Un[0] == Un[1])
//	{
//		if (UF[0] > UF[1])//UF[0] > UF[1]即为在Un[0]的概率大于Un[1]的概率
//			UF[1] = 0;//概率小的清零
//		else
//			UF[0] = 0;
//	}
//	if (Un[0] == Un[2])
//	{
//		if (UF[0] > UF[2])
//			UF[2] = 0;
//		else
//			UF[0] = 0;
//	}
//	if (Un[0] == Un[3])
//	{
//		if (UF[0] > UF[3])
//			UF[3] = 0;
//		else
//			UF[0] = 0;
//	}
//	if (Un[1] == Un[2])
//	{
//		if (UF[1] > UF[2])
//			UF[2] = 0;
//		else
//			UF[1] = 0;
//	}
//	if (Un[1] == Un[3])
//	{
//		if (UF[1] > UF[3])
//			UF[3] = 0;
//		else
//			UF[1] = 0;
//	}
//	if (Un[2] == Un[3])
//	{
//		if (UF[2] > UF[3])
//			UF[3] = 0;
//		else
//			UF[2] = 0;
//	}
//
//	a1=UF[0]*UFF[Un[0]];  
//	a2=UF[1]*UFF[Un[1]];
//	a3=UF[2]*UFF[Un[2]];
//	a4=UF[3]*UFF[Un[3]];
//
//	temp1 = a1+a2+a3+a4;
//	temp2 = UF[0] + UF[1] + UF[2] + UF[3];
//	U = (int)(temp1 / temp2);//加权平均
//	return U;
//}

/***************************************************************
	*	@brief	模糊计算
	*	@param	无
	*	@note	无
***************************************************************/
static int8 fuzzy0_e[7] = {-45, -30, -15, 0, 15, 30, 45};
static int8 fuzzy0_ec[5] = {-8, -4, 0, 4, 8};
static float fuzzy0_rule_kp[7][5] = {
//	-8,    -4,    0,   4,   8 ec	e
	{3,     3,    3,   2,   1},//	-45
	{3,     3,    3,   2,   1},//	-30	
	{3,     2,    1,   1,   1},//	-15
	{2,     1,    0,   1,   2},//	0
	{1,     1,    1,   2,   3},//	15
	{1,     2,    3,   3,   3},//	30
	{1,     2,    3,   3,   3}//	45
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


































