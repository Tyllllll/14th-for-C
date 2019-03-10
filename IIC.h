#ifndef _INFRARED_H_
#define _INFRARED_H_

#define IIC_I2Cx	I2C1
#define IIC_SCLPIN	PTC10
#define IIC_SDAPIN	PTC11

//定义SCL Bus Speed取值，外设总线为50Mhz时的计算结果
#define IIC_SCL_50KHZ	(0x33)
#define IIC_SCL_100KHZ	(0x2B)
#define IIC_SCL_150KHZ	(0x28)
#define IIC_SCL_200KHZ	(0x23)
#define IIC_SCL_250KHZ	(0x21)
#define IIC_SCL_300KHZ	(0x20)
#define IIC_SCL_400KHZ	(0x17)

#define MPU3050_DEV_ADDR	(0x1C)
#define VL53L0X_DEV_ADDR	(0x29)
#define	MPU6050_DEV_ADDR	(0x68)	//硬件I2C地址0x68 软件I2C地址0xD0

//MPU6050内部寄存器
#define	SMPLRT_DIV		(0x19)	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			(0x1A)	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		(0x1B)	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	(0x1C)	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	(0x3B)  //X轴加速度  加速度
#define	ACCEL_XOUT_L	(0x3C)
#define	ACCEL_YOUT_H	(0x3D)  //Y轴加速度
#define	ACCEL_YOUT_L	(0x3E)
#define	ACCEL_ZOUT_H	(0x3F)  //Z轴加速度
#define	ACCEL_ZOUT_L	(0x40)
#define	TEMP_OUT_H		(0x41)
#define	TEMP_OUT_L		(0x42)
#define	GYRO_XOUT_H		(0x43)  //X轴角速度  陀螺仪
#define	GYRO_XOUT_L		(0x44)	
#define	GYRO_YOUT_H		(0x45)  //Y轴角速度
#define	GYRO_YOUT_L		(0x46)
#define	GYRO_ZOUT_H		(0x47)  //Z轴角速度
#define	GYRO_ZOUT_L		(0x48)
#define	PWR_MGMT_1		(0x6B)	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		(0x75)	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	(0x68)	//硬件I2C地址0x68 软件I2C地址0xD0

typedef struct
{
	int16 distance;
}VL53L0X_Class;
extern VL53L0X_Class vl53l0x;

typedef struct
{
	uint8 angx;
	uint8 angy;
	uint8 angz;
	uint8 accx;
	uint8 accy;
	uint8 accz;
	int16 sum;
	int16 filter_neg;
	int16 filter_pos;
	int16 sum_set;
}Gyro_Class;
extern Gyro_Class gyro;

uint8 IIC_Init(void);
uint8 IIC_Read_Byte(uint8 addr, uint8 reg);
int16 IIC_Read_Word(uint8 addr, uint8 reg);
void IIC_Write_Byte(uint8 addr, uint8 reg, uint8 data);
void IIC_Delay(void);

void MPU6050_Init(void);
void Get_Angular_Velocity(void);
void Get_Acceleration(void);

void VL53L0X_Get_Distance(void);

#endif