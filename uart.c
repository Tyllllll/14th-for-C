#include "header.h"

uint8 uSendBuf[UartDataNum * 2] = {0};
uint8 FreeCarsDataNum = UartDataNum * 2;
double UartData[9] = {0};
SerialPortType SerialPortRx;

/***************************************************************
	*	@brief	将数据发送到freecars示波器显示
	*	@param	无
	*	@note	无
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
	*	@brief	接收上位机发送数据并赋值
	*	@param	无
	*	@note	无
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
	*	@brief	命令接收函数
	*	@param	CmdNum：0~255号命令，DATA：0~255个命令所对应的命令字
	*	@note	无
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
	*	@brief	UART0初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Uart_Init(void)
{
	static UART_InitTypeDef UART_InitStructure;
	UART_InitStructure.UART_Uartx = UART0;
	UART_InitStructure.UART_BaudRate = 115200;
	UART_InitStructure.UART_TxPin = PTA2;
	UART_InitStructure.UART_RxPin = PTA1;
	UART_InitStructure.UART_RxIntEnable = TRUE;//使能接收中断
	UART_InitStructure.UART_RxIsr = Uart0_Isr;//接收中断回调函数
	LPLD_UART_Init(UART_InitStructure);//初始化UART
	LPLD_UART_EnableIrq(UART_InitStructure);//使能串口数据接收中断
}

/***************************************************************
	*	@brief	UART0中断函数
	*	@param	无
	*	@note	无
***************************************************************/
void Uart0_Isr(void)
{
	int i, b, d1;
	uint32 d;
	//进入接收中断函数

	if((UART0->S1 & UART_S1_RDRF_MASK) && (UART0->C2 & UART_C2_RIE_MASK))
	{
		SerialPortRx.Data = LPLD_UART_GetChar(UART0);  
		if(SerialPortRx.Stack < UartRxBufferLen)
		{
			SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
			if(SerialPortRx.Stack >= UartRxDataLen//UartRxDataLen个数为一帧
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen] == 0xff//校验字头
					&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 1] == 0x55
						&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 2] == 0xaa
							&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 3] == 0x10)
			{
				//double data 9个通道数据校验
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxDataLen;//起始位
				for(i = b; i < SerialPortRx.Stack - 1; i++)//除校验位外的位进行校验
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{//校验成功，进行数据解算
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
					UartDebug();  //转去处理，把收到的数据付给变量
				}
				SerialPortRx.Stack = 0;
			}
			else if(SerialPortRx.Stack >= UartRxCmdLen//UartRxDataLen个数为一帧
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen] == 0xff
					&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
						&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
							&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77)//cmd
			{
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxCmdLen;//起始位
				for(i=b; i<SerialPortRx.Stack - 1; i++)//除校验位外的位进行校验
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{//校验成功
					UartCmd(UartCmdNum, UartCmdData);//处理接收到的命令，付给MCU命令变量
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
	*	@brief	轮询法发送一帧数据，消耗时间与数据长度有关
	*	@param	无
	*	@note	不可以放在中断里面周期性调用，消耗时间计算看帮助文档
***************************************************************/
void Send_Data_To_Scope(void)
{
	uint8 i = 0;
	uint8 sum = 0;
	//使用轮询的方式发送数据，当数据未发送，程序停在此处直到发送完成
	LPLD_UART_PutChar(UART0, (int8)251);
	LPLD_UART_PutChar(UART0, (int8)109);
	LPLD_UART_PutChar(UART0, (int8)37);
	sum += 251; //全部数据加入校验
	sum += 109;
	sum += 37;
	for(i = 0; i < FreeCarsDataNum; i++)
	{
		LPLD_UART_PutChar(UART0, uSendBuf[i]);
		sum += uSendBuf[i]; //全部数据加入校验
	}
	LPLD_UART_PutChar(UART0, sum);
}

/***************************************************************
	*	@brief	向示波器某个通道缓冲区填充数据
	*	@param	chanel：通道，data ：数据-32768~32767
	*	@note	用来将数据填充到缓冲区里面，显然date是16位的，因此用两个8位数来储存。
***************************************************************/
void push(uint8 chanel, uint16 data)
{
	uSendBuf[chanel * 2] = data / 256;
	uSendBuf[chanel * 2 + 1] = data % 256;
}