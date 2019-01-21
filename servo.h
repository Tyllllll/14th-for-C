#ifndef _Servo_H_
#define _Servo_H_
#include "common.h"
void Sevor_control(void);
void Sevor_pid(void);
void Servo_PIT_Isr(void);
void Sevor_output(void);

 typedef struct
{
   float Kp;
   float Ki;
   float Kd;
} PID_InitTypeDef;
extern PID_InitTypeDef Ser_PID;
extern int16 foresight;
extern int16 foresight1;
extern int16 servo_duty;
extern int16 mid;
extern int16 max;
extern int16 min;
extern int16 error[5];
extern int16 error_cha[4];
extern int16 S_d;
extern int16 S_p;
extern int16 fore_max;
extern int16 fore_min;
extern int16 speed_now;
extern float S_kp;
extern float S_kp_r;
extern float S_kd;

#define DEG_MAX  1612//ÓÒ1612
#define DEG_MID  1452//¿Õ´ò1452l
#define DEG_MIN  1292//×ó1292







#endif 