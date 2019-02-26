#ifndef _KEY_H_
#define _KEY_H_

#define KEY1_PTx		PTE
#define KEY1_Pinx		GPIO_Pin28
#define KEY2_PTx		PTE
#define KEY2_Pinx		GPIO_Pin27
#define KEY3_PTx		PTA
#define KEY3_Pinx		GPIO_Pin7
#define KEY4_PTx		PTA
#define KEY4_Pinx		GPIO_Pin5
#define KEY5_PTx		PTA
#define KEY5_Pinx		GPIO_Pin12
#define KEY6_PTx		PTA
#define KEY6_Pinx		GPIO_Pin11

#define KEY1	PTE28_I
#define KEY2	PTE27_I
#define KEY3	PTA7_I
#define KEY4	PTA5_I
#define KEY5	PTA12_I
#define KEY6	PTA11_I

void Key_Gpio_Init(void);
void Key_Delay(void);

#endif