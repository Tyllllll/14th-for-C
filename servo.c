#include "header.h"

Servo_Class servo;

/***************************************************************
	*	@brief	�����ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
void Servo_Gpio_Init(void)
{
	servo.duty = DEG_MID;
	servo.fore_max = 58;
	servo.kp_left = 2.1;
	servo.kp_right = 2.1;
	servo.ki = 0;
	servo.kd = 11.1;
	servo.dead_zone = 2;
	servo.dynamic_zone = 30;
	servo.dif_const_left = 7.4;//7.28;
	servo.dif_const_right = 5.2;//5.6;
    magnetic.s_kp = 200;
    magnetic.s_kd = 500;
	servo.enable = 1;
	
	static GPIO_InitTypeDef GPIO_InitStructure;
	//������ų�ʼ��
	GPIO_InitStructure.GPIO_PTx = SERVO_PTx;
	GPIO_InitStructure.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure.GPIO_Output = OUTPUT_L;
	GPIO_InitStructure.GPIO_Pins = SERVO_Pinx;
	LPLD_GPIO_Init(GPIO_InitStructure);
	//PIT3��ʱ�жϣ��������͵�ƽ
	static PIT_InitTypeDef PIT_LOW_InitStructure;
	PIT_LOW_InitStructure.PIT_Pitx = SERVO_LOW_PITx;
	PIT_LOW_InitStructure.PIT_PeriodUs = 100000;
	PIT_LOW_InitStructure.PIT_Isr = Servo_PIT_Isr;
	LPLD_PIT_Init(PIT_LOW_InitStructure);
	LPLD_PIT_EnableIrq(PIT_LOW_InitStructure);//ʹ���ж�
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
	//PIT0 20ms��ʱ�жϣ��������ߵ�ƽ
	static PIT_InitTypeDef PIT_HIGH_InitStructure;
	PIT_HIGH_InitStructure.PIT_Pitx = SERVO_HIGH_PITx;
	PIT_HIGH_InitStructure.PIT_PeriodMs = 20;
	PIT_HIGH_InitStructure.PIT_Isr = Servo_Output;
	LPLD_PIT_Init(PIT_HIGH_InitStructure);
	LPLD_PIT_EnableIrq(PIT_HIGH_InitStructure);//ʹ���ж�
}

/***************************************************************
	*	@brief	PIT0��ʱ�ж�
	*	@param	��
	*	@note	���ڵ��ã��������ߵ�ƽ
***************************************************************/
void Servo_Output(void)
{
	if(servo.duty > DEG_MAX)
	{
		servo.duty = DEG_MAX;
	}
	if(servo.duty < DEG_MIN)
	{
		servo.duty = DEG_MIN;
	}
	PIT->CHANNEL[3].LDVAL = (servo.duty) * (g_bus_clock / 1000000) - 1;
	SERVO = 1;
	PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;//��ʼ��ʱ
}

/***************************************************************
	*	@brief	PIT3��ʱ�ж�
	*	@param	��
	*	@note	�������͵�ƽ
***************************************************************/
void Servo_PIT_Isr(void)
{
	SERVO = 0;
	PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
}
/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	��
***************************************************************/
void Servo_Control(void)
{
	if(servo.which == 0)
	{
		servo.foresight = servo.fore_max;
		if(servo.foresight < feature.top_point)
		{
			servo.foresight = feature.top_point - 6;
		}
        if(feature.breramp == 4)
        {
            servo.foresight += 20;
        }
		servo.error[0] = (int16)(0.7 * Get_Mid_Average(servo.foresight) + 0.15 * servo.error[1] + 0.15 * servo.error[2]);
	}
    
    else if(servo.which == 1)
	{
//		float32 result = (magnetic.right_equivalent - magnetic.left_equivalent) / (magnetic.right_equivalent + magnetic.left_equivalent);
//		if(result > -0.12)
//		{
//			servo.error[0] = magnetic.a * (result - 0.12) * (result - 0.12);
//		}
//		else
//		{
//			servo.error[0] = -magnetic.a * (result - 0.12) * (result - 0.12);
//		}
	}
	constrain_16(&servo.error[0],60,-60);
    constrain_16(&servo.error[0],servo.error[1]+15,servo.error[1]-15);
	servo.error_differ[0] = (int16)(0.7 * (servo.error[0] - servo.error[1]) + 0.15 * (servo.error[2] - servo.error[3]) + 0.15 * (servo.error[4] - servo.error[5]));
	constrain_16(&servo.error_differ[0],servo.error_differ[1]+13,servo.error_differ[1]-13);
	for(uint8 i = 9; i > 0; i--)
	{
		servo.error[i] = servo.error[i - 1];
        servo.error_differ[i] = servo.error_differ[i - 1];
	}
	Servo_PID();
}

/***************************************************************
	*	@brief	���pid����
	*	@param	��
	*	@note	Servo_Control����
***************************************************************/
void Servo_PID(void)
{
    if(servo.which ==0 )
	{
		int16 p_value;
        int16 d_value;
        //��
         if(servo.error[0] <= 0)
         {
             if(servo.error[0] > -servo.dynamic_zone)
             {
                 servo.kp = - servo.kp_left * servo.error[0] / servo.dynamic_zone;
             }
             else
             {
                 servo.kp = servo.kp_left;
             }
         }
         //��
         else
         {
             if(servo.error[0] < servo.dynamic_zone)
             {
                 servo.kp = servo.kp_right * servo.error[0] / servo.dynamic_zone;
             }
             else
             {
                 servo.kp = servo.kp_right;
             }
         }
//		servo.kp = servo.kp + (float32)(Fuzzy(servo.error[0],servo.error_differ[0]))/100;	
//        if(fabs(servo.error[0])<20 && fabs(servo.error[2])<20 && fabs(servo.error[3])<20)
//		{
//			servo.kp *= 0.5;
//		}
//        if(servo.kp < 0.5)
//        {
//            servo.kp = 0.5;
//        }
//        if(servo.kp > 4.5)
//        {
//            servo.kp = 4.5;
//        }
        p_value = (int16)(servo.kp * servo.error[0]);
        d_value = (int16)(servo.kd * servo.error_differ[0]);
        servo.duty = fabs(servo.error[0]) < servo.dead_zone ? (int16)DEG_MID : (int16)(DEG_MID - p_value - d_value);
        constrain_16(&servo.duty,DEG_MAX,DEG_MIN);
    }
    else if(servo.which == 1)
    {
        int16 p_valu;
        int16 d_valu;
        magnetic.error = (float32)(magnetic.onceUni[MIDRIGHT] - magnetic.onceUni[MIDLEFT])/(magnetic.onceUni[MIDRIGHT] + magnetic.onceUni[MIDLEFT]);
        magnetic.diff_error = magnetic.error - magnetic.error_pre1;
        magnetic.error_pre1 = magnetic.error;
        p_valu = (int16)(magnetic.s_kp * magnetic.error);
        d_valu = (int16)(magnetic.s_kd * magnetic.diff_error);
        servo.duty = (int16)(DEG_MID - p_valu - d_valu); 
        if(magnetic.onceUni[MIDRIGHT]<5 && magnetic.onceUni[MIDLEFT]<5)
        {
            if(magnetic.onceUni[MIDRIGHT]<magnetic.onceUni[MIDLEFT])
            {
                servo.duty = DEG_MAX ;
            }
            else
            {
                servo.duty = DEG_MIN ;
            }
            
        }
        if(servo.duty>DEG_MAX)
        {
            servo.duty = DEG_MAX;
        }
        else if(servo.duty<DEG_MIN)
        {
            servo.duty = DEG_MIN;
        }
    }
}

int16 Get_Mid_Average(uint8 foresight)
{
    int16 outvalu = 0;
    for(int8 i= -4; i <= 0; i++)
    {
        outvalu += 2 * (line.midline[foresight+i] - 80);   
    }
    for(int8 i = 1; i <= 5; i++)
    {
        outvalu += line.midline[foresight+i] - 80; 
    }
    outvalu = (int16)(outvalu / 15);
    return outvalu;
}

/**********************a little funcitons**********************/
/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	��
***************************************************************/
void servo_up1(void)
{
	servo.duty += 1;
}
void servo_up5(void)
{
	servo.duty += 5;
}
void servo_up10(void)
{
	servo.duty += 10;
}
void servo_down1(void)
{
	servo.duty -= 1;
}
void servo_down5(void)
{
	servo.duty -= 5;
}
void servo_down10(void)
{
	servo.duty -= 10;
}


#define FMAX    100
int PFF[7] = { -50,-35,-20,0,20,35,50};
int DFF[5] = { -9, -4, 0, 4, 9};
int UFF[7] = {  0,15,25,35,55,75,95};//
int    U;           /*ƫ��,ƫ��΢���Լ����ֵ�ľ�ȷ��*/
unsigned int   PF[2]={0}, DF[2]={0}, UF[4]={0};   /*ƫ��,ƫ��΢���Լ����ֵ��������*/
int    Pn, Dn, Un[4];
float   temp1, temp2;
float  a1=0,a2=0,a3=0,a4=0;

int rule[7][5]={
  //  ecС��-8Ϊ-2  ec����-8С��0Ϊ-1  ec����0С��8Ϊ0    ec>8Ϊ1 
//  -2  -1   0   1  2  ec    e 
  { 6,  5,  5,  4,  4}, //  -2 e<-45
  { 5,  4,  4,  3,  3}, //  -1   -45<e<-20
  { 3,  2,  2,  1,  1}, //  0  -20<e<-0
  { 1,  0,  0,  0,  1}, //   1    0<e<20
  { 1,  1,  2,  2,  3}, //   2    20<e<45
  { 3,  3,  4,  4,  5}, //   3    45<e
  { 4,  4,  5,  5,  6}  //   
};
int   Fuzzy(int P, int D)   /*ģ����������*/
{
	/*�����ȵ�ȷ��*/
	/*����PD��ָ������ֵ�����Ч������*/
	if (P>PFF[0] && P<PFF[6])
	{
		if (P <= PFF[1])
		{
			Pn = -2;
			PF[0] = (int)(FMAX*((float)(PFF[1] - P) / (PFF[1] - PFF[0])));//��pֵռ������ı�����Ϊ�ڸ�����ĸ���
		}
		else if (P <=PFF[2])
		{
			Pn = -1;
			PF[0] = (int)(FMAX*((float)(PFF[2] - P) / (PFF[2] - PFF[1])));
		}
		else if (P <= PFF[3])
		{
			Pn = 0;
			PF[0] = (int)(FMAX*((float)(PFF[3] - P) / (PFF[3] - PFF[2])));
		}
		else if (P <= PFF[4])
		{
			Pn = 1; 
			PF[0] = (int)(FMAX*((float)(PFF[4] - P) / (PFF[4] - PFF[3])));
		}
		else if (P <= PFF[5])
		{
			Pn = 2; 
			PF[0] = (int)(FMAX*((float)(PFF[5] - P) / (PFF[5] - PFF[4])));
		}
		else if (P <= PFF[6])
		{
			Pn = 3; 
			PF[0] =(int)( FMAX*((float)(PFF[6] - P) / (PFF[6] - PFF[5])));
		}
	}
	else if (P <= PFF[0])
	{
	  Pn = -2;  
	  PF[0] = FMAX;
	}
	else if (P >= PFF[6])
	{
		Pn = 3;   PF[0] = 0;
	}
	PF[1] = FMAX - PF[0];//��pֵ������������ĸ���
	
	
	if (D > DFF[0] && D < DFF[4])//-10~10
	{
	  if (D <=DFF[1])//-10~-5
	  {
	    Dn = -2; 
	    DF[0] = (int)(FMAX*((float)(DFF[1] - D) / (DFF[1] - DFF[0])));
	  }
	  else if (D <= DFF[2])//-5~-0
	  {
	    Dn = -1;
	    DF[0] =(int)( FMAX*((float)(DFF[2] - D) / (DFF[2] - DFF[1])));
	  }
	  else if (D <= DFF[3])//0~5
	  {
	    Dn = 0;
	    DF[0] =(int)( FMAX*((float)(DFF[3] - D) / (DFF[3] - DFF[2])));
	  }
	  else if (D <= DFF[4])//5~10
	  {
	    Dn = 1;
	    DF[0] =(int)( FMAX*((float)(DFF[4] - D) / (DFF[4] - DFF[3])));
	  }
	}
	else if (D <= DFF[0])
	{
	  Dn = -2;
	  DF[0] = FMAX;
	}
	else if (D >= DFF[4])
	{
	  Dn = 1;
	  DF[0] = 0;
	}
	DF[1] = FMAX - DF[0];
	/*ʹ����Χ�Ż���Ĺ����rule[7][7]*/
	/*���ֵʹ��13����������,����ֵ��UFF[7]ָ��*/
	/*һ�㶼���ĸ�������Ч*/
	Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
	Un[1] = rule[Pn + 3][Dn - 1 + 3];
	Un[2] = rule[Pn - 1 + 3][Dn + 3];
	Un[3] = rule[Pn + 3][Dn + 3];
	if (PF[0] <= DF[0])
		UF[0] = PF[0];//Un[0]ȡֵ�ĸ���Ӧ��Ϊp��������ĸ��ʺ�d���ڵ�������ʵ���Сֵ
	else
		UF[0] = DF[0];
	
	if (PF[1] <= DF[0])
		UF[1] = PF[1];//Un[1]ȡֵ�ĸ���Ӧ��Ϊp�������������������ұ����򣩵ĸ��ʺ�d���ڵ�������ʵ���Сֵ
	else
		UF[1] = DF[0];
	
	if (PF[0] <= DF[1])
		UF[2] = PF[0];//Un[2]ȡֵ�ĸ���Ӧ��Ϊp��������ĸ��ʺ�d�������ڵ������������ұ����򣩸��ʵ���Сֵ
	else
		UF[2] = DF[1];
	
	if (PF[1] <= DF[1])  //Un[3]ȡֵ�ĸ���Ӧ��Ϊp�������������������ұ����򣩵ĸ��ʺ�d�������ڵ������������ұ����򣩸��ʵ���Сֵ
		UF[3] = PF[1];
	else
		UF[3] = DF[1];
	/*ͬ���������������ֵ���*/

	if (Un[0] == Un[1])
	{
		if (UF[0] > UF[1])//UF[0] > UF[1]��Ϊ��Un[0]�ĸ��ʴ���Un[1]�ĸ���
			UF[1] = 0;//����С������
		else
			UF[0] = 0;
	}
	if (Un[0] == Un[2])
	{
		if (UF[0] > UF[2])
			UF[2] = 0;
		else
			UF[0] = 0;
	}
	if (Un[0] == Un[3])
	{
		if (UF[0] > UF[3])
			UF[3] = 0;
		else
			UF[0] = 0;
	}
	if (Un[1] == Un[2])
	{
		if (UF[1] > UF[2])
			UF[2] = 0;
		else
			UF[1] = 0;
	}
	if (Un[1] == Un[3])
	{
		if (UF[1] > UF[3])
			UF[3] = 0;
		else
			UF[1] = 0;
	}
	if (Un[2] == Un[3])
	{
		if (UF[2] > UF[3])
			UF[3] = 0;
		else
			UF[2] = 0;
	}

	a1=UF[0]*UFF[Un[0]];  
	a2=UF[1]*UFF[Un[1]];
	a3=UF[2]*UFF[Un[2]];
	a4=UF[3]*UFF[Un[3]];

	temp1 = a1+a2+a3+a4;
	temp2 = UF[0] + UF[1] + UF[2] + UF[3];
	U = (int)(temp1 / temp2);//��Ȩƽ��
	return U;
}




































