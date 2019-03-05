#ifndef _UART_H_
#define _UART_H_

#define UartDataNum		17//��λ������ͨ����������������λ������һ��
#define UartRxBufferLen	100
#define UartRxDataLen	41//FreeCars��λ�����͸�������MCU���գ���Ҫ��
#define UartRxCmdLen	7//FreeCars��λ�������������ݳ��ȣ���Ҫ��
#define UartCmdNum		SerialPortRx.Buffer[SerialPortRx.Stack - 3]//�����
#define UartCmdData		SerialPortRx.Buffer[SerialPortRx.Stack - 2]//��������

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