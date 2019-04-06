#include "header.h"

reg_s regaCameraEagle[] =
{
	//寄存器，寄存器值次
	/*50帧：
	{COM4, 0xC1},
	{CLKRC, 0x02},
	
	75帧：
	{COM4, 0x41},
	{CLKRC, 0x00},


	112帧：
	{COM4, 0x81},
	{CLKRC, 0x00},


	150帧：
	{COM4, 0xC1},
	{CLKRC, 0x00},
	*/
	{OV7725_COM4,		0xC1},
	{OV7725_CLKRC,		0x00},
	\

	{OV7725_COM2,		0x03},
	{OV7725_COM3,		0xD0},
	{OV7725_COM7,		0x40},
	{OV7725_HSTART,		0x3F},
	{OV7725_HSIZE,		0x50},
	{OV7725_VSTRT,		0x03},
	{OV7725_VSIZE,		0x78},
	{OV7725_HREF,		0x00},
	{OV7725_SCAL0,		0x0A},
	{OV7725_AWB_Ctrl0,	0xE0},
	{OV7725_DSPAuto,	0xff},
	{OV7725_DSP_Ctrl2,	0x0C},
	{OV7725_DSP_Ctrl3,	0x00},
	{OV7725_DSP_Ctrl4,	0x00},

#if (CAMERA_W == 80)
	{OV7725_HOutSize, 0x14},
#elif (CAMERA_W == 160)
	{OV7725_HOutSize, 0x28},
#elif (CAMERA_W == 240)
	{OV7725_HOutSize, 0x3c},
#elif (CAMERA_W == 320)
	{OV7725_HOutSize, 0x50},
#else

#endif

#if (CAMERA_H == 60 )
	{OV7725_VOutSize, 0x1E},
#elif (CAMERA_H == 120 )
	{OV7725_VOutSize, 0x3c},
#elif (CAMERA_H == 180 )
	{OV7725_VOutSize, 0x5a},
#elif (CAMERA_H == 240 )
	{OV7725_VOutSize, 0x78},
#else

#endif

	{OV7725_EXHCH,		0x00},
	{OV7725_GAM1,		0x0c},
	{OV7725_GAM2,		0x16},
	{OV7725_GAM3,		0x2a},
	{OV7725_GAM4,		0x4e},
	{OV7725_GAM5,		0x61},
	{OV7725_GAM6,		0x6f},
	{OV7725_GAM7,		0x7b},
	{OV7725_GAM8,		0x86},
	{OV7725_GAM9,		0x8e},
	{OV7725_GAM10,		0x97},
	{OV7725_GAM11,		0xa4},
	{OV7725_GAM12, 		0xaf},
	{OV7725_GAM13, 		0xc5},
	{OV7725_GAM14,		0xd7},
	{OV7725_GAM15,		0xe8},
	{OV7725_SLOP,		0x20},
	{OV7725_LC_RADI,	0x00},
	{OV7725_LC_COEF,	0x13},
	{OV7725_LC_XC,		0x08},
	{OV7725_LC_COEFB,	0x14},
	{OV7725_LC_COEFR,	0x17},
	{OV7725_LC_CTR,		0x05},
	{OV7725_BDBase,		0x99},
	{OV7725_BDMStep,	0x03},
	{OV7725_SDE,		0x04},
	{OV7725_BRIGHT,		0x00},
	{OV7725_CNST,		0x45},//gamma
	{OV7725_SIGN,		0x06},
	{OV7725_UVADJ0,		0x11},
	{OV7725_UVADJ1,		0x02}
};
const uint8 c_ubyColor[] = {253, 0};
uint8 ubyCameraEagle_CfgNum = ARR_SIZE(regaCameraEagle);
Camera_Class camera;

/***************************************************************
	*	@brief	摄像头初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 ubyCamera_Init(void)
{
	camera.contrast = 75;
	camera.ready_write = 1;
	camera.which_buffer = 1;

	uint16 i = 0;
	uint8 ubyDeviceID = 0;
	LPLD_SCCB_Init();
	if(!Camera_Gpio_Init())
		return STATUS_FAILED;
	Camera_Delay();
	/* re++set */
	while(!LPLD_SCCB_WriteReg(OV7725_COM7, 0x80))
	{
		i++;
		if(i == 500)
		{
			return 1; // 通信失败
		}
	}
	Camera_Delay();
	if(!LPLD_SCCB_ReadReg(OV7725_VER, &ubyDeviceID, 1))
	{
		return 2; // 摄像头验证失败
	}
	Camera_Delay();
	if(ubyDeviceID == OV7725_ID)
	{
		for(i = 0; i < ubyCameraEagle_CfgNum; i++)
		{
			if(!LPLD_SCCB_WriteReg(regaCameraEagle[i].addr, regaCameraEagle[i].val))
			{
				return 3; // 寄存器初始化失败
			}
		}
		LPLD_SCCB_WriteReg(OV7725_CNST, camera.contrast);
	}
	else
	{
		return 4; // 摄像头不是7725
	}
	return 0;
}

/***************************************************************
	*	@brief	摄像头gpio初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 Camera_Gpio_Init(void)
{
	static GPIO_InitTypeDef CameraPT_InitStructure;
	static GPIO_InitTypeDef isr_InitStructure;
	static GPIO_InitTypeDef DMA_GPIO_InitStructure;
	static DMA_InitTypeDef DMA_InitStructure;

	CameraPT_InitStructure.GPIO_PTx = CAMERAPT_PTx;
	CameraPT_InitStructure.GPIO_Pins = CAMERAPT_Pinx;
	CameraPT_InitStructure.GPIO_Dir = DIR_INPUT;
	CameraPT_InitStructure.GPIO_PinControl = IRQC_DIS;
	if(!LPLD_GPIO_Init(CameraPT_InitStructure))
		return STATUS_FAILED;

	/* img interrupt */
	isr_InitStructure.GPIO_PTx = IMG_IRQ_PTx;
	isr_InitStructure.GPIO_Pins = IMG_IRQ_Pinx;
	isr_InitStructure.GPIO_Dir = DIR_INPUT;
	isr_InitStructure.GPIO_PinControl = IRQC_FA | INPUT_PF_EN;
	isr_InitStructure.GPIO_Isr = Img_Isr;
	if(!LPLD_GPIO_Init(isr_InitStructure))
		return STATUS_FAILED;
	if(!LPLD_GPIO_EnableIrq(isr_InitStructure))
		return STATUS_FAILED;

	DMA_GPIO_InitStructure.GPIO_PTx = CAMERA_DMA_PTx;
	DMA_GPIO_InitStructure.GPIO_Pins = CAMERA_DMA_Pinx;
	DMA_GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	DMA_GPIO_InitStructure.GPIO_PinControl = IRQC_DMAFA | INPUT_PULL_DIS;
	if(!LPLD_GPIO_Init(DMA_GPIO_InitStructure))
		return STATUS_FAILED;

	DMA_InitStructure.DMA_CHx = CAMERA_DMA_CHx;
	DMA_InitStructure.DMA_Req = CAMERA_DMA_REQ;	//PORTB为请求源
	DMA_InitStructure.DMA_MajorLoopCnt = ROW * COLUMN / 8;
	DMA_InitStructure.DMA_MinorByteCnt = 1;
	DMA_InitStructure.DMA_SourceAddr = (uint32)&PTC->PDIR;//源地址：PTC0-PTC7
	DMA_InitStructure.DMA_DestAddr = (uint32)camera.image_buf1;
	DMA_InitStructure.DMA_DestAddrOffset = 1;
	DMA_InitStructure.DMA_MajorCompleteIntEnable = TRUE;
	DMA_InitStructure.DMA_AutoDisableReq = TRUE;
	DMA_InitStructure.DMA_Isr = DMA_Complete_Isr;
	if(!LPLD_DMA_Init(DMA_InitStructure))
		return STATUS_FAILED;
	if(!LPLD_DMA_EnableIrq(DMA_InitStructure))
		return STATUS_FAILED;
	return STATUS_OK;
}

/***************************************************************
	*	@brief	摄像头延时
	*	@param	无
	*	@note	无
***************************************************************/
void Camera_Delay(void)
{
	int i, j;

	for(i = 0; i < 100; i++)
	{
		for(j = 0; j < 200; j++)
		{
			asm("nop");
		}
	}
}

/***************************************************************
	*	@brief	img中断(gpio)
	*	@param	无
	*	@note	进行DMA
***************************************************************/
void Img_Isr(void)
{
	static short i;
	if(LPLD_GPIO_IsPinxExt(IMG_IRQ_PORT, IMG_IRQ_Pinx))
	{
		i++;
		if(i >= 3)
		{
			if(camera.ready_write == 1)
			{
				camera.ready_write = 0;
				i = 0;
				LPLD_GPIO_ClearIntFlag(PORTB);
				if (camera.which_buffer == 1)
				{
					LPLD_DMA_LoadDstAddr(DMA_CH0, (uint8 *)camera.image_buf1);
				}
				else
				{
					LPLD_DMA_LoadDstAddr(DMA_CH0, (uint8 *)camera.image_buf2);
				}
				LPLD_DMA_EnableReq(DMA_CH0);
			}
		}
		LPLD_GPIO_ClearIntFlag(IMG_IRQ_PORT);
	}
}

/***************************************************************
	*	@brief	DMA中断
	*	@param	无
	*	@note	可读写，切换传图标志
***************************************************************/
void DMA_Complete_Isr(void)
{
	camera.ready_read = 1;
	camera.ready_write = 1;
	if(camera.which_buffer == 1)
	{
		camera.which_buffer = 2;
	}
	else
	{
		camera.which_buffer = 1;
	}
}

/***************************************************************
	*	@brief	解压图片
	*	@param	无
	*	@note	二值转换
***************************************************************/
void Img_Extract(void)
{
	uint8 i, j, k;
	camera.ready_read = 0;
//	Mid_Filter();
	if(camera.which_buffer == 1)
	{
		for(i = 0; i < ROW; i++)
		{
			for(j = 0; j < COLUMN / 8; j++)
			{
				for(k = 0; k < 8; k++)
				{
					camera.image[i][j * 8 + k] = c_ubyColor[(camera.image_buf1[i][j] >> (7 - k)) & 0x01];
				}
			}
		}
	}
	else
	{
		for(i = 0; i < ROW; i++)
		{
			for(j = 0; j < COLUMN / 8; j++)
			{
				for(k = 0; k < 8; k++)
				{
					camera.image[i][j * 8 + k] = c_ubyColor[(camera.image_buf2[i][j] >> (7 - k)) & 0x01];
				}
			}
		}
	}
}