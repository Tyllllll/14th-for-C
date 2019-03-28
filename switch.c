#include "header.h"

/***************************************************************
	*	@brief	拨码开关始化
	*	@param	无
	*	@note	开关拨上去为0
***************************************************************/
uint8 Switch_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = SWITCH1_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH1_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = SWITCH2_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH2_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = SWITCH3_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH3_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = SWITCH4_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH4_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	return STATUS_OK;
}
void flag_clean(void)
{
    feature.straight_state = 0;
    feature.pre_turn_state = 0;
    feature.turn_state = 0;
    feature.cross_state[0] = 0;
    feature.cross_state[1] = 0;
    feature.roundabouts_state = 0;
    feature.breakage_state = 0;
    feature.ramp_state = 0;
    feature.roadblock_state = 0;
    servo.enable = 1;
    servo.which = 0;    
}