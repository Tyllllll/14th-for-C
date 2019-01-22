#include "common.h"
#include "servo.h"
#include "motor.h"
#include "keyandswitch.h"

#define Max_neg_output  5000
#define Max_output  5000



float M_Kp=130;
float M_Ki=40;
float M_Kd=0;
float All_distance=0;
float jl_distance=0;
float set_dist=0;
int16 motor_flag=0;//启动标志
int16 stop_flag=0;//停车标志
int Qd_Result_L=0;//编码器值
int Qd_Result_R=0;//编码器值
int QD_value_L_test=0;
int QD_value_R_test=0;
//int all_Result=0;
int16 duzhuan_cnt=0;
void Motor_PIT(void)
{//PIT 周期调用

  Qd_Result_L = (int16)(LPLD_FTM_GetCounter(FTM1));// 获取脉冲数
  Qd_Result_R = (int16)(LPLD_FTM_GetCounter(FTM2));// 获取脉冲数
  Qd_Result_L = (Qd_Result_L>=0xF0)? 0 : Qd_Result_L;  // 刚转时会有FF的错误值
//  all_Result=all_Result+Qd_Result_L;//编码器返回测试
  /*********计算速度和距离***********/
  MotorPID.Speed_test_L[0]=(Qd_Result_L==0)?0:((int)(10000*(((float)(Qd_Result_L))/((float)(QD_value_L)))));//单位cm/s
  MotorPID.Speed_test_R[0]=(Qd_Result_R==0)?0:((int)(10000*(((float)(Qd_Result_R))/((float)(QD_value_R)))));//单位cm/s
  MotorPID.Speed_test[0]=(int)(0.5*MotorPID.Speed_test_L[0]+0.5*MotorPID.Speed_test_R[0]);
  
  MotorPID.Distance=((0.5*((float)Qd_Result_L)/((float)QD_value_L))+(0.5*((float)Qd_Result_R)/((float)QD_value_R)));//10ms前进的距离 单位m
  All_distance=All_distance+MotorPID.Distance;
////测量1m距离对应编码器脉冲数
//  QD_value_L_test += Qd_Result_L;
//  QD_value_R_test += Qd_Result_R;
//  if (key1 == 0)
//  {
//    key_delay();
//    if (key1 == 0)
//    {LPLD_UART_PutChar(UART0,QD_value_L_test );
//    LPLD_UART_PutChar(UART0,QD_value_R_test );
//    QD_value_L_test = 0;
//    QD_value_R_test = 0;
//    }
//  }
//  //向PC传编码器测得速度
//    LPLD_UART_PutChar(UART0,Qd_Result_L );
//    LPLD_UART_PutChar(UART0,Qd_Result_R );
//    LPLD_UART_PutChar(UART0,'/n');
  
  

  /******************************电机控制**********************************************/
  Motor_control();//电机控制
  MotorPID.Speed_test[4]=MotorPID.Speed_test[3];
  MotorPID.Speed_test[3]=MotorPID.Speed_test[2];
  MotorPID.Speed_test[2]=MotorPID.Speed_test[1];
  MotorPID.Speed_test[1]=MotorPID.Speed_test[0];
  
  LPLD_FTM_ClearCounter(FTM2);//清空计数器
  LPLD_FTM_ClearCounter(FTM1);//清空计数器

}


void Motor_control(void)
{
   Motor_pid();//赋予当前MotorPID.OutValue
  if(motor_flag>=1)
  {
    if(MotorPID.OutValue_L>Max_output)
    {//左边
      MotorPID.OutValue_L=Max_output;
    }
    if(MotorPID.OutValue_L<-Max_output)
    {
      MotorPID.OutValue_L=-Max_output;
    }
    
    if(MotorPID.OutValue_R>Max_output)
    {//右边
      MotorPID.OutValue_R=Max_output;
    }
    if(MotorPID.OutValue_R<-Max_output)
    {
      MotorPID.OutValue_R=-Max_output;
    }
    
    
    if(MotorPID.OutValue_L<0)
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);//可电机预热
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, -MotorPID.OutValue_L); 
    }
    if(MotorPID.OutValue_R<0)
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);//可电机预热
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, -MotorPID.OutValue_R);
    }
    if(MotorPID.OutValue_L>=0)
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, MotorPID.OutValue_L);
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0); 
    }
    if(MotorPID.OutValue_R>=0)
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, MotorPID.OutValue_R);
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);    
    }
    
    if(motor_flag>1)
    {
      motor_flag=motor_flag-1;
    }
    
    if(motor_flag==1&&MotorPID.OutValue>=4000)
    {//堵转关电机时间为1秒
      if(MotorPID.Speed_test[0]<=10&&MotorPID.Speed_test[1]<=10&&MotorPID.Speed_test[2]<=10)
      {//堵转
          duzhuan_cnt++;
          if(duzhuan_cnt>100)
          {//1秒
            motor_flag=-1;//准备关闭电机
          }
      }
    }
    else 
    {
      duzhuan_cnt=0;
    }
    
    if(motor_flag==1&&stop_flag>=1)
    {//stop_flag*10ms时间后关电机
      if(stop_flag>1)
      {
        stop_flag=stop_flag-1;
      }
      if(stop_flag==1)
      {//认为停车成功
        motor_flag=0;
        stop_flag=0;
        All_distance=0;
      }
    }
  }
  if(motor_flag<=0)
  { 
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);  
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0); 
    if(motor_flag==-1)
    {
      motor_flag=0;
    }
  }
  
}


MotorPID_TypeDef MotorPID={0,0,0,1,1,0,0,0,0,0,0,0,0,0};//Kp Ki Kd


void Motor_pid(void)
{
  float h=0.018;
  MotorPID.Kp=M_Kp;
  MotorPID.Ki=M_Ki;
  MotorPID.Kd=M_Kd; 
  
  if(stop_flag>=1)
  {
    MotorPID.SpeedSet=0;
  }
  
//  MotorPID.Speed_ave=(int)(0.6*MotorPID.Speed_test[0]+
//                           0.2*MotorPID.Speed_test[1]+
//                           0.2*MotorPID.Speed_test[2]);//对速度进行滤波
  MotorPID.Speed_ave=(int)(0.5*MotorPID.Speed_test[0]+
                           0.2*MotorPID.Speed_test[1]+
                           0.1*MotorPID.Speed_test[2]+
                           0.1*MotorPID.Speed_test[3]+
                           0.1*MotorPID.Speed_test[4]);//对速度进行滤波

  
  MotorPID.Speed_Error=MotorPID.SpeedSet-MotorPID.Speed_ave;//目标速度减去实际速度获取速度偏差
 
  
  MotorPID.IntSum_now=(int)(MotorPID.Ki*MotorPID.Speed_Error);//无变速I项
  
  /*************************/
  /*积分抗饱和 可用积分项限幅代替*/
  if(MotorPID.OutValue>5000&&MotorPID.IntSum_now<0)
  {
    MotorPID.IntSum_all+=MotorPID.IntSum_now;
  }
  else if(MotorPID.OutValue<-3000&&MotorPID.IntSum_now>0)
  {
    MotorPID.IntSum_all+=MotorPID.IntSum_now;
  } 
  else if(MotorPID.OutValue>=-3000&&MotorPID.OutValue<=5000)
  {
    MotorPID.IntSum_all+=MotorPID.IntSum_now;
  }

  /**********************/
  /*减速积分退饱和*/ 
  //为了在KI项大的情况下减速
  if(MotorPID.Speed_Error<-20)
  {
    if(MotorPID.IntSum_all>3000)
    {//看示波器设值
      if(MotorPID.Speed_Error<-100)
      {
        MotorPID.IntSum_all*=0.2;
      }
      else if(MotorPID.Speed_Error<-80)
      {
        MotorPID.IntSum_all*=0.4;
      }
      else if(MotorPID.Speed_Error<-60)
      {
        MotorPID.IntSum_all*=0.6;
      }
      else if(MotorPID.Speed_Error<-40)
      {
        MotorPID.IntSum_all*=0.8;
      }
      else if(MotorPID.Speed_Error<-20)
      {
        MotorPID.IntSum_all*=0.9;
      }
    }
  }
  /**********************/
  MotorPID.IntSum_all=MotorPID.IntSum_all>5000?5000:MotorPID.IntSum_all;//积分项限幅
  MotorPID.IntSum_all=MotorPID.IntSum_all<-3000?-3000:MotorPID.IntSum_all;

  
  MotorPID.P_value=MotorPID.Kp*MotorPID.Speed_Error;//计算kp项
  
  MotorPID.OutValue=(int)(MotorPID.IntSum_all+MotorPID.P_value);
  
  /****+*****差速控制方案*****/
  if(error[0]>-20 && error[0]<20)
      h = 0;
  if(error[0]>=0)
  {
      MotorPID.OutValue_L=(int)(((h * error[0])+1) * MotorPID.OutValue);
      MotorPID.OutValue_R=MotorPID.OutValue;    
  }
  else if(error[0]<0)
  {
     MotorPID.OutValue_L=MotorPID.OutValue;
     MotorPID.OutValue_R=(int)(((h * (-error[0]))+1) * MotorPID.OutValue);  
  }
  
  
  MotorPID.OutValue_L=MotorPID.OutValue_L>Max_output?Max_output:MotorPID.OutValue_L;
  MotorPID.OutValue_L=MotorPID.OutValue_L<-Max_neg_output?-Max_neg_output:MotorPID.OutValue_L;//输出限幅
  
  MotorPID.OutValue_R=MotorPID.OutValue_R>Max_output?Max_output:MotorPID.OutValue_R;
  MotorPID.OutValue_R=MotorPID.OutValue_R<-Max_neg_output?-Max_neg_output:MotorPID.OutValue_R;//输出限幅
}


