#include "header.h"

/***************************************************************
	*	@brief	电机初始化
	*	@param	无
	*	@note	FTM初始化pwm
***************************************************************/
void Motor_Pwm_Init(void)
{
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
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH1x, 0, MOTOR1_PORTPin1x, ALIGN_LEFT);
	//右正转
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH2x, 0, MOTOR1_PORTPin2x, ALIGN_LEFT);
}

/***************************************************************
	*	@brief	PIT0初始化
	*	@param	无
	*	@note	Motor_PIT为回调函数
***************************************************************/
void Motor_Pit0_Init(void)
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
	
}