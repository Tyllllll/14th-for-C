#ifndef _PINGDING_H
#define _PINGDING_H
#include "common.h"
extern int16 super_zhidao;
extern int16 s_zhi;
extern int16 s_max;
extern int16 s_wan1;
extern int16 s_wan2;
extern int16 s_cross;

extern int16 Emergency_flag[20];
extern int16 Emergencystop_flag;
extern int16 road_type[200];
extern int16 shortstraight_flag;
extern int16 flag1;
extern int16 flag2;


void zhidao(void);
void shortstraight(void);
void crossstraight(void);
void sudu_set(void);
float mid_xielv(void);
void Emergency_judge(void);




#endif