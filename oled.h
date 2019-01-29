#ifndef _OLED_H_
#define _OLED_H_

#define OLED_GPIO_PTx 		PTE
#define OLED_GPIO_SCLK_Pinx	GPIO_Pin6
#define OLED_SCLK_PTx		PTA
#define OLED_GPIO_SDA_Pinx	GPIO_Pin12
#define OLED_GPIO_RST_Pinx	GPIO_Pin11
#define OLED_GPIO_DC_Pinx	GPIO_Pin10

#define OLED_SCLK(x) LPLD_GPIO_Output_b(OLED_SCLK_PTx, 6, x)//d0
#define OLED_SDA(x)  LPLD_GPIO_Output_b(OLED_GPIO_PTx, 12, x)//d1
#define OLED_RST(x)  LPLD_GPIO_Output_b(OLED_GPIO_PTx, 11, x)//复位低能电平
#define OLED_DC(x)   LPLD_GPIO_Output_b(OLED_GPIO_PTx, 10, x)//偏置常低

void Oled_Gpio_Init(void);
void OLED_WriteData(unsigned char data);
void OLED_WriteCmd(unsigned char cmd);
void OLED_SetPosition(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char bmp_data);
void OLED_DelayMs(unsigned int ms);

void OLED_Put6x8Char(unsigned char x, unsigned char y, unsigned char ch);
void OLED_Put6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_PrintCharValue(unsigned char x, unsigned char y, unsigned char data);
void OLED_PrintIntValue(unsigned char x, unsigned char y, int data);
void OLED_PrintUintValue(unsigned char x, unsigned char y, unsigned int data);
void OLED_PrintFloatValue(unsigned char x, unsigned char y, float data);
void OLED_ShowImage(void);

void SetDisplay_On_Off(unsigned char cmd);
void SetDisplayClock(unsigned char cmd);
void SetMultiplexRatio(unsigned char cmd);
void SetDisplayOffset(unsigned char cmd);
void SetStartLine(unsigned char line);
void SetChargePump(unsigned char cmd);
void SetAddressingMode(unsigned char mode);
void SetSegmentRemap(unsigned char cmd);
void SetCommonRemap(unsigned char dir);
void SetCommonConfig(unsigned char cmd);
void SetContrastControl(unsigned char contrast);
void SetPrechargePeriod(unsigned char cmd);
void SetVCOMH(unsigned char cmd);
void SetEntireDisplay(unsigned char cmd);
void SetInverseDisplay(unsigned char cmd);

#endif