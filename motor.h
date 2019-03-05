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

#define MOTOR_MAX_OUTPUT 6000

typedef struct
{
	float32 kp;
	float32 ki;
	int8 start;
	int8 stop;
	uint8 stall_cnt;
	int16 speed_ave;
	int16 speed_set;
	int16 speed_current[5];
	int16 speed_current_left[5];
	int16 speed_current_right[5];
	int16 error;
	int16 error_integral;
	int16 output_value;
	int16 output_value_left;
	int16 output_value_right;
	int8 is_open_loop;
	
	float32 dif_const;
	float32 dif_fore;
	int16 speed_ave_left;
	int16 speed_ave_right;
	int16 speed_set_left;
	int16 speed_set_right;
	int16 error_left;
	int16 error_right;
	int16 error_integral_left;
	int16 error_integral_right;
}Motor_Class;
extern Motor_Class motor;

void Motor_Pwm_Init(void);
void Motor_Pit1_Init(void);
void Motor_PIT(void);
void Motor_Control(void);
void Motor_PID(void);

#endif