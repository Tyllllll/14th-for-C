#include "common.h"
#include "camera.h"
#include "findline.h"
#include "function.h"
#include "servo.h"
#include "motor.h"
#include "distinguish.h"

int16 super_zhidao=0;
int16 shortstraight_flag;
int16 crossstraight_flag;
int16 stdDeviation=0;
int16 s_zhi=240;//36 45 33 30 ��30-34 35 �ϰ�20-27 25 ����10-25
int16 s_max=280;//
int16 s_wan1=180;
int16 s_wan2=170;
int16 s_cross=180;  
int16 g_HighestSpeed=0;
int16 g_LowestSpeed=0;
int16 Emergency_flag[20]={0};
int16 Emergencystop_flag=0;
int16 error_ave=0;
int16 error_last=0;
int16 error_pre=0;
int16 road_type[200]={0};//�������ͼ�¼ 1Ϊ��ֱ����2Ϊ��ֱ����3Ϊ�䣬4Ϊ����5Ϊʮ�֣�6Ϊ�µ���7Ϊ������8Ϊ�ϰ�
int16 tiaobian_cnt=0;



/*10cm-i=95 top=100
20cm-i=82 top=87
30cm-i=70 top=75
40cm-i=61 top=66
50cm-i=54 top=59
60cm-i=49 top=54
70cm-i=45 top=50
80cm-i=42 top=47
90cm-i=39 top=44
100cm-i=37 top=42
110cm-i=35 top=40
120cm-i=33 top=38
130cm-i=32 top=37
140cm-i=31 top=36
170cm-i=27 top=32
toppoint��ʵ�ʶ����5��*/

/**************************ʵ�ֺ���********************************************
*����ԭ��:	  void Emergency_judge(void)
*��������:	  ����ͣ������
���������        ��
���������        ��
*******************************************************************************/
void Emergency_judge(void)
{
    int16 i=0,j=0,count_black=0;
    for(i=118;i>115;i--)
    {
        for(j=130;j>=30;j--)
        {
            if(Image[i][j]==0)
            {
                count_black=count_black+1;     
            }
        }
    }
    if(count_black>200)
    {
        Emergencystop_flag=1;
    }
    else
    {
        Emergencystop_flag=0;
    }
    
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	  zhidao����
*��������:	  �ж��Ƿ��ǳ�ֱ��	
���������        ��
���������        ��
���ʱ��         2017.04.10
*******************************************************************************/
#define zhidao_endrow  31
#define Mid_err  5
void zhidao(void)
{//��zhidao_endrow����ͷ��ξ����� 1���߲�̫ƫ 2Ҳ������ 3�Ҳ�Ϊʮ�ֺ��µ� 4��������ֱ������
    int16 lose_cnt=0;
    int16 i=0,flag1=0,flag2=0;
    flag1=1;//�����ߵ�Ҫ��
    flag2=1;//�Զ��ߵ�Ҫ��
    for(i=118;i>zhidao_endrow;i--)
    {
        if(f1.midline[i]>88||f1.midline[i]<72)
        {
            flag1=0;
            break;
        }
    }
    if(flag1)
    {//���߲��ܳ���3
        for(i=118;i>zhidao_endrow;i--)
        { 
            if(f1.leftlineflag[i]==0||f1.rightlineflag[i]==0)
            {
                lose_cnt++;
            }
        }
        if(lose_cnt>3)
        {
            flag2=0;
        }
    }
    if(f2.toppoint<=35&&flag1==1&&flag2==1)
    {
        super_zhidao=1;
    }
    
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	  void shortstraight()
*��������:	  �ж϶�ֱ������	
���������        ��
���������        ��
���ʱ��         2018/7/8
*******************************************************************************/
void shortstraight(void)
{
  stdDeviation=(int16)xielv();
  if(stdDeviation<=6)
  {
    shortstraight_flag=1;
  }
  else if(stdDeviation<=11)
  {
    shortstraight_flag=2;
  }
  
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:	  void crossstraight()
*��������:	  �ж�ʮ������	
���������        ��
���������        ��
���ʱ��         2018/7/19
*******************************************************************************/
void crossstraight(void)
{
    stdDeviation=(int16)xielv();
    if(stdDeviation < 4)
    {
        crossstraight_flag=1;
    }
    else if(stdDeviation <= 6)
    {
        crossstraight_flag=2;
    }
    
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:	  void sudu_set(void)
*��������:	  ��ȡ�ٶ�ģʽ	
���������        ��
���������        ��
ֻ�г�ֱ ֱ �µ� ʮ���ж�
*******************************************************************************/
void sudu_set(void)
{
    int16 i=0;
    error_ave=(int16)(0.6*error[0]+0.2*error_last+0.2*error_pre);//�˲�
    error_pre=error_last;
    error_last=error_ave;
    super_zhidao=0;
    
    if(f2.shiziflag[1]==0)
        zhidao();//ֱ���ж�
    
/**********�ٶȸ���*************/
    if(f2.shiziflag[1]==1)
    {  
        road_type[0]=5;
        LedOff(1);
        LedOff(3);
        LedOff(2);
        LedOn(3);
        LedOn(2);//��ɫ
        
        MotorPID.SpeedSet = s_cross; 
    }
    
    else if(super_zhidao==1)
    {
        road_type[0]=1;
        LedOff(1);
        LedOff(3);
        LedOff(2);
        LedOn(3);//��ɫ
        
        if(road_type[40]==3&&road_type[41]==3&&road_type[42]==3&&road_type[43]==3&&road_type[44]==3)
            MotorPID.SpeedSet=s_max+100;//������� δ���� �������ע��
        else
            MotorPID.SpeedSet=s_max;
    }
    
    else if(f2.toppoint<=37)
    {
        road_type[0]=2;
        LedOff(1);
        LedOff(3);
        LedOff(2);
        LedOn(1);//��ɫ
        
        if(road_type[6]==1&&road_type[7]==1&&road_type[8]==1&&road_type[9]==1&&road_type[10]==1)
        {//ǰ8֡�ǳ���ֱ��,�ȼ���һ��
            MotorPID.SpeedSet=s_wan1-70;
        }
        else if(road_type[3]==3&&road_type[4]==3&&road_type[5]==3)
        {//ǰ3֡����� ����ŷķ���ڼ���
            MotorPID.SpeedSet=s_wan1;
        }
        else
        { 
            MotorPID.SpeedSet=(int16)(s_zhi-(s_zhi-s_wan1)*(abs(error_ave)/35));//ƫ�����35������ٶ�
            MotorPID.SpeedSet=MotorPID.SpeedSet<s_wan1?s_wan1:MotorPID.SpeedSet; 
        }
    } 
    
    else 
    {
        road_type[0]=3;
        LedOff(1);
        LedOff(3);
        LedOff(2);
        LedOn(1);
        LedOn(3);//����ɫ
        
        if(road_type[9]==1&&road_type[10]==1&&road_type[11]==1&&road_type[12]==1&&road_type[13]==1)
        {//ǰ6֡�ǳ���ֱ��,�ȼ���һ��
            MotorPID.SpeedSet=s_wan1-50;
        }
        else
        {
            g_HighestSpeed=s_wan1;
            g_LowestSpeed=s_wan2;    
            //��̬��������ٶȺ�����ٶȣ���������ֱ����Ǳ��
            MotorPID.SpeedSet=g_HighestSpeed-(g_HighestSpeed-g_LowestSpeed)*(error_ave*error_ave)/(40*40);//ƫ�����40������ٶ�
            MotorPID.SpeedSet=MotorPID.SpeedSet<g_LowestSpeed?g_LowestSpeed:MotorPID.SpeedSet;
            MotorPID.SpeedSet=MotorPID.SpeedSet>g_HighestSpeed?g_HighestSpeed:MotorPID.SpeedSet;//��Сs_wan2���s_wan1
        }
        
    }
    
    for(i=200;i>=1;i--)
    {
        road_type[i]=road_type[i-1];
    }
}