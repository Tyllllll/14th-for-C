#ifndef  _Function_H_
#define  _Function_H_
#include "common.h"
#define BUZZER_ON LPLD_GPIO_Output_b(PTC,12,1)
#define BUZZER_OFF LPLD_GPIO_Output_b(PTC,12,0)
float GetBatteryVoltage(void);
float ABS(float a);
float invSqrt(unsigned int x);
float xielv(void);
float get_qulv(int16 point_Ax,int16 point_Ay,int16 point_Bx,int16 point_By,int16 point_Cx,int16 point_Cy);
float least_square_method(void);
void LedOn(uint8 num);
void LedOff(uint8 num);
void FM_Start_di(void);
void FM_Start_didi(void);
void FM_Start_dididi(void);
void sendData_to_PC(void);
void Datadefault(void);
int abs(int a);
int16 MAX(int16 a,int16 b);
int16 MIN(int16 a,int16 b);
extern int16 FM_mode;
extern int16 FM_flag;
extern int16 nn;
extern int16 problem_happen;

#endif