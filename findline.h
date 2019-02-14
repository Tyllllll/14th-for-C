#ifndef _Findline_H_
#define _Findline_H_
#include "common.h"
void bianxian(void);
void find_toppoint(void);
void find_curve1(void);//找弯一
int8 find_curve3(void);
void find_guai(void);
void shizi_judge(void);
void findline1(void);
void findline2(void);
int8 judge_leftlose(void);
int8 judge_rightlose(void);
int8 judge_bothlose(void);
int8 right(int16 row);
int8 left(int16 row);

typedef struct
{
  int16 midline[120];
  int16 leftline[120];
  int16 rightline[120];
  int16 leftlineflag[120];
  int16 rightlineflag[120];
}Findline1_TypeDef;
extern Findline1_TypeDef f1;

typedef struct
{
  int16 leftturnflag;
  int16 rightturnflag;
  int16 leftturn_row;
  int16 rightturn_row;
  int16 leftguai_column;
  int16 leftguai_row;
  int16 leftguaiflag;
  int16 rightguaiflag; 
  int16 rightguai_column;
  int16 rightguai_row;
  int16 leftguaiflag_up;
  int16 leftguai_column_up;
  int16 leftguai_row_up;
  int16 rightguaiflag_up;
  int16 rightguai_column_up;
  int16 rightguai_row_up;
  int16 toppoint;
  int16 toppoint_flag;
  int16 shiziflag[2];
  int16 huandaoflag;//0无 奇数左转 偶数右转
  					//12 进环前		34 进环		56 环内		78 出环
  float zhizi_k_L;
  float zhizi_k_R;
}Findline2_TypeDef;
extern Findline2_TypeDef f2;

extern int16  width[120];
extern int16 width_real[120];
extern int16 text_width[120];
extern int16 column_start;

void Tyl_judge_huandao(void);
int8 Tyl_judge_leftlose(void);
int8 Tyl_judge_rightlose(void);
extern int16 state3to5column;
extern int16 loselinecount;
extern int16 xielvcha;
extern int16 dianshu;
int16 isLeftZhi(int16);
int16 isRightZhi(int16);
#endif