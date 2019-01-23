#include "header.h"

/***************************************************************
	*	@brief	总初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Init_All(void)
{
    DisableInterrupts;
	Oled_Gpio_Init();
	OLED_Put6x8Str(20, 2, "initializing...");
	Led_Gpio_Init();
	Key_Gpio_Init();
	Switch_Gpio_Init();
	Buzzer_Gpio_Init();
	switch(ubyCamera_Init())
	{
	case 0:
		OLED_Put6x8Str(20, 2, "initialized!!!");
		break;
	case 1:
		OLED_Put6x8Str(20, 2, "Error: Camera 1.");
		break;
	case 2:
		OLED_Put6x8Str(20, 2, "Error: Camera 2.");
		break;
	case 3:
		OLED_Put6x8Str(20, 2, "Error: Camera 3.");
		break;
	case 4:
		OLED_Put6x8Str(20, 2, "Error: Camera 4.");
		break;
	default:
		OLED_Put6x8Str(20, 2, "Error: Unkonwn.");
		break;
	}
	Motor_Pit0_Init();
	Motor_Pwm_Init();
	Servo_Gpio_Init();
	Encoder_Init();
	//uart0_isr();
	EnableInterrupts;
}