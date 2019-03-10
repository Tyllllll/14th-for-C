#ifndef _INFRARED_H_
#define _INFRARED_H_

#define IIC_I2Cx	I2C1
#define IIC_SCLPIN	PTC10
#define IIC_SDAPIN	PTC11

//����SCL Bus Speedȡֵ����������Ϊ50Mhzʱ�ļ�����
#define IIC_SCL_50KHZ	(0x33)
#define IIC_SCL_100KHZ	(0x2B)
#define IIC_SCL_150KHZ	(0x28)
#define IIC_SCL_200KHZ	(0x23)
#define IIC_SCL_250KHZ	(0x21)
#define IIC_SCL_300KHZ	(0x20)
#define IIC_SCL_400KHZ	(0x17)

#define MPU3050_DEV_ADDR	(0x1C)
#define VL53L0X_DEV_ADDR	(0x29)
#define	MPU6050_DEV_ADDR	(0x68)	//Ӳ��I2C��ַ0x68 ���I2C��ַ0xD0

//MPU6050�ڲ��Ĵ���
#define	SMPLRT_DIV		(0x19)	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			(0x1A)	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		(0x1B)	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	(0x1C)	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	(0x3B)  //X����ٶ�  ���ٶ�
#define	ACCEL_XOUT_L	(0x3C)
#define	ACCEL_YOUT_H	(0x3D)  //Y����ٶ�
#define	ACCEL_YOUT_L	(0x3E)
#define	ACCEL_ZOUT_H	(0x3F)  //Z����ٶ�
#define	ACCEL_ZOUT_L	(0x40)
#define	TEMP_OUT_H		(0x41)
#define	TEMP_OUT_L		(0x42)
#define	GYRO_XOUT_H		(0x43)  //X����ٶ�  ������
#define	GYRO_XOUT_L		(0x44)	
#define	GYRO_YOUT_H		(0x45)  //Y����ٶ�
#define	GYRO_YOUT_L		(0x46)
#define	GYRO_ZOUT_H		(0x47)  //Z����ٶ�
#define	GYRO_ZOUT_L		(0x48)
#define	PWR_MGMT_1		(0x6B)	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		(0x75)	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	(0x68)	//Ӳ��I2C��ַ0x68 ���I2C��ַ0xD0

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