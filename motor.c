#include "header.h"

float32 kp = 65;
float32 ki = 20;
float32 kd = 0;
Motor_Class motor;

/***************************************************************
	*	@brief	�����ʼ��
	*	@param	��
	*	@note	FTM��ʼ��pwm
***************************************************************/
void Motor_Pwm_Init(void)
{
    static FTM_InitTypeDef FTM_InitStructure;
    FTM_InitStructure.FTM_Ftmx = MOTOR_FTMx;	//ѡ��FTMͨ��
    FTM_InitStructure.FTM_Mode = FTM_MODE_PWM;	//FTM0�Ĺ���ģʽ��PWM�����
    FTM_InitStructure.FTM_PwmDeadtimeCfg = MOTOR_PWMCFG
    FTM_InitStructure.FTM_PwmFreq = 10000;		//PWMƵ��10KHz    

	LPLD_FTM_Init(FTM_InitStructure);
	//����ת 
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR1_FTM_CH1x, 0, MOTOR1_PORTPin1x, ALIGN_LEFT);
	//��ת
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR1_FTM_CH2x, 0, MOTOR1_PORTPin2x, ALIGN_LEFT);
	//�ҷ�ת
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH1x, 0, MOTOR2_PORTPin1x, ALIGN_LEFT);
	//����ת
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH2x, 0, MOTOR2_PORTPin2x, ALIGN_LEFT);
}

/***************************************************************
	*	@brief	PIT1��ʼ��
	*	@param	��
	*	@note	Motor_PITΪ�ص�����
***************************************************************/
void Motor_Pit1_Init(void)
{
	static  PIT_InitTypeDef PIT_InitStructure; 
	PIT_InitStructure.PIT_Pitx = PIT1;
	PIT_InitStructure.PIT_PeriodMs = 10;	//��ʱ����10����
	PIT_InitStructure.PIT_Isr = Motor_PIT;	//�����жϺ���
	LPLD_PIT_Init(PIT_InitStructure);		//��ʼ��PIT1
	LPLD_PIT_EnableIrq(PIT_InitStructure);	//ʹ��PIT1
}

/***************************************************************
	*	@brief	����ٶȵ���
	*	@param	��
	*	@note	10ms���ڵ���
***************************************************************/
void Motor_PIT(void)
{
	Encoder_Get();
	//ʵʱ�ٶȣ���λcm/s
	motor.speed_current_left[0] = encoder.left_num == 0 ? 0 : (int16)(10000 * (float32)encoder.left_num / ENCODER_NUM_PER_METER_LEFT);
	motor.speed_current_right[0] = encoder.right_num == 0 ? 0 : (int16)(10000 * (float32)encoder.right_num / ENCODER_NUM_PER_METER_RIGHT);
	motor.speed_current[0] = (int16)(0.5 * motor.speed_current_left[0] + 0.5 * motor.speed_current_right[0]);
	Motor_Control();
	for(uint8 i = 4; i > 0; i--)
	{
		motor.speed_current[i] = motor.speed_current[i - 1];
	}
	//��ռ�����
	LPLD_FTM_ClearCounter(FTM2);
	LPLD_FTM_ClearCounter(FTM1);
}

/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	Motor_PIT����
***************************************************************/
void Motor_Control(void)
{
	Motor_PID();
	if(motor.start > 0)
	{
		if(motor.speed_ave >= 0.9 * motor.speed_set)
		{
			motor.output_value_left = motor.output_value_left > MOTOR_MAX_OUTPUT ? MOTOR_MAX_OUTPUT : motor.output_value_left;
			motor.output_value_left = motor.output_value_left < -MOTOR_MAX_OUTPUT ? -MOTOR_MAX_OUTPUT : motor.output_value_left;
			motor.output_value_right = motor.output_value_right > MOTOR_MAX_OUTPUT ? MOTOR_MAX_OUTPUT : motor.output_value_right;
			motor.output_value_right = motor.output_value_right < -MOTOR_MAX_OUTPUT ? -MOTOR_MAX_OUTPUT : motor.output_value_right;
		}
		else if(motor.speed_ave > 0)
		{
			motor.output_value_left = motor.output_value_left > 10000 ? 10000 : motor.output_value_left;
			motor.output_value_right = motor.output_value_right > 10000 ? 10000 : motor.output_value_right;
		}
	}
	else if(motor.start <= 0)
	{
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);
		motor.start = 0;
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
	//��ת(10ms����100��Ϊ1s)
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
				motor.start = 0;
			}
		}
		else
		{
			motor.stall_cnt = 0;
		}
	}
	//stop*10ms��ص��
	if(motor.start == 1 && motor.stop >= 1)
	{
		if(motor.stop > 1)
		{
			motor.stop--;
		}
		//ͣ���ɹ�
		else
		{
			motor.start = 0;
			motor.stop = 0;
		}
	}
}

/***************************************************************
	*	@brief	���pid����
	*	@param	��
	*	@note	Motor_Control����
***************************************************************/
void Motor_PID(void)
{
	motor.kp = kp;
	motor.ki = ki;
	motor.kp = kp;
	
	if(motor.stop >= 1)
	{
		motor.speed_set = 0;
	}
	motor.speed_ave = (int16)(0.5 * motor.speed_current[0] + 0.2 * motor.speed_current[1] + 0.1 * motor.speed_current[2] + 
							  0.1 * motor.speed_current[3] + 0.1 * motor.speed_current[4]);
	//��������
	if(motor.speed_ave < 0.9 * motor.speed_set)
	{
		motor.output_value = 9000;
	}
	//�ջ�pid����
	else
	{
		motor.error = motor.speed_set - motor.speed_ave;
		//���ֿ����ͣ����û����޷�����
		if(motor.output_value > 5000 && (int16)(motor.ki * motor.error) < 0)
		{
			motor.integral += (int16)(motor.ki * motor.error);
		}
		else if(motor.output_value < -3000 && (int16)(motor.ki * motor.error) > 0)
		{
			motor.integral += (int16)(motor.ki * motor.error);
		}
		else if(motor.output_value >= -3000 && motor.output_value <= 5000)
		{
			motor.integral += (int16)(motor.ki * motor.error);
		}
		//���ٻ����˱���
		if(motor.error < -20)
		{
			if(motor.integral > 3000)
			{
				if(motor.error < -100)
				{
					motor.integral *= 0.2;
				}
				else if(motor.error < -80)
				{
					motor.integral *= 0.4;
				}
				else if(motor.error < -60)
				{
					motor.integral *= 0.6;
				}
				else if(motor.error < -40)
				{
					motor.integral *= 0.8;
				}
				else if(motor.error < -20)
				{
					motor.integral *= 0.9;
				}
			}
		}
		motor.output_value = (int16)(motor.kp * motor.error + motor.integral);
	}
	//ת����ٿ���
}
