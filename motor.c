#include "header.h"

Motor_Class motor;
int16 P_value;
int16 I_value;
int16 D_value;
int16 QD_value_L_test;
int16 QD_value_R_test;
int8 deltaKpMatrix[7][7]={{PB,PB,PM,PM,PS,ZO,ZO},
                          {PB,PB,PM,PS,PS,ZO,NS},
                          {PM,PM,PM,PS,ZO,NS,NS},
                          {PM,PM,PS,ZO,NS,NM,NM},
                          {PS,PS,ZO,NS,NS,NM,NM},
                          {PS,ZO,NS,NM,NM,NM,NB},
                          {ZO,ZO,NM,NM,NM,NB,NB}};
int8 deltaKiMatrix[7][7]={{NB,NB,NM,NM,NS,ZO,ZO},
                          {NB,NB,NM,NS,NS,ZO,ZO},
                          {NB,NM,NS,NS,ZO,PS,PS},
                          {NM,NM,NS,ZO,PS,PM,PM},
                          {NM,NS,ZO,PS,PS,PM,PB},
                          {ZO,ZO,PS,PS,PM,PB,PB},
                          {ZO,ZO,PS,PM,PM,PB,PB}};
int8 deltaKdMatrix[7][7]={{PS,NS,NB,NB,NB,NM,PS},
                          {PS,NS,NB,NM,NM,NS,ZO},
                          {ZO,NS,NM,NM,NS,NS,ZO},
                          {ZO,NS,NS,NS,NS,NS,ZO},
                          {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                          {PB,NS,PS,PS,PS,PS,PB},
                          {PB,PM,PM,PM,PS,PS,PB}};

int8 mf_paras[]={-3,-3,-2,-3,-2,-1,-2,-1,0,-1,0,1,0,1,2,1,2,3,2,3,3};
/***************************************************************
	*	@brief	电机初始化
	*	@param	无
	*	@note	FTM初始化pwm
***************************************************************/
void Motor_Pwm_Init(void)
{
	motor.kp = 16;
	motor.ki = 3.4;
	motor.kd = 0.7;
	motor.is_open_loop = 0;
	motor.dif_const = 0.96;
	motor.dif_fore = 1;
    motor.delta_kp = 0;
    motor.delta_kp_max = 50;
    motor.diff_error_max = 20;
    motor.error_max = 180;	
    static FTM_InitTypeDef FTM_InitStructure;
    FTM_InitStructure.FTM_Ftmx = MOTOR_FTMx;	//选择FTM通道
    FTM_InitStructure.FTM_Mode = FTM_MODE_PWM;	//FTM0的工作模式是PWM波输出
    FTM_InitStructure.FTM_PwmDeadtimeCfg = MOTOR_PWMCFG
    FTM_InitStructure.FTM_PwmFreq = 10000;		//PWM频率10KHz    

	LPLD_FTM_Init(FTM_InitStructure);
	//左正转 
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR1_FTM_CH1x, 0, MOTOR1_PORTPin1x, ALIGN_LEFT);
	//左反转
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR1_FTM_CH2x, 0, MOTOR1_PORTPin2x, ALIGN_LEFT);
	//右反转
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH1x, 0, MOTOR2_PORTPin1x, ALIGN_LEFT);
	//右正转
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH2x, 0, MOTOR2_PORTPin2x, ALIGN_LEFT);
}

/***************************************************************
	*	@brief	PIT1初始化
	*	@param	无
	*	@note	Motor_PIT为回调函数
***************************************************************/
void Motor_Pit1_Init(void)
{
	static  PIT_InitTypeDef PIT_InitStructure; 
	PIT_InitStructure.PIT_Pitx = PIT1;
	PIT_InitStructure.PIT_PeriodMs = 10;	//定时周期10毫秒
	PIT_InitStructure.PIT_Isr = Motor_PIT;	//设置中断函数
	LPLD_PIT_Init(PIT_InitStructure);		//初始化PIT1
	LPLD_PIT_EnableIrq(PIT_InitStructure);	//使能PIT1
}

/***************************************************************
	*	@brief	电机速度调整
	*	@param	无
	*	@note	10ms周期调用
***************************************************************/
void Motor_PIT(void)
{
    float32 distance;
	Encoder_Get();
	//实时速度，单位cm/s
	if(encoder.left_num != 0)
	{
		motor.speed_current_L[0] = (int16)(10000 * (float32)encoder.left_num / ENCODER_NUM_PER_METER_LEFT);
	}
	else
	{
		motor.speed_current_L[0] = 0;
	}
	if(encoder.right_num != 0)
	{
		motor.speed_current_R[0] = (int16)(10000 * (float32)encoder.right_num / ENCODER_NUM_PER_METER_RIGHT);
	}
	else
	{
		motor.speed_current_R[0] = 0;
	}
	motor.speed_current[0] = (int16)(0.5 * motor.speed_current_L[0] + 0.5 * motor.speed_current_R[0]);
	Motor_Control();
	for(uint8 i = 4; i > 0; i--)
	{
		motor.speed_current[i] = motor.speed_current[i - 1];
		motor.speed_current_L[i] = motor.speed_current_L[i - 1];
		motor.speed_current_R[i] = motor.speed_current_R[i - 1];
	}
    distance = motor.speed_current[0] * 0.01; //单位cm
    motor.alldist += distance;  
    //测量1m距离对应编码器脉冲数
  QD_value_L_test += encoder.left_num;
  QD_value_R_test += encoder.right_num;
  if (KEY1 == 0)
  {
    Key_Delay();
    if (KEY1 == 0)
    {LPLD_UART_PutChar(UART0,QD_value_L_test );
    LPLD_UART_PutChar(UART0,QD_value_R_test );
    QD_value_L_test = 0;
    QD_value_R_test = 0;
    }
  }
//  //向PC传编码器测得速度
//    LPLD_UART_PutChar(UART0,encoder.left_num );
//    LPLD_UART_PutChar(UART0,encoder.right_num );
//    LPLD_UART_PutChar(UART0,'/n');
    
    
    
    
    
	//清空计数器
	LPLD_FTM_ClearCounter(FTM2);
	LPLD_FTM_ClearCounter(FTM1);
}

/***************************************************************
	*	@brief	电机控制
	*	@param	无
	*	@note	Motor_PIT调用
***************************************************************/
void Motor_Control(void)
{
	Motor_PID();
	if(motor.start >= 1)
	{
		if(motor.speed_ave >= 0.9 * motor.speed_set)
		{
			if(motor.output_value_L > MOTOR_MAX_OUTPUT)
			{
				motor.output_value_L = MOTOR_MAX_OUTPUT;
			}
			if(motor.output_value_L < -MOTOR_MAX_OUTPUT)
			{
				motor.output_value_L = -MOTOR_MAX_OUTPUT;
			}
			if(motor.output_value_R > MOTOR_MAX_OUTPUT)
			{
				motor.output_value_R = MOTOR_MAX_OUTPUT;
			}
			if(motor.output_value_R < -MOTOR_MAX_OUTPUT)
			{
				motor.output_value_R = -MOTOR_MAX_OUTPUT;
			}
		}
		if(motor.output_value_L >= 0)
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, motor.output_value_L);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0); 
		}
		else
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, -motor.output_value_L); 
		}
		if(motor.output_value_R >= 0)
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, motor.output_value_R);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0); 
		}
		else
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, -motor.output_value_R); 
		}
		if(motor.start > 1)
		{
			motor.start--;
		}
		//堵转(10ms调用100次为1s)
		if(motor.start == 1 && motor.output_value[0] >= 4000)
		{
			if(motor.speed_current[0] <= 10 && motor.speed_current[1] <= 10 && motor.speed_current[2] <= 10)
			{
				if(motor.stall_cnt <= 100)
				{
					motor.stall_cnt++;
				}
				else
				{
					motor.start = -1;
				}
			}
		}
		else
		{
			motor.stall_cnt = 0;
		}
		//stop*10ms后关电机
		if(motor.start == 1 && motor.stop >= 1)
		{
			motor.stop--;
			//停车成功
			if(motor.stop == 1)
			{
				motor.start = 0;
				motor.stop = 0;
			}
		}
		
	}
	else
	{
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);
		motor.start = 0;
        motor.stop = 0;
	}
}

/***************************************************************
	*	@brief	电机pid计算
	*	@param	无
	*	@note	Motor_Control调用，还没有根据示波器来测常数
***************************************************************/
void Motor_PID(void)
{
	int16 integral_current_L = 0;
	int16 integral_current_R = 0;
	int16 index_L = 0;
	int16 index_R = 0;
	if(motor.stop >= 1)
	{
		motor.speed_set = 0;
	}
	//差速速度设定
	if(motor.speed_set != 0)
	{
		if(servo.error[0] < - 2 * servo.dead_zone)   //左转
		{
			motor.speed_set_L = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
			motor.speed_set_R = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
		}
		else if(servo.error[0] > 2 * servo.dead_zone) //右转
		{
			motor.speed_set_L = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (0.515 * servo.duty - 632.2)));
			motor.speed_set_R = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (0.515 * servo.duty - 632.2)));
		}
		else
		{
			motor.speed_set_L = motor.speed_set;
			motor.speed_set_R = motor.speed_set;
		}
		if(motor.speed_set_L > SPEED_MAX)
		{
			motor.speed_set_L = SPEED_MAX;
		}
		else if(motor.speed_set_L < SPEED_MIN)
		{
			motor.speed_set_L = SPEED_MIN;
		}
		if(motor.speed_set_R > SPEED_MAX)
		{
			motor.speed_set_R = SPEED_MAX;
		}
		else if(motor.speed_set_R < SPEED_MIN)
		{
			motor.speed_set_R = SPEED_MIN;
		}
	}
	
	motor.speed_ave = (int16)(0.5 * motor.speed_current[0] + 0.2 * motor.speed_current[1] + 0.1 * motor.speed_current[2] + 
							  0.1 * motor.speed_current[3] + 0.1 * motor.speed_current[4]);
	motor.add_speed = motor.speed_ave - motor.speed_ave_last;
    motor.speed_ave_L = (int16)(0.5 * motor.speed_current_L[0] + 0.2 * motor.speed_current_L[1] + 0.1 * motor.speed_current_L[2] + 
							  0.1 * motor.speed_current_L[3] + 0.1 * motor.speed_current_L[4]);
	motor.speed_ave_R = (int16)(0.5 * motor.speed_current_R[0] + 0.2 * motor.speed_current_R[1] + 0.1 * motor.speed_current_R[2] + 
							  0.1 * motor.speed_current_R[3] + 0.1 * motor.speed_current_R[4]);
	//开环加速
	if(motor.speed_ave < 0.9 * motor.speed_set && motor.is_open_loop == 1)
	{
		motor.output_value_L = 9000;
        motor.output_value_R = 9000;
	}
	//闭环pid控制
	else
	{

		//计算delta_kp并更新kp
//		motor.delta_kp = (float32)(0.001 * deltaFuzzyPID(1));
//		motor.kp = (float32)(motor.kp - motor.delta_kp);
		motor.error_L = motor.speed_set_L - motor.speed_ave_L;
		motor.error_R = motor.speed_set_R - motor.speed_ave_R;
		motor.diff_error_L = motor.error_pre_2_L - motor.error_pre_1_L;
		motor.diff_error_R = motor.error_pre_2_R - motor.error_pre_1_R;
		
		
		
		integral_current_L = (int16)(motor.ki * motor.error_L);
		integral_current_R = (int16)(motor.ki * motor.error_R);
		//积分抗饱和，可用积分限幅代替
		
		if(motor.output_value_L > 5000 && motor.error_L < 0) // 加速度a>0 & speed>speed_set 只累加负偏差  
		{
			if(motor.error_L < -80)
			{
				index_L = 0;
			}
			else
			{
				index_L = 1;
				motor.error_integral_L += integral_current_L;
			}
		}
		else if(motor.output_value_L < -3000 && motor.error_L > 0) // a<0 & speed<speed_set 只累加正偏差
		{
			if(motor.error_L > 80)
			{
				index_L = 0;
			}
			else
			{
				index_L = 1;
				motor.error_integral_L += integral_current_L;
			}
		}
		else if(motor.output_value_L >= -3000 && motor.output_value_L <= 5000)
		{
			if(fabs(motor.error_L) > 80)
			{
				index_L = 0;
			}
			else
			{
				index_L = 1;
				motor.error_integral_L += integral_current_L;
			}
		}
		
		if(motor.output_value_R > 5000 && motor.error_R < 0) // 加速度a>0 & speed>speed_set 只累加负偏差  
		{
			if(motor.error_R < -80)
			{
				index_R = 0;
			}
			else
			{
				index_R = 1;
				motor.error_integral_R += integral_current_R;
			}
		}
		else if(motor.output_value_R < -3000 && motor.error_R > 0) // a<0 & speed<speed_set 只累加正偏差
		{
			if(motor.error_R > 80)
			{
				index_R = 0;
			}
			else
			{
				index_R = 1;
				motor.error_integral_R += integral_current_R;
			}
		}
		else if(motor.output_value_R >= -3000 && motor.output_value_R <= 5000)
		{
			if(fabs(motor.error_R) > 80)
			{
				index_R = 0;
			}
			else
			{
				index_R = 1;
				motor.error_integral_R += integral_current_R;
			}
		}		
		
		//减速积分退饱和
		if(motor.error_L < -20)  //speed_set < speed 减速
 		{
			if(motor.error_integral_L > 3000)
			{
				if(motor.error_L < -100)
				{
					motor.error_integral_L *= 0.2;
				}
				else if(motor.error_L < -80)
				{
					motor.error_integral_L *= 0.4;
				}
				else if(motor.error_L < -60)
				{
					motor.error_integral_L *= 0.6;
				}
				else if(motor.error_L < -40)
				{
					motor.error_integral_L *= 0.8;
				}
				else if(motor.error_L < -20)
				{
					motor.error_integral_L *= 0.9;
				}
			}
		}
		if(motor.error_R < -20)
		{
			if(motor.error_integral_R > 3000)
			{
				if(motor.error_R < -100)
				{
					motor.error_integral_R *= 0.2;
				}
				else if(motor.error_R < -80)
				{
					motor.error_integral_R *= 0.4;
				}
				else if(motor.error_R < -60)
				{
					motor.error_integral_R *= 0.6;
				}
				else if(motor.error_R < -40)
				{
					motor.error_integral_R *= 0.8;
				}
				else if(motor.error_R < -20)
				{
					motor.error_integral_R *= 0.9;
				}
			}
		}
        motor.p_valu = (int16)motor.kp * motor.error_L;
        motor.i_valu = (int16)motor.ki * motor.error_integral_L;
        motor.d_valu = (int16)motor.kd * motor.diff_error_L;
        
		motor.output_value_L = (int16)(motor.kp * motor.error_L + index_L * motor.ki * motor.error_integral_L + motor.kd * motor.diff_error_L);
		motor.output_value_R = (int16)(motor.kp * motor.error_R + index_R * motor.ki * motor.error_integral_R + motor.kd * motor.diff_error_R);
        motor.output_value[0] = (motor.output_value_L + motor.output_value_R)/2;
        for(uint8 i=5;i>0;i++)
        {
            motor.output_value[i] = motor.output_value[i-1];
        }
        
	}
	motor.error_pre_2_L = motor.error_pre_1_L;
	motor.error_pre_1_L = motor.error_L;
	motor.error_pre_2_R = motor.error_pre_1_R;
	motor.error_pre_1_R = motor.error_R;
		
}

/***************************************************************
        *	@brief	三角隶属度计算
        *	@param	x：归一化到[-3,3]的值,abc:边界值
	*	@note	无
***************************************************************/
float32 trimf(float32 x,int8 a,int8 b,int8 c)
{
        float32 u;
        if(x>=a&&x<=b)
                u = (x-a)/(b-a);
        else if(x>b&&x<=c)
                u = (c-x)/(c-b);
        else
                u = 0.0;
        return u;
}

/***************************************************************
        *	@brief	模糊pid计算
	*	@param	type=1 delta_kp,type=2 delta_ki,type=3 delta_kd
	*	@note	Motor_PID调用，
***************************************************************/
int16 deltaFuzzyPID(int8 type,int8 e_mark[3],int8 ec_mark[3],float32 error_mf[7],float32 diff_error_mf[7])
{
        float32 sum=0,outvalue=0;
        int16 delta;
        if(type == 1)
        {
                for(int8 m=0;m<3;m++)
                        for(int8 n=0;n<3;n++)
                        {
                                sum+=error_mf[e_mark[m]]*diff_error_mf[ec_mark[n]]*deltaKpMatrix[e_mark[m]][ec_mark[n]];
                        }
                outvalue = sum*motor.diff_error_max/3;
                if(outvalue>=motor.delta_kp_max)        outvalue = motor.delta_kp_max;
                else if(outvalue<=-motor.delta_kp_max)  outvalue = -motor.delta_kp_max;
        }
        else if(type == 2)
        {
                for(int8 m=0;m<3;m++)
                        for(int8 n=0;n<3;n++)
                        {
                                sum+=error_mf[e_mark[m]]*diff_error_mf[ec_mark[n]]*deltaKiMatrix[e_mark[m]][ec_mark[n]];
                        }
                outvalue = sum*motor.diff_error_max/3;
                if(outvalue>=motor.delta_ki_max)        outvalue = motor.delta_ki_max;
                else if(outvalue<=-motor.delta_ki_max)  outvalue = -motor.delta_ki_max;         
        }
        else if(type == 3)
        {
                for(int8 m=0;m<3;m++)
                        for(int8 n=0;n<3;n++)
                        {
                                sum+=error_mf[e_mark[m]]*diff_error_mf[ec_mark[n]]*deltaKdMatrix[e_mark[m]][ec_mark[n]];
                        }
                outvalue = sum*motor.diff_error_max/3;
                if(outvalue>=motor.delta_kd_max)        outvalue = motor.delta_kd_max;
                else if(outvalue<=-motor.delta_kd_max)  outvalue = -motor.delta_kd_max;         
        }
        delta = (int16)(1000 * outvalue);
        return delta;
}
