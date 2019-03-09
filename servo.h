#ifndef _SERVO_H_
#define _SERVO_H_

#define SERVO_PTx		PTD
#define SERVO_Pinx		GPIO_Pin2
#define SERVO			PTD2_O
#define SERVO_LOW_PITx 	PIT3
#define SERVO_HIGH_PITx PIT0

//Æ«´ó×óÆ«£¬Æ«Ð¡ÓÒÆ«
#define DEG_MAX  1630
#define DEG_MID  1448
#define DEG_MIN  1285

typedef struct
{
	int16 duty;
	uint8 foresight;
	uint8 fore_default;
	float32 kp;
	float32 kp_record[3];
	float32 kd;
	int16 error[5];
	int16 error_differ[4];
	uint8 dead_zone;
	int8 enable;
	int8 which;
	int8 counter;
	int8 ramp_change;
}Servo_Class;
extern Servo_Class servo;

uint8 Servo_Init(void);
void Servo_PIT_Isr(void);
void Servo_Output(void);
void Servo_Control(void);
void Servo_Foresight_Change(void);
void Servo_Fuzzy(void);

void servo_up1(void);
void servo_up5(void);
void servo_up10(void);
void servo_down1(void);
void servo_down5(void);
void servo_down10(void);

#endif