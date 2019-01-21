#include "head_file.h"
void portb_isr(void);
void DMA_Complete_ISR(void);
int8 WhichBuffer = 0;
DMA_InitTypeDef dma_init_struct;
GPIO_InitTypeDef ptb6_init;
GPIO_InitTypeDef ptb16_init;
GPIO_InitTypeDef ptc_init;
unsigned char Flag_Test_ImageComplete = 1;
void NVIC_Init(void)
{
  static NVIC_InitTypeDef nvic_init_struct;
  nvic_init_struct.NVIC_IRQChannel=PIT3_IRQn;//舵机打脚中断函数抢占优先级最高
  nvic_init_struct.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority=0;
  nvic_init_struct.NVIC_IRQChannelSubPriority=1;
  LPLD_NVIC_Init(nvic_init_struct);
  
  nvic_init_struct.NVIC_IRQChannel=PORTB_IRQn;//场中断函数
  nvic_init_struct.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority=0;
  nvic_init_struct.NVIC_IRQChannelSubPriority=2;
  LPLD_NVIC_Init(nvic_init_struct);
  
  nvic_init_struct.NVIC_IRQChannel=DMA0_IRQn;//DMA中断函数
  nvic_init_struct.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority=1;
  nvic_init_struct.NVIC_IRQChannelSubPriority=0;
  LPLD_NVIC_Init(nvic_init_struct);
  
  
  nvic_init_struct.NVIC_IRQChannel=PIT1_IRQn;//电机输出中断函数
  nvic_init_struct.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority=1;
  nvic_init_struct.NVIC_IRQChannelSubPriority=1;
  LPLD_NVIC_Init(nvic_init_struct);
  
  
  nvic_init_struct.NVIC_IRQChannel=UART0_RX_TX_IRQn;//串口中断
  nvic_init_struct.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority=0;
  nvic_init_struct.NVIC_IRQChannelSubPriority=0;
  LPLD_NVIC_Init(nvic_init_struct); 
}
/*************************UART串口配置初始化******************************/
void UART_Init(void)//串口配置初始化
{
  static UART_InitTypeDef term_port_structure;
  term_port_structure.UART_Uartx = UART0;       
  term_port_structure.UART_BaudRate = 115200;
  term_port_structure.UART_TxPin=PTA2;
  term_port_structure.UART_RxPin=PTA1;
  term_port_structure.UART_RxIntEnable = TRUE;//使能接收中断
  term_port_structure.UART_RxIsr = uart0_isr;//接收中断回调函数
  LPLD_UART_Init(term_port_structure);//初始化UART
  LPLD_UART_EnableIrq(term_port_structure);//使能串口数据接收中断
}
/***********************************************************/   


/*****************GPIO配置(摄像头引脚配置)****************/
void gpio_init(void)
{ 
  //OV数据口初始化：PTC0~PTC7
  static GPIO_InitTypeDef ptc_init;
  ptc_init.GPIO_PTx = PTC;      
  ptc_init.GPIO_Dir = DIR_INPUT;
  ptc_init.GPIO_Pins = GPIO_Pin0_7;
  ptc_init.GPIO_PinControl = IRQC_DIS | INPUT_PULL_DIS;
  LPLD_GPIO_Init(ptc_init);
  
  //OV场信号接口初始化：PTB16
  static GPIO_InitTypeDef ptb16_init;
  ptb16_init.GPIO_PTx = PTB;
  ptb16_init.GPIO_Dir = DIR_INPUT;
  ptb16_init.GPIO_Pins = GPIO_Pin16;
  ptb16_init.GPIO_PinControl = IRQC_RI|INPUT_PULL_DOWN|INPUT_PF_EN ;
  ptb16_init.GPIO_Isr = portb_isr;
  LPLD_GPIO_Init(ptb16_init); 
  LPLD_GPIO_EnableIrq(ptb16_init);
  
  //OV PCLK信号接口初始化：PTB6
  static GPIO_InitTypeDef ptb6_init;
  ptb6_init.GPIO_PTx = PTB;
  ptb6_init.GPIO_Pins = GPIO_Pin6;
  ptb6_init.GPIO_Dir = DIR_INPUT;
  ptb6_init.GPIO_PinControl = IRQC_DMAFA | INPUT_PULL_DIS;
  LPLD_GPIO_Init(ptb6_init); 
}
/********************************************************************/



/************************摄像头图像采集DMA参数配置**************************/
void dma_init(void)
{
  static DMA_InitTypeDef dma_init_struct;
  dma_init_struct.DMA_CHx = DMA_CH0;//CH0通道
  dma_init_struct.DMA_Req = PORTB_DMAREQ;//PORTB为请求源
  dma_init_struct.DMA_MajorLoopCnt = ROW *COLUMN/8;//主循环计数值：行采集点数，宽度
  dma_init_struct.DMA_MinorByteCnt = 1;//次循环字节计数：每次读入1字节
  dma_init_struct.DMA_SourceAddr = (uint32)&PTC->PDIR;//        源地址：PTC0~7
  dma_init_struct.DMA_DestAddr = (uint32)Image_Buf;//配置目的数据地址目的地址：存放图像的数组
  dma_init_struct.DMA_DestAddrOffset = 1;//目的地址偏移：每次读入增加1
  dma_init_struct.DMA_MajorCompleteIntEnable = TRUE;//完成中断请求
  dma_init_struct.DMA_AutoDisableReq =TRUE;
  dma_init_struct.DMA_Isr =DMA_Complete_ISR;
  LPLD_DMA_Init(dma_init_struct);
  LPLD_DMA_EnableIrq(dma_init_struct); 
 }
 void DMA_Complete_ISR(void) 
 {
  sampleover = 1;
  Flag_Test_ImageComplete = 1;
  if (WhichBuffer == 1) 
  {
    WhichBuffer = 0;
  } 
  else 
  {
    WhichBuffer = 1;
  }
 }
/***************************************************************************/

/****************************Servo初始化**********************************/
void Servo_gpio_init(void)//舵机初始化
{
 static  GPIO_InitTypeDef Servo_GPIO; //舵机引脚初始化
 Servo_GPIO.GPIO_PTx=PTD;
 Servo_GPIO.GPIO_Dir=DIR_OUTPUT;
 Servo_GPIO.GPIO_Output=OUTPUT_L;
 Servo_GPIO.GPIO_Pins=GPIO_Pin2;
 LPLD_GPIO_Init(Servo_GPIO);
 
 static  PIT_InitTypeDef Servo_PIT; //PIT3定时中断 负责拉低电平
 Servo_PIT.PIT_Pitx=PIT3;
 Servo_PIT.PIT_PeriodUs=100000;
 Servo_PIT.PIT_Isr=Servo_PIT_Isr;
 LPLD_PIT_Init(Servo_PIT);
 LPLD_PIT_EnableIrq(Servo_PIT);//使能中断
 PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
 
 static  PIT_InitTypeDef Servo_PIT_H; //PIT0 20ms定时中断 负责拉高电平
 Servo_PIT_H.PIT_Pitx=PIT0;
 Servo_PIT_H.PIT_PeriodMs=20;
 Servo_PIT_H.PIT_Isr=Sevor_output;
 LPLD_PIT_Init(Servo_PIT_H);
 LPLD_PIT_EnableIrq(Servo_PIT_H);//使能中断
 
}
/**************************************************************************/


/************************************************************************/
void Motor_pwm_init(void)
{
  static FTM_InitTypeDef ftm_motor_init_struct;
  ftm_motor_init_struct.FTM_Ftmx=FTM0;//选择FTM通道
  ftm_motor_init_struct.FTM_Mode=FTM_MODE_PWM;//FTM0的工作模式是PWM波输出
  ftm_motor_init_struct.FTM_PwmDeadtimeCfg=DEADTIME_CH67 | DEADTIME_CH45;
  ftm_motor_init_struct.FTM_PwmFreq = 10000; //PWM频率10KHz    
  
  LPLD_FTM_Init(ftm_motor_init_struct);
  LPLD_FTM_PWM_Enable(FTM0, //使用FTM0                    左正转
                      FTM_Ch4, //使能Ch0通道
                      0, //初始化0%
                      PTD4, //使用Ch0通道的PTC1引脚
                      ALIGN_LEFT        //脉宽左对齐
                        );
  LPLD_FTM_PWM_Enable(FTM0, //使用FTM0                    右正转
                      FTM_Ch5, //使能Ch1通道
                      0, //初始化0%
                      PTD5, //使用Ch1通道的PTC2引脚
                      ALIGN_LEFT        //脉宽左对齐
                        );
  LPLD_FTM_PWM_Enable(FTM0, //使用FTM0                     右反转
                      FTM_Ch6, //使能Ch1通道
                      0, //初始化0%
                      PTD6, //使用Ch1通道的PTC2引脚
                      ALIGN_LEFT        //脉宽左对齐
                        );
  LPLD_FTM_PWM_Enable(FTM0, //使用FTM0                     左反转
                      FTM_Ch7, //使能Ch1通道
                      0, //初始化0%
                      PTD7, //使用Ch1通道的PTC2引脚
                      ALIGN_LEFT        //脉宽左对齐
                        );
  
}

/************************************************************/
void Oled_GPIO_Init(void)
{
  static GPIO_InitTypeDef oled_Init_GPIO_Struct;
  oled_Init_GPIO_Struct.GPIO_PTx = PTA;
  oled_Init_GPIO_Struct.GPIO_Dir = DIR_OUTPUT;
  oled_Init_GPIO_Struct.GPIO_Pins = GPIO_Pin6;
  oled_Init_GPIO_Struct.GPIO_Output = OUTPUT_H;
  oled_Init_GPIO_Struct.GPIO_PinControl = IRQC_DIS;
  LPLD_GPIO_Init(oled_Init_GPIO_Struct);
  
  static GPIO_InitTypeDef oled_Init_GPIO_Struct1;
  oled_Init_GPIO_Struct1.GPIO_PTx = PTE;
  oled_Init_GPIO_Struct1.GPIO_Dir = DIR_OUTPUT;
  oled_Init_GPIO_Struct1.GPIO_Pins = GPIO_Pin10 | GPIO_Pin11 | GPIO_Pin12;
  oled_Init_GPIO_Struct1.GPIO_Output = OUTPUT_H;
  oled_Init_GPIO_Struct1.GPIO_PinControl = IRQC_DIS;
  LPLD_GPIO_Init(oled_Init_GPIO_Struct1);
    LED_SCLK(1);		
    LED_RST(0);
    LED_DLY_ms(50);
    LED_RST(1);
    LED_WrCmd(0xae);//--turn off oled panel
    LED_WrCmd(0x00);//---set low column address
    LED_WrCmd(0x10);//---set high column address
    LED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    LED_WrCmd(0x81);//--set contrast control register
    LED_WrCmd(0xcf);// Set SEG Output Current Brightness
    LED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    LED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    LED_WrCmd(0xa6);//--set normal display
    LED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    LED_WrCmd(0x3f);//--1/64 duty
    LED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    LED_WrCmd(0x00);//-not offset
    LED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    LED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    LED_WrCmd(0xd9);//--set pre-charge period
    LED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    LED_WrCmd(0xda);//--set com pins hardware configuration
    LED_WrCmd(0x12);
    LED_WrCmd(0xdb);//--set vcomh
    LED_WrCmd(0x40);//Set VCOM Deselect Level
    LED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    LED_WrCmd(0x02);//
    LED_WrCmd(0x8d);//--set Charge Pump enable/disable
    LED_WrCmd(0x14);//--set(0x10) disable
    LED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    LED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    LED_WrCmd(0xaf);//--turn on oled panel
    LED_Fill(0x00);//初始清屏
    LED_Set_Pos(0,0);
    
}
/***************************************************************/
void Encoder_QD_init(void)
{
  static GPIO_InitTypeDef encoder_init_struct;
  encoder_init_struct.GPIO_PTx = PTB;
  encoder_init_struct.GPIO_Pins =GPIO_Pin18|GPIO_Pin19;
  encoder_init_struct.GPIO_Dir = DIR_INPUT;        //输入
  encoder_init_struct.GPIO_PinControl = INPUT_PULL_UP;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(encoder_init_struct);
  
  static FTM_InitTypeDef Init_FTM_StructR;
  Init_FTM_StructR.FTM_Ftmx = FTM2;                  //FTM1 与 FTM2 的引脚范围有要求，此处应该FTM1与FTM2 需要交换 改了
  Init_FTM_StructR.FTM_Mode = FTM_MODE_QD;
  Init_FTM_StructR.FTM_QdMode = QD_MODE_CNTDIR;
  LPLD_FTM_Init(Init_FTM_StructR);
  LPLD_FTM_QD_Enable(FTM2, PTB18, PTB19);
  
  
  static GPIO_InitTypeDef encoder_init_struct2;
  encoder_init_struct2.GPIO_PTx = PTA;
  encoder_init_struct2.GPIO_Pins =GPIO_Pin8|GPIO_Pin9;
  encoder_init_struct2.GPIO_Dir = DIR_INPUT;        //输入
  encoder_init_struct2.GPIO_PinControl = INPUT_PULL_UP;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(encoder_init_struct2);
  
  static FTM_InitTypeDef Init_FTM_Struct2;
  Init_FTM_Struct2.FTM_Ftmx = FTM1;
  Init_FTM_Struct2.FTM_Mode = FTM_MODE_QD;
  Init_FTM_Struct2.FTM_QdMode = QD_MODE_CNTDIR;
  LPLD_FTM_Init(Init_FTM_Struct2);
  LPLD_FTM_QD_Enable(FTM1, PTA8, PTA9);
  
  

}

/*******************************************************************************/
void key_init_gpio()
{
  static GPIO_InitTypeDef anjian_struct;
  anjian_struct.GPIO_PTx = PTA;     
  anjian_struct.GPIO_Pins = GPIO_Pin11|GPIO_Pin12|GPIO_Pin5|GPIO_Pin7;   
  anjian_struct.GPIO_Dir = DIR_INPUT;        //输入
  anjian_struct.GPIO_PinControl = INPUT_PULL_UP|IRQC_DIS;     //内部上拉|不产生中断
  LPLD_GPIO_Init(anjian_struct);
  
  static GPIO_InitTypeDef anjian_struct1;
  anjian_struct1.GPIO_PTx = PTE;     
  anjian_struct1.GPIO_Pins = GPIO_Pin27|GPIO_Pin28;   
  anjian_struct1.GPIO_Dir = DIR_INPUT;        //输入
  anjian_struct1.GPIO_PinControl = INPUT_PULL_UP|IRQC_DIS;     //内部上拉|不产生中断
  LPLD_GPIO_Init(anjian_struct1);
}

/********************************************************************************/
 void boma_init_gpio(void)
{
  static GPIO_InitTypeDef boma1_io_struct;
  boma1_io_struct.GPIO_PTx=PTA;
  boma1_io_struct.GPIO_Pins= GPIO_Pin24|GPIO_Pin25;
  boma1_io_struct.GPIO_Dir=DIR_INPUT;
  boma1_io_struct.GPIO_PinControl=INPUT_PULL_UP;
  LPLD_GPIO_Init(boma1_io_struct);
  
  static GPIO_InitTypeDef boma1_io_struct1;
  boma1_io_struct1.GPIO_PTx=PTE;
  boma1_io_struct1.GPIO_Pins= GPIO_Pin24|GPIO_Pin26;
  boma1_io_struct1.GPIO_Dir=DIR_INPUT;
  boma1_io_struct1.GPIO_PinControl=INPUT_PULL_UP;
  LPLD_GPIO_Init(boma1_io_struct1);
  
}

void  BatteryVoltage_Init(void)
{
  static ADC_InitTypeDef Init_ADC_Struct;
	Init_ADC_Struct.ADC_Adcx = ADC0;
	Init_ADC_Struct.ADC_BitMode = SE_8BIT;
	Init_ADC_Struct.ADC_CalEnable = TRUE;
//	Init_ADC_Struct.ADC_MuxSel = MUX_ADXXB;
	LPLD_ADC_Init(Init_ADC_Struct);
	LPLD_ADC_Chn_Enable(ADC0, AD18); 
}

void led_init(void)
{
  static GPIO_InitTypeDef led_io_struct;
      led_io_struct.GPIO_PTx=PTB;
      led_io_struct.GPIO_Pins=GPIO_Pin0|GPIO_Pin1|GPIO_Pin2;
      led_io_struct.GPIO_Dir=DIR_OUTPUT;
      led_io_struct.GPIO_Output=OUTPUT_L;
      LPLD_GPIO_Init(led_io_struct);

}
void Buzzer_gpio_init(void)
{
  static GPIO_InitTypeDef buzzer_init_struct;
  buzzer_init_struct.GPIO_PTx = PTC;
  buzzer_init_struct.GPIO_Dir = DIR_OUTPUT;
  buzzer_init_struct.GPIO_Pins = GPIO_Pin12;
  buzzer_init_struct.GPIO_Output = OUTPUT_L;
  
  LPLD_GPIO_Init(buzzer_init_struct);
}


void pit0_init(void)
{//电机PIT周期调用
  //配置PIT0参数
  static  PIT_InitTypeDef pit0_init_struct; 
  pit0_init_struct.PIT_Pitx = PIT1;
  pit0_init_struct.PIT_PeriodMs =10;//定时周期5毫秒
  pit0_init_struct.PIT_Isr = Motor_PIT;//设置中断函数
  LPLD_PIT_Init(pit0_init_struct);//初始化PIT0 
  LPLD_PIT_EnableIrq(pit0_init_struct);//使能PIT0
}


