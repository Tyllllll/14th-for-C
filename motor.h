#ifndef _MOTOR_H_
#define _MOTOR_H_

#define MOTOR_Pitx PIT1
#define MOTOR_FTMx FTM0
#define MOTOR_PWMCFG DEADTIME_CH67 | DEADTIME_CH45;
#define MOTOR1_FTM_CH1x FTM_Ch4		//左正
#define MOTOR1_PORTPin1x PTD4
#define MOTOR1_FTM_CH2x FTM_Ch5		//左反
#define MOTOR1_PORTPin2x PTD5
#define MOTOR2_FTM_CH1x FTM_Ch6		//右反
#define MOTOR2_PORTPin1x PTD6
#define MOTOR2_FTM_CH2x FTM_Ch7		//右正
#define MOTOR2_PORTPin2x PTD7

void Motor_Pwm_Init(void);
void Motor_Pit1_Init(void);
void Motor_PIT(void);

#endif