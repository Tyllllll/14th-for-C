#ifndef _SERVO_H_
#define _SERVO_H_
#include "common.h"

#define SERVO_PTx	PTD
#define SERVO_Pinx	GPIO_Pin2
#define SERVO_LOW_PITx PIT3
#define SERVO_HIGH_PITx PIT0

//Æ«´ó×óÆ«£¬Æ«Ð¡ÓÒÆ«
#define DEG_MAX  1612
#define DEG_MID  1452
#define DEG_MIN  1292

void Servo_Gpio_Init(void);
void Servo_PIT_Isr(void);
void Sevor_Output(void);

void servo_up1(void);
void servo_up5(void);
void servo_up10(void);
void servo_down1(void);
void servo_down5(void);
void servo_down10(void);

#endif