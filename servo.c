#include "head_file.h"

PID_InitTypeDef Ser_PID={1,0,5};
int16 foresight1=46;//������

//int16 fore_offset=12;//ǰհ����涯��
int16 fore_min=13;//ʹ����47
int16 fore_max=28;//ʹ����63
int16 foresight=27;//ʹ����61       U�����top58

int16 mid_error[3];
int16 error[30]={0};
int16 error_cha[4]={0,0,0,0};
int16 error_d=0;
int16 S_d=0;//΢�����ܺ�
int16 S_p=0;//�������ܺ�
int16 servo_duty=1452;
int16 mid=DEG_MID;
int16 max=DEG_MAX;
int16 min=DEG_MIN;
int16 speed_now=0;


float S_kp=4.1;//ʵ���Ҳ���
float S_kp_r=4.0;
float S_kd=11.1;//16.9


int16 servo_duty_text=1515;

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
void Sevor_control(void)
{/******************��̬ǰհ******************************/
  
  speed_now=(int16)(0.6*MotorPID.Speed_test[0]+0.2*MotorPID.Speed_test[1]+0.2*MotorPID.Speed_test[2]);//��������ֵ���и�Ƶ����

    if(speed_now>400)
    {
      foresight=fore_min;
    }
    
    else if(speed_now<250)
    {
      foresight=fore_max;
    }
    else
    {
      //    foresight=(int16)(fore_max+(float)(fore_min-fore_max)*(speed_now-200)/150);
      //    //һ�κ������ speedΪ����ֵʱǰհΪmax Ϊ���ϳ���ʱǰհΪmin ������Ч������� ��ΪǰհԽ��ʱ10cm��Ӧ�����仯ԽС
      foresight=(int16)(fore_min+(float)(fore_max-fore_min)*(300-speed_now)*(300-speed_now)/(150*150));
      //���κ������ ǰհ������fore_min
      
    }
    
//    if(abs(f1.midline[foresight]-80)>10)
//    {//ǰհͬʱ�����ƫ��仯
//      foresight=foresight+(int16)(fore_offset*(f1.midline[foresight]-80)*(f1.midline[foresight]-80)/(80*80));
//    }
    
//    foresight=foresight>?65:foresight; ����û���� 


  mid_error[0]=8*(f1.midline[foresight]-80);
  mid_error[1]=3*(f1.midline[foresight+1]-80);
  mid_error[2]=1*(f1.midline[foresight+2]-80);
  error[0]=(int16)((mid_error[0]+mid_error[1]+mid_error[2])/12);//���������͵����ȳ˷���Ч�ʲ��
  
  
  if(error[0]>60)
  {//error�޷� ����60
    error[0]=60;
  }
  if(error[0]<-60)
  {
    error[0]= -60;
  }
  if(error[0]-error[1]>15)
  {//errorǰ��仯�޷�����15����
    error[0]=error[1]+15;
  }
  if(error[0]-error[1]<-15)
  {
    error[0]=error[1]-15;
  }
  
  error_cha[0]=error[0]-error[1]; 
  if(error_cha[0]-error_cha[1]>13)
  {//error΢�ֵı仯�޷� ����ec0���40 ʵ��ʾ�������ز���15
    error_cha[0]=error_cha[1]+13;
  }
  if(error_cha[0]-error_cha[1]<-13)
  {
    error_cha[0]=error_cha[1]-13;
  }
//  error_d=(int16)(0.6*error_cha[3]+0.2*error_cha[2]+0.1*error_cha[1]+0.1*error_cha[0]);//4֡D
//  error_d=(int16)(0.8*error_cha[1]+0.2*error_cha[0]);//2֡D
  
  error_cha[3]=error_cha[2];
  error_cha[2]=error_cha[1];
  error_cha[1]=error_cha[0];
  int i;
  for(i=29;i>0;i--)
  {//Ϊʲô��error������30 ��������
   error[i]=error[i-1];
  }
  Sevor_pid();
}


void Sevor_pid(void)
{

/*******************Normal P***********/  
 if(error[0]>0)
  {//�ұ߷���Kp
	  if(error[0] < 20 )
	  {
		  Ser_PID.Kp=S_kp_r*error[0]*error[0]/400;
	  }else
	    Ser_PID.Kp=S_kp_r;
  }
  else
  { 
	  if(error[0] >  -20)
	  {
		  Ser_PID.Kp=S_kp*error[0]*error[0]/400;
	  }else
	Ser_PID.Kp=S_kp;
  }

  Ser_PID.Kd=S_kd;
  
  
  /************P��������****************/
  S_p=(int16)(Ser_PID.Kp*error[0]);
  /************��֡D����****************/
  S_d=(int16)(Ser_PID.Kd*error_cha[0]);
  /************��֡D����****************/
  //  S_d=(int16)(Ser_PID.Kd*error_d);
  /************************************/  
  servo_duty=abs(error[0])>20?(short)(-1.1*(S_d+S_p)+mid):(short)(-(S_d+S_p)+mid);      //���ת����Ԥ���෴ ����S_d+S_p��ȡ�� 
  
  
  if(servo_duty>max)
  {//�޷�
    servo_duty=max;	
  }
  if(servo_duty<min)
  {
    servo_duty=min;
  }  
  
  sudu_set();//�ٶ��趨 �ı�MotorPID.SpeedSet
}
void Sevor_output(void)
{//PIT���ڵ���
  if(servo_duty>max)
  {//�޷�
    servo_duty=max;	
  }
  if(servo_duty<min)
  {
    servo_duty=min;
  }

  PIT->CHANNEL[3].LDVAL = (servo_duty)*(g_bus_clock / 1000000) - 1;//�����ڴ� servo_duty_text
  PTD2_O = 1;
  PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//��ʼ��ʱ 
}
void Servo_PIT_Isr(void)
{//���͵�ƽ
  PTD2_O = 0;
  PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
}

