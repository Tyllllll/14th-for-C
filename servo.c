#include "head_file.h"

PID_InitTypeDef Ser_PID={1,0,5};
int16 foresight1=46;//调试用

//int16 fore_offset=12;//前瞻弯道随动量
int16 fore_min=13;//使用中47
int16 fore_max=28;//使用中63
int16 foresight=27;//使用中61       U弯最大top58

int16 mid_error[3];
int16 error[30]={0};
int16 error_cha[4]={0,0,0,0};
int16 error_d=0;
int16 S_d=0;//微分项总和
int16 S_p=0;//比例项总和
int16 servo_duty=1452;
int16 mid=DEG_MID;
int16 max=DEG_MAX;
int16 min=DEG_MIN;
int16 speed_now=0;


float S_kp=4.1;//实验室参数
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
toppoint与实际顶点差5行*/
void Sevor_control(void)
{/******************动态前瞻******************************/
  
  speed_now=(int16)(0.6*MotorPID.Speed_test[0]+0.2*MotorPID.Speed_test[1]+0.2*MotorPID.Speed_test[2]);//编码器的值会有高频抖动

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
      //    //一次函数拟合 speed为后面值时前瞻为max 为加上除数时前瞻为min 理论上效果不会好 因为前瞻越长时10cm对应行数变化越小
      foresight=(int16)(fore_min+(float)(fore_max-fore_min)*(300-speed_now)*(300-speed_now)/(150*150));
      //二次函数拟合 前瞻集中在fore_min
      
    }
    
//    if(abs(f1.midline[foresight]-80)>10)
//    {//前瞻同时随控制偏差变化
//      foresight=foresight+(int16)(fore_offset*(f1.midline[foresight]-80)*(f1.midline[foresight]-80)/(80*80));
//    }
    
//    foresight=foresight>?65:foresight; 好像没有用 


  mid_error[0]=8*(f1.midline[foresight]-80);
  mid_error[1]=3*(f1.midline[foresight+1]-80);
  mid_error[2]=1*(f1.midline[foresight+2]-80);
  error[0]=(int16)((mid_error[0]+mid_error[1]+mid_error[2])/12);//整数除法和单精度乘法的效率差不多
  
  
  if(error[0]>60)
  {//error限幅 极限60
    error[0]=60;
  }
  if(error[0]<-60)
  {
    error[0]= -60;
  }
  if(error[0]-error[1]>15)
  {//error前后变化限幅限制15以内
    error[0]=error[1]+15;
  }
  if(error[0]-error[1]<-15)
  {
    error[0]=error[1]-15;
  }
  
  error_cha[0]=error[0]-error[1]; 
  if(error_cha[0]-error_cha[1]>13)
  {//error微分的变化限幅 理论ec0最大40 实际示波器返回不超15
    error_cha[0]=error_cha[1]+13;
  }
  if(error_cha[0]-error_cha[1]<-13)
  {
    error_cha[0]=error_cha[1]-13;
  }
//  error_d=(int16)(0.6*error_cha[3]+0.2*error_cha[2]+0.1*error_cha[1]+0.1*error_cha[0]);//4帧D
//  error_d=(int16)(0.8*error_cha[1]+0.2*error_cha[0]);//2帧D
  
  error_cha[3]=error_cha[2];
  error_cha[2]=error_cha[1];
  error_cha[1]=error_cha[0];
  int i;
  for(i=29;i>0;i--)
  {//为什么把error扩到了30 现在忘了
   error[i]=error[i-1];
  }
  Sevor_pid();
}


void Sevor_pid(void)
{

/*******************Normal P***********/  
 if(error[0]>0)
  {//右边分离Kp
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
  
  
  /************P项计算计算****************/
  S_p=(int16)(Ser_PID.Kp*error[0]);
  /************单帧D计算****************/
  S_d=(int16)(Ser_PID.Kd*error_cha[0]);
  /************多帧D计算****************/
  //  S_d=(int16)(Ser_PID.Kd*error_d);
  /************************************/  
  servo_duty=abs(error[0])>20?(short)(-1.1*(S_d+S_p)+mid):(short)(-(S_d+S_p)+mid);      //舵机转向与预期相反 将（S_d+S_p）取反 
  
  
  if(servo_duty>max)
  {//限幅
    servo_duty=max;	
  }
  if(servo_duty<min)
  {
    servo_duty=min;
  }  
  
  sudu_set();//速度设定 改变MotorPID.SpeedSet
}
void Sevor_output(void)
{//PIT周期调用
  if(servo_duty>max)
  {//限幅
    servo_duty=max;	
  }
  if(servo_duty<min)
  {
    servo_duty=min;
  }

  PIT->CHANNEL[3].LDVAL = (servo_duty)*(g_bus_clock / 1000000) - 1;//测试在此 servo_duty_text
  PTD2_O = 1;
  PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//开始计时 
}
void Servo_PIT_Isr(void)
{//拉低电平
  PTD2_O = 0;
  PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
}

