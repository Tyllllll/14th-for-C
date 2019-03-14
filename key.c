#include "header.h"

/***************************************************************
	*	@brief	������ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
uint8 Key_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = KEY1_PTx;     
	GPIO_InitStructure.GPIO_Pins = KEY1_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;//����
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP | IRQC_DIS;//�ڲ�����|�������ж�
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
	*	@brief	�жϳ�ʼ��oled
	*	@param	��
	*	@note	��
***************************************************************/
void Reinit_Oled(void)
{
	Oled_Init();
}

/***************************************************************
	*	@brief	������ʱ
	*	@param	��
	*	@note	��
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
