#ifndef _OLED_H
#define _OLED_H
#include "common.h"
#define LED_SCLK(x) LPLD_GPIO_Output_b(PTA, 6, x)// d0
#define LED_SDA(x)  LPLD_GPIO_Output_b(PTE, 12, x)//d1
#define LED_RST(x)  LPLD_GPIO_Output_b(PTE, 11, x)//复位低能电平
#define LED_DC(x)   LPLD_GPIO_Output_b(PTE, 10, x)//偏置常低
void LED_Writedata(unsigned char data);
void LED_WrCmd(unsigned char cmd);
void LED_Set_Pos(uint8 x,uint8 y);
void LED_Fill(uint8 bmp_data);
void LED_CLS(void);
void LED_DLY_ms(uint16 ms);
void LED_P6x8Char(unsigned char x,unsigned char y,unsigned char ch);
void LED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LED_Point(unsigned int x,unsigned int y);
void LED_PrintValueI(unsigned char x, unsigned char y, int data);
void LED_PrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num);
void LED_PrintValueFPP(unsigned char x, unsigned char y, unsigned int data, unsigned char num);
void LED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
void showimage(void);
void Welcome_page(void);
void display(int8 display_flag);
void LED_Init(void);
void OLED_DispAImg(void);
















#endif