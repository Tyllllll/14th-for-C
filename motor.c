#include "header.h"

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
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH1x, 0, MOTOR1_PORTPin1x, ALIGN_LEFT);
	//����ת
	LPLD_FTM_PWM_Enable(MOTOR_FTMx, MOTOR2_FTM_CH2x, 0, MOTOR1_PORTPin2x, ALIGN_LEFT);
}

/***************************************************************
	*	@brief	PIT0��ʼ��
	*	@param	��
	*	@note	Motor_PITΪ�ص�����
***************************************************************/
void Motor_Pit0_Init(void)
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
	
}