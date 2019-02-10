#ifndef _SERVO_H_
#define _SERVO_H_

#define SERVO_PTx		PTD
#define SERVO_Pinx		GPIO_Pin2
#define SERVO_O			PTD2_O
#define SERVO_LOW_PITx 	PIT3
#define SERVO_HIGH_PITx PIT0

//Æ«´ó×óÆ«£¬Æ«Ð¡ÓÒÆ«
#define DEG_MAX  1612
#define DEG_MID  1452
#define DEG_MIN  1287

typedef struct
{
	int16 duty;
	uint8 foresight;
	uint8 fore_min;
	uint8 fore_max;
	float32 kp_left;
	float32 kp_right;
	float32 kp;
	float32 ki;
	float32 kd;
	int16 error[4];
	int16 error_differ[3];
	uint8 dead_zone;
	uint8 dynamic_zone;
}Servo_Class;
extern Servo_Class servo;

void Servo_Gpio_Init(void);
void Servo_PIT_Isr(void);
void Servo_Output(void);
void Servo_Control(void);
void Servo_PID(void);

void servo_up1(void);
void servo_up5(void);
void servo_up10(void);
void servo_down1(void);
void servo_down5(void);
void servo_down10(void);

#endif