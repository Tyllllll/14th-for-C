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
	OLED_Put6x8Str(20, 2, "Initializing...");
	NVIC_Init();
	Uart_Init();
	Led_Gpio_Init();
	Key_Gpio_Init();
	Switch_Gpio_Init();
	Buzzer_Gpio_Init();
	Magnetic_Adc_Init();
	Motor_Pit1_Init();
	Motor_Pwm_Init();
	Servo_Gpio_Init();
	Encoder_Init();
	Speed_Init();
    MPU6050_Init();
	switch(ubyCamera_Init())
	{
	case 0:
		OLED_Put6x8Str(20, 2, "Initialized!!!  ");
		break;
	case 1:
		OLED_Put6x8Str(20, 2, "Error: Camera 1.");
		while(1);
	case 2:
		OLED_Put6x8Str(20, 2, "Error: Camera 2.");
		while(1);
	case 3:
		OLED_Put6x8Str(20, 2, "Error: Camera 3.");
		while(1);
	case 4:
		OLED_Put6x8Str(20, 2, "Error: Camera 4.");
		while(1);
	default:
		OLED_Put6x8Str(20, 2, "Error: Unkonwn.");
		while(1);
	}
	OLED_Fill(0x00);
	EnableInterrupts;
}

/***************************************************************
	*	@brief	中断优先级分配
	*	@param	无
	*	@note	无
***************************************************************/
void NVIC_Init(void)
{
	static NVIC_InitTypeDef NVIC_InitStructure;
	//串口中断
	NVIC_InitStructure.NVIC_IRQChannel = UART0_RX_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	LPLD_NVIC_Init(NVIC_InitStructure); 
	//舵机打脚中断
	NVIC_InitStructure.NVIC_IRQChannel = PIT3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	LPLD_NVIC_Init(NVIC_InitStructure);
	//场中断函数
	NVIC_InitStructure.NVIC_IRQChannel = PORTB_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	LPLD_NVIC_Init(NVIC_InitStructure);
	//DMA中断函数
	NVIC_InitStructure.NVIC_IRQChannel = DMA0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	LPLD_NVIC_Init(NVIC_InitStructure);
	//电机输出中断函数
	NVIC_InitStructure.NVIC_IRQChannel = PIT1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	LPLD_NVIC_Init(NVIC_InitStructure);
}