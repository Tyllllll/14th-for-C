#include "header.h"

const unsigned char F6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x02, 0x04 ,0x08, 0x10, 0x20 },   // '\'
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x08, 0x08, 0x49, 0x2A, 0x14, 0x08 }    // cursor
};

/***************************************************************
	*	@brief	oled初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Oled_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = OLED_SCLK_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Pins =  OLED_GPIO_SCLK_Pinx;
	GPIO_InitStructure.GPIO_Output = OUTPUT_H;
	GPIO_InitStructure.GPIO_PinControl = IRQC_DIS;
	LPLD_GPIO_Init(GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PTx = OLED_GPIO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Pins =  OLED_GPIO_SDA_Pinx | OLED_GPIO_RST_Pinx | OLED_GPIO_DC_Pinx;
	GPIO_InitStructure.GPIO_Output = OUTPUT_H;
	GPIO_InitStructure.GPIO_PinControl = IRQC_DIS;
	LPLD_GPIO_Init(GPIO_InitStructure);
	
	OLED_DC(0);
	OLED_SDA(0);
	OLED_SCLK(0);
	OLED_RST(0);
	OLED_DelayMs(50);
	OLED_SCLK(1);
	OLED_RST(0);
	OLED_DelayMs(50);
	OLED_RST(1);

	SetDisplay_On_Off(0x00);	// Display Off (0x00/0x01)
	SetDisplayClock(0x80);		// Set Clock as 100 Frames/Sec
	SetMultiplexRatio(0x3F);	// 1/64 Duty (0x0F~0x3F)
	SetDisplayOffset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	SetStartLine(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	SetChargePump(0x04);		// Enable Embedded DC/DC Converter (0x00/0x04)
	SetAddressingMode(0x02);	// Set Page Addressing Mode (0x00/0x01/0x02)
	SetSegmentRemap(0x01);		// Set SEG/Column Mapping     0x00左右反置 0x01正常
	SetCommonRemap(0x08);		// Set COM/Row Scan Direction 0x00上下反置 0x08正常
	SetCommonConfig(0x10);		// Set Sequential Configuration (0x00/0x10)
	SetContrastControl(0xCF);	// Set SEG Output Current
	SetPrechargePeriod(0xF1);	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	SetVCOMH(0x40);				// Set VCOM Deselect Level
	SetEntireDisplay(0x00);		// Disable Entire Display On (0x00/0x01)
	SetInverseDisplay(0x00);	// Disable Inverse Display On (0x00/0x01)
	SetDisplay_On_Off(0x01);	// Display On (0x00/0x01)
	OLED_Fill(0x00);
	OLED_SetPosition(0,0);
}

/***************************************************************
	*	@brief	写数据
	*	@param	data：数据
	*	@note	8位二进制
***************************************************************/
void OLED_WriteData(unsigned char data)
{
	unsigned char i = 8;

	OLED_DC(1);
	OLED_SCLK(0);

	while(i--)
	{
		if(data & 0x80)
			OLED_SDA(1);
		else
			OLED_SDA(0);

		OLED_SCLK(1);
		asm("nop");
		OLED_SCLK(0);

		data <<= 1;
	}
}

/***************************************************************
	*	@brief	写命令
	*	@param	cmd：命令
	*	@note	8位二进制
***************************************************************/
void OLED_WriteCmd(unsigned char cmd)
{
	unsigned char i = 8;

	OLED_DC(0);
	OLED_SCLK(0);

	while(i--)
	{
		if(cmd & 0x80)
			OLED_SDA(1);
		else
			OLED_SDA(0);

		OLED_SCLK(1);
		asm("nop");
		OLED_SCLK(0);

		cmd <<= 1;
	}
}

/***************************************************************
	*	@brief	设置坐标
	*	@param	(x, y)：坐标
	*	@note	无
***************************************************************/
void OLED_SetPosition(unsigned char x, unsigned char y)
{
	OLED_WriteCmd(0xB0 + y);
	OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10);
	OLED_WriteCmd((x & 0x0F) | 0x00);
}

/***************************************************************
	*	@brief	写全屏
	*	@param	bmp_data：数据
	*	@note	无
***************************************************************/
void OLED_Fill(unsigned char bmp_data)
{
	unsigned char x, y;

	for(y = 0 ; y < 8 ; y++)
	{
		OLED_WriteCmd(0xB0 + y);	// 0xb0 + 0~7 means page 0~7
		OLED_WriteCmd(0x00);	// 0x00 + 0~16 means 128 pixs / 16
		OLED_WriteCmd(0x10);	// 0x10 + 0~16 means 128 pixs / 16 groups

		for(x = 0 ; x < 128 ; x++)
			OLED_WriteData(bmp_data);
	}
}

/***************************************************************
	*	@brief	oled延时
	*	@param	ms：延时时间（毫秒）
	*	@note	无
***************************************************************/
void OLED_DelayMs(unsigned int ms)
{
	uint16 i;
	while(ms--)
	{
		i = 6675;
		while(i--);
	}
}

/***************************************************************
	*	@brief	指定位置输出字符
	*	@param	(x, y)：坐标		ch：字符
	*	@note	无
***************************************************************/
void OLED_Put6x8Char(uint8 x, uint8 y, int8 ch)
{
	unsigned char c = 0, i = 0;

	c = ch - 32;

	if(x > 122)
	{
		x = 0;
		y++;
	}
	OLED_SetPosition(x, y);

	for(i = 0 ; i < 6 ; i++)
	{
		OLED_WriteData(F6x8[c][i]);
	}
}

/***************************************************************
	*	@brief	指定位置输出字符串
	*	@param	(x, y)：坐标		ch：字符串
	*	@note	无
***************************************************************/
void OLED_Put6x8Str(uint8 x, uint8 y, int8 ch[])
{
	unsigned char c = 0, i = 0, j = 0;

	while(ch[j] != '\0')
	{
		c = ch[j] - 32;

		if(x > 126)
		{
			x = 0;
			y++;
		}
		OLED_SetPosition(x, y);

		for(i = 0 ; i < 6 ; i++)
		{
			OLED_WriteData(F6x8[c][i]);
		}
		x += 6;
		j++;
	}
}

/***************************************************************
	*	@brief	指定位置输出字符的值
	*	@param	(x, y)：坐标		data：字符
	*	@note	无
***************************************************************/
void OLED_PrintCharValue(unsigned char x, unsigned char y, unsigned char data)
{
	OLED_Put6x8Char(x, y, (data / 100) + 48);
	OLED_Put6x8Char(x + 6, y, ((data % 100) / 10) + 48);
	OLED_Put6x8Char(x + 12, y, (data % 10) + 48);
}

/***************************************************************
	*	@brief	指定位置输出整数的值
	*	@param	(x, y)：坐标		data：整型变量
	*	@note	无
***************************************************************/
void OLED_PrintIntValue(unsigned char x, unsigned char y, int data)
{
	if(data < 0)
	{
		OLED_Put6x8Char(x, y, '-');
		data = -data;
	}
	else
	{
		OLED_Put6x8Char(x, y, '+');
	}

	OLED_Put6x8Char(x + 6, y, (data / 1000) + 48);
	OLED_Put6x8Char(x + 12, y, ((data % 1000) / 100) + 48);
	OLED_Put6x8Char(x + 18, y, ((data % 100) / 10) + 48);
	OLED_Put6x8Char(x + 24, y, (data % 10) + 48);
}

/***************************************************************
	*	@brief	指定位置输出无符号整数的值
	*	@param	(x, y)：坐标		data：无符号整数
	*	@note	无
***************************************************************/
void OLED_PrintUintValue(unsigned char x, unsigned char y, unsigned int data)
{
	OLED_Put6x8Char(x + 0, y, (data / 100) + 48);
	OLED_Put6x8Char(x + 6, y, ((data % 100) / 10) + 48);
	OLED_Put6x8Char(x + 12, y, (data % 10) + 48);
//	OLED_Put6x8Char(x + 18, y, (data % 10) + 48);
}

/***************************************************************
	*	@brief	指定位置输出浮点型的值
	*	@param	(x, y)：坐标		data：浮点型
	*	@note	无
***************************************************************/
void OLED_PrintFloatValue(unsigned char x, unsigned char y, float data)
{
	int temp;

	temp = (int)data;
	OLED_Put6x8Char(x + 0, y, ((temp % 1000) / 100) + 48);
	OLED_Put6x8Char(x + 6, y, ((temp % 100) / 10) + 48);
	OLED_Put6x8Char(x + 12, y, (temp % 10) + 48);

	OLED_Put6x8Char(x + 18, y, '.');

	data = data * 100000 + 1;
	temp = (int)data;
	temp %= 100000;
	OLED_Put6x8Char(x + 24, y, (temp / 10000) + 48);
	OLED_Put6x8Char(x + 30, y, ((temp % 10000) / 1000) + 48);
	OLED_Put6x8Char(x + 36, y, ((temp % 1000) / 100) + 48);
}

/***************************************************************
	*	@brief	将摄像头的画面显示在oled上
	*	@param	无
	*	@note	无
***************************************************************/
void OLED_ShowImage(void)
{
	uint8 i, j;
	uint8 data;
	int8 cnt;
	for(i = 118; i > 5; i--)
	{
		camera.image[i][line.left_line[i]] = 0;
		camera.image[i][line.right_line[i]] = 0;
		camera.image[i][line.midline[i]] = 0;
	}
	for(i = 0; i < 159; i++)
	{
		camera.image[feature.top_point][i] = 0;
	}
	for(i = 0; i < 8; i++)
	{
		OLED_WriteCmd(0xb0 + i);
		OLED_WriteCmd(0x01);
		OLED_WriteCmd(0x10);
		for(j = 0; j < 80; j++)
		{
			data = 0;
			for(cnt = 7; cnt >= 0; cnt--)
			{
				data <<= 1;
				data |= camera.image[(i * 8 + cnt) * 2][2 * j] && 1;
			}
			OLED_WriteData(data);
		}
	}
	OLED_Put6x8Str(80, 0, "top");
	OLED_PrintUintValue(100, 0, feature.top_point);
	OLED_Put6x8Str(80, 1, "zh");
	OLED_PrintUintValue(100, 1, feature.straight_state);
	OLED_Put6x8Str(80, 2, "wa");
	OLED_PrintUintValue(100, 2, feature.turn_state);
	OLED_Put6x8Str(80, 3, "du");
	OLED_PrintUintValue(100, 3, feature.breakage_state);
	OLED_Put6x8Str(80, 4, "po");
	OLED_PrintUintValue(100, 4, feature.ramp_state);
	OLED_PrintIntValue(80, 5, feature.breakage_radius_curvature);
	OLED_Put6x8Str(80, 6, "which");
	OLED_PrintUintValue(100, 6, servo.which);
//	OLED_PrintFloatValue(80, 6, magnetic.angle * 57.3);
//	OLED_PrintIntValue(80, 7, is_Lose_All(90));
//	OLED_Put6x8Str(80, 4, "rt");
//	OLED_PrintUintValue(100, 4, feature.road_type[0]);
//	OLED_PrintUintValue(80, 4, feature.left_flection_row);
//	OLED_PrintUintValue(100, 4, line.left_line[feature.left_flection_row - 1]);
//	OLED_PrintUintValue(80, 5, feature.right_flection_row);
//	OLED_PrintUintValue(100, 5, line.right_line[feature.right_flection_row - 1]);
//	OLED_PrintUintValue(80, 6, feature.left_flection2_row);
//	OLED_PrintUintValue(100, 6, line.left_line[feature.left_flection2_row - 1]);
//	OLED_PrintUintValue(80, 7, feature.right_flection2_row);
//	OLED_PrintUintValue(100, 7, line.right_line[feature.right_flection2_row - 1]);
//	OLED_PrintUintValue(80, 6, motor.speed_set_left);
//	OLED_PrintUintValue(100, 6, motor.speed_set_right);
	OLED_PrintUintValue(80, 7, motor.speed_current_left[0]);
	OLED_PrintUintValue(100, 7, motor.speed_current_right[0]);
//	OLED_PrintIntValue(80, 7, servo.duty);
}

/***************************************************************
	*	@brief	以下均为写命令函数
	*	@param	**
	*	@note	基本为初始化调用，无需自行调用
***************************************************************/
void SetDisplay_On_Off(unsigned char cmd)
{
	OLED_WriteCmd(0xAE | cmd);	// Set Display On/Off
								// Default => 0xAE
								// 0xAE (0x00) => Display Off
								// 0xAF (0x01) => Display On
}

void SetDisplayClock(unsigned char cmd)
{
	OLED_WriteCmd(0xD5);	// Set Display Clock Divide Ratio / Oscillator Frequency
	OLED_WriteCmd(cmd);		// Default => 0x80
							// D[3:0] => Display Clock Divider
							// D[7:4] => Oscillator Frequency
}

void SetMultiplexRatio(unsigned char cmd)
{
	OLED_WriteCmd(0xA8);	// Set Multiplex Ratio
	OLED_WriteCmd(cmd);		// Default => 0x3F (1/64 Duty)
}

void SetDisplayOffset(unsigned char cmd)
{
	OLED_WriteCmd(0xD3);	// Set Display Offset
	OLED_WriteCmd(cmd);		// Default => 0x00
}

void SetStartLine(unsigned char line)
{
	OLED_WriteCmd(0x40 | line);	// Set Display Start Line
								// Default => 0x40 (0x00)
}

void SetChargePump(unsigned char cmd)
{
	OLED_WriteCmd(0x8D);		// Set Charge Pump
	OLED_WriteCmd(0x10 | cmd);	// Default => 0x10
								// 0x10 (0x00) => Disable Charge Pump
								// 0x14 (0x04) => Enable Charge Pump
}

void SetAddressingMode(unsigned char mode)
{
	OLED_WriteCmd(0x20);	// Set Memory Addressing Mode
	OLED_WriteCmd(mode);	// Default => 0x02
							// 0x00 => Horizontal Addressing Mode
							// 0x01 => Vertical Addressing Mode
							// 0x02 => Page Addressing Mode
}

void SetSegmentRemap(unsigned char cmd)
{
	OLED_WriteCmd(0xA0 | cmd);	// Set Segment Re-Map
								// Default => 0xA0
								// 0xA0 (0x00) => Column Address 0 Mapped to SEG0
								// 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void SetCommonRemap(unsigned char dir)
{
	OLED_WriteCmd(0xC0 | dir);	// Set COM Output Scan Direction
								// Default => 0xC0
								// 0xC0 (0x00) => Scan from COM0 to 63
								// 0xC8 (0x08) => Scan from COM63 to 0
}

void SetCommonConfig(unsigned char cmd)
{
	OLED_WriteCmd(0xDA);		// Set COM Pins Hardware Configuration
	OLED_WriteCmd(0x02 | cmd);	// Default => 0x12 (0x10)
								// Alternative COM Pin Configuration
								// Disable COM Left/Right Re-Map
}

void SetContrastControl(unsigned char contrast)
{
	OLED_WriteCmd(0x81);		// Set Contrast Control
	OLED_WriteCmd(contrast);	// Default => 0x7F
}

void SetPrechargePeriod(unsigned char cmd)
{
	OLED_WriteCmd(0xD9);	// Set Pre-Charge Period
	OLED_WriteCmd(cmd);		// Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
							// D[3:0] => Phase 1 Period in 1~15 Display Clocks
							// D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void SetVCOMH(unsigned char cmd)
{
	OLED_WriteCmd(0xDB);	// Set VCOMH Deselect Level
	OLED_WriteCmd(cmd);		// Default => 0x20 (0.77*VCC)
}

void SetEntireDisplay(unsigned char cmd)
{
	OLED_WriteCmd(0xA4 | cmd);	// Set Entire Display On / Off
								// Default => 0xA4
								// 0xA4 (0x00) => Normal Display
								// 0xA5 (0x01) => Entire Display On
}

void SetInverseDisplay(unsigned char cmd)
{
	OLED_WriteCmd(0xA6 | cmd);	// Set Inverse Display On/Off
								// Default => 0xA6
								// 0xA6 (0x00) => Normal Display
								// 0xA7 (0x01) => Inverse Display On
}