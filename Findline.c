
/*10cm-i=95 top=100
20cm-i=82 top=87	100
30cm-i=70 top=75	76
40cm-i=61 top=66	60
50cm-i=54 top=59	45
60cm-i=49 top=54	37
70cm-i=45 top=50	34
80cm-i=42 top=47	26
90cm-i=39 top=44	22
100cm-i=37 top=42	21
110cm-i=35 top=40
120cm-i=33 top=38
130cm-i=32 top=37
140cm-i=31 top=36
170cm-i=27 top=32
toppoint与实际顶点差5行*/
//自测的顶点行与实际距离的表格，具体要根据自己的车进行测量
#include "head_file.h"
/*******十字标志，定义******/
int16 shizi_time=0;//十字计时器
int16 shizi;  //十字计数器 
int16 shizidistance; //十字距离计数
int16 L=0;//左弯时补线后中线消失的行数
int16 R=0;//右弯时补线后中线消失的行数
int16 both_line=0;//十字拐点消失时找到前面不丢线的行
int16 xie_flag=0;
float k_left[3]={0,0,0};//十字补线斜率记录
float k_right[3]={0,0,0};
float shizi_distance=0;
int16 start_cnt_distance=0;
int16 shizi_cnt=0;
/*******其余标志，定义******/
int16 js1=0;
int16 js2=0;
int16 js3=0;
int16 js4=0;
int16 js5=0;
int16 js6=0;
int16 js7=0;
int16 js8=0;
int16 js9=0;
int16 js10=0;
int16 set_startline=25;
Findline1_TypeDef f1;
Findline2_TypeDef f2;
int16 leftlineflag_reg[120];
int16 rightlineflag_reg[120];
int16 column_start=80;//基础寻线的每行的起始搜索列
int16 text_width[120]={0};


int16 width_real[120] = {14, 17, 19, 20, 22, 23, 24, 24, 25, 25,//0
						27, 28, 29, 30, 31, 32, 33, 35, 36, 36,//1
						37, 39, 40, 41, 42, 43, 44, 45, 46, 47,//2
						48, 49, 50, 51, 52, 53, 53, 54, 55, 56,//3
						57, 58, 59, 60, 61, 62, 63, 64, 65, 65,//4
						66, 67, 68, 69, 70, 71, 72, 72, 73, 74,//5
						75, 76, 76, 76, 77, 77, 77, 78, 78, 79,//6
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//7
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//8
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//9
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//10
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79//11
};

int16  width[120]={1,1,2,2,2,3,3,4,4,4,  //赛道全宽 ，具体要根据自己的车进行测量
5,5,6,7,7,8,9,10,10,10,
11,11,13,15,15,17,17,19,20,21,                 //38-28
23,23,25,27,27,29,30,31,32,33,                //48-38
35,36,37,38,38,39,42,42,44,44,               //58-48
46,47,49,49,51,52,53,54,55,56,              //68-58
57,57,59,60,61,63,63,64,65,67,             //68-58
68,69,70,71,72,74,75,76,77,78,             //78-68
79,80,82,83,84,85,86,87,89,89,            //88-78
91,93,93,95,95,97,97,99,100,101,         //98-88
102,103,104,106,106,108,108,110,111,112,//108-98
113,114,115,116,118,119,120,121,122,122};//118-108

int16 width_test[120]={0};

int16 state3to5column = 68;
int16 loselinecount = 40;
int16 xielvcha = 25;
int16 dianshu = 8;
void findline1(void)  //基础寻线
{
    int16 i=0,j=0,k=0;
    f2.toppoint=0;
    f2.toppoint_flag=0; 

    
    
    for(i=118;i>5;i--)//从118行一直找到10行  
    {
        k=column_start;   //80列
        f1.leftline[i]=0;
        f1.leftlineflag[i]=0;
        f1.midline[i]=0;
        f1.rightline[i]=0;
        f1.rightlineflag[i]=0;
        for(j=k;j>2;j--)   //Image[i][j]==0为黑，Image[i][j]!=0为白
        {
            if(Image[i][j]!=0&&Image[i][j-1]!=0&&Image[i][j-2]==0)
            {//寻找左跳变点
                f1.leftline[i]=j-1;
                f1.leftlineflag[i]=1;
                break;
            }
        }
        for(j=k;j<157;j++)
        {
			if(Image[i][j]!=0&&Image[i][j+1]!=0&&Image[i][j+2]==0)
			{//寻找右跳变点
				f1.rightline[i]=j+1;
				f1.rightlineflag[i]=1;
				break;
			}
        }
//        
//        if(i>10&&i<116) //滤除噪点
//        {
//          if(f1.leftlineflag[i]!=leftlineflag_reg[i+1]&&f1.leftlineflag[i]!=leftlineflag_reg[i+2])
//          {
//            f1.leftlineflag[i] = leftlineflag_reg[i+1];
//          }
//          if(f1.rightlineflag[i]!=rightlineflag_reg[i+1]&&f1.rightlineflag[i]!=rightlineflag_reg[i+2])
//          {
//            f1.rightlineflag[i] = rightlineflag_reg[i+1];
//          }
//        }        
		if(f2.huandaoflag == 7)
		{
			if(i==118)
			{
				f1.midline[i]=f1.leftlineflag[i]? f1.leftline[i] + width_real[i]: 10;
			}
			else
			{
				f1.midline[i]=f1.leftlineflag[i]? f1.leftline[i] + width_real[i]: f1.midline[i+1];
			}
		}
		else if(f2.huandaoflag == 8)
		{
			if(i==118)
			{
				f1.midline[i]=f1.rightlineflag[i]? f1.rightline[i] - width_real[i]: 150;
			}
			else
			{
				f1.midline[i]=f1.rightlineflag[i]? f1.rightline[i] - width_real[i]: f1.midline[i+1];
			}
		}
		else
		{
			if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==1)
			{//两边都找到跳变点
				f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
	//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
	//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];  
			}
			else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==1)
			{//左边无跳变点，右边有跳变点
				f1.leftline[i]=0;
				f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
	//            if(i<95&&f1.rightline[i]<=f1.rightline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)//直线上不可能出现该情况，仅存在于转弯处
	//            {
	//                f1.midline[i]=f1.midline[i+1];
	//            }  
				f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
				f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
			}
			else if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==0)
			{//左边有跳变点，右边无跳变点
				f1.rightline[i]=159;
				f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
	//            if(i<99&&f1.leftline[i]>=f1.leftline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)  //弯道右转
	//            {
	//                f1.midline[i]=f1.midline[i+1];
	//            }
				f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
				f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
			}
			else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==0)//&&f2.shiziflag[1]==0)
			{//左右无跳变点
				f1.leftline[i]=0;
				f1.rightline[i]=159;
				if(i==118)
				{
					f1.midline[i]=80;
				}
				else
				{
					f1.midline[i]=f1.midline[i+1];//用上一场的
				}
				f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
				f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
			}
		}
        column_start=f1.midline[i];
    }  
//    for(i=118;i>10;i--)
//    {
//      leftlineflag_reg[i] = f1.leftlineflag[i];
//      rightlineflag_reg[i] = f1.rightlineflag[i];
//    }=
//    for(i=118;i>10;i--)//从118行一直找到10行
//    {
//        f1.leftline[i]=0;
//        f1.leftlineflag[i]=0;
//        f1.midline[i]=0;
//        f1.rightline[i]=0;
//        f1.rightlineflag[i]=0;
//        nolineflag=1;
//        for(j=2;j<=159;j++)   //Image[i][j]==0为黑，Image[i][j]!=0为白
//        {
//            if(Image[i][j]!=0&&Image[i][j-1]!=0&&Image[i][j-2]==0)
//            {//寻找左跳变点
//                f1.leftline[i]=j-1;
//                f1.leftlineflag[i]=1;
//                nolineflag=0;
//                break;
//            }
//        }
//        f1.leftline[i] = f1.leftlineflag[i]==1?f1.leftline[i]:0;
//        if(nolineflag == 1)  //i行没有左边线
//        {
//          for(j=157;j>0;j--)
//          {
//              if(Image[i][j]!=0&&Image[i][j+1]!=0&&Image[i][j+2]==0)
//              {//寻找右跳变点
//                  f1.rightline[i]=j+1;
//                  f1.rightlineflag[i]=1;
//                  break;
//              }
//          }
//        }
//        else   //i行有左边线
//        {
//          for(j=157;)
//          f1.rightline[i]=159;
//          f1.rightlineflag[i]=0;
//        }
//    }
//    
//    for(i=118;i>10;i--)    //去除边线噪点
//    {
//      if((f1.leftlineflag[i]!=f1.leftlineflag[i-1])&&(f1.leftlineflag[i]!=f1.leftlineflag[i+1])) 
//      {
//        leftlineflag_reg[i] = f1.leftlineflag[i-1];
//      }
//      else
//      {
//        leftlineflag_reg[i] = f1.leftlineflag[i];
//      }
//      if((f1.rightlineflag[i]!=f1.rightlineflag[i-1])&&(f1.rightlineflag[i]!=f1.rightlineflag[i+1])) 
//      {
//        rightlineflag_reg[i] = f1.rightlineflag[i-1];
//      }
//      else
//      {
//        rightlineflag_reg[i] = f1.rightlineflag[i];
//      }
//
//    }
//    for(i=118;i>10;i--)
//    { 
//      f1.leftlineflag[i]  = leftlineflag_reg[i];
//      f1.rightlineflag[i] = rightlineflag_reg[i];
//    }
//    for(i=118;i>10;i--)
//    {
//      if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==1)
//        {//两边都找到跳变点
//            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//            
//        }
//        else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==1)
//        {//左边无跳变点，右边有跳变点
//            f1.leftline[i]=0;
//            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            if(i<99&&f1.rightline[i]<=f1.rightline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)//直线上不可能出现该情况，仅存在于转弯处
//            {
//                f1.midline[i]=f1.midline[i+1];
//            }  
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//        }
//        else if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==0)
//        {//左边有跳变点，右边无跳变点
//            f1.rightline[i]=159;
//            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            if(i<99&&f1.leftline[i]>=f1.leftline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)  //弯道右转
//            {
//                f1.midline[i]=f1.midline[i+1];
//            }
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//        }
//        else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==0)//&&f2.shiziflag[1]==0)
//        {//左右无跳变点
//            f1.leftline[i]=0;
//            f1.rightline[i]=159;
//            if(i==118)
//            {
//                f1.midline[i]=80;
//            }
//            else
//            {
//                f1.midline[i]=f1.midline[i+1];//用上一场的
//            }
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//    }
//    }
    
    
    
//    column_start=f1.midline[118];//每场之间的起始点设定
    //计算赛道半宽
    for (i=118;i>1;i-- )
    {width_test[i] = ( - f1.leftline[i] + f1.rightline[i]) / 2;
    
    }
    if(f2.toppoint_flag==0)
    {
        find_toppoint();//寻找一场图像的赛道顶点
    }
}

void findline2(void)
{
  int16 i=0; 
  f2.leftturnflag=0;f2.leftturn_row=0;
  f2.rightturnflag=0;f2.rightturn_row=0;
  f2.leftguaiflag=0;f2.leftguai_column=0;f2.leftguai_row=0;
  f2.rightguaiflag=0;f2.rightguai_column=0;f2.rightguai_row=0;
  f2.leftguaiflag_up=0;f2.leftguai_column_up=0;f2.leftguai_row_up=0;
  f2.rightguaiflag_up=0;f2.rightguai_column_up=0;f2.rightguai_row_up=0;
  L=0;R=0;both_line=0;
  
  uint8 Tyl_huan12_noguai = 0;
  uint8 Tyl_huan12_top = 0;
  static uint8 Tyl_huan34_state;
  uint8 Tyl_huan34_noguai = 0;
  uint8 Tyl_huan34_top = 0;

  if(f2.leftguaiflag==0&&f2.rightguaiflag==0)
  {//寻找赛道上是否存在拐点
    find_guai(); 
  }

  if(f2.toppoint!=0)
  {//有顶点时找小弯
    find_curve1();  
  } 

  if(f2.toppoint!=0&&f2.toppoint<95&&f2.leftturnflag==0&&f2.rightturnflag==0)
  {//判断是否深弯
    find_curve3();
  }
  

  
  /********************************************十字判断*******************************************************************/
  if(!f2.huandaoflag)
  {
	  if(f2.leftguaiflag==1||f2.rightguaiflag==1)
	  {//拐点存在 
		  shizi_judge();
		  if(f2.leftguaiflag==1&&f2.rightguaiflag==1)
		  {
			  
			  if(f2.leftguai_row>f2.rightguai_row&&f2.leftguai_column>f2.rightguai_column)  
			  {//左拐点在右拐点右边时 保留近处拐点
				  f2.rightguaiflag=0;
			  }
			  if(f2.rightguai_row>f2.leftguai_row&&f2.leftguai_column>f2.rightguai_column)
			  {
				  f2.leftguaiflag=0;
			  }  
			  
		  }   
		  
		  
		  bianxian();//检查左右拐点的两边是否存在丢线  
		  if(f2.leftguaiflag==1||f2.rightguaiflag==1)
		  {
			  
			  if(f2.shiziflag[1]==0)
			  {
				  shizi_distance=0;
				  f2.shiziflag[0]=f2.shiziflag[0]+1;
			  }
			  if(f2.shiziflag[0]==3) //满足4帧图像，确认为十字
			  {
				  f2.shiziflag[1]=1;
				  f2.shiziflag[0]=0;
			  }
			  else
			  {
				  f2.shiziflag[1]=0;
			  }
			  
			  
			  if(f2.leftturnflag!=0||f2.rightturnflag!=0) //拐点存在，销去转弯标志位
			  {
				  f2.leftturnflag=0;
				  f2.rightturnflag=0;
			  }
		  }
		  
		  if(f2.leftguaiflag==0) 
		  {
			  f2.leftguaiflag=0;
			  f2.leftguai_row=0;
			  f2.leftguai_column=0;    
		  }
		  if(f2.rightguaiflag==0)
		  {
			  f2.rightguaiflag=0;
			  f2.rightguai_row=0;
			  f2.rightguai_column=0;     
		  }
	  }
  }

  /**********************************十字补线***********************************************/
 if(f2.shiziflag[1]==1)
 {
      if(f2.leftguaiflag==1&&f2.rightguaiflag==1)
      {
          if(f2.leftguai_row+8<118)
          {
              k_left[0]=((float)(f2.leftguai_column-f1.leftline[f2.leftguai_row+8])/8.0);
              f2.zhizi_k_L=(float)(0.6*k_left[0]+0.2*k_left[1]+0.2*k_left[2]);  //加权左线斜率
          }
          if(f2.leftguai_row+8>=118)
          {
              k_left[0]=((float)(f2.leftguai_column-f1.leftline[118])/(float)(118-f2.leftguai_row));
              
              f2.zhizi_k_L=(float)(0.6*k_left[0]+0.2*k_left[1]+0.2*k_left[2]); //加权左线斜率
              
          }
          
          if(f2.rightguai_row+8<118)
          {
              k_right[0]=((float)(f2.rightguai_column-f1.rightline[f2.rightguai_row+8])/8.0);
              f2.zhizi_k_R=(float)(0.6*k_right[0]+0.2*k_right[1]+0.2*k_right[2]);//加权右线斜率      
          }
          if(f2.rightguai_row+8>=118)
          {
              k_right[0]=((float)(f2.rightguai_column-f1.rightline[118])/(float)(118-f2.rightguai_row));
              
              f2.zhizi_k_R=(float)(0.6*k_right[0]+0.2*k_right[1]+0.2*k_right[2]);//加权右线斜率 
          }  
          
          for(i=f2.rightguai_row;i>=10;i--)   
          {        //计算右边补线点
              f1.rightline[i]=(int16)(f2.rightguai_column+f2.zhizi_k_R*(f2.rightguai_row-i));
              if(f1.rightline[i]>159)
              {
                  f1.rightline[i]=159;
              }
              if(f1.rightline[i]<0)
              {
                  f1.rightline[i]=0;
              }
          }
          
          for(i=f2.leftguai_row;i>=10;i--)
          { //计算左边补线点
              f1.leftline[i]=(int16)(f2.leftguai_column+f2.zhizi_k_L*(f2.leftguai_row-i));
              if(f1.leftline[i]>159)
              {
                  f1.leftline[i]=159;
              }
              if(f1.leftline[i]<0)
              {
                  f1.leftline[i]=0;
              }   
          }
          for(i=MAX(f2.leftguai_row,f2.rightguai_row);i>10;i--)
          {
              f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
              
              if(f1.midline[i]>159)
              {
                  f1.midline[i]=159;
              }
              if(f1.midline[i]<0)
              {
                  f1.midline[i]=0;
              }     
              
          }
          
          
          k_left[2]=k_left[1];
          k_left[1]=k_left[0];
          
          k_right[2]=k_right[1];   
          k_right[1]=k_right[0];
          
      }
      
      
      if(f2.leftguaiflag==0&&f2.rightguaiflag==1)
      {
          if(f2.rightguai_row+8<118)
          {
              k_right[0]=((float)(f2.rightguai_column-f1.rightline[f2.rightguai_row+8])/8.0);
              f2.zhizi_k_R=(float)(0.6*k_right[0]+0.2*k_right[1]+0.2*k_right[2]);      
          }
          if(f2.rightguai_row+8>=118)
          {
              k_right[0]=((float)(f2.rightguai_column-f1.rightline[118])/(float)(118-f2.rightguai_row));
              f2.zhizi_k_R=(float)(0.6*k_right[0]+0.2*k_right[1]+0.2*k_right[2]);
              
          }      
          for(i=f2.rightguai_row;i>=10;i--)
          {
              f1.rightline[i]=(int16)(f2.rightguai_column+f2.zhizi_k_R*(f2.rightguai_row-i));
              if(f1.rightline[i]>159)
              {
                  f1.rightline[i]=159;
              }
              if(f1.rightline[i]<0)
              {
                  f1.rightline[i]=0;
              }
          }
          
          
          
          for(i=118;i>10;i--)    
          {
              f1.midline[i]=f1.rightline[i]-(int16)(width_real[i])-10;  //右边线补半宽
              
              
              if(f1.midline[i]>159)
              {
                  f1.midline[i]=159;
              }
              if(f1.midline[i]<0)
              {
                  f1.midline[i]=0;
              }     
          } 
          
          
          k_right[2]=k_right[1];   
          k_right[1]=k_right[0];
      }
      
      if(f2.leftguaiflag==1&&f2.rightguaiflag==0)
      {
          if(f2.leftguai_row+8<118)
          {
              k_left[0]=((float)(f2.leftguai_column-f1.leftline[f2.leftguai_row+8])/8.0);
              f2.zhizi_k_L=(float)(0.6*k_left[0]+0.2*k_left[1]+0.2*k_left[2]);
          }
          if(f2.leftguai_row+8>=118)
          {
              k_left[0]=((float)(f2.leftguai_column-f1.leftline[118])/(float)(118-f2.leftguai_row));
              f2.zhizi_k_L=(float)(0.6*k_left[0]+0.2*k_left[1]+0.2*k_left[2]);
          }    
          for(i=f2.leftguai_row;i>=10;i--)
          { 
              f1.leftline[i]=(int16)(f2.leftguai_column+f2.zhizi_k_L*(f2.leftguai_row-i));
              if(f1.leftline[i]>159)
              {
                  f1.leftline[i]=159;
              }
              if(f1.leftline[i]<0)
              {
                  f1.leftline[i]=0;
              }   
          }
          
          for(i=118;i>10;i--)    
          {
              f1.midline[i]=f1.leftline[i]+(int16)(width_real[i])+10;  //左边线补半宽
              
              
              if(f1.midline[i]>159)
              {
                  f1.midline[i]=159;
              }
              if(f1.midline[i]<0)
              {
                  f1.midline[i]=0;
              }     
          } 
          
          
          
          k_left[2]=k_left[1];
          k_left[1]=k_left[0];
          
      }
      find_toppoint();//补完十字后重新定义一个新的顶点
      
  }
  
  
  /******************十字补线结束******************/
  
//  if(f2.huandaoflag == 0)
//  {
//	  if(abs(xielv()) < 10 && !f2.leftguaiflag && !f2.rightguaiflag && !judge_bothlose())
//	  {
//		  if(f1.leftlineflag[6] == 1 || f1.leftlineflag[7] == 1 || f1.rightlineflag[6] == 1 || f1.rightlineflag[7] == 1)
//		  {
//	  		Tyl_judge_huandao();
//		  }
//	  }
//  }
	Magnetic_GetAdc();
  if(magnetic.left_mag > 58 || magnetic.right_mag > 58)
  {
	  FM_Start_di();
	Tyl_judge_huandao();
  }
  /*if(f2.huandaoflag == 1)
  {
	  Magnetic_GetAdc();
	  if(magnetic.left_mag < 900)
	  {
		  Magnetic_GetAdc();
		  if(magnetic.left_mag < 900)
		  {
			f2.huandaoflag = 0;
		  }
	  }

  }
  else if(f2.huandaoflag == 2)
  {
	  Magnetic_GetAdc();
	  if(magnetic.right_mag < 900)
	  {
		  Magnetic_GetAdc();
		  if(magnetic.right_mag < 900)
		  {
			f2.huandaoflag = 0;
		  }
	  }
  }
  else if(f2.huandaoflag == 5 || f2.huandaoflag == 6|| f2.huandaoflag == 7|| f2.huandaoflag == 8)
  {
	  if(f2.toppoint < 13)
	  {
		  f2.huandaoflag = 0;
	  }
  }
  else
  {
	  LedOn(1);
	  LedOn(2);
	  LedOn(3);
  }*/

  /***********************************/
    //  弯道补线
  /************************************/
  
 if(f2.leftturnflag!=0||f2.rightturnflag!=0)
 {
   if(f2.leftturnflag==1)
   {
     for(i=f2.leftturn_row;i>f2.toppoint;i--)
     {
       if(f1.rightline[i]==159)
       {
	 f1.midline[i]=f1.midline[i+1]-1;  //丢线部分中线逐渐往左偏
       }
       else
       {
	 f1.midline[i]=f1.midline[i+1]-(f1.rightline[i+1]-f1.rightline[i]);    //丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
       }
       if(f1.midline[i]<=0)
       {
	 f1.midline[i]=0;
	 L=i;   //左弯时补线后中线消失的行数
	 break;
       }
     }
     L=L==0?f2.toppoint:L;
     for(i=L;i>0;i--)
     {
       f1.leftline[i]=0;
       f1.rightline[i]=0;
       f1.midline[i]=0;
       
     }
     
   }
   
   if(f2.leftturnflag==3)
   {
     for(i=118;i>0;i--)
     {
       f1.midline[i]=f1.rightline[i]-80;      //直接以右边界减去80，将中线轨迹向左偏离幅度更大
       f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
       f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];//限幅
       if(f1.midline[i]<=0)
       {
	 f1.midline[i]=0;
	 L=i;    //左弯时补线后中线消失的行数
	 break;
       }
     }
     L=L==0?f2.toppoint:L;
     for(i=L;i>0;i--)
     {
       f1.leftline[i]=0;
       f1.rightline[i]=0;
       f1.midline[i]=0;     
     }
   }
  

  
   if(f2.rightturnflag==1)
   {
       for(i=f2.rightturn_row;i>0;i--)
       {
           if(f1.leftline[i]==0)
           {
               f1.midline[i]=f1.midline[i]+1; //丢线部分，中线往右边偏
           }
           else
           {
               f1.midline[i]=f1.midline[i+1]+(f1.leftline[i]-f1.leftline[i+1]);        //丢线部分中线逐渐往右偏，且偏离的弧度跟左边界向右偏离程度相同
           }
           
           if(f1.midline[i]>=159)
           {
               f1.midline[i]=159;
               R=i;
               break;
           }   
           
       }
       R=R==0?f2.toppoint:R;
       for(i=R;i>0;i--)
       {
           f1.leftline[i]=159;
           f1.rightline[i]=159;
           f1.midline[i]=159;
           
       }
   }
  
  if(f2.rightturnflag==3)
  {
    for(i=118;i>0;i--)
    {
      f1.midline[i]=f1.leftline[i]+80;//直接以左边界加80，将中线轨迹向右偏离幅度更大
      if(f1.midline[i]>=159)
      {
	f1.midline[i]=159;
	R=i;
	break;
      }
    }
    R=R==0?f2.toppoint:R;
    for(i=R;i>0;i--)
    {
      f1.leftline[i]=159;
      f1.rightline[i]=159;
      f1.midline[i]=159;   
    }

  }
  
 }
 
		  
/***************************************************************
	*	@brief	环岛状态机
	*	@param	无
	*	@note	无
***************************************************************/
	//左环
	if(f2.huandaoflag == 1)
	{
		Tyl_huan12_noguai = 0;
		Tyl_huan12_top = 0;
		//进环前
		for(i=118;i>10;i--)//从118行一直找到10行  
		{
			f1.midline[i] = f1.rightline[i] - width_real[i];
		}
		for(i = 118; i > 10; i--)
		{
			if(f1.leftline[i] - f1.leftline[i + 3] > 20 && f1.leftline[i + 1] - f1.leftline[i + 4] > 20 && f1.leftline[i + 2] - f1.leftline[i + 5] > 20)
			{
				Tyl_huan12_top = i;
				break;
			}
		}
		for(i=118;i>Tyl_huan12_top;i--)
		{
			if(f1.leftlineflag[i])
			{
				Tyl_huan12_noguai++;
			}
		}
		if(Tyl_huan12_noguai < 8)
		{
			f2.huandaoflag = 3;
		}
	}
	else if(f2.huandaoflag == 3)
	{
		//进环
		Tyl_huan34_noguai = 0;
		Tyl_huan34_top = 0;
		if(!Tyl_huan34_state)
		{
			for(i = 40; i < 50; i++)
			{
				if(f1.rightline[i] != 159)
				{
					Tyl_huan34_noguai++;
				}
			}
			if(Tyl_huan34_noguai < 3)
			{
				Tyl_huan34_state = 1;
			}
		}
		else
		{
			for(i = 40; i < 50; i++)
			{
				if(f1.rightline[i] == 159)
				{
					Tyl_huan34_noguai++;
				}
			}
			if(Tyl_huan34_noguai < 3)
			{
				Tyl_huan34_state = 0;
				f2.huandaoflag = 5;
			}
		}
		for(i = 118; i > 5; i--)
		{
			if(f1.leftline[i] - f1.leftline[i + 3] > 20)
			{
				Tyl_huan34_top = i;
				break;
			}
		}
		for(i = 118; i > 10; i--)
		{
			if(Tyl_huan34_top < i)
			{
//				f1.midline[i] = (int16)(f1.leftline[i] + width_real[i] * (i -  Tyl_huan34_top)/(118 - Tyl_huan34_top));
				f1.midline[i] = f1.leftline[i] + width_real[i] - 10;
			}
			else
			{
				f1.midline[i] = 0;
			}
		}
	}
	else if(f2.huandaoflag == 5)
	{
		//环内
		for(i=108;i>f2.toppoint+4;i--)   //拐点要在顶点之前
		{
			if(i>45&&f1.rightline[i]>20&&f1.rightline[i]<f1.rightline[i+3]&&f1.rightline[i]<f1.rightline[i-3]&&f1.rightline[i]<f1.rightline[i+5])
			{//i>45是为了控制拐点在40行到115行内
				if(f1.rightline[i-3]-f1.rightline[i+3]>-2&&f1.rightlineflag[i+3]==1)
				{
					if(f1.rightline[i+3]+f1.rightline[i-3]-2*f1.rightline[i]>4)
					{//寻找右拐点
						for(i = 118; i > f2.toppoint; i--)
						{
							f1.midline[i] = width_real[i] * (i - f2.toppoint) / (118 - f2.toppoint);
						}
						
					}
				}
			}
		}
		for(i=118;i>f2.toppoint+4;i--)   //拐点要在顶点之前
		{
			if(i>45&&f1.rightline[i]>20&&f1.rightline[i]<f1.rightline[i+3]&&f1.rightline[i]<f1.rightline[i-3]&&f1.rightline[i]<f1.rightline[i+5])
			{//i>45是为了控制拐点在40行到115行内
				if(f1.rightline[i-3]-f1.rightline[i+3]>-2&&f1.rightlineflag[i+3]==1)
				{
					if(f1.rightline[i+3]+f1.rightline[i-3]-2*f1.rightline[i]>4)
					{//寻找右拐点
						if(f1.rightline[i] > 100)
						{
							f2.huandaoflag = 7;
							break; 
						}
					}
				}
			}
		}
	}
	else if(f2.huandaoflag == 7)
	{
		//出环
		//强行改中线
		if(f2.toppoint < 20 && f1.leftlineflag[f2.toppoint] == 0 && f1.rightlineflag[f2.toppoint] == 1)
		{
			f2.huandaoflag = 0;
		}
	}
  	//右环
	else if(f2.huandaoflag == 2)
	{
		Tyl_huan12_noguai = 0;
		Tyl_huan12_top = 0;
		//进环前
		for(i=118;i>10;i--)//从118行一直找到10行  
		{
			f1.midline[i] = f1.leftline[i] + width_real[i];
		}
		for(i = 118; i > 10; i--)
		{
			if(f1.rightline[i] - f1.rightline[i + 3] < -20 && f1.rightline[i + 1] - f1.rightline[i + 4] < -20 && f1.rightline[i + 2] - f1.rightline[i + 5] < -20)
			{
				Tyl_huan12_top = i;
				break;
			}
		}
		for(i=118;i>Tyl_huan12_top;i--)
		{
			if(f1.rightlineflag[i])
			{
				Tyl_huan12_noguai++;
			}
		}
		if(Tyl_huan12_noguai < 5)
		{
			f2.huandaoflag = 4;
		}
	}
	else if(f2.huandaoflag == 4)
	{
		//进环
		Tyl_huan34_noguai = 0;
		Tyl_huan34_top = 0;
		if(!Tyl_huan34_state)
		{
			for(i = 40; i < 50; i++)
			{
				if(f1.leftline[i] != 0)
				{
					Tyl_huan34_noguai++;
				}
			}
			if(Tyl_huan34_noguai < 3)
			{
				Tyl_huan34_state = 1;
			}
		}
		else
		{
			for(i = 40; i < 50; i++)
			{
				if(f1.leftline[i] == 0)
				{
					Tyl_huan34_noguai++;
				}
			}
			if(Tyl_huan34_noguai < 3)
			{
				Tyl_huan34_state = 0;
				f2.huandaoflag = 6;
			}
		}
		for(i = 118; i > 5; i--)
		{
			if(f1.rightline[i] - f1.rightline[i + 3] < -20)
			{
				Tyl_huan34_top = i;
				break;
			}
		}
		for(i = 118; i > 10; i--)
		{
			if(Tyl_huan34_top < i)
			{
//				f1.midline[i] = (int16)(f1.rightline[i] - width_real[i] * (i -  Tyl_huan34_top)/(118 - Tyl_huan34_top));
				f1.midline[i] = f1.rightline[i] - width_real[i] + 10;
			}
			else
			{
				f1.midline[i] = 159;
			}
		}
	}
  	else if(f2.huandaoflag == 6)
	{
		//环内
		for(i=108;i>f2.toppoint+4;i--)   //拐点要在顶点之前
		{
			if(f2.leftguaiflag==0)
			{
				if(i>45&&f1.leftline[i]<140&&f1.leftline[i]>f1.leftline[i+3]&&f1.leftline[i]>f1.leftline[i-3]&&f1.leftline[i]>f1.leftline[i+5]&&f1.leftline[i]>f1.leftline[i-2]&&f1.leftline[i]>f1.leftline[i-4])
				{//i>45是为了控制拐点在40行到115行内
					if(f1.leftline[i-3]-f1.leftline[i+3]<2&&f1.leftlineflag[i+3]==1&&f1.leftlineflag[i+5]==1&&f1.leftlineflag[i]==1)
					{
						if(2*f1.leftline[i]-f1.leftline[i-3]-f1.leftline[i+3]>4)
						{//寻找左拐点
							for(i = 118; i > f2.toppoint; i--)
							{
								f1.midline[i] = 159 - width_real[i] * (i - f2.toppoint) / (118 - f2.toppoint);
							}
						}
					}
				}
			}
		} 
		
		for(i=108;i>f2.toppoint+4;i--)   //拐点要在顶点之前
		{
			if(i>45&&f1.leftline[i]<140&&f1.leftline[i]>f1.leftline[i+3]&&f1.leftline[i]>f1.leftline[i-3]&&f1.leftline[i]>f1.leftline[i+5]&&f1.leftline[i]>f1.leftline[i-2]&&f1.leftline[i]>f1.leftline[i-4])
			{//i>45是为了控制拐点在40行到115行内
				if(f1.leftline[i-3]-f1.leftline[i+3]<2&&f1.leftlineflag[i+3]==1&&f1.leftlineflag[i+5]==1&&f1.leftlineflag[i]==1)
				{
					if(2*f1.leftline[i]-f1.leftline[i-3]-f1.leftline[i+3]>4)
					{//寻找左拐点
						if(f1.leftline[i] < 60)
						{
							f2.huandaoflag = 8;
							break;
						}
					}
				}
			}
		} 
	}
  	else if(f2.huandaoflag == 8)
	{
		//出环
		//强行改中线
		if(f2.toppoint < 20 && f1.rightlineflag[f2.toppoint] == 0 && f1.leftlineflag[f2.toppoint] == 1)
		{
			f2.huandaoflag = 0;
		}
	}
 
find_toppoint();
column_start=f1.midline[118];//每场之间的起始点设定
column_start=column_start>150?150:column_start;
column_start=column_start<10?10:column_start;
}//f2.line的终点
/**************************************************************
判定区:有弯道判断           
       十字判断
       顶点判断
       拐点判断
***************************************************************/
/**************************************************************
   找顶点，顶点的定义为某一行的中线上的为白色点，但是该中线列的后面的点为黑点，即赛道边界
***************************************************************/

void find_toppoint(void)
{
  int16 i=0;
  for(i=100;i>5;i--)
  {
    if((Image[i][f1.midline[i]]!=0)&&(Image[i-1][f1.midline[i]]==0||Image[i-3][f1.midline[i]]==0||Image[i-5][f1.midline[i]]==0))
    {
      f2.toppoint=i;
      f2.toppoint_flag=1;
      break;
    }
													  
  }
 if(f2.toppoint_flag==0)
 {
  for(i=118;i>2;i--)
  {
    if(f1.midline[i]>150||f1.midline[i]<10)
    {
      f2.toppoint=i;
      f2.toppoint_flag=1;
      break;
    }
  }
 }
}


/***************************** 
函数名称：find_curve1
函数功能：找左右小弯

****************************/
void find_curve1(void)
{
    int i=0,lose_cnt;
    for(i=115;i>f2.toppoint+3;i--)    
    {
        if(f1.leftlineflag[i]==0)
        {
            if(f2.leftturnflag==0&&f1.leftlineflag[i+1]==1&&f1.leftlineflag[i+2]==1&&f1.rightlineflag[i]==1)
            {//找到左边开始丢线的那一行
                if(f1.rightlineflag[i-5]==1&&i>30) 
                {
                    f2.leftturn_row=i;//是个小弯
                    f2.leftturnflag=1;
                    if(f2.leftturn_row<40)
                    {//行数很靠前时防止在十字后远方看过去的时候误判
                        if(f1.rightline[f2.leftturn_row-5]>100&&f1.rightline[f2.leftturn_row-5]>f1.rightline[f2.leftturn_row])
                        {
                            if(f1.rightline[f2.leftturn_row-5]-f1.rightline[f2.leftturn_row]>10)
                            {
                                f2.rightturn_row=0;
                                f2.rightturnflag=0;
                            }
                        }
                    }
                    if(f2.shiziflag[1]==1&&f2.leftturnflag==1)         //防止十字给小弯道判断带来的影响
                    {
                        for(i=f2.leftturn_row;i>f2.toppoint;i--)
                        {//右边不能丢线太多，防止丢十字进去后拐点丢失的几帧误判
                            if(f1.rightlineflag[i]==0)
                            {
                                lose_cnt++;
                            }
                        }	 
                        if(lose_cnt>5)
                        {
                            f2.leftturn_row=0;//是个小弯
                            f2.leftturnflag=0;
                        }
                    }
                    break;
                }
            }
        }
        
        else if(f1.rightlineflag[i]==0)
        {
            if(f2.rightturnflag==0&&f1.rightlineflag[i+1]==1&&f1.rightlineflag[i+2]==1&&f1.leftlineflag[i]==1)
            {
                if(f1.leftlineflag[i-5]==1&&i>30)
                {
                    f2.rightturn_row=i;
                    f2.rightturnflag=1;
                    if(f2.rightturn_row<=40)
                    {//行数很靠前时防止在十字后远方看过去的时候误判
                        if(f1.leftline[f2.rightturn_row-5]<60&&f1.leftline[f2.rightturn_row-5]<f1.leftline[f2.rightturn_row])
                        {
                            if(f1.leftline[f2.rightturn_row]-f1.leftline[f2.rightturn_row-5]>10)
                            {
                                f2.rightturn_row=0;
                                f2.rightturnflag=0;
                            }
                        }
                    }
                    if(f2.shiziflag[1]==1)
                    {
                        for(i=f2.rightturn_row;i>f2.toppoint;i--)
                        {//右边不能丢线太多，防止丢十字进去后拐点丢失的几帧误判
                            if(f1.leftlineflag[i]==0)
                            {
                                lose_cnt++;
                            }
                        }	 
                        if(lose_cnt>5)
                        {
                            f2.rightturn_row=0;//是个小弯
                            f2.rightturnflag=0;
                        }
                    }
                    break;
                }
            }
        } 
    }
}




/*****************************
函数名称：find_curve3
函数功能：找左右深弯

****************************/

int8 find_curve3(void)  
{
    int i=0,flag_L=0,flag_R=0; 
    if(f2.leftturnflag==0&&f2.rightturnflag==0)
    {
        for(i=118;i>112;i--)   //扩大搜索范围
        {
            if(f1.leftlineflag[i]==0&&flag_L==0)    //左线118-112行有丢线
            {
                flag_L=1;
            }
            if(f1.rightlineflag[i]==0&&flag_R==0)   //左线118-112行有丢线
            {
                flag_R=1;
            }
        }
        if(flag_L==1)
        {
            for(i=100;i>f2.toppoint+10;i--)   //100减到90，放松条件，提前转弯
            {
                if(f1.leftlineflag[i]==1||f1.rightlineflag[i]==0)
                {
                    flag_L=0;
                }
            }
        }
        if(flag_R==1)
        {
            for(i=100;i>f2.toppoint+10;i--)   //100减到90，放松条件，提前转弯
            {
                if(f1.leftlineflag[i]==0||f1.rightlineflag[i]==1)
                {
                    flag_R=0;
                }
            }
        }
        if(flag_L==1&&flag_R==0)
        {
            f2.leftturnflag=3;
			return 1;
        }
        else if(flag_L==0&&flag_R==1)
        {
            f2.rightturnflag=3;
			return 1;
        }
    }
	return 0;
}

/*****************************
函数名称：find_guai
函数功能：找拐点 ，拐点是十字的特征

****************************/
void find_guai(void)
{
  int16 i=0;
  if(f2.toppoint<70&&f2.toppoint>5)
  {
    for(i=108;i>f2.toppoint+4;i--)   //拐点要在顶点之前
    {
      if(f2.leftguaiflag==0)
      {
	if(i>45&&f1.leftline[i]<140&&f1.leftline[i]>f1.leftline[i+3]&&f1.leftline[i]>f1.leftline[i-3]&&f1.leftline[i]>f1.leftline[i+5]&&f1.leftline[i]>f1.leftline[i-2]&&f1.leftline[i]>f1.leftline[i-4])
	{//i>45是为了控制拐点在40行到115行内
	  if(f1.leftline[i-3]-f1.leftline[i+3]<2&&f1.leftlineflag[i+3]==1&&f1.leftlineflag[i+5]==1&&f1.leftlineflag[i]==1)
	  {
	    if(2*f1.leftline[i]-f1.leftline[i-3]-f1.leftline[i+3]>4)
	    {//寻找左拐点
	      f2.leftguaiflag=1;
	      f2.leftguai_row=i;
	      f2.leftguai_column=f1.leftline[i]; 
	    }
	  }
	}
	
      }
    } 
    
    for(i=108;i>f2.toppoint+4;i--)   //拐点要在顶点之前
    {
      
      if(f2.rightguaiflag==0)
      {
	if(i>45&&f1.rightline[i]>20&&f1.rightline[i]<f1.rightline[i+3]&&f1.rightline[i]<f1.rightline[i-3]&&f1.rightline[i]<f1.rightline[i+5])
	{//i>45是为了控制拐点在40行到115行内
	  if(f1.rightline[i-3]-f1.rightline[i+3]>-2&&f1.rightlineflag[i+3]==1)
	  {
	    if(f1.rightline[i+3]+f1.rightline[i-3]-2*f1.rightline[i]>4)
	    {//寻找右拐点	    
	      f2.rightguaiflag=1;
	      f2.rightguai_row=i;
	      f2.rightguai_column=f1.rightline[i];

              
	    }
	    
	  }
	}	        
      }
      
    }
  }
}



/*****************************
函数名称：shizi_judge
函数功能：十字判断
****************************/
void shizi_judge(void)
{
  if(f2.leftguaiflag==1)
  {
    if(f2.leftguai_column>140)   
      //在左边拐弯的标志为1的前提下，左拐列数太大，不宜再进行左拐，应将左拐标志取消
    {
      f2.leftguaiflag=0;
      f2.leftguai_row=0;
      f2.leftguai_column=0;
    }
    //或者在此时右边没有丢线的情况下，左拐标志应为零（十字时候，才存在既有左拐点，同时在左拐点对应的行数上面判断右边线丢线）
    else if(right(f2.leftguai_row))
    {
      f2.leftguaiflag=0;
      f2.leftguai_row=0;
      f2.leftguai_column=0;    
    }
  }
  
  if(f2.rightguaiflag==1)
  {
    if(f2.rightguai_column<20)
    {
      f2.rightguaiflag=0;
      f2.rightguai_row=0;
      f2.rightguai_column=0;
    }
    else if(left(f2.rightguai_row))
    {
      f2.rightguaiflag=0;
      f2.rightguai_row=0;
      f2.rightguai_column=0;   
    }  
  }

}


/***************************************************************
	*	@brief	环岛判断
	*	@param	无
	*	@note	近到远，边线宽大于赛道，顶点，大于赛道
***************************************************************/
void Tyl_judge_huandao(void)
{
	for(int i = 100; i > 5; i--)
	{
		if(f1.leftline[i] - f1.leftline[i - 3] <= -20 && f1.leftline[i + 1] - f1.leftline[i - 2] <= -20)
		{
			if(isRightZhi(60))
			{
				//左环
				f2.huandaoflag = 3;
				return;
			}
		}
		if(f1.rightline[i] - f1.rightline[i - 3] >= 20 && f1.rightline[i + 1] - f1.rightline[i - 2] >= 20)
		{
			if(isLeftZhi(60))
			{
				//右环
				f2.huandaoflag = 4;
				return;  
			}
		}
	}
}
/*int8 Tyl_judge_huandao(void)
{
	int16 i = 0, flag = 0;
	int16 top = 0;
	for(i = 0; i < 30; i++)
	{
		if(road_type[i] == 5 || road_type[i] == 3)
		{
			return 0;
		}
	}
	for(i = 100; i > 5; i--)
	{
//		if(f1.rightline[i] - f1.leftline[i] > 2 * width_real[i] && flag == 0)
//		{
//			if(f1.rightlineflag[i])
//			{
//				flag = 1;//左环出口
//				continue;
//			}
//		}
		if(!f2.leftguaiflag && 
		   f1.leftline[i] >= f1.leftline[i + 2] && abs(f1.leftline[i] - f1.leftline[i + 2]) < 10 &&
			   f1.leftline[i] >= f1.leftline[i - 2] && abs(f1.leftline[i] - f1.leftline[i - 2]) < 10 &&
//				   f1.leftline[i - 1] >= f1.leftline[i + 1] && abs(f1.leftline[i - 1] - f1.leftline[i + 1]) < 10 &&
//					   f1.leftline[i - 1] >= f1.leftline[i - 1] && abs(f1.leftline[i - 1] - f1.leftline[i - 1]) < 10
//		   f1.leftline[i] > f1.leftline[i + 4] && abs(f1.leftline[i] - f1.leftline[i + 4]) < 12 &&
//			   f1.leftline[i] > f1.leftline[i - 4] && abs(f1.leftline[i] - f1.leftline[i - 4]) < 12 &&
//				   f1.leftline[i - 1] >= f1.leftline[i + 3] && abs(f1.leftline[i - 1] - f1.leftline[i + 3]) < 12 &&
//					   f1.leftline[i - 1] >= f1.leftline[i - 3] && abs(f1.leftline[i - 1] - f1.leftline[i - 3]) < 12 &&
//			f1.leftline[i] > f1.leftline[i + 6] && abs(f1.leftline[i] - f1.leftline[i + 6]) < 14 &&
//				f1.leftline[i] > f1.leftline[i - 6] && abs(f1.leftline[i] - f1.leftline[i - 6]) < 14 &&
//					f1.leftline[i - 1] >= f1.leftline[i + 5] && abs(f1.leftline[i - 1] - f1.leftline[i + 5]) < 14 &&
//						f1.leftline[i - 1] >= f1.leftline[i - 5] && abs(f1.leftline[i - 1] - f1.leftline[i - 5]) < 14 &&
				flag == 0)
		{
			if(right(i + 5))
			{
				flag = 3;//左环中点
				continue;
			}
		}
		if(flag == 3)
		{
			if(!f1.leftlineflag[i])
			{
				flag = 5;//左环入口丢线
				continue;
			}
		}
//		if(f1.rightline[i] - f1.leftline[i] > 2.1 * width_real[i] && flag == 3)
//		{
//			top = top ? top : i;
//			 if(isRightZhi(top))
//			 {
//				flag = 5;//左环入口
//				f2.huandaoflag = 1;
//			 }
//			return flag;
//		}
		if(flag == 5)
		{
			if(f1.leftline[i] - f1.leftline[i - 3] <= -20 && f1.leftline[i + 1] - f1.leftline[i - 2] <= -20 && f1.leftline[i + 2] - f1.leftline[i - 1] <= -20)
			{
				top = top ? top : i;
				 if(isRightZhi(top))
				 {
					flag = 7;//左环入口
					f2.huandaoflag = 1;
				 }
				return flag;
			}
	}
//		if(f1.rightline[i] - f1.leftline[i] > 2 * width_real[i] && flag == 0)
//		{
//			if(f1.leftlineflag[i])
//			{
//				flag = 2;//右环出口
//				continue;
//			}
//		}
		if(!f2.rightguaiflag &&
		   f1.rightline[i] < f1.rightline[i + 2] && abs(f1.rightline[i] - f1.rightline[i + 2]) < 10 &&
			   f1.rightline[i] < f1.rightline[i - 2] && abs(f1.rightline[i] - f1.rightline[i - 2]) < 10 &&
//				   f1.rightline[i - 1] <= f1.rightline[i + 1] && abs(f1.rightline[i - 1] - f1.rightline[i + 1]) < 10 &&
//					   f1.rightline[i - 1] <= f1.rightline[i - 1] && abs(f1.rightline[i - 1] - f1.rightline[i - 1]) < 10)
//		   f1.rightline[i] < f1.rightline[i + 4] && abs(f1.rightline[i] - f1.rightline[i + 4]) < 12 &&
//			   f1.rightline[i] < f1.rightline[i - 4] && abs(f1.rightline[i] - f1.rightline[i - 4]) < 12 &&
//				   f1.rightline[i - 1] <= f1.rightline[i + 3] && abs(f1.rightline[i - 1] - f1.rightline[i + 3]) < 12 &&
//					   f1.rightline[i - 1] <= f1.rightline[i - 3] && abs(f1.rightline[i - 1] - f1.rightline[i - 3]) < 12 &&
//			f1.rightline[i] < f1.rightline[i + 6] && abs(f1.rightline[i] - f1.rightline[i + 6]) < 14 &&
//				f1.rightline[i] < f1.rightline[i - 6] && abs(f1.rightline[i] - f1.rightline[i - 6]) < 14 &&
//					f1.rightline[i - 1] <= f1.rightline[i + 5] && abs(f1.rightline[i - 1] - f1.rightline[i + 5]) < 14 &&
//						f1.rightline[i - 1] <= f1.rightline[i - 5] && abs(f1.rightline[i - 1] - f1.rightline[i - 5]) < 14 &&
					flag == 0)
		{
			if(left(i + 5))
			{
				flag = 4;//右环中点
				continue;
			}
		}
		if(flag == 4)
		{
			if(!f1.rightlineflag[i])
			{
				flag = 6;//右环入口丢线
				continue;
			}
		}
//		if(f1.rightline[i] - f1.leftline[i] > 2.1 * width_real[i] && flag == 4)
//		{
//			top = top ? top : i;
//			 if(isLeftZhi(top))
//			 {
//				flag = 6;//右环入口
//				f2.huandaoflag = 2;
//			 }
//			return flag;
//		}
		if(flag == 6)
		{
			if(f1.rightline[i] - f1.rightline[i - 3] >= 20 && f1.rightline[i + 1] - f1.rightline[i - 2] >= 20 && f1.rightline[i + 2] - f1.rightline[i - 1] >= 20)
			{
				top = top ? top : i;
				 if(isLeftZhi(top))
				 {
					flag = 6;//右环入口
					f2.huandaoflag = 2;
				 }
				return flag;
			}
		}
	}
	return flag;
}*/

/*****************************
函数名称：judge_leftlose,judge_rightlose，judge_bothlose
函数功能：分别判断是否丢线
****************************/
int8 judge_leftlose(void)
{
  int16 i=0, xielv[40], sum = 0, flag = 0;
  for(i=60;i>20;i--)
  {
	  xielv[60 - i] = f1.leftline[i - 5] - f1.leftline[i];
	  if(abs(xielv[60 - i]) > xielvcha)
	  {
		  xielv[60 - i] = i == 60 ? 0: xielv[59 - i];
		  sum += sum/(60 - i);
	  }
	  else
	  {
	  	sum += xielv[60 - i];
	  }
	  if(abs(xielv[60 - i] - sum / (61 - i)) > dianshu)
	  {
		  flag++;
	  }
  }
  if(flag > 10)
  	return 1;
  else
	  return 0;
}
int8 judge_rightlose(void)
{
  int16 i=0, xielv[40], sum = 0, flag = 0;
  for(i=60;i>20;i--)
  {
	  xielv[60 - i] = f1.rightline[i - 5] - f1.rightline[i];
	  if(abs(xielv[70 - i]) > xielvcha)
	  {
		  xielv[60 - i] = i == 60 ? 0: xielv[59 - i];
		  sum += i == 60 ? 0: sum/(60 - i);
	  }
	  else
	  {
	  	sum += xielv[60 - i];
	  }
	  if(abs(xielv[60 - i] - sum / (61 - i)) > dianshu)
	  {
		  flag++;
	  }
  }
  if(flag >= 10)
  	return 1;
  else
	  return 0;
}


int8 judge_bothlose(void)
{
  int16 i=0,count=0,flag=0;
  for(i=70;i>25;i--)
  {
    if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==0)  //如果左右边界均为0
    {
     count++;
    }
  }
  if(count>20)      //且丢线场数大于10，这认定此时已经丢线
  {
    flag=1;
  }
  return flag;
}

int8 Tyl_judge_leftlose(void)
{
  int16 i=0,count=0,flag=0;
  for(i=80;i>30;i--)
  {
    if(f1.leftlineflag[i]==0)//左边界不存在
    {
     count++;        //计数加一
    }
  }
  if(count>loselinecount)   
  {
    flag=1;
  }
  return flag;
}
int8 Tyl_judge_rightlose(void)
{
  int16 i=0,count=0,flag=0;
  for(i=80;i>30;i--)
  {
    if(f1.rightlineflag[i]==0)//右边界不存在
    {
     count++;        //计数加一
    }
  }
  if(count>loselinecount)   
  {
    flag=1;
  }
  return flag;
}
/*****************************
函数名称：right(row)
函数功能：判断第row行右边线是否丢线
****************************/
int8 right(int16 row) 
{
  int16 i=0,flag=0;
  flag=1;
  for(i=row;i>f2.toppoint;i--)
  { 
    if(f1.rightlineflag[i]==0)
    {
      flag=0;
      break;
    }
  }
  return flag;
}


/*****************************
函数名称：left(row)
函数功能：判断第row行左边线上边是否丢线
****************************/
int8 left(int16 row) 
{
  int16 i=0,flag=0;
  flag=1;
  for(i=row;i>f2.toppoint;i--)
  { 
    if(f1.leftlineflag[i]==0)
    {
      flag=0;
      break;
    }
  }
  return flag;
}




/*****************************
函数名称：bianxian
函数功能：判断第row行左右边线上边是否丢线
//检查左右拐点的两边是否存在丢线  
****************************/
void bianxian(void) 
{
  int16 i=0,flag=0;
  if(f2.leftguaiflag==1)
  {
    flag=1;
    for(i=f2.leftguai_row+5;i>f2.leftguai_row-20;i--)
    { 
      if(f1.leftlineflag[i]==0||f1.rightlineflag[i]==0)
      {
	flag=0;
	break;
      }
    }
    if(flag==1)  //如果左或右拐点有丢线，原有拐点标志不变，如果没有丢线，原有拐点标志变为0
    {
      f2.leftguaiflag=0;
    }
  }
  if(f2.rightguaiflag==1)
  {
    flag=1;
    for(i=f2.rightguai_row+5;i>f2.rightguai_row-20;i--)
    { 
      if(f1.leftlineflag[i]==0||f1.rightlineflag[i]==0)
      {
	flag=0;
	break;
      }
    }
    if(flag==1)
    {
      f2.rightguaiflag=0;
    }
  }
}


/***************************************************************
	*	@brief	判定左右边界没丢线部分是否为直线
	*	@param	top：上边界
	*	@note	下边界从没丢线的最低点开始（简单滤波）
***************************************************************/
int16 isLeftZhi(int16 top)
{
	int16 bottom = 118;
	top = 6;
	int16 max = 0, min = 0;
	int16 xielv, xielv_last;
	while(bottom != top)
	{
		if(f1.leftlineflag[bottom])
		{
			break;
		}
		else
		{
			bottom--;
		}
	}
	for(int i = bottom; i > top; i--)
	{
		if(f1.leftline[i] > f1.leftline[i + 2] && abs(f1.leftline[i] - f1.leftline[i + 2]) < 10 &&
			   f1.leftline[i] > f1.leftline[i - 2] && abs(f1.leftline[i] - f1.leftline[i - 2]) < 10 &&
				   f1.leftline[i - 1] >= f1.leftline[i + 1] && abs(f1.leftline[i - 1] - f1.leftline[i + 1]) < 10 &&
					   f1.leftline[i - 1] >= f1.leftline[i - 1] && abs(f1.leftline[i - 1] - f1.leftline[i - 1]) < 10 &&
		   f1.leftline[i] > f1.leftline[i + 4] && abs(f1.leftline[i] - f1.leftline[i + 4]) < 12 &&
			   f1.leftline[i] > f1.leftline[i - 4] && abs(f1.leftline[i] - f1.leftline[i - 4]) < 12 &&
				   f1.leftline[i - 1] >= f1.leftline[i + 3] && abs(f1.leftline[i - 1] - f1.leftline[i + 3]) < 12 &&
					   f1.leftline[i - 1] >= f1.leftline[i - 3] && abs(f1.leftline[i - 1] - f1.leftline[i - 3]) < 12 &&
			f1.leftline[i] > f1.leftline[i + 6] && abs(f1.leftline[i] - f1.leftline[i + 6]) < 14 &&
				f1.leftline[i] > f1.leftline[i - 6] && abs(f1.leftline[i] - f1.leftline[i - 6]) < 14 &&
					f1.leftline[i - 1] >= f1.leftline[i + 5] && abs(f1.leftline[i - 1] - f1.leftline[i + 5]) < 14 &&
						f1.leftline[i - 1] >= f1.leftline[i - 5] && abs(f1.leftline[i - 1] - f1.leftline[i - 5]) < 14)
		{
			max = i;
		}
		if(f1.leftline[i] < f1.leftline[i + 2] && abs(f1.leftline[i] - f1.leftline[i + 2]) < 10 &&
			   f1.leftline[i] < f1.leftline[i - 2] && abs(f1.leftline[i] - f1.leftline[i - 2]) < 10 &&
				   f1.leftline[i - 1] <= f1.leftline[i + 1] && abs(f1.leftline[i - 1] - f1.leftline[i + 1]) < 10 &&
					   f1.leftline[i - 1] <= f1.leftline[i - 1] && abs(f1.leftline[i - 1] - f1.leftline[i - 1]) < 10 &&
		   f1.leftline[i] < f1.leftline[i + 4] && abs(f1.leftline[i] - f1.leftline[i + 4]) < 12 &&
			   f1.leftline[i] < f1.leftline[i - 4] && abs(f1.leftline[i] - f1.leftline[i - 4]) < 12 &&
				   f1.leftline[i - 1] <= f1.leftline[i + 3] && abs(f1.leftline[i - 1] - f1.leftline[i + 3]) < 12 &&
					   f1.leftline[i - 1] <= f1.leftline[i - 3] && abs(f1.leftline[i - 1] - f1.leftline[i - 3]) < 12 &&
			f1.leftline[i] < f1.leftline[i + 6] && abs(f1.leftline[i] - f1.leftline[i + 6]) < 14 &&
				f1.leftline[i] < f1.leftline[i - 6] && abs(f1.leftline[i] - f1.leftline[i - 6]) < 14 &&
					f1.leftline[i - 1] <= f1.leftline[i + 5] && abs(f1.leftline[i - 1] - f1.leftline[i + 5]) < 14 &&
						f1.leftline[i - 1] <= f1.leftline[i - 5] && abs(f1.leftline[i - 1] - f1.leftline[i - 5]) < 14)
		{
			min = i;
		}
	}
	if(max && min)
	{
		if(max > min)
		{
			if(abs(f1.leftline[max] - f1.leftline[min] - f1.leftline[min] + f1.leftline[top]) > 5)
			{
				return 0;
			}
		}
		else
		{
			if(abs(f1.leftline[min] - f1.leftline[max] - f1.leftline[max] + f1.leftline[top]) > 5)
			{
				return 0;
			}
		}
	}
	else if(!max && min)
	{
		if(abs(f1.leftline[bottom] - f1.leftline[min] - f1.leftline[min] + f1.leftline[top]) > 5)
		{
			return 0;
		}
	}
	else if(max && !min)
	{
		if(abs(f1.leftline[bottom] - f1.leftline[max] - f1.leftline[max] + f1.leftline[top]) > 5)
		{
			return 0;
		}
	}
	do
	{
		xielv = f1.leftline[bottom] - f1.leftline[bottom - 10];
		bottom--;
	}while(xielv < -20);
	for(int i = bottom - 4; i >= (top>21?top:21); i -= 4)
	{
		xielv_last = xielv;
		xielv = f1.leftline[i] - f1.leftline[i - 10];
		if(xielv < -20)
		{
			continue;
		}
		if(abs(xielv - xielv_last) > 6)
		{
			return 0;
		}
	}
	return 1;
}

int16 isRightZhi(int16 top)
{
	int16 bottom = 118;
	top = 6;
	int16 max = 0, min = 0;
	int16 xielv, xielv_last;
	while(bottom != top)
	{
		if(f1.rightlineflag[bottom])
		{
			break;
		}
		else
		{
			bottom--;
		}
	}
	for(int i = bottom; i > top; i--)
	{
		if(f1.rightline[i] < f1.rightline[i + 2] && abs(f1.rightline[i] - f1.rightline[i + 2]) < 10 &&
			   f1.rightline[i] < f1.rightline[i - 2] && abs(f1.rightline[i] - f1.rightline[i - 2]) < 10 &&
				   f1.rightline[i - 1] <= f1.rightline[i + 1] && abs(f1.rightline[i - 1] - f1.rightline[i + 1]) < 10 &&
					   f1.rightline[i - 1] <= f1.rightline[i - 1] && abs(f1.rightline[i - 1] - f1.rightline[i - 1]) < 10 &&
		   f1.rightline[i] < f1.rightline[i + 4] && abs(f1.rightline[i] - f1.rightline[i + 4]) < 12 &&
			   f1.rightline[i] < f1.rightline[i - 4] && abs(f1.rightline[i] - f1.rightline[i - 4]) < 12 &&
				   f1.rightline[i - 1] <= f1.rightline[i + 3] && abs(f1.rightline[i - 1] - f1.rightline[i + 3]) < 12 &&
					   f1.rightline[i - 1] <= f1.rightline[i - 3] && abs(f1.rightline[i - 1] - f1.rightline[i - 3]) < 12 &&
			f1.rightline[i] < f1.rightline[i + 6] && abs(f1.rightline[i] - f1.rightline[i + 6]) < 14 &&
				f1.rightline[i] < f1.rightline[i - 6] && abs(f1.rightline[i] - f1.rightline[i - 6]) < 14 &&
					f1.rightline[i - 1] <= f1.rightline[i + 5] && abs(f1.rightline[i - 1] - f1.rightline[i + 5]) < 14 &&
						f1.rightline[i - 1] <= f1.rightline[i - 5] && abs(f1.rightline[i - 1] - f1.rightline[i - 5]) < 14)
		{
			min = i;
		}
		if(f1.rightline[i] > f1.rightline[i + 2] && abs(f1.rightline[i] - f1.rightline[i + 2]) < 10 &&
			   f1.rightline[i] > f1.rightline[i - 2] && abs(f1.rightline[i] - f1.rightline[i - 2]) < 10 &&
				   f1.rightline[i - 1] >= f1.rightline[i + 1] && abs(f1.rightline[i - 1] - f1.rightline[i + 1]) < 10 &&
					   f1.rightline[i - 1] >= f1.rightline[i - 1] && abs(f1.rightline[i - 1] - f1.rightline[i - 1]) < 10 &&
		   f1.rightline[i] > f1.rightline[i + 4] && abs(f1.rightline[i] - f1.rightline[i + 4]) < 12 &&
			   f1.rightline[i] > f1.rightline[i - 4] && abs(f1.rightline[i] - f1.rightline[i - 4]) < 12 &&
				   f1.rightline[i - 1] >= f1.rightline[i + 3] && abs(f1.rightline[i - 1] - f1.rightline[i + 3]) < 12 &&
					   f1.rightline[i - 1] >= f1.rightline[i - 3] && abs(f1.rightline[i - 1] - f1.rightline[i - 3]) < 12 &&
			f1.rightline[i] > f1.rightline[i + 6] && abs(f1.rightline[i] - f1.rightline[i + 6]) < 14 &&
				f1.rightline[i] > f1.rightline[i - 6] && abs(f1.rightline[i] - f1.rightline[i - 6]) < 14 &&
					f1.rightline[i - 1] >= f1.rightline[i + 5] && abs(f1.rightline[i - 1] - f1.rightline[i + 5]) < 14 &&
						f1.rightline[i - 1] >= f1.rightline[i - 5] && abs(f1.rightline[i - 1] - f1.rightline[i - 5]) < 14)
		{
			max = i;
		}
	}
	if(max)
	{
		if(abs(f1.rightline[bottom] - f1.rightline[max] - f1.rightline[max] + f1.rightline[top]) > 5)
		{
			return 0;
		}
	}
	if(min)
	{
		if(abs(f1.rightline[bottom] - f1.rightline[min] - f1.rightline[min] + f1.rightline[top]) > 5)
		{
			return 0;
		}
	}
	do
	{
		xielv = f1.rightline[bottom] - f1.rightline[bottom - 10];
		bottom--;
	}while(xielv > 20);
	for(int i = bottom - 4; i >= (top>20?top:20); i -= 4)
	{
		xielv_last = xielv;
		xielv = f1.rightline[i] - f1.rightline[i - 10];
		if(xielv > 20)
		{
			continue;
		}
		if(abs(xielv - xielv_last) > 6)
		{
			return 0;
		}
	}
	return 1;
}

