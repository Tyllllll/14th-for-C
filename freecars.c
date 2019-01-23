#include "header.h"

double UartData[9] = {0};
SerialPortType SerialPortRx;
//��ʾFreecars��λ���޸�����

/***************************************************************
	*	@brief	uart��ʼ��
	*	@param	��
	*	@note	������115200
***************************************************************/
void UART_Init(void)
{
	static UART_InitTypeDef term_port_structure;
	term_port_structure.UART_Uartx = UART0;       
	term_port_structure.UART_BaudRate = 115200;
	term_port_structure.UART_TxPin=PTA2;
	term_port_structure.UART_RxPin=PTA1;
	term_port_structure.UART_RxIntEnable = TRUE;	//ʹ�ܽ����ж�
	term_port_structure.UART_RxIsr = uart0_isr;		//�����жϻص�����
	LPLD_UART_Init(term_port_structure);			//��ʼ��UART
	LPLD_UART_EnableIrq(term_port_structure);		//ʹ�ܴ������ݽ����ж�
}

/***************************************************************
	*	@brief	uart�����жϻص�����
	*	@param	��
	*	@note	��
***************************************************************/
void uart0_isr(void)
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
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+1] == 0x55
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+2] == 0xaa
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+3] == 0x10)
			{
				//double data 9��ͨ������У��
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxDataLen;//��ʼλ
				for(i=b; i<SerialPortRx.Stack-1; i++)//��У��λ���λ����У��
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{
					//У��ɹ����������ݽ���
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
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  == 0xff
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77)//cmd
			{
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxCmdLen;//��ʼλ
				for(i = b; i < SerialPortRx.Stack - 1; i++)//��У��λ���λ����У��
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{
					//У��ɹ�
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

void UartDebug(void)
{
	push(15, (int16)UartData[0]);//�����ݷ��ͻ�ȥ�۲�
	push(1, (int16)UartData[6]);//�����ݷ��ͻ�ȥ�۲�
	push(2, (int16)UartData[7]);//�����ݷ��ͻ�ȥ�۲�
	push(3, (int16)UartData[3]);//�����ݷ��ͻ�ȥ�۲�
	push(4, (int16)UartData[4]);//�����ݷ��ͻ�ȥ�۲�
	push(5, (int16)UartData[5]);//�����ݷ��ͻ�ȥ�۲�
	if(UartData[0] != 0)
	{
		MotorPID.SpeedSet = (int16)UartData[0];
	}
	if(UartData[1] != 0)
	{
		MotorPID.Kp = (int16)(UartData[1]);
	}
	if(UartData[2] != 0)
	{
		MotorPID.Ki = (int16)(UartData[2]);
	}
	if(UartData[3] != 0)
	{
		MotorPID.Kd = (int16)(UartData[3]);
	}
	if(UartData[4] != 0)
	{
		S_kp = (UartData[4]);
	}
	if(UartData[5] != 0)
	{
		S_kd = (UartData[5]);
	}
	if(UartData[6] != 0)
	{
		foresight1 = (int)(UartData[6]);
	}
	if(UartData[7] != 0)
	{
		//speedset = (int16)(UartData[7]);
	}
	if(UartData[8] != 0)
	{
		//d = (int16)(UartData[8]);
	}
}

/***************************************************************
	*	@brief	������պ���
	*	@param	CmdNum: 0~255������; Data: 0~255����������Ӧ��������
	*	@note	��
***************************************************************/
void UartCmd(uint8 CmdNum,uint8 Data)
{
	if(CmdNum == 2 && Data == 102)
	{
		motor_flag = 50;
		MotorPID.IntSum_all = 0;
		BUZZER_ON;
		LPLD_LPTMR_DelayMs(400);
		stop_flag = 0;
		BUZZER_OFF;  
	}
	if(CmdNum == 2 && Data == 103)
	{
		motor_flag = 0;
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);//����
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);//������
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);
		BUZZER_ON;
		LPLD_LPTMR_DelayMs(400);
		BUZZER_OFF; 
	}
}

/***************************************************************
	*	@brief	��ʾ����ĳ��ͨ���������������
	*	@param	chanel��ͨ��; data ������-32768~32767
	*	@note	push��������������䵽���������棬��Ȼdate��16λ�ģ����������8λ�������档
***************************************************************/
void push(uint8 chanel,uint16 data)
{
	uSendBuf[chanel * 2] = data / 256;
	uSendBuf[chanel * 2 + 1] = data % 256;
}