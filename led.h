#ifndef _LED_H_
#define _LED_H_
#include "common.h"

#define LED_PTx		PTB
#define LED1_Pinx	GPIO_Pin0
#define LED2_Pinx	GPIO_Pin1
#define LED3_Pinx	GPIO_Pin2

#define LED1_O	PTB0_O
#define LED2_O	PTB1_O
#define LED3_O	PTB2_O

#define LED1_Toggle()	PTB0_O = !PTB0_O
#define LED2_Toggle()	PTB1_O = !PTB1_O
#define LED3_Toggle()	PTB2_O = !PTB2_O

void Led_Gpio_Init(void);

#endif