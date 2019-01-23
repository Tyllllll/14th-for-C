#ifndef _FREECARS_H_
#define _FREECARS_H_
#include "common.h"

//FreeCars��λ��������������ʾ�򣬽���ͨ������������λ�����øı�
#define UartDataNum 17		//��λ������ͨ����������������λ������һ��
#define UartRxBufferLen  100
#define UartRxDataLen 41	//FreeCars��λ�����͸�������MCU���գ���Ҫ��
#define UartRxCmdLen 7		//FreeCars��λ�������������ݳ��ȣ���Ҫ��
#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack - 3]//�����
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack - 2]//��������

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