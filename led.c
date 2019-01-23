#include "header.h"

/***************************************************************
	*	@brief	LED初始化
	*	@param	无
	*	@note	默认三个灯亮
***************************************************************/
void Led_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = LED_PTx;
	GPIO_InitStructure.GPIO_Pins = LED1_Pinx | LED2_Pinx | LED3_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	LPLD_GPIO_Init(GPIO_InitStructure);
}