#ifndef _INFRARED_H_
#define _INFRARED_H_

#define IIC_I2Cx	I2C1
#define IIC_SCLPIN	PTC10
#define IIC_SDAPIN	PTC11

//定义SCL Bus Speed取值，外设总线为50Mhz时的计算结果
#define IIC_SCL_50KHZ	0x33
#define IIC_SCL_100KHZ	0x2B
#define IIC_SCL_150KHZ	0x28
#define IIC_SCL_200KHZ	0x23
#define IIC_SCL_250KHZ	0x21
#define IIC_SCL_300KHZ	0x20
#define IIC_SCL_400KHZ	0x17

#define MPU3050_DEV_ADDR	0x1C
#define VL53L0X_DEV_ADDR	0x29

typedef struct
{
	uint8 temp_addr;
	uint8 temp_value;
	uint8 value[255];
}VL53L0X_Class;
extern VL53L0X_Class vl53l0x;

void IIC_Init(void);
uint8 IIC_Read_Byte(uint8 addr, uint8 reg);
int16 IIC_Read_Word(uint8 addr, uint8 reg, uint8 high_first);
void IIC_Write_Byte(uint8 addr, uint8 reg, uint8 data);
void IIC_Delay(void);

void VL53L0X_test(void);

#endif