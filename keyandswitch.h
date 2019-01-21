#ifndef _Keyandswitch_H_
#define _Keyandswitch_H_
#include "common.h"
#define key1  PTE28_I
#define key2  PTE27_I 
#define key3  PTA7_I
#define key4  PTA5_I
#define key5  PTA12_I
#define key6  PTA11_I


#define SWITCH1  PTE24_I
#define SWITCH2  PTA25_I
#define SWITCH3  PTE26_I
#define SWITCH4  PTA24_I


void Pageinit(signed char sPageNum);
void SaveData(void);
void key_Down(void);
void key_up(void);
void Key_DataDown(void);
void Key_DataUp(void);
void key_delay(void);
unsigned char key_scan(void);
void keytest(void);
void key_process(void);
extern int times;
extern signed char Course;
extern float ScreenData[8][8];
extern int8 ScreenStr[8][8][30];













#endif