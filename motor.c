#include "header.h"

Motor_Class motor;

/***************************************************************
	*	@brief	电机初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Motor_Init(void)
{
	motor.kp = 65;
	motor.ki = 8;
	motor.is_open_loop = 0;
	motor.dif_const = 1;
	motor.dif_fore = 1;
	motor.distance_set = 0;
	
    static FTM_InitTypeDef FTM_InitStructure;
    FTM_InitStructure.FTM_Ftmx = MOTOR_FTMx;	//选择FTM通道
    FTM_InitStructure.FTM_Mode = FTM_MODE_PWM;	//FTM0的工作模式是PWM波输出
    FTM_InitStructure.FTM_PwmDeadtimeCfg = MOTOR_PWMCFG
    FTM_InitStructure.FTM_PwmFreq = 10000;		//PWM频率10KHz    
	
	if(!LPLD_FTM_Init(FTM_InitStructure))
		return STATUS_FAILED;
	//左正转
	if(!LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR1_FTM_CH1x, 0, MOTOR1_PORTPin1x, ALIGN_LEFT))
		return STATUS_FAILED;
	//左反转
	if(!LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR1_FTM_CH2x, 0, MOTOR1_PORTPin2x, ALIGN_LEFT))
		return STATUS_FAILED;
	//右反转
	if(!LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH1x, 0, MOTOR2_PORTPin1x, ALIGN_LEFT))
		return STATUS_FAILED;
	//右正转
	if(!LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH2x, 0, MOTOR2_PORTPin2x, ALIGN_LEFT))
		return STATUS_FAILED;
	
	static PIT_InitTypeDef PIT_InitStructure; 
	PIT_InitStructure.PIT_Pitx = PIT1;
	PIT_InitStructure.PIT_PeriodMs = 10;	//定时周期10毫秒
	PIT_InitStructure.PIT_Isr = Motor_PIT;	//设置中断函数
	if(!LPLD_PIT_Init(PIT_InitStructure))	//初始化PIT1
		return STATUS_FAILED;
	LPLD_PIT_EnableIrq(PIT_InitStructure);	//使能PIT1
	
	return STATUS_OK;
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
	motor.distance_all += (50.0 * encoder.left_num / ENCODER_NUM_PER_METER_LEFT) + (50.0 * encoder.right_num / ENCODER_NUM_PER_METER_RIGHT);//10ms前进的距离 单位cm
	if(motor.distance_cnt_en == 1)
	{
		motor.distance_temp += (50.0 * encoder.left_num / ENCODER_NUM_PER_METER_LEFT) + (50.0 * encoder.right_num / ENCODER_NUM_PER_METER_RIGHT);//10ms前进的距离 单位cm
	}
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
			else if(motor.output_value_left < -MOTOR_MAX_OUTPUT)
			{
				motor.output_value_left = -MOTOR_MAX_OUTPUT;
			}
			if(motor.output_value_right > MOTOR_MAX_OUTPUT)
			{
				motor.output_value_right = MOTOR_MAX_OUTPUT;
			}
			else if(motor.output_value_right < -MOTOR_MAX_OUTPUT)
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
	int16 integral_current_left = 0;
	int16 integral_current_right = 0;
	if(motor.stop >= 1)
	{
		motor.speed_set = 0;
	}
	if(motor.speed_set != 0)
	{
		if(servo.error[0] < - 10 * servo.dead_zone)
		{
//			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
//			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (-0.515 * servo.duty + 862.4)));
//			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (-0.9878 * servo.duty + 1658)));
//			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (-0.9878 * servo.duty + 1658)));
			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0
																				/ (0.007436 * servo.duty * servo.duty - 24.34 * servo.duty + 19980)));
			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0
																				 / (0.007436 * servo.duty * servo.duty - 24.34 * servo.duty + 19980)));
		}
		else if(servo.error[0] > 10 * servo.dead_zone)
		{
//			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (0.515 * servo.duty - 632.2)));
//			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (0.515 * servo.duty - 632.2)));
//			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0 / (1.232 * servo.duty - 1534)));
//			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0 / (1.232 * servo.duty - 1534)));
			motor.speed_set_left = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore + MODEL_WIDTH / 2.0
																				/ (0.006818 * servo.duty * servo.duty - 17.11 * servo.duty + 10790)));
			motor.speed_set_right = (int16)(motor.dif_const * motor.speed_set * (motor.dif_fore - MODEL_WIDTH / 2.0
																				 / (0.006818 * servo.duty * servo.duty - 17.11 * servo.duty + 10790)));
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
		motor.error_left = motor.speed_set_left - motor.speed_ave_left;
		motor.error_right = motor.speed_set_right - motor.speed_ave_right;
		integral_current_left = (int16)(motor.ki * motor.error_left);
		integral_current_right = (int16)(motor.ki * motor.error_right);
		//积分抗饱和，可用积分限幅代替
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
		motor.output_value_left = (int16)(motor.kp * motor.error_left + motor.error_integral_left);
		motor.output_value_right = (int16)(motor.kp * motor.error_right + motor.error_integral_right);
		motor.output_value = (motor.output_value_left + motor.output_value_right) / 2;
	}
}
