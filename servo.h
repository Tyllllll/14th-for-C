#ifndef _SERVO_H_
#define _SERVO_H_

#define SERVO_PTx		PTD
#define SERVO_Pinx		GPIO_Pin2
#define SERVO			PTD2_O
#define SERVO_LOW_PITx 	PIT3
#define SERVO_HIGH_PITx PIT0

//Æ«´ó×óÆ«£¬Æ«Ð¡ÓÒÆ«
#ifdef NEWCAR

#define DEG_MAX  1587
#define DEG_MID  1427
#define DEG_MIN  1257

#else

#define DEG_MAX  1630
#define DEG_MID  1448
#define DEG_MIN  1285

#endif

typedef struct
{
	int16 duty;
	int16 duty_record[10];
	uint8 foresight;
	float32 kp;
	float32 kd;
	int16 error[5];
	int16 error_differ;
	uint8 dead_zone;
	int8 enable;
	int8 which;
	int8 counter;
}Servo_Class;
extern Servo_Class servo;

uint8 Servo_Init(void);
void Servo_PIT_Isr(void);
void Servo_Output(void);
void Servo_Control(void);
void Servo_Error_Filter(void);
void Servo_Fuzzy(void);
int16 Servo_Get_Foresight(void);

void servo_up1(void);
void servo_up5(void);
void servo_up10(void);
void servo_down1(void);
void servo_down5(void);
void servo_down10(void);

#endif