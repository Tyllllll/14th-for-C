
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
//�Բ�Ķ�������ʵ�ʾ���ı�񣬾���Ҫ�����Լ��ĳ����в���
#include "head_file.h"
/*******ʮ�ֱ�־������******/
int16 shizi_time=0;//ʮ�ּ�ʱ��
int16 shizi;  //ʮ�ּ����� 
int16 shizidistance; //ʮ�־������
int16 L=0;//����ʱ���ߺ�������ʧ������
int16 R=0;//����ʱ���ߺ�������ʧ������
int16 both_line=0;//ʮ�ֹյ���ʧʱ�ҵ�ǰ�治���ߵ���
int16 xie_flag=0;
float k_left[3]={0,0,0};//ʮ�ֲ���б�ʼ�¼
float k_right[3]={0,0,0};
float shizi_distance=0;
int16 start_cnt_distance=0;
int16 shizi_cnt=0;
/*******�����־������******/
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
int16 column_start=80;//����Ѱ�ߵ�ÿ�е���ʼ������
int16 text_width[120]={0};
int16 width_real[120]=  {0, 1,  2,  2,  5,  9,  15, 20, 32, 33,   //0-9
                        33, 34, 35, 35, 37, 38, 39, 40, 41, 42,   //10-11
                        43, 43, 45, 46, 47, 47, 48, 49, 50, 51,   //...
                        52, 53, 54, 55, 56, 57, 58, 59, 60, 60,
                        62, 63, 63, 64, 65, 66, 67, 68, 68, 69,
                        70, 71, 72, 73, 74, 75, 75, 75, 75, 75,
                        75, 75, 75, 75, 75, 75, 75, 79, 79, 79,
                        79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
                        79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
                        79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
                        79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
                        79, 79, 79, 79, 79, 79, 79, 79, 79, 79};

int16  width[120]={1,1,2,2,2,3,3,4,4,4,  //����ȫ�� ������Ҫ�����Լ��ĳ����в���
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
void findline1(void)  //����Ѱ��
{
    int16 i=0,j=0,k=0;
    f2.toppoint=0;
    f2.toppoint_flag=0; 

    
    
    for(i=118;i>10;i--)//��118��һֱ�ҵ�10��
    {
        k=column_start;   //80��
        f1.leftline[i]=0;
        f1.leftlineflag[i]=0;
        f1.midline[i]=0;
        f1.rightline[i]=0;
        f1.rightlineflag[i]=0;
        for(j=k;j>2;j--)   //Image[i][j]==0Ϊ�ڣ�Image[i][j]!=0Ϊ��
        {
            if(Image[i][j]!=0&&Image[i][j-1]!=0&&Image[i][j-2]==0)
            {//Ѱ���������
                f1.leftline[i]=j-1;
                f1.leftlineflag[i]=1;
                break;
            }
        }
        for(j=k;j<157;j++)
        {
            if(Image[i][j]!=0&&Image[i][j+1]!=0&&Image[i][j+2]==0)
            {//Ѱ���������
                f1.rightline[i]=j+1;
                f1.rightlineflag[i]=1;
                break;
            }
        }
//        
//        if(i>10&&i<116) //�˳����
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
        
        if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==1)
        {//���߶��ҵ������
            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];  
        }
        else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==1)
        {//���������㣬�ұ��������
            f1.leftline[i]=0;
            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            if(i<95&&f1.rightline[i]<=f1.rightline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)//ֱ���ϲ����ܳ��ָ��������������ת�䴦
//            {
//                f1.midline[i]=f1.midline[i+1];
//            }  
            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
        }
        else if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==0)
        {//���������㣬�ұ��������
            f1.rightline[i]=159;
            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            if(i<99&&f1.leftline[i]>=f1.leftline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)  //�����ת
//            {
//                f1.midline[i]=f1.midline[i+1];
//            }
            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
        }
        else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==0)//&&f2.shiziflag[1]==0)
        {//�����������
            f1.leftline[i]=0;
            f1.rightline[i]=159;
            if(i==118)
            {
                f1.midline[i]=80;
            }
            else
            {
                f1.midline[i]=f1.midline[i+1];//����һ����
            }
            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
        }
        column_start=f1.midline[i];
    }  
//    for(i=118;i>10;i--)
//    {
//      leftlineflag_reg[i] = f1.leftlineflag[i];
//      rightlineflag_reg[i] = f1.rightlineflag[i];
//    }=
//    for(i=118;i>10;i--)//��118��һֱ�ҵ�10��
//    {
//        f1.leftline[i]=0;
//        f1.leftlineflag[i]=0;
//        f1.midline[i]=0;
//        f1.rightline[i]=0;
//        f1.rightlineflag[i]=0;
//        nolineflag=1;
//        for(j=2;j<=159;j++)   //Image[i][j]==0Ϊ�ڣ�Image[i][j]!=0Ϊ��
//        {
//            if(Image[i][j]!=0&&Image[i][j-1]!=0&&Image[i][j-2]==0)
//            {//Ѱ���������
//                f1.leftline[i]=j-1;
//                f1.leftlineflag[i]=1;
//                nolineflag=0;
//                break;
//            }
//        }
//        f1.leftline[i] = f1.leftlineflag[i]==1?f1.leftline[i]:0;
//        if(nolineflag == 1)  //i��û�������
//        {
//          for(j=157;j>0;j--)
//          {
//              if(Image[i][j]!=0&&Image[i][j+1]!=0&&Image[i][j+2]==0)
//              {//Ѱ���������
//                  f1.rightline[i]=j+1;
//                  f1.rightlineflag[i]=1;
//                  break;
//              }
//          }
//        }
//        else   //i���������
//        {
//          for(j=157;)
//          f1.rightline[i]=159;
//          f1.rightlineflag[i]=0;
//        }
//    }
//    
//    for(i=118;i>10;i--)    //ȥ���������
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
//        {//���߶��ҵ������
//            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//            
//        }
//        else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==1)
//        {//���������㣬�ұ��������
//            f1.leftline[i]=0;
//            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            if(i<99&&f1.rightline[i]<=f1.rightline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)//ֱ���ϲ����ܳ��ָ��������������ת�䴦
//            {
//                f1.midline[i]=f1.midline[i+1];
//            }  
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//        }
//        else if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==0)
//        {//���������㣬�ұ��������
//            f1.rightline[i]=159;
//            f1.midline[i]=(f1.leftline[i]+f1.rightline[i])/2;
//            if(i<99&&f1.leftline[i]>=f1.leftline[i+1]&&f1.leftlineflag[i+15]==0&&f1.rightlineflag[i+15]==0)  //�����ת
//            {
//                f1.midline[i]=f1.midline[i+1];
//            }
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//        }
//        else if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==0)//&&f2.shiziflag[1]==0)
//        {//�����������
//            f1.leftline[i]=0;
//            f1.rightline[i]=159;
//            if(i==118)
//            {
//                f1.midline[i]=80;
//            }
//            else
//            {
//                f1.midline[i]=f1.midline[i+1];//����һ����
//            }
//            f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
//            f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];
//    }
//    }
    
    
    
//    column_start=f1.midline[118];//ÿ��֮�����ʼ���趨
//    //�����������
//    for (i=11;i<=118;i++ )
//    {width_test[i] = ( - f1.leftline[i] + f1.rightline[i]) / 2;
//    LPLD_UART_PutChar(UART0,width_test[i] );
//    LPLD_UART_PutChar(UART0,',' );
//    
//    }
    
    if(f2.toppoint_flag==0)
    {
        find_toppoint();//Ѱ��һ��ͼ�����������
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

//  if(f2.leftguaiflag==0&&f2.rightguaiflag==0)
//  {//Ѱ���������Ƿ���ڹյ�
//    find_guai(); 
//  }

  if(f2.toppoint!=0)
  {//�ж���ʱ��С��
    find_curve1();  
  } 

  if(f2.toppoint!=0&&f2.toppoint<95&&f2.leftturnflag==0&&f2.rightturnflag==0)
  {//�ж��Ƿ�����
    find_curve3();
  }
  

  
  /********************************************ʮ���ж�*******************************************************************/
  if(f2.leftguaiflag==1||f2.rightguaiflag==1)
  {//�յ���� 
      shizi_judge();
      if(f2.leftguaiflag==1&&f2.rightguaiflag==1)
      {
          
          if(f2.leftguai_row>f2.rightguai_row&&f2.leftguai_column>f2.rightguai_column)  
          {//��յ����ҹյ��ұ�ʱ ���������յ�
              f2.rightguaiflag=0;
          }
          if(f2.rightguai_row>f2.leftguai_row&&f2.leftguai_column>f2.rightguai_column)
          {
              f2.leftguaiflag=0;
          }  
          
      }   
      
      
      bianxian();//������ҹյ�������Ƿ���ڶ���  
      if(f2.leftguaiflag==1||f2.rightguaiflag==1)
      {
          
          if(f2.shiziflag[1]==0)
          {
              shizi_distance=0;
              f2.shiziflag[0]=f2.shiziflag[0]+1;
          }
          if(f2.shiziflag[0]==4) //����4֡ͼ��ȷ��Ϊʮ��
          {
              f2.shiziflag[1]=1;
              f2.shiziflag[0]=0; 
          }
          
          
          if(f2.leftturnflag!=0||f2.rightturnflag!=0) //�յ���ڣ���ȥת���־λ
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
  
  
  
  
  
  /**********************************ʮ�ֲ���***********************************************/
  if(f2.shiziflag[1]==1)
  {
      if(f2.leftguaiflag==1&&f2.rightguaiflag==1)
      {
          if(f2.leftguai_row+8<118)
          {
              k_left[0]=((float)(f2.leftguai_column-f1.leftline[f2.leftguai_row+8])/8.0);
              f2.zhizi_k_L=(float)(0.6*k_left[0]+0.2*k_left[1]+0.2*k_left[2]);  //��Ȩ����б��
          }
          if(f2.leftguai_row+8>=118)
          {
              k_left[0]=((float)(f2.leftguai_column-f1.leftline[118])/(float)(118-f2.leftguai_row));
              
              f2.zhizi_k_L=(float)(0.6*k_left[0]+0.2*k_left[1]+0.2*k_left[2]); //��Ȩ����б��
              
          }
          
          if(f2.rightguai_row+8<118)
          {
              k_right[0]=((float)(f2.rightguai_column-f1.rightline[f2.rightguai_row+8])/8.0);
              f2.zhizi_k_R=(float)(0.6*k_right[0]+0.2*k_right[1]+0.2*k_right[2]);//��Ȩ����б��      
          }
          if(f2.rightguai_row+8>=118)
          {
              k_right[0]=((float)(f2.rightguai_column-f1.rightline[118])/(float)(118-f2.rightguai_row));
              
              f2.zhizi_k_R=(float)(0.6*k_right[0]+0.2*k_right[1]+0.2*k_right[2]);//��Ȩ����б�� 
          }  
          
          for(i=f2.rightguai_row;i>=10;i--)   
          {        //�����ұ߲��ߵ�
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
          { //������߲��ߵ�
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
              f1.midline[i]=f1.rightline[i]-(int16)(width_real[i])-10;  //�ұ��߲����
              
              
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
              f1.midline[i]=f1.leftline[i]+(int16)(width_real[i])+10;  //����߲����
              
              
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
      find_toppoint();//����ʮ�ֺ����¶���һ���µĶ���
      
  }
  
  
  /******************ʮ�ֲ��߽���******************/
  
  
  /***********************************/
    //  �������
  /************************************/
  
 if(f2.leftturnflag!=0||f2.rightturnflag!=0)
 {
   if(f2.leftturnflag==1)
   {
     for(i=f2.leftturn_row;i>f2.toppoint;i--)
     {
       if(f1.rightline[i]==159)
       {
	 f1.midline[i]=f1.midline[i+1]-1;  //���߲�������������ƫ
       }
       else
       {
	 f1.midline[i]=f1.midline[i+1]-(f1.rightline[i+1]-f1.rightline[i]);    //���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
       }
       if(f1.midline[i]<=0)
       {
	 f1.midline[i]=0;
	 L=i;   //����ʱ���ߺ�������ʧ������
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
       f1.midline[i]=f1.rightline[i]-80;      //ֱ�����ұ߽��ȥ80�������߹켣����ƫ����ȸ���
       f1.midline[i]=f1.midline[i]>159?159:f1.midline[i];
       f1.midline[i]=f1.midline[i]<0?0:f1.midline[i];//�޷�
       if(f1.midline[i]<=0)
       {
	 f1.midline[i]=0;
	 L=i;    //����ʱ���ߺ�������ʧ������
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
               f1.midline[i]=f1.midline[i]+1; //���߲��֣��������ұ�ƫ
           }
           else
           {
               f1.midline[i]=f1.midline[i+1]+(f1.leftline[i]-f1.leftline[i+1]);        //���߲�������������ƫ����ƫ��Ļ��ȸ���߽�����ƫ��̶���ͬ
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
      f1.midline[i]=f1.leftline[i]+80;//ֱ������߽��80�������߹켣����ƫ����ȸ���
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
 


find_toppoint();
column_start=f1.midline[118];//ÿ��֮�����ʼ���趨
column_start=column_start>150?150:column_start;
column_start=column_start<10?10:column_start;
}//f2.line���յ�
/**************************************************************
�ж���:������ж�           
       ʮ���ж�
       �����ж�
       �յ��ж�
***************************************************************/
/**************************************************************
   �Ҷ��㣬����Ķ���Ϊĳһ�е������ϵ�Ϊ��ɫ�㣬���Ǹ������еĺ���ĵ�Ϊ�ڵ㣬�������߽�
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
�������ƣ�find_curve1
�������ܣ�������С��

****************************/
void find_curve1(void)
{
    int i=0,lose_cnt;
    for(i=115;i>f2.toppoint+3;i--)    
    {
        if(f1.leftlineflag[i]==0)
        {
            if(f2.leftturnflag==0&&f1.leftlineflag[i+1]==1&&f1.leftlineflag[i+2]==1&&f1.rightlineflag[i]==1)
            {//�ҵ���߿�ʼ���ߵ���һ��
                if(f1.rightlineflag[i-5]==1&&i>30) 
                {
                    f2.leftturn_row=i;//�Ǹ�С��
                    f2.leftturnflag=1;
                    if(f2.leftturn_row<40)
                    {//�����ܿ�ǰʱ��ֹ��ʮ�ֺ�Զ������ȥ��ʱ������
                        if(f1.rightline[f2.leftturn_row-5]>100&&f1.rightline[f2.leftturn_row-5]>f1.rightline[f2.leftturn_row])
                        {
                            if(f1.rightline[f2.leftturn_row-5]-f1.rightline[f2.leftturn_row]>10)
                            {
                                f2.rightturn_row=0;
                                f2.rightturnflag=0;
                            }
                        }
                    }
                    if(f2.shiziflag[1]==1&&f2.leftturnflag==1)         //��ֹʮ�ָ�С����жϴ�����Ӱ��
                    {
                        for(i=f2.leftturn_row;i>f2.toppoint;i--)
                        {//�ұ߲��ܶ���̫�࣬��ֹ��ʮ�ֽ�ȥ��յ㶪ʧ�ļ�֡����
                            if(f1.rightlineflag[i]==0)
                            {
                                lose_cnt++;
                            }
                        }	 
                        if(lose_cnt>5)
                        {
                            f2.leftturn_row=0;//�Ǹ�С��
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
                    {//�����ܿ�ǰʱ��ֹ��ʮ�ֺ�Զ������ȥ��ʱ������
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
                        {//�ұ߲��ܶ���̫�࣬��ֹ��ʮ�ֽ�ȥ��յ㶪ʧ�ļ�֡����
                            if(f1.leftlineflag[i]==0)
                            {
                                lose_cnt++;
                            }
                        }	 
                        if(lose_cnt>5)
                        {
                            f2.rightturn_row=0;//�Ǹ�С��
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
�������ƣ�find_curve3
�������ܣ�����������

****************************/

void find_curve3(void)  
{
    int i=0,flag_L=0,flag_R=0; 
    if(f2.leftturnflag==0&&f2.rightturnflag==0)
    {
        for(i=118;i>112;i--)   //����������Χ
        {
            if(f1.leftlineflag[i]==0&&flag_L==0)    //����118-112���ж���
            {
                flag_L=1;
            }
            if(f1.rightlineflag[i]==0&&flag_R==0)   //����118-112���ж���
            {
                flag_R=1;
            }
        }
        if(flag_L==1)
        {
            for(i=100;i>f2.toppoint+10;i--)   //100����90��������������ǰת��
            {
                if(f1.leftlineflag[i]==1||f1.rightlineflag[i]==0)
                {
                    flag_L=0;
                }
            }
        }
        if(flag_R==1)
        {
            for(i=100;i>f2.toppoint+10;i--)   //100����90��������������ǰת��
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
        }
        else if(flag_L==0&&flag_R==1)
        {
            f2.rightturnflag=3;
        }
    }
}

/*****************************
�������ƣ�find_guai
�������ܣ��ҹյ� ���յ���ʮ�ֵ�����

****************************/
void find_guai(void)
{
  int16 i=0;
  if(f2.toppoint<70&&f2.toppoint>5)
  {
    for(i=108;i>f2.toppoint+4;i--)   //�յ�Ҫ�ڶ���֮ǰ
    {
      if(f2.leftguaiflag==0)
      {
	if(i>45&&f1.leftline[i]<140&&f1.leftline[i]>f1.leftline[i+3]&&f1.leftline[i]>f1.leftline[i-3]&&f1.leftline[i]>f1.leftline[i+5]&&f1.leftline[i]>f1.leftline[i-2]&&f1.leftline[i]>f1.leftline[i-4])
	{//i>45��Ϊ�˿��ƹյ���40�е�115����
	  if(f1.leftline[i-3]-f1.leftline[i+3]<2&&f1.leftlineflag[i+3]==1&&f1.leftlineflag[i+5]==1&&f1.leftlineflag[i]==1)
	  {
	    if(2*f1.leftline[i]-f1.leftline[i-3]-f1.leftline[i+3]>4)
	    {//Ѱ����յ�
	      f2.leftguaiflag=1;
	      f2.leftguai_row=i;
	      f2.leftguai_column=f1.leftline[i]; 
	    }
	  }
	}
	
      }
    } 
    
    for(i=108;i>f2.toppoint+4;i--)   //�յ�Ҫ�ڶ���֮ǰ
    {
      
      if(f2.rightguaiflag==0)
      {
	if(i>45&&f1.rightline[i]>20&&f1.rightline[i]<f1.rightline[i+3]&&f1.rightline[i]<f1.rightline[i-3]&&f1.rightline[i]<f1.rightline[i+5])
	{//i>45��Ϊ�˿��ƹյ���40�е�115����
	  if(f1.rightline[i-3]-f1.rightline[i+3]>-2&&f1.rightlineflag[i+3]==1)
	  {
	    if(f1.rightline[i+3]+f1.rightline[i-3]-2*f1.rightline[i]>4)
	    {//Ѱ���ҹյ�	    
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
�������ƣ�shizi_judge
�������ܣ�ʮ���ж�
****************************/
void shizi_judge(void)
{
  if(f2.leftguaiflag==1)
  {
    if(f2.leftguai_column>140)   
      //����߹���ı�־Ϊ1��ǰ���£��������̫�󣬲����ٽ�����գ�Ӧ����ձ�־ȡ��
    {
      f2.leftguaiflag=0;
      f2.leftguai_row=0;
      f2.leftguai_column=0;
    }
    //�����ڴ�ʱ�ұ�û�ж��ߵ�����£���ձ�־ӦΪ�㣨ʮ��ʱ�򣬲Ŵ��ڼ�����յ㣬ͬʱ����յ��Ӧ�����������ж��ұ��߶��ߣ�
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

/*****************************
�������ƣ�judge_leftlose,judge_rightlose��judge_bothlose
�������ܣ��ֱ��ж��Ƿ���
****************************/
int8 judge_leftlose(void)
{
  int16 i=0,count=0,flag=0;
  for(i=118;i>40;i--)
  {
    if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==1)//��߽���ڣ��ұ߽粻����
    {
     count++;        //������һ
    }
  }
  if(count>35)   
  {
    flag=1;        //�����߽綪�߳�������35�У����߱�־��һ
  }
  return flag;
}
int8 judge_rightlose(void)
{
  int16 i=0,count=0,flag=0;
  for(i=118;i>40;i--)
  {
    if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==0)
    {
     count++;
    }
  }
  if(count>35)
  {
    flag=1;
  }
  return flag;
}


int8 judge_bothlose(void)
{
  int16 i=0,count=0,flag=0;
  for(i=118;i>40;i--)
  {
    if(f1.leftlineflag[i]==0&&f1.rightlineflag[i]==0)  //������ұ߽��Ϊ0
    {
     count++;
    }
  }
  if(count>10)      //�Ҷ��߳�������10�����϶���ʱ�Ѿ�����
  {
    flag=1;
  }
  return flag;
}
/*****************************
�������ƣ�right(row)
�������ܣ��жϵ�row���ұ����Ƿ���
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
�������ƣ�left(row)
�������ܣ��жϵ�row��������ϱ��Ƿ���
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
�������ƣ�bianxian
�������ܣ��жϵ�row�����ұ����ϱ��Ƿ���
//������ҹյ�������Ƿ���ڶ���  
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
    if(flag==1)  //�������ҹյ��ж��ߣ�ԭ�йյ��־���䣬���û�ж��ߣ�ԭ�йյ��־��Ϊ0
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
