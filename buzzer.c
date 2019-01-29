#include "header.h"

/***************************************************************
	*	@brief	蜂鸣器初始化
	*	@param	无
	*	@note	默认不响
***************************************************************/
void Buzzer_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = BUZZER_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Pins = BUZZER_Pinx;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	LPLD_GPIO_Init(GPIO_InitStructure);
}