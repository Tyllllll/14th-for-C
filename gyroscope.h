#ifndef _GYROSCOPE_H_
#define _GYROSCOPE_H_

#include "header.h"
   

//定义I2C设备地址
#define I2C_MMA8451_ADR  0X1C
#define I2C_MPU3050_ADR  0x68

//#define I2C_SCL_PIN    PTC10
//#define I2C_SDA_PIN    PTC11

#define I2C_SCL        PTC10_O   
#define I2C_SDA_I      PTC11_I
#define I2C_SDA_O      PTC11_O 
//定义SDA输入输出
#define I2C_SDA_OUT()   PTC->PDDR |= GPIO_Pin11;  //设置为输出
#define I2C_SDA_IN()    PTC->PDDR &= ~(GPIO_Pin11);//设置为输入

typedef struct
{
	void (*I2C_Init)(void);
	void (*Get_Attitude)(void);
} I2C_CLASS;

extern I2C_CLASS i2c;

typedef struct
{	
  float ACC_Angle;
  float Gravity_Angle;
  float Car_Angle;
  float CarY_Angle;
  float Angle_Speed;
  float Turn_Speed; 
  float TurnAngle_Integral; 
  uint8 speed_err_flag;
	
} GYRO_CLASS;
extern GYRO_CLASS gyro;

extern int read_buff[6];  
extern float Acc_X,Acc_Y,Acc_Z,Gyro_X,Gyro_Y,Gyro_Z;
void Get_Attitude();
void Kalman_Filter(float angle_m,float gyro_m);
void Kalman_Filter2(float angle_m,float gyro_m);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void I2C_SendByte(uint8);
uint8 I2C_ReceiveByte(void);
void I2C_DELAY();

void I2C_Init(void);

void I2C_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data);

uint8 I2C_ReadByte(uint8 dev_addr,uint8 reg_addr);//读一个字节的数据

int16 I2C_ReadWord(uint8 dev_addr,uint8 reg_addr);//读两个字节的数据

void I2C_ReadGryo(uint8 dev_addr,uint8 reg_addr,int16 *x,int16 *y);

int16 Get_X_Acc();
int16 Get_Y_Acc();
int16 Get_Z_Acc();

int16 Get_X_Gyro();
int16 Get_Y_Gyro();
int16 Get_Z_Gyro();
  //MPU3050初始化
void  MPU3050_Init();
  //MMA8451初始化
void  MMA8451_Init();


#endif 
