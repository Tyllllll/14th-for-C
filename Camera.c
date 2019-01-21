#include "head_file.h"
  reg_s ov7725_eagle_reg[] =
  {
	  //寄存器，寄存器值次

	  /*50帧：
	  {COM4         , 0xC1},
	  {CLKRC        , 0x02},


	  75帧：
	  {COM4         , 0x41},
	  {CLKRC        , 0x00},


	  112帧：
	  {COM4         , 0x81},
	  {CLKRC        , 0x00},


	  150帧：
	  {COM4         , 0xC1},
	  {CLKRC        , 0x00},
	  */
	  { OV7725_COM4, 0xC1 },
	  { OV7725_CLKRC, 0x00 },


	  { OV7725_COM2, 0x03 },
	  { OV7725_COM3, 0xD0 },
	  { OV7725_COM7, 0x40 },
	  { OV7725_HSTART, 0x3F },
	  { OV7725_HSIZE, 0x50 },
	  { OV7725_VSTRT, 0x03 },
	  { OV7725_VSIZE, 0x78 },
	  { OV7725_HREF, 0x00 },
	  { OV7725_SCAL0, 0x0A },
	  { OV7725_AWB_Ctrl0, 0xE0 },
	  { OV7725_DSPAuto, 0xff },
	  { OV7725_DSP_Ctrl2, 0x0C },
	  { OV7725_DSP_Ctrl3, 0x00 },
	  { OV7725_DSP_Ctrl4, 0x00 },

  #if (CAMERA_W == 80)
	  { OV7725_HOutSize, 0x14 },
  #elif (CAMERA_W == 160)
	  { OV7725_HOutSize, 0x28 },
  #elif (CAMERA_W == 240)
	  { OV7725_HOutSize, 0x3c },
  #elif (CAMERA_W == 320)
	  { OV7725_HOutSize, 0x50 },
  #else

  #endif

  #if (CAMERA_H == 60 )
	  { OV7725_VOutSize, 0x1E },
  #elif (CAMERA_H == 120 )
	  { OV7725_VOutSize, 0x3c },
  #elif (CAMERA_H == 180 )
	  { OV7725_VOutSize, 0x5a },
  #elif (CAMERA_H == 240 )
	  { OV7725_VOutSize, 0x78 },
  #else

  #endif

	  { OV7725_EXHCH, 0x00 },
	  { OV7725_GAM1, 0x0c },
	  { OV7725_GAM2, 0x16 },
	  { OV7725_GAM3, 0x2a },
	  { OV7725_GAM4, 0x4e },
	  { OV7725_GAM5, 0x61 },
	  { OV7725_GAM6, 0x6f },
	  { OV7725_GAM7, 0x7b },
	  { OV7725_GAM8, 0x86 },
	  { OV7725_GAM9, 0x8e },
	  { OV7725_GAM10, 0x97 },
	  { OV7725_GAM11, 0xa4 },
	  { OV7725_GAM12, 0xaf },
	  { OV7725_GAM13, 0xc5 },
	  { OV7725_GAM14, 0xd7 },
	  { OV7725_GAM15, 0xe8 },
	  { OV7725_SLOP, 0x20 },
	  { OV7725_LC_RADI, 0x00 },
	  { OV7725_LC_COEF, 0x13 },
	  { OV7725_LC_XC, 0x08 },
	  { OV7725_LC_COEFB, 0x14 },
	  { OV7725_LC_COEFR, 0x17 },
	  { OV7725_LC_CTR, 0x05 },
	  { OV7725_BDBase, 0x99 },
	  { OV7725_BDMStep, 0x03 },
	  { OV7725_SDE, 0x04 },
	  { OV7725_BRIGHT, 0x00 },
	  { OV7725_CNST, 0x70 },
	  { OV7725_SIGN, 0x06 },
	  { OV7725_UVADJ0, 0x11 },
	  { OV7725_UVADJ1, 0x02 },

  };
  uint8 Camera_CNST = 140;//对比度 越大白色越多 根据场地调
  uint8 ov7725_eagle_cfgnum = ARR_SIZE(ov7725_eagle_reg);
  uint8 Image_Buf[ROW + 10][COLUMN / 8], Image_Buf2[ROW + 10][COLUMN / 8];
  unsigned char Image[ROW][COLUMN];
  int sample_row;
  int8  sampleover = 0;
  uint8 row_DMA_flag;
  uint8 Flag_ImageStart = 0;




  void portb_isr() 
  {
    static short i = 0;
    static uint8 *p;
    if (LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin16)) //检测PORTX的引脚PINX是否产生外部中断
    {
      if(i<5)i++;
      if (i >=3) //产生三次场中断
      {
	if (Flag_Test_ImageComplete == 1) 
	{
	  Flag_Test_ImageComplete = 0;
	  i = 0;
	  Flag_ImageStart = 1;
	  LPLD_GPIO_ClearIntFlag(PORTB);
	  if (WhichBuffer == 1) 
	  {
	    p = (uint8*)Image_Buf2;
	    LPLD_DMA_LoadDstAddr(DMA_CH0, p);//加载目的地址
	  } 
	  else 
	  {
	    p = (uint8*)Image_Buf;
	    LPLD_DMA_LoadDstAddr(DMA_CH0, p);
	  }
	  LPLD_DMA_EnableReq(DMA_CH0);
	}
	LPLD_GPIO_ClearIntFlag(PORTB);//清除中断
      }
      
    }
    LPLD_GPIO_ClearIntFlag(PORTB);
  }
  
  void camera_delay(void) 
  {
    uint16 i, n;
    for (i = 0; i < 30000; i++) 
    {
      for (n = 0; n < 200; n++)
      {
	asm("nop");
      }
    }
  }
  
  uint8 ov7725_init(void) 
  {
    uint16 i = 0, j = 0;
    uint8 device_id = 0;
    
    LPLD_SCCB_Init();
    
    //LedBlinkBlinkBlink();
    camera_delay();
    
    //LedBlinkBlinkBlink();
    //复位OV7670寄存器
    while (0 == LPLD_SCCB_WriteReg(OV7725_COM7, 0x80)) 
    {
      i++;
      if (i == 500) 
      {
	return 0;
      }
    }
    for (i = 0; i < 120; i++) 
    {
      for (j = 0; j < 160; j++) 
      {
	Image[i][j] = 0;
      }
    }//   让数组置零
    for (i = 0; i < 120; i++) 
    {
      for (j = 0; j < 20; j++) 
      {
	Image_Buf[i][j] = 255;
	Image_Buf2[i][j] = 255;
	
      }
    }   
    camera_delay();   
    //读取设备ID
    if (0 == LPLD_SCCB_ReadReg(OV7725_VER, &device_id, 1))    /* 读取sensor ID号*/
    {
      return 0;
    }
    camera_delay();
    if (device_id == OV7725_ID)
    {
      for (i = 0; i < ov7725_eagle_cfgnum; i++)
      {
	if (0 == LPLD_SCCB_WriteReg(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val)) 
	{
	  return 0;
	}
      }
      LPLD_SCCB_WriteReg(OV7725_CNST, Camera_CNST);
    } 
    else 
    {
      return 0;
    }
    return 1;
  }



  void img_extract(void) 
  {
    uint8 colour[2] = { 253,0 }; //253 白 0 黑
    uint8 Which;
    Which = WhichBuffer;
    //注：野火的摄像头 0 表示 白色，1表示 黑色
    uint8 i = 0, j = 0;
    if (Which == 1) 
    {
      for (i = 0; i < 120; i++) 
      {
	for (j = 0; j < COLUMN / 8; j++) 
	{
	  Image[i][j * 8 + 0] = colour[(Image_Buf[i][j] >> 7) & 0x01];
	  Image[i][j * 8 + 1] = colour[(Image_Buf[i][j] >> 6) & 0x01];
	  Image[i][j * 8 + 2] = colour[(Image_Buf[i][j] >> 5) & 0x01];
	  Image[i][j * 8 + 3] = colour[(Image_Buf[i][j] >> 4) & 0x01];
	  Image[i][j * 8 + 4] = colour[(Image_Buf[i][j] >> 3) & 0x01];
	  Image[i][j * 8 + 5] = colour[(Image_Buf[i][j] >> 2) & 0x01];
	  Image[i][j * 8 + 6] = colour[(Image_Buf[i][j] >> 1) & 0x01];
	  Image[i][j * 8 + 7] = colour[(Image_Buf[i][j] >> 0) & 0x01];
	}
      }
    } 
    else 
    {
      for (i = 0; i < 120; i++) 
      {
	for (j = 0; j < COLUMN / 8; j++) 
	{
	  Image[i][j * 8 + 0] = colour[(Image_Buf2[i][j] >> 7) & 0x01];
	  Image[i][j * 8 + 1] = colour[(Image_Buf2[i][j] >> 6) & 0x01];
	  Image[i][j * 8 + 2] = colour[(Image_Buf2[i][j] >> 5) & 0x01];
	  Image[i][j * 8 + 3] = colour[(Image_Buf2[i][j] >> 4) & 0x01];
	  Image[i][j * 8 + 4] = colour[(Image_Buf2[i][j] >> 3) & 0x01];
	  Image[i][j * 8 + 5] = colour[(Image_Buf2[i][j] >> 2) & 0x01];
	  Image[i][j * 8 + 6] = colour[(Image_Buf2[i][j] >> 1) & 0x01];
	  Image[i][j * 8 + 7] = colour[(Image_Buf2[i][j] >> 0) & 0x01];
	}
      }
    }
  }







