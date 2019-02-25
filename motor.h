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
#define NB -3
#define NM -2
#define NS -1
#define ZO 0
#define PS 1
#define PM 2
#define PB 3
typedef struct
{
	float32 kp;
	float32 ki;
	float32 kd;
    float32 delta_kp;
    float32 delta_ki;
    float32 delta_kd;
    float32 delta_kp_max;
    float32 delta_ki_max;
    float32 delta_kd_max;
    int16 error_max;
    int16 error;
    int16 diff_error;
    int16 error_pre_1_L;
    int16 error_pre_1_R;
    int16 error_pre_2_L;
    int16 error_pre_2_R;
    int16 diff_error_L;
    int16 diff_error_R;
    int16 diff_error_max;
	int8 start;
	int8 stop;
	uint8 stall_cnt;
    int16 speed_ave;
	int16 speed_ave_L;
    int16 speed_ave_R;
    int16 speed_set;
	int16 speed_set_L;
    int16 speed_set_R;
	int16 speed_current[5];
	int16 speed_current_L[5];
	int16 speed_current_R[5];
	int16 error_L;
    int16 error_R;
	int16 error_integral;
	int16 output_value;
	int16 output_value_L;
	int16 output_value_R;
	int8 is_open_loop;
	float32 dif_const;
	float32 dif_fore;
	int16 error_integral_L;
	int16 error_integral_R;
    int16 p_valu;
    int16 i_valu;
    int16 d_valu;
    float32 alldist;
}Motor_Class;
extern Motor_Class motor;
extern int8 mf_paras[];
extern int8 deltaKpMatrix[7][7];
extern int8 deltaKiMatrix[7][7];
extern int8 deltaKdMatrix[7][7];
extern int16 P_value;
extern int16 I_value;
extern int16 D_value;
extern int16 QD_value_L_test;
extern int16 QD_value_R_test;
void Motor_Pwm_Init(void);
void Motor_Pit1_Init(void);
void Motor_PIT(void);
void Motor_Control(void);
void Motor_PID(void);
int16 deltaFuzzyPID(int8 type,int8 e_mark[3],int8 ec_mark[3],float32 error_mf[7],float32 diff_error_mf[7]);
float32 trimf(float32 x,int8 a,int8 b,int8 c);
#endif