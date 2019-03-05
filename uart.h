#ifndef _UART_H_
#define _UART_H_

#define UartDataNum		17//上位机数据通道数量，必须与上位机设置一样
#define UartRxBufferLen	100
#define UartRxDataLen	41//FreeCars上位机发送浮点数据MCU接收，不要改
#define UartRxCmdLen	7//FreeCars上位机接收命令数据长度，不要改
#define UartCmdNum		SerialPortRx.Buffer[SerialPortRx.Stack - 3]//命令号
#define UartCmdData		SerialPortRx.Buffer[SerialPortRx.Stack - 2]//命令数据

typedef struct 
{
	int Stack;
	uint8 Data;
	uint8 PreData;
	uint8 Buffer[UartRxBufferLen];
	uint8 Enable;
	uint8 Check;
}SerialPortType;

void Send_Data_to_FreeCars(void);
void UartDebug(void);
uint8 Uart_Init(void);
void Uart0_Isr(void);
void UartCmd(uint8 CmdNum,uint8 Data);
void Send_Data_To_Scope(void);
void push(uint8 chanel, uint16 data);

#endif