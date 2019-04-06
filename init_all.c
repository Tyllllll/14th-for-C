#include "header.h"

/***************************************************************
	*	@brief	总初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Init_All(void)
{
    DisableInterrupts;
	if(!Oled_Init())
		while(1);
	OLED_Put6x8Str(20, 2, "initializing...");
	if(!NVIC_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error NVIC");
		return STATUS_FAILED;
	}
	if(!Adc_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error ADC");
		return STATUS_FAILED;
	}
	OLED_Fill(0x00);
	OLED_Put6x8Str(20, 2, "initializing");
	OLED_Put6x8Str(15, 3, "ADC zero drift");
	if(!Adc_Find_Zero_Drift())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error Magnetic");
		OLED_Put6x8Str(20, 3, "Zero Drift");
		while(!Adc_Find_Zero_Drift());
	}
	if(!Uart_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error UART");
		return STATUS_FAILED;
	}
	if(!Led_Gpio_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error LED");
		return STATUS_FAILED;
	}
	if(!Key_Gpio_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error KEY");
		return STATUS_FAILED;
	}
	if(!Switch_Gpio_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error SWITCH");
		return STATUS_FAILED;
	}
	if(!Buzzer_Gpio_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error BUZZER");
		return STATUS_FAILED;
	}
    i2c.I2C_Init();
//	if(!IIC_Init())
//	{
//		OLED_Fill(0x00);
//		OLED_Put6x8Str(20, 2, "Error IIC");
//		return STATUS_FAILED;
//	}
//	MPU6050_Init();
//	MPU6050_Zero_Drift();
	if(!Motor_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error Motor");
		return STATUS_FAILED;
	}
	if(!Servo_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error Servo");
		return STATUS_FAILED;
	}
	if(!Encoder_Init())
	{
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error Encoder");
		return STATUS_FAILED;
	}
	Speed_Init();
	switch(ubyCamera_Init())
	{
	case 0:
		break;
	case 1:
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error: Camera 1.");
		return STATUS_FAILED;
	case 2:
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error: Camera 2.");
		return STATUS_FAILED;
	case 3:
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error: Camera 3.");
		return STATUS_FAILED;
	case 4:
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error: Camera 4.");
		return STATUS_FAILED;
	default:
		OLED_Fill(0x00);
		OLED_Put6x8Str(20, 2, "Error: Unkonwn.");
		return STATUS_FAILED;
	}
	Adc_Find_Max_Value();
	OLED_Fill(0x00);
	OLED_Put6x8Str(20, 2, "initialized!!!");
	LPLD_LPTMR_DelayMs(1000);
	EnableInterrupts;
	return STATUS_OK;
}

/***************************************************************
	*	@brief	中断优先级分配
	*	@param	无
	*	@note	无
***************************************************************/
uint8 NVIC_Init(void)
{
	static NVIC_InitTypeDef NVIC_InitStructure;
	//串口中断
	NVIC_InitStructure.NVIC_IRQChannel = UART0_RX_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	if(!LPLD_NVIC_Init(NVIC_InitStructure))
		return STATUS_FAILED;
	//舵机打脚中断
	NVIC_InitStructure.NVIC_IRQChannel = PIT3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	if(!LPLD_NVIC_Init(NVIC_InitStructure))
		return STATUS_FAILED;
	//场中断函数
	NVIC_InitStructure.NVIC_IRQChannel = PORTB_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	if(!LPLD_NVIC_Init(NVIC_InitStructure))
		return STATUS_FAILED;
	//DMA中断函数
	NVIC_InitStructure.NVIC_IRQChannel = DMA0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	if(!LPLD_NVIC_Init(NVIC_InitStructure))
		return STATUS_FAILED;
	//电机输出中断函数
	NVIC_InitStructure.NVIC_IRQChannel = PIT1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	if(!LPLD_NVIC_Init(NVIC_InitStructure))
		return STATUS_FAILED;
	return STATUS_OK;
}