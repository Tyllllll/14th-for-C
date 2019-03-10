#ifndef _SERVO_H_
#define _SERVO_H_

#define SERVO_PTx		PTD
#define SERVO_Pinx		GPIO_Pin2
#define SERVO			PTD2_O
#define SERVO_LOW_PITx 	PIT3
#define SERVO_HIGH_PITx PIT0

//ƫ����ƫ��ƫС��ƫ
#define DEG_MAX  1587   //1632            
#define DEG_MID  1427  //1462
#define DEG_MIN  1257   //1292
//#define DEG_MAX  1632            
//#define DEG_MID  1462
//#define DEG_MIN  1292
typedef struct
{
	int16 duty;
	uint8 foresight;
    uint8 fore_default;
	uint8 fore_min;
	uint8 fore_max;
	float32 kp_left;
	float32 kp_right;
	float32 kp;
	float32 ki;
	float32 kd;
	int16 error[10];
	int16 error_differ[10];
	uint8 dead_zone;
	uint8 dynamic_zone;
	float32 dif_const_left;
	float32 dif_const_right;
	int8 enable;
	int8 which;
	int16 duty_record[20];
	int8 counter;
}Servo_Class;
extern Servo_Class servo;

void Servo_Gpio_Init(void);
void Servo_PIT_Isr(void);
void Servo_Output(void);
void Servo_Control(void);
void Servo_PID(void);
int16 Get_Mid_Average(uint8 foresight);
void servo_up1(void);
void servo_up5(void);
void servo_up10(void);
void servo_down1(void);
void servo_down5(void);
void servo_down10(void);
//int  Fuzzy(int P, int D);
void Servo_Fuzzy(void);
#endif