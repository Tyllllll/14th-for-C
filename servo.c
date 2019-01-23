#include "header.h"

int16 servo_duty = DEG_MID;
int16 max = DEG_MAX;
int16 min = DEG_MIN;

/***************************************************************
	*	@brief	舵机初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Servo_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure; //舵机引脚初始化
	GPIO_InitStructure.GPIO_PTx = SERVO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	GPIO_InitStructure.GPIO_Pins = SERVO_Pinx;
	LPLD_GPIO_Init(GPIO_InitStructure);

	static PIT_InitTypeDef PIT_InitStructure; //PIT3定时中断，负责拉低电平
	PIT_InitStructure.PIT_Pitx = SERVO_LOW_PITx;
	PIT_InitStructure.PIT_PeriodUs = 100000;
	PIT_InitStructure.PIT_Isr = Servo_PIT_Isr;
	LPLD_PIT_Init(PIT_InitStructure);
	LPLD_PIT_EnableIrq(PIT_InitStructure);//使能中断
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时

	//PIT0 20ms定时中断，负责拉高电平
	PIT_InitStructure.PIT_Pitx = SERVO_HIGH_PITx;
	PIT_InitStructure.PIT_PeriodMs = 20;
	PIT_InitStructure.PIT_Isr = Sevor_Output;
	LPLD_PIT_Init(PIT_InitStructure);
	LPLD_PIT_EnableIrq(PIT_InitStructure);//使能中断
}

/***************************************************************
	*	@brief	PIT0定时中断
	*	@param	无
	*	@note	周期调用，负责拉高电平
***************************************************************/
void Sevor_Output(void)
{
	//限幅
	servo_duty = servo_duty > max ? max : servo_duty;
	servo_duty = servo_duty < min ? min : servo_duty;
	PIT->CHANNEL[3].LDVAL = (servo_duty) * (g_bus_clock / 1000000) - 1;//测试在此 servo_duty_test
	PTD2_O = 1;
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//开始计时 
}

/***************************************************************
	*	@brief	PIT3定时中断
	*	@param	无
	*	@note	负责拉低电平
***************************************************************/
void Servo_PIT_Isr(void)
{
	//拉低电平
	PTD2_O = 0;
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
}

/***************************************************************
	*	@brief	舵机测试
	*	@param	无
	*	@note	无
***************************************************************/
void servo_up1(void)
{
	servo_duty += 1;
}
void servo_up5(void)
{
	servo_duty += 5;
}
void servo_up10(void)
{
	servo_duty += 10;
}
void servo_down1(void)
{
	servo_duty -= 1;
}
void servo_down5(void)
{
	servo_duty -= 5;
}
void servo_down10(void)
{
	servo_duty -= 10;
}