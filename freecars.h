#ifndef _FREECARS_H_
#define _FREECARS_H_
#include "common.h"

//FreeCars上位机，串口数据显示框，接收通道数，按照上位机设置改变
#define UartDataNum 17		//上位机数据通道数量，必须与上位机设置一样
#define UartRxBufferLen  100
#define UartRxDataLen 41	//FreeCars上位机发送浮点数据MCU接收，不要改
#define UartRxCmdLen 7		//FreeCars上位机接收命令数据长度，不要改
#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack - 3]//命令号
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack - 2]//命令数据

typedef struct 
{
	int Stack;
	uint8 Data;
	uint8 PreData;
	uint8 Buffer[UartRxBufferLen];
	uint8 Enable;
	uint8 Check;
}SerialPortType;

void UART_Init(void);
void uart0_isr(void);
void UartDebug(void);
void UartCmd(uint8 CmdNum,uint8 Data);
void push(uint8 chanel,uint16 data);

#endif