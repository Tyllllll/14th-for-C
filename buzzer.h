#ifndef _BUZZER_H_
#define _BUZZER_H_
#include "common.h"

#define BUZZER_PTx	PTC
#define BUZZER_Pinx	GPIO_Pin12
#define BUZZER_ON LPLD_GPIO_Output_b(PTC,12,1)
#define BUZZER_OFF LPLD_GPIO_Output_b(PTC,12,0)

void Buzzer_Gpio_Init(void);

#endif