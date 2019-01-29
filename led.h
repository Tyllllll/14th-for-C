#ifndef _LED_H_
#define _LED_H_

#define LED_PTx		PTB
#define LED1_Pinx	GPIO_Pin0
#define LED2_Pinx	GPIO_Pin1
#define LED3_Pinx	GPIO_Pin2

#define LED1_O	PTB0_O
#define LED2_O	PTB1_O
#define LED3_O	PTB2_O

#define LED1_Toggle()	LED1_O = !LED1_O
#define LED2_Toggle()	LED2_O = !LED2_O
#define LED3_Toggle()	LED3_O = !LED3_O

void Led_Gpio_Init(void);

#endif