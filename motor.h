#ifndef _Motor_H_
#define _Motor_H_
#include "common.h"
#define QD_value_L  5746 //编码器一米累加的脉冲数
#define QD_value_R  5746 //编码器一米累加的脉冲数


void Motor_control(void);
void Motor_pid(void);
extern int16 motor_flag;
typedef struct
{
  float Kp;
  float Ki;
  float Kd;
  float Kp_float;
  float Ki_float;
  int Speed_ave;   //平均速度
  int SpeedSet;    //目标速度
  int Speed_test[5];
  int Speed_test_L[5];
  int Speed_test_R[5];
  int OutValue;  //车体输出速度
  int OutValue_L;
  int OutValue_R;
  float Distance;
  int IntSum_now;
  int IntSum_all; 
  float P_value;
  float I_value;
  int16 differential[2];//微分先行
  int16 Speed_Error;

}MotorPID_TypeDef ;

extern MotorPID_TypeDef MotorPID;
extern float All_distance;
extern float jl_distance;
extern float set_dist;

extern int Qd_Result;
extern int8 stop_flag;
extern float M_Kp;
extern float M_Ki;
extern float M_Kd;

#endif