#include "header.h"

Encoder_Class encoder;

/***************************************************************
	*	@brief	编码器初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Encoder_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = ENCODER1_PTx;
	GPIO_InitStructure.GPIO_Pins = ENCODER1_Pin1x | ENCODER1_Pin2x;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;				//输入
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;	//内部上拉|下降沿中断
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	static FTM_InitTypeDef FTM_InitStrcture;
	FTM_InitStrcture.FTM_Ftmx = FTM1;
	FTM_InitStrcture.FTM_Mode = FTM_MODE_QD;
	FTM_InitStrcture.FTM_QdMode = QD_MODE_CNTDIR;
	if(!LPLD_FTM_Init(FTM_InitStrcture))
		return STATUS_FAILED;
	LPLD_FTM_QD_Enable(ENCODER1_FTMx, ENCODER1_PORTPin1x, ENCODER1_PORTPin2x);
	
	GPIO_InitStructure.GPIO_PTx = ENCODER2_PTx;
	GPIO_InitStructure.GPIO_Pins = ENCODER2_Pin1x | ENCODER2_Pin2x;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;				//输入
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;	//内部上拉|下降沿中断
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	FTM_InitStrcture.FTM_Ftmx = FTM2;
	FTM_InitStrcture.FTM_Mode = FTM_MODE_QD;
	FTM_InitStrcture.FTM_QdMode = QD_MODE_CNTDIR;
	if(!LPLD_FTM_Init(FTM_InitStrcture))
		return STATUS_FAILED;
	LPLD_FTM_QD_Enable(ENCODER2_FTMx, ENCODER2_PORTPin1x, ENCODER2_PORTPin2x);
	return STATUS_OK;
}

/***************************************************************
	*	@brief	获取编码器脉冲数
	*	@param	无
	*	@note	无
***************************************************************/
void Encoder_Get(void)
{        
    /******************** 老车 **************************/
	encoder.left_num = (int16)LPLD_FTM_GetCounter(FTM1);
	encoder.right_num = (int16)LPLD_FTM_GetCounter(FTM2);
    /******************** 新车 **************************/
//    encoder.right_num = (int16)(-LPLD_FTM_GetCounter(FTM2));
//	encoder.left_num = (int16)(LPLD_FTM_GetCounter(FTM1));
}

/***************************************************************
	*	@brief	清空计数器
	*	@param	无
	*	@note	无
***************************************************************/
void Encoder_FTM_Clear(void)
{


	encoder.left_num_sum += encoder.left_num;
	encoder.right_num_sum += encoder.right_num;

	LPLD_FTM_ClearCounter(FTM1);
	LPLD_FTM_ClearCounter(FTM2);
}
