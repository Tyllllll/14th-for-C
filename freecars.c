#include "head_file.h"
uint8 uSendBuf[UartDataNum*2]={0};
uint8 FreeCarsDataNum=UartDataNum*2;//����ͨ���� UartDataNum*2(��Ϊ������Int16�͵�)
double UartData[9] = {0};

SerialPortType SerialPortRx;
//��ʾFreecars��λ���޸�����

int aa;
void UartDebug(void)
{
  push(15,(int16)UartData[0]);//�����ݷ��ͻ�ȥ�۲�
  push(1,(int16)UartData[6]);//�����ݷ��ͻ�ȥ�۲�
  push(2,(int16)UartData[7]);//�����ݷ��ͻ�ȥ�۲�
  push(3,(int16)UartData[3]);//�����ݷ��ͻ�ȥ�۲�
  push(4,(int16)UartData[4]);//�����ݷ��ͻ�ȥ�۲�
  push(5,(int16)UartData[5]);//�����ݷ��ͻ�ȥ�۲�
  if(UartData[0]!=0)
  {
    MotorPID.SpeedSet=(int16)UartData[0];
  }
  if(UartData[1]!=0)
  {
   M_Kp=(int16)(UartData[1]);
  }
  if(UartData[2]!=0)
  {
   M_Ki=(int16)(UartData[2]);
  }
  if(UartData[3]!=0)
  {
    M_Kd=(int16)(UartData[3]);
  }
  if(UartData[4]!=0)
  {
    S_kp=(UartData[4]);
  }
  if(UartData[5]!=0)
  {
    S_kd=(UartData[5]);
  }
  if(UartData[6]!=0)
  {
   foresight1=(int)(UartData[6]);
  }
  if(UartData[7]!=0)
  {
  //  speedset=(int16)(UartData[7]);
  }
  if(UartData[8]!=0)
  {
 //   d=(int16)(UartData[8]);
  }
  BUZZER_ON;
  LPLD_LPTMR_DelayMs(100);
  BUZZER_OFF;
}
/*
������պ���
CmdNum��0~255������
DATA  ��0~255����������Ӧ��������
*/
void UartCmd(uint8 CmdNum,uint8 Data)
{
  if(CmdNum==2&&Data==102)
  {
    motor_flag=50;
    MotorPID.IntSum_all=0;

    BUZZER_ON;
    LPLD_LPTMR_DelayMs(400);
    stop_flag=0;
    BUZZER_OFF;  
    
  }
  if(CmdNum==2&&Data==103)
  {
    motor_flag=0;
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);//����
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);//������
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);
     BUZZER_ON;
    LPLD_LPTMR_DelayMs(400);
    BUZZER_OFF; 

  }
}
/*��ѯ������һ֡����
����ʱ�������ݳ����й�
�����Է����ж����������Ե���
����ʱ����㿴�����ĵ�
*/
static void myPrintf(uint8 data)
{
    LPLD_UART_PutChar(UART0,data); 
}

void sendDataToScope(void)
{
  uint8 i,sum=0;
  //ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�ֱ���������
  myPrintf(251);
  LPLD_UART_PutChar(UART0,109); 
  LPLD_UART_PutChar(UART0,37); 
  sum+=(251); //ȫ�����ݼ���У��
  sum+=(109);
  sum+=(37);
  for(i=0;i<FreeCarsDataNum;i++)
  {
    LPLD_UART_PutChar(UART0,uSendBuf[i]);
    sum+=uSendBuf[i]; //ȫ�����ݼ���У��
  }
  LPLD_UART_PutChar(UART0,sum);
}
/*
��ʾ����ĳ��ͨ���������������
chanel��ͨ��
data ������-32768~32767
push ��������������䵽���������棬��Ȼdate��16λ�ģ����������8λ�������档
*/
void push(uint8 chanel,uint16 data)
{
  uSendBuf[chanel*2]=data/256;
  uSendBuf[chanel*2+1]=data%256;
}
//�����жϻص�����
void uart0_isr(void)
{
  int i,b,d1;
  uint32 d;
  //��������жϺ���
  
  if((UART0->S1 & UART_S1_RDRF_MASK) && (UART0->C2 & UART_C2_RIE_MASK))
  {
    SerialPortRx.Data = LPLD_UART_GetChar(UART0);  
    if(SerialPortRx.Stack < UartRxBufferLen)
    {
      SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
      if(SerialPortRx.Stack >= UartRxDataLen//UartRxDataLen����Ϊһ֡
         && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen]  ==0xff//У����ͷ
           && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+1]==0x55
             && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+2]==0xaa
               && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+3]==0x10)
      {//double data 9��ͨ������У��
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxDataLen;//��ʼλ
        for(i=b;i<SerialPortRx.Stack-1;i++)//��У��λ���λ����У��
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
        }
        if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1])
        {//У��ɹ����������ݽ���
          for(i = 0;i<9;i++)
          {
            d = SerialPortRx.Buffer[b+i*4+4]*0x1000000L + SerialPortRx.Buffer[b+i*4+5]*0x10000L + SerialPortRx.Buffer[b+i*4+6]*0x100L + SerialPortRx.Buffer[b+i*4+7];
            if(d>0x7FFFFFFF)
            {
              d1 = 0x7FFFFFFF - d;
            }
            else
            {
              d1 = d;
            }
            UartData[i]=d1;
            UartData[i]/=65536.0;
          }
          UartDebug();  //תȥ�������ܵ������ݸ�������
        }
        SerialPortRx.Stack = 0;
      }
      else if(SerialPortRx.Stack >= UartRxCmdLen//UartRxDataLen����Ϊһ֡
              && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  ==0xff
                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+1]==0x55
                  && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+2]==0xaa
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+3]==0x77)//cmd
      {
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxCmdLen;//��ʼλ
        for(i=b; i<SerialPortRx.Stack-1; i++)//��У��λ���λ����У��
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
        }
        if(SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1])
        {//У��ɹ�
          UartCmd(UartCmdNum,UartCmdData);//������յ����������MCU�������
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
void PutcharCamera(int8 sele)
{
  int i,j,Image_Mode_selection=sele;
       LedOn(3);
       LedOff(2);
       LPLD_LPTMR_DelayMs(400);
       LED_Fill(0x00);
       LED_P6x8Str(20,4, "da ga hao");
       LED_P6x8Str(50,6, "wo xi zzh");
    for(i = 119;i>=0;i--)
    {
      Image[i][f1.midline[i]]=150;
      Image[i][f1.leftline[i]]=150;
      Image[i][f1.rightline[i]]=150;
      
    }
  
  if(Image_Mode_selection==4)
  {
    for( i = 119;i>=0;i--)
    {
     Image[i][80]=200;  
     Image[i][116]=200;
    }
    //�м����ߣ���������
    
    //���ߣ���������
    for(i=0;i<COLUMN;i++)
    {
      Image[f2.toppoint][i]=150;
      Image[foresight][i]=150;
    }
  }
  

//  LPLD_UART_PutChar(UART0, 0x01);
//  LPLD_UART_PutChar(UART0, 0xFE);
//
//	for (i = 0; i < ROW; i++)
//	{
//		for (j = 0; j < (COLUMN >> 3); j++)
//		{
//			temp = 0;
//			for (k = 0; k < 8; k++)
//			{
//#if SEND_INFO
//				temp |= (ubyInfo[i][(j << 3) + (7 - k)] && 1) << k;
//#else
//				temp |= (Image[i][(j << 3) + (7 - k)] && 1) << k;
//#endif /* SEND_INFO */
//			}
//			  LPLD_UART_PutChar(UART0,~temp);
//		}
//	}
//
//  LPLD_UART_PutChar(UART0, 0xFE);
//  LPLD_UART_PutChar(UART0, 0x01);

  
  LPLD_UART_PutChar(UART0, 0x01);
  LPLD_UART_PutChar(UART0, 0xFE);
  for(i=0;i<ROW;i++)
    for(j=0;j<COLUMN;j++)
    {

      LPLD_UART_PutChar(UART0, Image[i][j]);
    }
  LPLD_UART_PutChar(UART0, 0xFE);
  LPLD_UART_PutChar(UART0, 0x01);


  
  
  for(int t=0;t<168000;t++){
    asm("nop");
  }
  	 LedOff(1);
         LedOff(3);
         LedOff(2);
         LPLD_LPTMR_DelayMs(400);
  LED_Fill(0x00);
}

void UI_Send(void)  //���Ͷ�ֵ��ͼ
{
	int i, j, k;
	uint8 temp;

  LPLD_UART_PutChar(UART0, 0x01);
  LPLD_UART_PutChar(UART0, 0xFE);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < (COLUMN >> 3); j++)
		{
			temp = 0;
			for (k = 0; k < 8; k++)
			{
#if SEND_INFO
				temp |= (ubyInfo[i][(j << 3) + (7 - k)] && 1) << k;
#else
				temp |= (Image[i][(j << 3) + (7 - k)] && 1) << k;
#endif /* SEND_INFO */
			}
			  LPLD_UART_PutChar(UART0,~temp);
		}
	}

  LPLD_UART_PutChar(UART0, 0xFE);
  LPLD_UART_PutChar(UART0, 0x01);
  
}






