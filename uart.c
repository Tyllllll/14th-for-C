#include "header.h"

uint8 uSendBuf[UartDataNum * 2] = {0};
uint8 FreeCarsDataNum = UartDataNum * 2;
double UartData[9] = {0};
SerialPortType SerialPortRx;

/***************************************************************
	*	@brief	�����ݷ��͵�freecarsʾ������ʾ
	*	@param	��
	*	@note	��
***************************************************************/
void Send_Data_to_FreeCars(void)
{
	push(0, (uint16)servo.duty);
	push(1, (uint16)motor.speed_ave);
	push(2, (uint16)motor.speed_set);
	push(3, (uint16)magnetic.hongwaiceju);
	Send_Data_To_Scope();
}

/***************************************************************
	*	@brief	������λ���������ݲ���ֵ
	*	@param	��
	*	@note	��
***************************************************************/
void UartDebug(void)
{
	if(UartData[0] != 0)
	{
		servo.kp_default = (int16)UartData[0];
	}
	if(UartData[1] != 0)
	{
		servo.kd = (int16)UartData[1];
	}
	if(UartData[2] != 0)
	{
		motor.kp = (int16)UartData[2];
	}
	if(UartData[3] != 0)
	{
		motor.ki = (int16)UartData[3];
	}
	if(UartData[7] != 0)
	{
		motor.start = (int16)UartData[7];
	}
	if(UartData[8] != 0)
	{
		motor.stop = (int16)UartData[8];
	}
	BUZZER_ON;
	LPLD_LPTMR_DelayMs(100);
	BUZZER_OFF;
}

/***************************************************************
	*	@brief	������պ���
	*	@param	CmdNum��0~255�����DATA��0~255����������Ӧ��������
	*	@note	��
***************************************************************/
void UartCmd(uint8 CmdNum,uint8 Data)
{
	if(CmdNum == 2 && Data == 102)
	{
		motor.start = 50;
		motor.error_integral = 0;
		BUZZER_ON;
		LPLD_LPTMR_DelayMs(400);
		motor.stop = 0;
		BUZZER_OFF;  
	}
	if(CmdNum == 2 && Data == 103)
	{
		motor.start = 0;
		motor.stop = 25;
		BUZZER_ON;
		LPLD_LPTMR_DelayMs(400);
		BUZZER_OFF; 
	}
}

/***************************************************************
	*	@brief	UART0��ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
void Uart_Init(void)
{
	static UART_InitTypeDef UART_InitStructure;
	UART_InitStructure.UART_Uartx = UART0;
	UART_InitStructure.UART_BaudRate = 115200;
	UART_InitStructure.UART_TxPin = PTA2;
	UART_InitStructure.UART_RxPin = PTA1;
	UART_InitStructure.UART_RxIntEnable = TRUE;//ʹ�ܽ����ж�
	UART_InitStructure.UART_RxIsr = Uart0_Isr;//�����жϻص�����
	LPLD_UART_Init(UART_InitStructure);//��ʼ��UART
	LPLD_UART_EnableIrq(UART_InitStructure);//ʹ�ܴ������ݽ����ж�
}

/***************************************************************
	*	@brief	UART0�жϺ���
	*	@param	��
	*	@note	��
***************************************************************/
void Uart0_Isr(void)
{
	int i, b, d1;
	uint32 d;
	//��������жϺ���

	if((UART0->S1 & UART_S1_RDRF_MASK) && (UART0->C2 & UART_C2_RIE_MASK))
	{
		SerialPortRx.Data = LPLD_UART_GetChar(UART0);  
		if(SerialPortRx.Stack < UartRxBufferLen)
		{
			SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
			if(SerialPortRx.Stack >= UartRxDataLen//UartRxDataLen����Ϊһ֡
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen] == 0xff//У����ͷ
					&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 1] == 0x55
						&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 2] == 0xaa
							&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 3] == 0x10)
			{
				//double data 9��ͨ������У��
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxDataLen;//��ʼλ
				for(i = b; i < SerialPortRx.Stack - 1; i++)//��У��λ���λ����У��
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{//У��ɹ����������ݽ���
					for(i = 0; i < 9; i++)
					{
						d = SerialPortRx.Buffer[b + i * 4 + 4] * 0x1000000L + SerialPortRx.Buffer[b + i * 4 + 5] * 0x10000L + SerialPortRx.Buffer[b + i * 4 + 6] * 0x100L + SerialPortRx.Buffer[b + i * 4 + 7];
						if(d > 0x7FFFFFFF)
						{
							d1 = 0x7FFFFFFF - d;
						}
						else
						{
							d1 = d;
						}
						UartData[i] = d1;
						UartData[i] /= 65536.0;
					}
					UartDebug();  //תȥ�������յ������ݸ�������
				}
				SerialPortRx.Stack = 0;
			}
			else if(SerialPortRx.Stack >= UartRxCmdLen//UartRxDataLen����Ϊһ֡
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen] == 0xff
					&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
						&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
							&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77)//cmd
			{
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxCmdLen;//��ʼλ
				for(i=b; i<SerialPortRx.Stack - 1; i++)//��У��λ���λ����У��
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{//У��ɹ�
					UartCmd(UartCmdNum, UartCmdData);//������յ����������MCU�������
				}
				SerialPortRx.Stack = 0;
			}
		}
		else
		{
			SerialPortRx.Stack = 0;
		}
	}
}

/***************************************************************
	*	@brief	��ѯ������һ֡���ݣ�����ʱ�������ݳ����й�
	*	@param	��
	*	@note	�����Է����ж����������Ե��ã�����ʱ����㿴�����ĵ�
***************************************************************/
void Send_Data_To_Scope(void)
{
	uint8 i = 0;
	uint8 sum = 0;
	//ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�ֱ���������
	LPLD_UART_PutChar(UART0, (int8)251);
	LPLD_UART_PutChar(UART0, (int8)109);
	LPLD_UART_PutChar(UART0, (int8)37);
	sum += 251; //ȫ�����ݼ���У��
	sum += 109;
	sum += 37;
	for(i = 0; i < FreeCarsDataNum; i++)
	{
		LPLD_UART_PutChar(UART0, uSendBuf[i]);
		sum += uSendBuf[i]; //ȫ�����ݼ���У��
	}
	LPLD_UART_PutChar(UART0, sum);
}

/***************************************************************
	*	@brief	��ʾ����ĳ��ͨ���������������
	*	@param	chanel��ͨ����data ������-32768~32767
	*	@note	������������䵽���������棬��Ȼdate��16λ�ģ����������8λ�������档
***************************************************************/
void push(uint8 chanel, uint16 data)
{
	uSendBuf[chanel * 2] = data / 256;
	uSendBuf[chanel * 2 + 1] = data % 256;
}