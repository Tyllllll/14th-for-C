#include "head_file.h"
#include "math.h"
int16 FM_flag=0;
int16 nn=0;
int16 FM_mode=0;
int16 problem_happen=1;
float math_sqrt(unsigned int x);
void LedOn(uint8 num)
{
 switch (num)
  {
  case 1:LPLD_GPIO_Output_b(PTB, 0, 0); break;
  case 2:LPLD_GPIO_Output_b(PTB, 1, 0); break;
  case 3:LPLD_GPIO_Output_b(PTB, 2, 0); break;
 
  }

}

void LedOff(uint8 num)
{
  switch (num)
  {
  case 1:LPLD_GPIO_Output_b(PTB, 0, 1); break;
  case 2:LPLD_GPIO_Output_b(PTB, 1, 1); break;
  case 3:LPLD_GPIO_Output_b(PTB, 2, 1); break;
 
  }
}

float GetBatteryVoltage(void)
{
	return LPLD_ADC_Get(ADC0, AD18)*3.3 * 12.4 / 3 / 256/1.75;
}

int abs(int a)
{ 
  if(a>=0)
  {
    a=a;
  }
  if(a<0)
  {
    a=-a;
  }
 return a;
}

int16 MAX(int16 a,int16 b)
{ 
  int16 t=0;
  if(a>=b)
  {
   t=a;
  }
  if(a<b)
  {
    t=b;
  }
 return t;
}

int16 MIN(int16 a,int16 b)
{ 
  int16 t=0;
  if(a<=b)
  {
   t=a;
  }
  if(a>b)
  {
    t=b;
  }
 return t;
}

float ABS(float a)
{
  if(a>=0)
  {
    a=a;
  }
  if(a<0)
  {
    a=-a;
  }
 return a;
}

/**************************实现函数********************************************
*函数原型:	   FM_Start_di
*功　　能:	   使蜂鸣器响不同次数	
输入参数： 
输出参数： 
备注：     苟且吴写的蜂鸣程序有很大问题
*******************************************************************************/
void FM_Start_di(void)
{//一声
  
  if(nn>0)
  {
    BUZZER_ON;
    nn--;
  }
  else
  {
    BUZZER_OFF;
    FM_mode=0;
  }
  
}
void FM_Start_didi(void)
{//两声
  
  if(nn>4)
  {
    BUZZER_ON;
    nn--;
  }
  else if(nn<=4&&nn>2)
  {
    BUZZER_OFF;
    nn--;
  }
  else if(nn<=2&&nn>0)
  {
    BUZZER_ON;
    nn--;
  }
  else if(nn==0)
  {
    BUZZER_OFF;
    FM_mode=0;
  }
}
void FM_Start_dididi(void)
{//三声
  
  
  if(nn>10)
  {
    BUZZER_ON;
    nn--;
  }
  else if(nn<=10&&nn>7)
  {
    BUZZER_OFF;
    nn--;
  }
  else if(nn<=7&&nn>5)
  {
    BUZZER_ON;
    nn--;
  }
  else if(nn<=5&&nn>2)
  {
    BUZZER_OFF;
    nn--;    
  }
  else if(nn<=2&&nn>0)
  {
    BUZZER_ON;
    nn--;
  }
  else if(nn==0)
  {
    BUZZER_OFF;
    FM_mode=0;
  }
}

/**************************实现函数********************************************
*函数原型:	   float invSqrt(double x)
*功　　能:	   快速计算 Sqrt(x) 	
输入参数： 要计算的值
输出参数： 结果
*******************************************************************************/
float invSqrt(unsigned int x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return 1/y;
}

/**************************实现函数********************************************
*函数原型:	  float xielv(void)
*功　　能:	  求100-30行范围内中线标准差	
输入参数：        无
输出参数：        无
添加时间         2018/7/8
*******************************************************************************/
float xielv(void)
{
  int16 i,Sum1=0;
  float S=0,Av=0,Sum2=0;
  for(i=100;i>30;i--)
  {
    Sum1+=f1.midline[i];
  }
  Av=Sum1/70.0;
  for(i=100;i>30;i--)
  {
    Sum2+=(f1.midline[i]-Av)*(f1.midline[i]-Av);
//    Sum2+=(f1.midline[i]*f1.midline[i]-Av*Av);//类似标准差 效率提升很多
  }
  S=sqrt(Sum2/70.0);
  return S;
}

/**************************实现函数********************************************
*函数原型:	  Datadefault(void)
*功　　能:	  跑前数据初始化	
输入参数：        无
输出参数：        无
添加时间         2018/7/9
*******************************************************************************/


/**************************实现函数********************************************
*函数原型:	  float get_qulv(.....)
*功　　能:	  算三点曲率
输入参数：        三个点的坐标
输出参数：        曲率
*******************************************************************************/

float get_qulv(int16 point_Ax,int16 point_Ay,int16 point_Bx,int16 point_By,int16 point_Cx,int16 point_Cy)
{//x坐标是列,y坐标是行
  float length_AB=0,length_AC=0,length_BC=0,k_qulv=0;
  int S_ABC=0;
  int PF_ab=0,PF_ac=0,PF_bc=0;
  int vector_AB=0,vector_AC=0;//向量AB，向量AC 
  PF_ab=(point_Ax-point_Bx)*(point_Ax-point_Bx)+(point_Ay-point_By)*(point_Ay-point_By);//直线AB的距离平方向
  length_AB=invSqrt(PF_ab);//直线AB的距离
  PF_ac=(point_Ax-point_Cx)*(point_Ax-point_Cx)+(point_Ay-point_Cy)*(point_Ay-point_Cy);//直线AC的距离平方向
  length_AC=invSqrt(PF_ac);//直线AC的距离
  PF_bc=(point_Bx-point_Cx)*(point_Bx-point_Cx)+(point_By-point_Cy)*(point_By-point_Cy);//直线BC的距离平方向
  length_BC=invSqrt(PF_bc);//直线BC的距离
  vector_AB=(point_Bx-point_Ax)*abs(point_Cy-point_Ay);
  vector_AC=(point_Cx-point_Ax)*abs(point_By-point_Ay);
  S_ABC=(int)((abs(vector_AB-vector_AC))/2);
  k_qulv=((float)(4*S_ABC))/((float)(length_AB*length_AC*length_BC));
  return k_qulv;
}

/**************************实现函数********************************************
*函数原型:	  void  least square method(void)
*功　　能:	  最小二乘法获取斜率
输入参数：        无
输出参数：        无
*******************************************************************************/

float  least_square_method(void)
{
  int i=0,sum_UP=0,sum_Dowm=0,averageX =0,averageY=0,sum_x=0,sum_y=0,temp=0;
  float parameterA=0,parameterB=0;
  sum_UP=0;
  sum_Dowm=0;
  sum_x=0;
  sum_y=0;
  temp=0;
  if(f2.toppoint<40)
  {
    for(i=40;i<60;i++)
    {
      if(Image[i][f1.midline[i]]!=0x00)
      {
	temp++;
	sum_UP+=(i*f1.midline[i]);
	sum_Dowm+=(i*i);
	sum_x+=i;
	sum_y+=f1.midline[i];
      }
    }
    averageX=(sum_x/temp);
    averageY=(sum_y/temp);
    parameterA=(float)(sum_UP-temp*averageX*averageY);
    parameterB=(float)(sum_Dowm-temp*averageX*averageX);
    if(parameterB!=0)
    {
      return parameterA/parameterB;
    }
    if(parameterB==0)
    {
      return 0;
    }
  }
  else if(f2.toppoint+21<119)
  {
    for(i=f2.toppoint+1;i<f2.toppoint+21;i++)
    {
      if(Image[i][f1.midline[i]]!=0x00)
      {
	temp++;//统计数目
	sum_UP+=(i*f1.midline[i]);
	sum_Dowm+=(i*i);
	sum_x+=i;
	sum_y+=f1.midline[i];
      }
    }
    averageX=(sum_x/temp);
    averageY=(sum_y/temp);
    parameterA=(float)(sum_UP-temp*averageX*averageY);
    parameterB=(float)(sum_Dowm-temp*averageX*averageX);
    if(parameterB!=0)
    {
      return parameterA/parameterB;
    }
    if(parameterB==0)
    {
      return 0;
    }  
  
  }
  else
  {
    for(i=118;i>98;i--)
    {
      if(Image[i][f1.midline[i]]!=0x00)
      {
	temp++;
	sum_UP+=(i*f1.midline[i]);
	sum_Dowm+=(i*i);
	sum_x+=i;
	sum_y+=f1.midline[i];
      }
    }
    averageX=(sum_x/temp);
    averageY=(sum_y/temp);
    parameterA=(float)(sum_UP-temp*averageX*averageY);
    parameterB=(float)(sum_Dowm-temp*averageX*averageX);
    if(parameterB!=0)
    {
      return parameterA/parameterB;
    }
    if(parameterB==0)
    {
      return 0;
    }   
  
  
  
  }
  return 0;

}

void sendData_to_PC(void)
{
      push(0,(uint16)MotorPID.SpeedSet);//
      push(1,(uint16)MotorPID.Speed_ave);//
      push(2,(uint16)MotorPID.IntSum_all);//
      push(3,(uint16)MotorPID.P_value);//
      push(4,(uint16)MotorPID.OutValue);//
      push(5,(uint16)servo_duty);//
      push(6,(uint16)S_p);//
      push(7,(uint16)S_d);//
//      push(5,(uint16)ramp);//
//      push(6,(uint16)(ramp_pre*100));//
//      push(7,(uint16)Gyroy);//
//      push(8,(uint16)which_cross);//
//      push(9,(uint16)shizhi_order);//
      push(10,(uint16)MotorPID.Speed_test[0]);// 

}







