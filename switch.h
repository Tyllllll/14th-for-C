#ifndef _SWITCH_H_
#define _SWITCH_H_

#define SWITCH1_PTx		PTE
#define SWITCH1_Pinx	GPIO_Pin24
#define SWITCH2_PTx		PTA
#define SWITCH2_Pinx	GPIO_Pin25
#define SWITCH3_PTx		PTE
#define SWITCH3_Pinx	GPIO_Pin26
#define SWITCH4_PTx		PTA
#define SWITCH4_Pinx	GPIO_Pin24

#define SWITCH1	PTE24_I
#define SWITCH2	PTA25_I
#define SWITCH3	PTE26_I
#define SWITCH4	PTA24_I

uint8 Switch_Gpio_Init(void);
void flag_clean(void);
#endif