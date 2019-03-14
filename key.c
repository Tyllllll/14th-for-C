#include "header.h"

/***************************************************************
	*	@brief	按键初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Key_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = KEY1_PTx;     
	GPIO_InitStructure.GPIO_Pins = KEY1_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;//输入
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP | IRQC_DIS;//内部上拉|不产生中断
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = KEY2_PTx;     
	GPIO_InitStructure.GPIO_Pins = KEY2_Pinx;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = KEY3_PTx;     
	GPIO_InitStructure.GPIO_Pins = KEY3_Pinx;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = KEY4_PTx;     
	GPIO_InitStructure.GPIO_Pins = KEY4_Pinx;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = KEY5_PTx;     
	GPIO_InitStructure.GPIO_Pins = KEY5_Pinx;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = KEY6_PTx;     
	GPIO_InitStructure.GPIO_Pins = KEY6_Pinx;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	return STATUS_OK;
}

/***************************************************************
	*	@brief	中断初始化oled
	*	@param	无
	*	@note	无
***************************************************************/
void Reinit_Oled(void)
{
	Oled_Init();
}

/***************************************************************
	*	@brief	按键延时
	*	@param	无
	*	@note	无
***************************************************************/
void Key_Delay(void)
{
	int16 i,j;
	for(i = 0; i < 19000; i++)
	{
		for(j = 0; j < 100; j++)
		{
			asm("nop");
		}
	}
}
