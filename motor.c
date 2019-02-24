#include "header.h"

Motor_Class motor;

/***************************************************************
	*	@brief	电机初始化
	*	@param	无
	*	@note	FTM初始化pwm
***************************************************************/
void Motor_Pwm_Init(void)
{
	motor.kp = 65;
	motor.ki = 20;
	motor.kd = 0;
	motor.is_open_loop = 0;
	motor.dif_const = 1;
	motor.dif_fore = 1;
	
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
	Encoder_Get();
	//实时速度，单位cm/s
	if(encoder.left_num != 0)
	{
		motor.speed_current_left[0] = (int16)(10000 * (float32)encoder.left_num / ENCODER_NUM_PER_METER_LEFT);
	}
	else
	{
		motor.speed_current_left[0] = 0;
	}
	if(encoder.right_num != 0)
	{
		motor.speed_current_right[0] = (int16)(10000 * (float32)encoder.right_num / ENCODER_NUM_PER_METER_RIGHT);
	}
	else
	{
		motor.speed_current_right[0] = 0;
	}
	motor.speed_current[0] = (int16)(0.5 * motor.speed_current_left[0] + 0.5 * motor.speed_current_right[0]);
	Motor_Control();
	for(uint8 i = 4; i > 0; i--)
	{
		motor.speed_current[i] = motor.speed_current[i - 1];
		motor.speed_current_left[i] = motor.speed_current_left[i - 1];
		motor.speed_current_right[i] = motor.speed_current_right[i - 1];
	}
	Encoder_FTM_Clear();
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
			if(motor.output_value_left > MOTOR_MAX_OUTPUT)
			{
				motor.output_value_left = MOTOR_MAX_OUTPUT;
			}
			if(motor.output_value_left < -MOTOR_MAX_OUTPUT)
			{
				motor.output_value_left = -MOTOR_MAX_OUTPUT;
			}
			if(motor.output_value_right > MOTOR_MAX_OUTPUT)
			{
				motor.output_value_right = MOTOR_MAX_OUTPUT;
			}
			if(motor.output_value_right < -MOTOR_MAX_OUTPUT)
			{
				motor.output_value_right = -MOTOR_MAX_OUTPUT;
			}
		}
		if(motor.output_value_left >= 0)
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, motor.output_value_left);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0); 
		}
		else
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, -motor.output_value_left); 
		}
		if(motor.output_value_right >= 0)
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, motor.output_value_right);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0); 
		}
		else
		{
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, -motor.output_value_right); 
		}
		if(motor.start > 1)
		{
			motor.start--;
		}
		//堵转(10ms调用100次为1s)
		if(motor.start == 1 && motor.output_value >= 4000)
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
	*	@note	Motor_Control调用
***************************************************************/
void Motor_PID(void)
{
	int16 integral_current = 0;
	int16 integral_current_left = 0;
	int16 integral_current_right = 0;
	if(motor.stop >= 1)
	{
		motor.speed_set = 0;
	}
	if(motor.speed_set != 0)
	{
		if(servo.error[0] < - 2 * servo.dead_zone)
		{
			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
		}
		else if(servo.error[0] > 2 * servo.dead_zone)
		{
			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (0.515 * servo.duty - 632.2)));
			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (0.515 * servo.duty - 632.2)));
		}
		else
		{
			motor.speed_set_left = motor.speed_set;
			motor.speed_set_right = motor.speed_set;
		}
		if(motor.speed_set_left > SPEED_MAX)
		{
			motor.speed_set_left = SPEED_MAX;
		}
		else if(motor.speed_set_left < SPEED_MIN)
		{
			motor.speed_set_left = SPEED_MIN;
		}
		if(motor.speed_set_right > SPEED_MAX)
		{
			motor.speed_set_right = SPEED_MAX;
		}
		else if(motor.speed_set_right < SPEED_MIN)
		{
			motor.speed_set_right = SPEED_MIN;
		}
	}
	motor.speed_ave = (int16)(0.5 * motor.speed_current[0] + 0.2 * motor.speed_current[1] + 0.1 * motor.speed_current[2] + 
							  0.1 * motor.speed_current[3] + 0.1 * motor.speed_current[4]);
	motor.speed_ave_left = (int16)(0.5 * motor.speed_current_left[0] + 0.2 * motor.speed_current_left[1] + 0.1 * motor.speed_current_left[2] + 
							  0.1 * motor.speed_current_left[3] + 0.1 * motor.speed_current_left[4]);
	motor.speed_ave_right = (int16)(0.5 * motor.speed_current_right[0] + 0.2 * motor.speed_current_right[1] + 0.1 * motor.speed_current_right[2] + 
							  0.1 * motor.speed_current_right[3] + 0.1 * motor.speed_current_right[4]);
	//开环加速
	if(motor.speed_ave < 0.9 * motor.speed_set && motor.is_open_loop == 1)
	{
		motor.output_value = 9000;
	}
	//闭环pid控制
	else
	{
		motor.error = motor.speed_set - motor.speed_ave;
		motor.error_left = motor.speed_set_left - motor.speed_ave_left;
		motor.error_right = motor.speed_set_right - motor.speed_ave_right;
		integral_current = (int16)(motor.ki * motor.error);
		integral_current_left = (int16)(motor.ki * motor.error_left);
		integral_current_right = (int16)(motor.ki * motor.error_right);
		//积分抗饱和，可用积分限幅代替
		if(motor.output_value > 5000 && integral_current < 0)
		{
			motor.error_integral += integral_current;
		}
		else if(motor.output_value < -3000 && integral_current > 0)
		{
			motor.error_integral += integral_current;
		}
		else if(motor.output_value >= -3000 && motor.output_value <= 5000)
		{
			motor.error_integral += integral_current;
		}
		if(motor.output_value_left > 5000 && integral_current_left < 0)
		{
			motor.error_integral_left += integral_current_left;
		}
		else if(motor.output_value_left < -3000 && integral_current_left > 0)
		{
			motor.error_integral_left += integral_current_left;
		}
		else if(motor.output_value_left >= -3000 && motor.output_value_left <= 5000)
		{
			motor.error_integral_left += integral_current_left;
		}
		if(motor.output_value_right > 5000 && integral_current_right < 0)
		{
			motor.error_integral_right += integral_current_right;
		}
		else if(motor.output_value_right < -3000 && integral_current_right > 0)
		{
			motor.error_integral_right += integral_current_right;
		}
		else if(motor.output_value_right >= -3000 && motor.output_value_right <= 5000)
		{
			motor.error_integral_right += integral_current_right;
		}
		//减速积分退饱和
		if(motor.error < -20)
		{
			if(motor.error_integral > 3000)
			{
				if(motor.error < -100)
				{
					motor.error_integral *= 0.2;
				}
				else if(motor.error < -80)
				{
					motor.error_integral *= 0.4;
				}
				else if(motor.error < -60)
				{
					motor.error_integral *= 0.6;
				}
				else if(motor.error < -40)
				{
					motor.error_integral *= 0.8;
				}
				else if(motor.error < -20)
				{
					motor.error_integral *= 0.9;
				}
			}
		}
		if(motor.error_left < -20)
		{
			if(motor.error_integral_left > 3000)
			{
				if(motor.error_left < -100)
				{
					motor.error_integral_left *= 0.2;
				}
				else if(motor.error_left < -80)
				{
					motor.error_integral_left *= 0.4;
				}
				else if(motor.error_left < -60)
				{
					motor.error_integral_left *= 0.6;
				}
				else if(motor.error_left < -40)
				{
					motor.error_integral_left *= 0.8;
				}
				else if(motor.error_left < -20)
				{
					motor.error_integral_left *= 0.9;
				}
			}
		}
		if(motor.error_right < -20)
		{
			if(motor.error_integral_right > 3000)
			{
				if(motor.error_right < -100)
				{
					motor.error_integral_right *= 0.2;
				}
				else if(motor.error_right < -80)
				{
					motor.error_integral_right *= 0.4;
				}
				else if(motor.error_right < -60)
				{
					motor.error_integral_right *= 0.6;
				}
				else if(motor.error_right < -40)
				{
					motor.error_integral_right *= 0.8;
				}
				else if(motor.error_right < -20)
				{
					motor.error_integral_right *= 0.9;
				}
			}
		}
		motor.output_value = (int16)(motor.kp * motor.error + motor.error_integral);
		motor.output_value_left = (int16)(motor.kp * motor.error_left + motor.error_integral_left);
		motor.output_value_right = (int16)(motor.kp * motor.error_right + motor.error_integral_right);
	}
	//转向差速控制
//	if(servo.error[0] < -servo.dead_zone)
//	{
//		//左转
//		motor.output_value_left = (int16)(motor.dif_const * motor.output_value * (motor.dif_fore - MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
//		motor.output_value_right = (int16)(motor.dif_const * motor.output_value * (motor.dif_fore + MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
//	}
//	else if(servo.error[0] > servo.dead_zone)
//	{
//		//右转
//		motor.output_value_left = (int16)(motor.dif_const * motor.output_value * (motor.dif_fore + MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
//		motor.output_value_right = (int16)(motor.dif_const * motor.output_value * (motor.dif_fore - MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
//	}
//	if(servo.error[0] > servo.dead_zone)
//	{
//		//右转
//		motor.output_value_left = (int16)((-0.004435 * servo.duty + servo.dif_const_left) * (motor.output_value / 10 - 20) + 20) * 10;
//		motor.output_value_right = motor.output_value;
//	}
//	else if(servo.error[0] < - servo.dead_zone)
//	{
//		//左转
//		motor.output_value_left = motor.output_value;
//		motor.output_value_right = (int16)((0.004435 * servo.duty - servo.dif_const_right) * (motor.output_value / 10 - 20) + 20) * 10;
//	}
}
