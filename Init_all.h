#ifndef _INIT_ALL_H_
#define _INITH_All_H_
#include "common.h"
  extern int8 WhichBuffer;
  extern unsigned char Flag_Test_ImageComplete;

void UART_Init(void);
void dma_init(void);
void gpio_init(void);
void Servo_gpio_init(void);
void Motor_pwm_init(void);
void key_init_gpio();
void Encoder_QD_init(void);
void boma_init_gpio(void);
void Oled_GPIO_Init(void);
void  BatteryVoltage_Init(void);
void pwm_duoji_init(void);
void led_init(void);
void Buzzer_gpio_init(void);
void pit0_init(void);
void Motor_PIT(void);
void NVIC_Init(void);
//extern /*static*/  PIT_InitTypeDef pit0_init_struct;
//extern /*static*/  PIT_InitTypeDef Servo_PIT; 
//extern /*static*/ DMA_InitTypeDef dma_init_struct;
//extern /*static*/ GPIO_InitTypeDef ptb16_init;
//extern /*static*/ UART_InitTypeDef term_port_structure;













#endif