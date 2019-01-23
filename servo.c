#include "header.h"

int16 servo_duty = DEG_MID;
int16 max = DEG_MAX;
int16 min = DEG_MIN;

/***************************************************************
	*	@brief	�����ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
void Servo_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure; //������ų�ʼ��
	GPIO_InitStructure.GPIO_PTx = SERVO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	GPIO_InitStructure.GPIO_Pins = SERVO_Pinx;
	LPLD_GPIO_Init(GPIO_InitStructure);

	static PIT_InitTypeDef PIT_InitStructure; //PIT3��ʱ�жϣ��������͵�ƽ
	PIT_InitStructure.PIT_Pitx = SERVO_LOW_PITx;
	PIT_InitStructure.PIT_PeriodUs = 100000;
	PIT_InitStructure.PIT_Isr = Servo_PIT_Isr;
	LPLD_PIT_Init(PIT_InitStructure);
	LPLD_PIT_EnableIrq(PIT_InitStructure);//ʹ���ж�
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ

	//PIT0 20ms��ʱ�жϣ��������ߵ�ƽ
	PIT_InitStructure.PIT_Pitx = SERVO_HIGH_PITx;
	PIT_InitStructure.PIT_PeriodMs = 20;
	PIT_InitStructure.PIT_Isr = Sevor_Output;
	LPLD_PIT_Init(PIT_InitStructure);
	LPLD_PIT_EnableIrq(PIT_InitStructure);//ʹ���ж�
}

/***************************************************************
	*	@brief	PIT0��ʱ�ж�
	*	@param	��
	*	@note	���ڵ��ã��������ߵ�ƽ
***************************************************************/
void Sevor_Output(void)
{
	//�޷�
	servo_duty = servo_duty > max ? max : servo_duty;
	servo_duty = servo_duty < min ? min : servo_duty;
	PIT->CHANNEL[3].LDVAL = (servo_duty) * (g_bus_clock / 1000000) - 1;//�����ڴ� servo_duty_test
	PTD2_O = 1;
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//��ʼ��ʱ 
}

/***************************************************************
	*	@brief	PIT3��ʱ�ж�
	*	@param	��
	*	@note	�������͵�ƽ
***************************************************************/
void Servo_PIT_Isr(void)
{
	//���͵�ƽ
	PTD2_O = 0;
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
}

/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	��
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