#include "head_file.h"
signed char PageNum = 0x00;
int8 ScreenStr[8][8][30];
float ScreenData[8][8];
signed char Course = 0;
int times=0;
void key_delay()
{
  int16 i,n;
  for(i=0;i<19000;i++)
  {
    for(n=0;n<100;n++)
    {
      asm("nop");
    }
  }
}


void Pageinit(signed char sPageNum)
{
  int i;
  PageNum = sPageNum;
  sprintf(ScreenStr[0][0], "MotorPID.OutValue");
  sprintf(ScreenStr[0][1], "dianshu");
  sprintf(ScreenStr[0][2], "set_dist");
  sprintf(ScreenStr[0][7], "s_cross");
  
  sprintf(ScreenStr[1][0], "s_zhi");
  sprintf(ScreenStr[1][1], "s_max");
  sprintf(ScreenStr[1][2], "s_wan1");
  sprintf(ScreenStr[1][3], "s_wan2");
  sprintf(ScreenStr[1][4], "s_cross");
  sprintf(ScreenStr[1][5], "huan");

 
   
  sprintf(ScreenStr[2][0], "Servo_set");
  sprintf(ScreenStr[2][1], "S_kp");
  sprintf(ScreenStr[2][2], "S_kp_r");
  sprintf(ScreenStr[2][3], "S_kd");
  sprintf(ScreenStr[2][4], "kp_huan_inc");
  sprintf(ScreenStr[2][5], "kp_obs_inc");
  
  sprintf(ScreenStr[3][0], "S_chg_kp");
  sprintf(ScreenStr[3][1], "S_chg_40");
  sprintf(ScreenStr[3][2], "S_chg_50");
  sprintf(ScreenStr[3][3], "S_chg_60");
  sprintf(ScreenStr[3][4], "max");    //max
  sprintf(ScreenStr[3][5], "mid");     //mid
  sprintf(ScreenStr[3][6], "min");      //min
  sprintf(ScreenStr[3][7], "Mid_er_max");
  
  sprintf(ScreenStr[4][0], "Foresight");
  sprintf(ScreenStr[4][1], "fore_obs");
  sprintf(ScreenStr[4][2], "fore_ramp");
  sprintf(ScreenStr[4][3], "fore_offset");
  sprintf(ScreenStr[4][4], "fore_min");
  sprintf(ScreenStr[4][5], "fore_max");
  sprintf(ScreenStr[4][6], "huan_sight");
  sprintf(ScreenStr[4][7], "huan_sight2");

 
  
  sprintf(ScreenStr[5][0], "po_mode");
  sprintf(ScreenStr[5][1], "Gyro_set");
  
  
  
  sprintf(ScreenStr[6][0], "Some mode");
  sprintf(ScreenStr[6][1], "sta_r_obs");
  sprintf(ScreenStr[6][2], "sta_l_obs");
  sprintf(ScreenStr[6][3], "shizi_obs");




  
  

  ScreenData[0][0]=MotorPID.OutValue;
  ScreenData[0][1]=dianshu;
//  ScreenData[0][2]=set_dist;
  ScreenData[0][7]=s_cross;
  
 
  ScreenData[1][0]=s_zhi;
  ScreenData[1][1]=s_max;
  ScreenData[1][2]=s_wan1;
  ScreenData[1][3]=s_wan2;
  ScreenData[1][4]=s_cross;
  ScreenData[1][5]=s_huan;

  
  ScreenData[2][1]=S_kp;
  ScreenData[2][2]=S_kp_r;
  ScreenData[2][3]=S_kd;
//  ScreenData[2][4]=kp_huan_inc;
//  ScreenData[2][5]=kp_obs_inc;

   
//  ScreenData[3][0]=S_chg_kp;
//  ScreenData[3][1]=S_chg_40;
//  ScreenData[3][2]=S_chg_50;
//  ScreenData[3][3]=S_chg_60;
  ScreenData[3][4]=max;
  ScreenData[3][5]=mid;
  ScreenData[3][6]=min;
//  ScreenData[3][7]=Mid_error_max;
  
 
//  ScreenData[4][1]=fore_obs;
//  ScreenData[4][2]=fore_ramp;
//  ScreenData[4][3]=fore_offset;
  ScreenData[4][4]=fore_min;
  ScreenData[4][5]=fore_max;

  
 



  
  key_delay();
  key_delay();
  LED_Fill(0);
  key_delay();
  if(sPageNum==0)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[0][0]);
      LED_PrintValueF(70, 0, ScreenData[0][0], 2);
      LED_P6x8Str(115, 0, "P1");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[0][i]);
          LED_PrintValueF(70, i, ScreenData[0][i], 2);
      }
  }
  
  else if(sPageNum==1)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[1][0]);
      LED_PrintValueF(70, 0, ScreenData[1][0], 2);
      LED_P6x8Str(115, 0, "P2");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[1][i]);
          LED_PrintValueF(70, i, ScreenData[1][i], 2);
      }
  }
  
  else if(sPageNum==2)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[2][0]);
      LED_P6x8Str(115, 0, "P3");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[2][i]);
          LED_PrintValueF(70, i, ScreenData[2][i], 2);
      }
  }
  
  else if(sPageNum==3)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[3][0]);
      LED_PrintValueF(70, 0, ScreenData[3][0], 2);
      LED_P6x8Str(115, 0, "P4");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[3][i]);
          LED_PrintValueF(70, i, ScreenData[3][i], 2);
          
      }
  }
 
  else if(sPageNum==4)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[4][0]);
      LED_P6x8Str(115, 0, "P5");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[4][i]);
          LED_PrintValueF(70, i, ScreenData[4][i], 2);
      }
  }
  
  else if(sPageNum==5)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[5][0]);
      LED_PrintValueF(70, 0, ScreenData[5][0], 2);
      LED_P6x8Str(115, 0, "P6");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[5][i]);
          LED_PrintValueF(70, i, ScreenData[5][i], 2);
      }
  }
  
  else if(sPageNum==6)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[6][0]);
      LED_P6x8Str(115, 0, "P7");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[6][i]);
          LED_PrintValueF(70, i, ScreenData[6][i], 2);
      }
  }
  
  else if(sPageNum==7)
  {
      LED_P6x8Str(8, 0, (unsigned char*) ScreenStr[7][0]);
      LED_PrintValueF(70, 0, ScreenData[7][0], 2);
      LED_P6x8Str(115, 0, "P8");
      for(i=1;i<8;i++)
      {
          LED_P6x8Str(8, i, (unsigned char*) ScreenStr[7][i]);
          LED_PrintValueF(70, i, ScreenData[7][i], 2);
      }
  }
  LED_P6x8Char(0, Course, '*');
}

void key_process(void)
{
  DisableInterrupts;
  Pageinit(PageNum);
  unsigned char temp = 10;
  while(1)
  { 
    temp=key_scan();
    LPLD_LPTMR_DelayMs(20);
    BUZZER_OFF;
    if(temp==13)
    {//开关一是否合上
      key_delay();
      key_delay();
      if(SWITCH1==1||SWITCH2==1||SWITCH3==1||SWITCH4==1)
      {
	key_delay();
	LED_Fill(0);
	key_delay();
	key_delay();
      }
      break;
    }
    else if(temp==5&&PageNum!=7)
    {//右切页
      if(PageNum>=0&&PageNum<7)
      {
	PageNum=PageNum+1;
        Pageinit(PageNum);
      }
    }
    else if(temp==5&&PageNum==7)
      {
       PageNum=0;
       Pageinit(PageNum);
      }

    
    else if(temp==6&&PageNum!=0)
    {//左切页
      if(PageNum>0&&PageNum<=7)
      {
	PageNum=PageNum-1;
	Pageinit(PageNum);
      }
    }
    else  if(temp==6&&PageNum==0)
    {
      PageNum=7;
      Pageinit(PageNum);
    }
    
    else
    {
      switch (temp)
      {
      case 1:key_up(); key_delay();  break;
      case 2:Key_DataUp(); key_delay();  break;
      case 3:key_Down(); key_delay();  break;
      case 4:Key_DataDown(); key_delay();  break;
      }
    }
  }
    LED_Fill(0x00);
   EnableInterrupts;

}
/********************
 key_scan函数return的值：
key2上一项（2）      key1增（4）  
key3下一项 （1）     key4减（3） 
key6左切页4）        key5右切页（4）

当前 
key1上一个 key6上一页
key2加     key5下一页
key3下一个 key4减
修改完
key1上一页 key6下一页
key2上一项 key5下一项
key3增     key4减

任一开关被推上去打开返回13退出菜单栏
********************/
unsigned char key_scan(void)
{ 
  if(SWITCH1==1||SWITCH2==1||SWITCH3==1||SWITCH4==1)
  {
    key_delay();
    key_delay();
    if(SWITCH1==1||SWITCH2==1||SWITCH3==1||SWITCH4==1)
    {
      return 13;
    }
  }
  
  else if (key1==0)
  {
     key_delay();
     if (key1==0)
     {
      BUZZER_ON;
      return 6;
     }
  }
  
  else if (key2==0)
  {
     key_delay();
     if (key2==0)
     {
       BUZZER_ON;
      return 1;
     }
  }
  else if (key3==0)
  {
    
     key_delay();
     if (key3==0)
     {
      BUZZER_ON;
      return 2;
     }
  }
  
   else if (key4==0)
  {

     key_delay();
     if (key4==0)
     {
      BUZZER_ON;
      return 4;
     }
  } 
  
   else if (key5==0)
  {

     key_delay();
     if (key5==0)
     {
      BUZZER_ON;
      return 3;
     }
  }
  
   else if (key6==0)
  {

     key_delay();
     if (key6==0)
     {
      BUZZER_ON;
      return 5;
     }
  }
  
//     else if (key1==0)  //一键发车
//  {
//
//     key_delay();
//     if (key1==0)
//     {
//      BUZZER_ON;
//      return 7;
//     }
//  }
return 10;

}

void key_up(void)
{//上一项
 LED_P6x8Char(0, (uint8)Course, ' ');
 if (PageNum == 0)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
    }
  }
  else if (PageNum == 1)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
      PageNum = 0;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 2)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
      PageNum = 1;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 3)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
      PageNum = 2;
      Pageinit(PageNum);
    }
  }
 else if (PageNum == 4)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
      PageNum = 3;
      Pageinit(PageNum);
    }
  }
 else if (PageNum == 5)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
      PageNum = 4;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 6)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
      PageNum = 5;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 7)
  {
    Course--;
    if (Course < 0)
    {
      Course = 7;
      PageNum = 6;
      Pageinit(PageNum);
    }
  }
 LED_P6x8Char(0, (uint8)Course, '*');
}

void key_Down(void)
{//下一项
 LED_P6x8Char(0, (uint8)Course, ' ');
 if (PageNum == 0)
  {
    Course++;
    if (Course>7)
    {
      PageNum = 1;
      Pageinit(PageNum);
      Course = 0;
    }
  }
  else if (PageNum == 1)
  {
    Course++;
    if (Course>7)
    {
      Course = 0;
      PageNum =2;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 2)
  {
    Course++;
    if (Course>7)
    {
      Course = 0;
      PageNum = 3;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 3)
  {
    Course++;
    if (Course>7)
    {
      Course = 0;
      PageNum = 4;
      Pageinit(PageNum);
    }
  }
 else if (PageNum == 4)
  {
    Course++;
    if (Course>7)
    {
      Course = 0;
      PageNum = 5;
      Pageinit(PageNum);
    }
  }
 else if (PageNum == 5)
  {
    Course++;
    if (Course>7)
    {
      Course = 0;
      PageNum = 6;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 6)
  {
    Course++;
    if (Course>7)
    {
      Course = 0;
      PageNum = 7;
      Pageinit(PageNum);
    }
  }
  else if (PageNum == 7)
  {
    Course++;
    if (Course>7)
    {
      Course = 0;
      PageNum = 0;
      Pageinit(PageNum);
    }
  }
 LED_P6x8Char(0, (uint8)Course, '*');
}

void Key_DataUp(void)    //改动加数值
{
  if(PageNum==0)
  {
    switch (Course)
    {
    case 0: ScreenData[PageNum][Course] += 200; break;
    case 1: ScreenData[PageNum][Course] += 1; break;
    case 2: ScreenData[PageNum][Course] += 1.0; break;
    case 3: ScreenData[PageNum][Course] += 10.0; break;
    case 4: ScreenData[PageNum][Course] += 10.0; break;
    case 5: ScreenData[PageNum][Course] += 10.0; break;
    case 6: ScreenData[PageNum][Course] += 10.0; break;
    case 7: ScreenData[PageNum][Course] += 10.0; break;
        
    }
   SaveData();
  }
  else if (PageNum == 1)
  {
    switch (Course)
    {
    case 0:ScreenData[PageNum][Course] += 10.0; break;
    case 1:ScreenData[PageNum][Course] += 10.0; break;
    case 2:ScreenData[PageNum][Course] += 10.0; break;
    case 3:ScreenData[PageNum][Course] += 10.0; break;
    case 4:ScreenData[PageNum][Course] += 10.0; break;
    case 5:ScreenData[PageNum][Course] += 10.0; break;
    case 6:ScreenData[PageNum][Course] += 10.0; break;
    case 7:ScreenData[PageNum][Course] += 10.0; break;
    }
    SaveData();
  }
  else if (PageNum == 2)
  {
    switch (Course)
    {
    case 0:ScreenData[PageNum][Course] += 0.01; break;
    case 1:ScreenData[PageNum][Course] += 0.01; break;
    case 2:ScreenData[PageNum][Course] += 0.01; break;
    case 3:ScreenData[PageNum][Course] += 0.1; break;
    case 4:ScreenData[PageNum][Course] += 0.1; break;
    case 5:ScreenData[PageNum][Course] += 0.1; break;
    case 6:ScreenData[PageNum][Course] += 0.1; break;
    case 7:ScreenData[PageNum][Course] += 0.1f; break;

    }
    SaveData();
  }
  else if (PageNum == 3)
  {
    switch (Course)
    {
    case 0:ScreenData[PageNum][Course] += 0.1; break;
    case 1:ScreenData[PageNum][Course] += 0.01; break;
    case 2:ScreenData[PageNum][Course] += 0.01; break;
    case 3:ScreenData[PageNum][Course] += 0.1; break;
    case 4:ScreenData[PageNum][Course] += 1.0; break;
    case 5:ScreenData[PageNum][Course] += 1.0; break;
    case 6:ScreenData[PageNum][Course] += 1.0; break;
    case 7:ScreenData[PageNum][Course] += 1.0; break;    

    }
    SaveData();
  }
  else if (PageNum == 4)
  {
    switch (Course)
    {

    case 0:ScreenData[PageNum][Course] += 1.0f; break;
    case 1:ScreenData[PageNum][Course] += 1.0f; break;
    case 2:ScreenData[PageNum][Course] += 1.0f; break;
    case 3:ScreenData[PageNum][Course] += 1.0f; break;
    case 4:ScreenData[PageNum][Course] += 1.0f; break;
    case 5:ScreenData[PageNum][Course] += 1.0f; break;
    case 6:ScreenData[PageNum][Course] += 1.0f; break;
    case 7:ScreenData[PageNum][Course] += 1.0; break;
    }
    SaveData();
  }
  else if (PageNum == 5)
  {
    switch (Course)
    {
    case 0:ScreenData[PageNum][Course] += 1.0f; break;
    case 1:ScreenData[PageNum][Course] += 10.0; break;
    case 2:ScreenData[PageNum][Course] += 0.5f; break;
    case 3:ScreenData[PageNum][Course] += 1.0; break;
    case 4:ScreenData[PageNum][Course] += 1.0; break;
    case 5:ScreenData[PageNum][Course] += 1.0; break;
    case 6:ScreenData[PageNum][Course] += 1.0; break;
    case 7:ScreenData[PageNum][Course] += 1.0; break;
    }
    SaveData();
  }
  else if (PageNum == 6)
  {
    switch (Course)
    {
    case 0:ScreenData[PageNum][Course] += 1.0f; break;
    case 1:ScreenData[PageNum][Course] += 1.0f; break;
    case 2:ScreenData[PageNum][Course] += 1.0; break;
    case 3:ScreenData[PageNum][Course] += 1.0; break;
    case 4:ScreenData[PageNum][Course] += 1.0; break;
    case 5:ScreenData[PageNum][Course] += 1.0f; break;
    case 6:ScreenData[PageNum][Course] += 1.0f; break;
    case 7:ScreenData[PageNum][Course] += 1.0f; break;
    }
    SaveData();
  }
  else if (PageNum == 7)
  {
    switch (Course)
    {
    case 0:ScreenData[PageNum][Course] += 1.0f; break;
    case 1:ScreenData[PageNum][Course] += 0.5; break;
    case 2:ScreenData[PageNum][Course] += 0.5; break;
    case 3:ScreenData[PageNum][Course] += 1.0; break;
    case 4:ScreenData[PageNum][Course] += 0.5; break;
    case 5:ScreenData[PageNum][Course] += 0.5; break;
    case 6:ScreenData[PageNum][Course] += 10.0; break;
    case 7:ScreenData[PageNum][Course] += 5.0; break;
    }
    SaveData();
  }
  LED_P6x8Str(70, Course, "      ");
  LED_PrintValueF(70, Course, ScreenData[PageNum][Course], 2); 
}

void Key_DataDown(void)
{
  if (PageNum == 0)
  {
    switch (Course)
    {
    case 0: ScreenData[PageNum][Course] -= 200; break;
    case 1: ScreenData[PageNum][Course] -= 1; break;
    case 2: ScreenData[PageNum][Course] -= 1.0f; break;
    case 3: ScreenData[PageNum][Course] -= 10.0; break;
    case 4: ScreenData[PageNum][Course] -= 10.0; break;
    case 5: ScreenData[PageNum][Course] -= 10.0; break;
    case 6: ScreenData[PageNum][Course] -= 10.0; break;
    case 7: ScreenData[PageNum][Course] -= 10.0; break;
    }
    SaveData();
  }
  
  if (PageNum == 1)
  {
    switch (Course)
    {
    case 0: ScreenData[PageNum][Course] -= 10.0f; break;
    case 1: ScreenData[PageNum][Course] -= 10.0f; break;
    case 2: ScreenData[PageNum][Course] -= 10.0f; break;
    case 3: ScreenData[PageNum][Course] -= 10.0f; break;
    case 4: ScreenData[PageNum][Course] -= 10.0f; break;
    case 5: ScreenData[PageNum][Course] -= 10.0f; break;
    case 6: ScreenData[PageNum][Course] -= 10.0f; break;
    case 7: ScreenData[PageNum][Course] -= 10.0f; break;
    }
    SaveData();
  }
  
  if (PageNum == 2)
  {
    switch (Course)
    {

    case 0: ScreenData[PageNum][Course] -= 0.01;break;  
    case 1: ScreenData[PageNum][Course] -= 0.01; break;
    case 2: ScreenData[PageNum][Course] -= 0.01; break;
    case 3: ScreenData[PageNum][Course] -= 0.1; break;
    case 4: ScreenData[PageNum][Course] -= 0.1; break;
    case 5: ScreenData[PageNum][Course] -= 0.1; break;
    case 6: ScreenData[PageNum][Course] -= 0.1; break;
    case 7: ScreenData[PageNum][Course] -= 0.1f; break;
    }
    SaveData();
  }
  
  if (PageNum == 3)
  {
    switch (Course)
    {
    

    case 0: ScreenData[PageNum][Course] -= 0.1;break;  
    case 1: ScreenData[PageNum][Course] -= 0.01; break;
    case 2: ScreenData[PageNum][Course] -= 0.01; break;
    case 3: ScreenData[PageNum][Course] -= 0.1; break;
    case 4: ScreenData[PageNum][Course] -= 1.0; break;
    case 5: ScreenData[PageNum][Course] -= 1.0; break;
    case 6: ScreenData[PageNum][Course] -= 1.0; break;
    case 7: ScreenData[PageNum][Course] -= 1.0; break;
    }
    SaveData();
  }
  
  if (PageNum == 4)
  {
    switch (Course)
    {

    case 0: ScreenData[PageNum][Course] -= 1.0f;break;  
    case 1: ScreenData[PageNum][Course] -= 1.0f; break;
    case 2: ScreenData[PageNum][Course] -= 1.0f; break;
    case 3: ScreenData[PageNum][Course] -= 1.0f; break;
    case 4: ScreenData[PageNum][Course] -= 1.0f; break;
    case 5: ScreenData[PageNum][Course] -= 1.0f; break;
    case 6: ScreenData[PageNum][Course] -= 1.0f; break;
    case 7: ScreenData[PageNum][Course] -= 1.0; break;
    }
    SaveData();
  }
  
  if (PageNum == 5)
  {
    switch (Course)
    {
    case 0: ScreenData[PageNum][Course] -= 1.0f;break;  
    case 1: ScreenData[PageNum][Course] -= 10.0; break;
    case 2: ScreenData[PageNum][Course] -= 0.5f; break;
    case 3: ScreenData[PageNum][Course] -= 1.0f; break;
    case 4: ScreenData[PageNum][Course] -= 1.0f; break;
    case 5: ScreenData[PageNum][Course] -= 1.0f; break;
    case 6: ScreenData[PageNum][Course] -= 1.0f; break;
    case 7: ScreenData[PageNum][Course] -= 1.0f; break;
    }
    SaveData();
  }
  
  if (PageNum == 6)
  {
    switch (Course)
    {
    case 0: ScreenData[PageNum][Course] -= 1.0f;break;  
    case 1: ScreenData[PageNum][Course] -= 1.0f; break;
    case 2: ScreenData[PageNum][Course] -= 1.0; break;
    case 3: ScreenData[PageNum][Course] -= 1.0; break;
    case 4: ScreenData[PageNum][Course] -= 1.0; break;
    case 5: ScreenData[PageNum][Course] -= 1.0f; break;
    case 6: ScreenData[PageNum][Course] -= 1.0f; break;
    case 7: ScreenData[PageNum][Course] -= 1.0f; break;
    }
    SaveData();
  }
  if (PageNum == 7)
  {
    switch (Course)
    {
    case 0: ScreenData[PageNum][Course] -= 1.0f;break;  
    case 1: ScreenData[PageNum][Course] -= 0.5; break;
    case 2: ScreenData[PageNum][Course] -= 0.5; break;
    case 3: ScreenData[PageNum][Course] -= 1.0; break;
    case 4: ScreenData[PageNum][Course] -= 0.5; break;
    case 5: ScreenData[PageNum][Course] -= 0.5; break;
    case 6: ScreenData[PageNum][Course] -= 10.0; break;
    case 7: ScreenData[PageNum][Course] -= 5.0; break;
    }
    SaveData();
  }
  LED_P6x8Str(70, Course, "      ");
  LED_PrintValueF(70, Course, ScreenData[PageNum][Course], 2);
}

void SaveData(void)
{
  MotorPID.OutValue = (int16)ScreenData[0][0]; 
  dianshu= (int16)ScreenData[0][1];
  set_dist=ScreenData[0][2];
  s_cross=(int16)ScreenData[0][7];
  
 
  s_zhi=(int16)ScreenData[1][0];
  s_max=(int16)ScreenData[1][1];
  s_wan1= (int16)ScreenData[1][2];
  s_wan2=(int16)ScreenData[1][3];
  s_cross=(int16)ScreenData[1][3];
  s_huan=(int16)ScreenData[1][3];

  
  
  S_kp=ScreenData[2][1];
  S_kp_r=ScreenData[2][2];
  S_kd=ScreenData[2][3];
//  kp_huan_inc=ScreenData[2][4];
//  kp_obs_inc=ScreenData[2][5];

  
  
//  S_chg_kp= ScreenData[3][0];
//  S_chg_40= ScreenData[3][1];
//  S_chg_50= ScreenData[3][2];
//  S_chg_60= ScreenData[3][3];
  max= (int16)ScreenData[3][4];
  mid= (int16)ScreenData[3][5];
  min= (int16)ScreenData[3][6];
//  Mid_error_max= (int16)ScreenData[3][7];

  

//  fore_obs=(int16)ScreenData[4][1];
//  fore_ramp=(int16)ScreenData[4][2];
//  fore_offset=(int16)ScreenData[4][3];
  fore_min=(int16)ScreenData[4][4];
  fore_max=(int16)ScreenData[4][5];

}





