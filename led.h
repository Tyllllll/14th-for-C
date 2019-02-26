#ifndef _LED_H_
#define _LED_H_

#define LED_PTx		PTB
#define LED1_Pinx	GPIO_Pin0
#define LED2_Pinx	GPIO_Pin1
#define LED3_Pinx	GPIO_Pin2

#define LED1	PTB0_O
#define LED2	PTB1_O
#define LED3	PTB2_O

#define LED1_Toggle()	LED1 = !LED1
#define LED2_Toggle()	LED2 = !LED2
#define LED3_Toggle()	LED3 = !LED3

void Led_Gpio_Init(void);

#endif