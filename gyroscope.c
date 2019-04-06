#include "header.h"


float Acc_X,Acc_Y,Acc_Z,Gyro_X,Gyro_Y,Gyro_Z;
int read_buff[6];
float angle_speed[4]={0};
I2C_CLASS i2c=
{
    &I2C_Init,
    &Get_Attitude

};
GYRO_CLASS gyro;

//float temp;
void Get_Attitude()
{
//	static float s=0;
	static float faiZ,faiY;
	float ratio_gyro=0.048;//�������ǵ�ֵת��Ϊ ��ÿ�� ��ϵ��
//	float ratio_acc =90.0/8192;//�����ٶȵ�ֵ��һ��Ϊ-90��90�����ƽǶȣ� ��ϵ��

//	Acc_X =   (float)(Get_X_Acc());//LPButterworth((float)(Get_X_Acc()),&Butter_Buffer_ACC_X,&Butter_30HZ_Parameter_Acce);
	Acc_Y = (float)(Get_Y_Acc());//LPButterworth((float)(Get_Y_Acc()),&Butter_Buffer_ACC_Y,&Butter_30HZ_Parameter_Acce);
	Acc_Z = (float)(Get_Z_Acc());//LPButterworth((float)(Get_Z_Acc()),&Butter_Buffer_ACC_Z,&Butter_10HZ_Parameter_Acce);//  
	Gyro_X = LPButterworth((float)(Get_X_Gyro() - 240.5), &Butter_Buffer_GyroX, &Butter_10HZ_Parameter_Acce);//(float)(Get_X_Gyro() + 260);//
	Gyro_Y = LPButterworth((float)(Get_Y_Gyro() - 100), &Butter_Buffer_GyroY, &Butter_10HZ_Parameter_Acce); //(float)(Get_Y_Gyro()+ 332);//    
	//Gyro_Z= LPButterworth((float)(Get_Z_Gyro() + 14),&Butter_Buffer_GyroZ,&Butter_30HZ_Parameter_Acce); 

	gyro.Turn_Speed = 0.8 * gyro.Turn_Speed + 0.2 * Gyro_X * ratio_gyro;
	faiZ = Acc_Z / 16384.0f;
	if (faiZ > 1)
		faiZ = 1;
	if (faiZ < -1)
		faiZ = -1;
	gyro.ACC_Angle = asin(faiZ) / 0.01745329;

	faiY = Acc_Y / 16384.0f;
	if (faiY > 1)
		faiY = 1;
	if (faiY < -1)
		faiY = -1;
	gyro.Gravity_Angle = 0.99 * gyro.Gravity_Angle + 0.01 * asin(faiY) / 0.01745329;
	gyro.Angle_Speed = -Gyro_Y * 0.03051757;

	/*angle_speed[3]=angle_speed[2];
	angle_speed[2]=angle_speed[1];
	angle_speed[1]=angle_speed[0];
	angle_speed[0]=s;
	Angle_Speed=0.4*angle_speed[0]+0.3*angle_speed[1]+0.2*angle_speed[2]+0.1*angle_speed[3];
	*/

	Kalman_Filter(gyro.ACC_Angle, gyro.Angle_Speed);            //���ÿ������˲�����
	
    gyro.CarY_Angle = 0.9 * gyro.CarY_Angle + 0.1 * gyro.Gravity_Angle;
	//���ǶȻ��� 
//	if((Findline.Process == Right_Roundabout_IN || Findline.Process == Right_Roundabout_OUT
//		|| Findline.Process == Left_Roundabout_IN || Findline.Process == Left_Roundabout_OUT)
//			|| (Findline.Process == Crossmeeting && crossmeet.PostureIntegral == 1)
//				|| (Findline.Process == Transome && transome.PostureIntegral == 1)//�ᳵ�ǶȻ���
//	)
//    if(feature.roundabouts_state != 0)
    if(feature.roadblock_state == 1 || feature.roadblock_state == 2)
        gyro.TurnAngle_Integral += gyro.Turn_Speed / 1000.0;
	else
		gyro.TurnAngle_Integral = 0;
}


//float Q_angle=0.001, Q_gyro=0.003, R_angle=10

//float Q_angle=0.50, Q_gyro=0.070, R_angle=4;
//float Q_angle=0.2, Q_gyro=0.1, R_angle=0.3;
//float Q_angle=0.02, Q_gyro=0.01, R_angle=0.03;
//float Q_angle=0.01, Q_gyro=0.003, R_angle=10;
//float Q_angle=0.001, Q_gyro=0.1, R_angle=10;
//float Q_angle=0.01, Q_gyro=0.1, R_angle=10;
static float Q_angle=0.001, Q_gyro=0.003, R_angle=10, dt=0.005;
	//Q���󣬶�̬��Ӧ����
static float Pk[2][2] = { {1, 0}, {0, 1 }};
	
static float Pdot[4] ={0,0,0,0};

static float q_bias=0, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;



//-------------------------------------------------------
void Kalman_Filter(float angle_m,float gyro_m)			
{
	gyro.Car_Angle += (gyro_m - q_bias) * dt; ///Ԥ��ֵ
	Pdot[0] = Q_angle - Pk[0][1] - Pk[1][0];
	Pdot[1] = -Pk[1][1];
	Pdot[2] = -Pk[1][1];
	Pdot[3] = Q_gyro;
	
	Pk[0][0] += Pdot[0] * dt;
	Pk[0][1] += Pdot[1] * dt;
	Pk[1][0] += Pdot[2] * dt;
	Pk[1][1] += Pdot[3] * dt;
	
	angle_err = angle_m - gyro.Car_Angle;///����ֵ-Ԥ��ֵ
	
	PCt_0 = Pk[0][0];
	PCt_1 = Pk[1][0];
	
	E = R_angle + PCt_0;
	
	K_0 = PCt_0 / E; ///����������
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = Pk[0][1];

	Pk[0][0] -= K_0 * t_0;
	Pk[0][1] -= K_0 * t_1;
	Pk[1][0] -= K_1 * t_0;
	Pk[1][1] -= K_1 * t_1;
	
	gyro.Car_Angle += K_0 * angle_err; ///���ŽǶ�=Ԥ��ֵ+����������*(����ֵ-Ԥ��ֵ)
	q_bias += K_1 * angle_err;
	gyro.Angle_Speed = gyro_m-q_bias;
}


/*
 * I2C_Init
 * ��ʼ��I2C��������
 */
void I2C_Init(void)
{ 
	//��ʼ��SCL�ܽ�
	//��ʼ��SDA�ܽ�
	static GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_PTx = PTC;
	GPIO_InitStructure1.GPIO_Pins = GPIO_Pin10 | GPIO_Pin11;
	GPIO_InitStructure1.GPIO_Dir = DIR_OUTPUT;
	GPIO_InitStructure1.GPIO_Output = OUTPUT_L;
	LPLD_GPIO_Init(GPIO_InitStructure1);

	MPU3050_Init();
	MMA8451_Init();
	Butterworth_Parameter_Init();//�˲���ʼ��
}
/*
 * I2C_WriteReg
 * дI2C�豸�Ĵ���
 */
void I2C_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data)
{		
  I2C_Start();
  I2C_SendByte(dev_addr<<1);         
  I2C_SendByte(reg_addr );   
  I2C_SendByte(data);   
  I2C_Stop(); 
}
//���Ĵ���
uint8 I2C_ReadByte(uint8 dev_addr,uint8 reg_addr)
{
  uint8 data;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr ); 
  I2C_Start();	
  I2C_SendByte((dev_addr<<1)+1); 
  data= I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return data;
}

//���Ĵ���
int16 I2C_ReadWord(uint8 dev_addr,uint8 reg_addr)
{
  char h,l;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr); 
  I2C_Start();	
  I2C_SendByte((dev_addr<<1)+1); 
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return (h<<8)+l;
}

void I2C_ReadGryo(uint8 dev_addr,uint8 reg_addr,int16 *x,int16 *y)
{
  char h,l;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr); 
  I2C_Start();	
  I2C_SendByte((dev_addr<<1)+1); 
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_Ack();
  *x=(h<<8)+l;
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_Ack();
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_NoAck();
  *y=(h<<8)+l;
  I2C_Stop();
}
/*
 * I2C_Start
 * I2C��ʼ�źţ��ڲ�����
 */
void I2C_Start(void)
{
  I2C_SDA_OUT();
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=1; 
  I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=0; 
  I2C_DELAY();
  I2C_DELAY();
  I2C_SCL=0;
  I2C_DELAY();
  I2C_DELAY();
}

/*
 * I2C_Stop
 * I2Cֹͣ�źţ��ڲ�����
 */
 void I2C_Stop(void)
{ 
    I2C_SDA_O=0;
    I2C_SCL=0; 
  I2C_DELAY();
  I2C_DELAY();
    I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
    I2C_SDA_O=1;
  I2C_DELAY();
  I2C_DELAY();
    I2C_SCL=0;
}

/*
 * I2C_Stop
 * I2CӦ���źţ��ڲ�����
 */
void I2C_Ack(void)
{	
  I2C_SCL=0;
  I2C_DELAY();
  
  I2C_SDA_O=0;
  I2C_DELAY();
  
  I2C_SCL=1;
  I2C_DELAY();
  
  I2C_SCL=0;
  I2C_DELAY();
}

/*
 * I2C_NoAck
 * I2C��Ӧ���źţ��ڲ�����
 */
 void I2C_NoAck(void)
{	
  I2C_SCL=0;
  I2C_DELAY();
  I2C_SDA_O=1;
  I2C_DELAY();
  I2C_SCL=1;
  I2C_DELAY();
  I2C_SCL=0;
  I2C_DELAY();
}


/*
 * I2C_SendByte
 * I2C�������ݣ��ڲ�����
 */
void I2C_SendByte(uint8 data) 
{
  uint8 i=8;
  while(i--)
  {
    I2C_SCL=0;
    if(data&0x80)
      I2C_SDA_O=1; 
    else 
      I2C_SDA_O=0;   
    data<<=1;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();
  }
  I2C_SCL=0;
  I2C_DELAY();
  I2C_SDA_I=1;
  I2C_DELAY();
  I2C_SCL=1; 
  I2C_DELAY();
  I2C_SCL=0;
}

/*
 * I2C_SendByte
 * I2C�������ݣ��ڲ�����
 */
 uint8 I2C_ReceiveByte(void)  
{ 
  uint8 i=8;
  uint8 ReceiveByte=0;
  
  I2C_SDA_O=1;	
  I2C_SDA_IN();	
  
  while(i--)
  {
    ReceiveByte<<=1;      
    I2C_SCL=0;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();	
    if(I2C_SDA_I)
    {
      ReceiveByte|=0x01;
    }
    
  }
  I2C_SDA_OUT();
  I2C_SCL=0;
  
  return ReceiveByte;
}

/*
 * I2C_SendByte
 * I2C��ʱ�������ڲ�����
 */
void I2C_DELAY()
{
  int i=10; //48mhz��Ƶ��ԼΪ 400k 
  while(i) 
    i--; 
}

/******************���Z����ٶ�******************/
int16 Get_X_Acc()
{
  int16 temp;
  temp=I2C_ReadWord(I2C_MMA8451_ADR,0X01);
  return temp;
}
/******************���Z����ٶ�******************/
int16 Get_Y_Acc()
{
  int16 temp;
  temp=I2C_ReadWord(I2C_MMA8451_ADR,0X03);
  return temp;
}
/******************���Z����ٶ�******************/
int16 Get_Z_Acc()
{
  int16 temp;
  temp=I2C_ReadWord(I2C_MMA8451_ADR,0X05);
  return temp;
}
/*****************���x����ٶ�*****************/
int16 Get_X_Gyro()
{
  int16 temp;
  temp=I2C_ReadWord(I2C_MPU3050_ADR,0X1D);
  return temp;
}
/*****************���y����ٶ�*****************/
int16 Get_Y_Gyro()
{
  int16 temp; 
  temp=I2C_ReadWord(I2C_MPU3050_ADR,0X1f);
  return temp;
}
/*****************���Z����ٶ�*****************/
int16 Get_Z_Gyro()
{
  int16 temp; 
  temp=I2C_ReadWord(I2C_MPU3050_ADR,0X21);
  return temp;
}

void MPU3050_Init()
{
	I2C_WriteReg(I2C_MPU3050_ADR,0x3E , 0X80); //��λMPU3050
	I2C_DELAY();
	I2C_WriteReg(I2C_MPU3050_ADR,0x15,  0);    //��������Ƶ
	I2C_DELAY();
	I2C_WriteReg(I2C_MPU3050_ADR,0x16 , 0x11);  //��������8kHz&&������Χ��1000��
	I2C_DELAY();
	I2C_WriteReg(I2C_MPU3050_ADR,0x17 , 0);    //�������ж�
	I2C_DELAY();
	I2C_WriteReg(I2C_MPU3050_ADR,0x3E , 0X00); //����MPU3050
}

void MMA8451_Init()
{ 
	I2C_WriteReg(I2C_MMA8451_ADR,0x0E, 0);  //0Ϊ+-2g��1Ϊ+-4g��2Ϊ+-8g
	I2C_DELAY();
	I2C_WriteReg(I2C_MMA8451_ADR,0x0F, 0X10);  // ʹ�ܵ�ͨ�˲�
	I2C_DELAY();
	I2C_WriteReg(I2C_MMA8451_ADR,0x2A , 1); //0x2aΪ���ƼĴ���1 ����mma8451
	I2C_DELAY();
}
	
