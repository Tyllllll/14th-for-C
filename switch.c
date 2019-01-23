#include "header.h"

/***************************************************************
	*	@brief	���뿪��ʼ��
	*	@param	��
	*	@note	���ز���ȥΪ0
***************************************************************/
void Switch_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = SWITCH1_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH1_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	LPLD_GPIO_Init(GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PTx = SWITCH2_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH2_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	LPLD_GPIO_Init(GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PTx = SWITCH3_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH3_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	LPLD_GPIO_Init(GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PTx = SWITCH4_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH4_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	LPLD_GPIO_Init(GPIO_InitStructure);
}