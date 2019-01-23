#include "header.h"

double UartData[9] = {0};
SerialPortType SerialPortRx;
//显示Freecars上位机修改数据

/***************************************************************
	*	@brief	uart初始化
	*	@param	无
	*	@note	波特率115200
***************************************************************/
void UART_Init(void)
{
	static UART_InitTypeDef term_port_structure;
	term_port_structure.UART_Uartx = UART0;       
	term_port_structure.UART_BaudRate = 115200;
	term_port_structure.UART_TxPin=PTA2;
	term_port_structure.UART_RxPin=PTA1;
	term_port_structure.UART_RxIntEnable = TRUE;	//使能接收中断
	term_port_structure.UART_RxIsr = uart0_isr;		//接收中断回调函数
	LPLD_UART_Init(term_port_structure);			//初始化UART
	LPLD_UART_EnableIrq(term_port_structure);		//使能串口数据接收中断
}

/***************************************************************
	*	@brief	uart接受中断回调函数
	*	@param	无
	*	@note	无
***************************************************************/
void uart0_isr(void)
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
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+1] == 0x55
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+2] == 0xaa
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+3] == 0x10)
			{
				//double data 9个通道数据校验
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxDataLen;//起始位
				for(i=b; i<SerialPortRx.Stack-1; i++)//除校验位外的位进行校验
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{
					//校验成功，进行数据解算
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
					UartDebug();  //转去处理，把收到的数据赋给变量
				}
				SerialPortRx.Stack = 0;
			}
			else if(SerialPortRx.Stack >= UartRxCmdLen//UartRxDataLen个数为一帧
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  == 0xff
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
				&& SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77)//cmd
			{
				SerialPortRx.Check = 0;
				b = SerialPortRx.Stack - UartRxCmdLen;//起始位
				for(i = b; i < SerialPortRx.Stack - 1; i++)//除校验位外的位进行校验
				{
					SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
				}
				if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
				{
					//校验成功
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

void UartDebug(void)
{
	push(15, (int16)UartData[0]);//将数据发送回去观察
	push(1, (int16)UartData[6]);//将数据发送回去观察
	push(2, (int16)UartData[7]);//将数据发送回去观察
	push(3, (int16)UartData[3]);//将数据发送回去观察
	push(4, (int16)UartData[4]);//将数据发送回去观察
	push(5, (int16)UartData[5]);//将数据发送回去观察
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
	*	@brief	命令接收函数
	*	@param	CmdNum: 0~255号命令; Data: 0~255个命令所对应的命令字
	*	@note	无
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
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);//右正
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);//左轮正
		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);
		BUZZER_ON;
		LPLD_LPTMR_DelayMs(400);
		BUZZER_OFF; 
	}
}

/***************************************************************
	*	@brief	向示波器某个通道缓冲区填充数据
	*	@param	chanel：通道; data ：数据-32768~32767
	*	@note	push是用来将数据填充到缓冲区里面，显然date是16位的，因此用两个8位数来储存。
***************************************************************/
void push(uint8 chanel,uint16 data)
{
	uSendBuf[chanel * 2] = data / 256;
	uSendBuf[chanel * 2 + 1] = data % 256;
}