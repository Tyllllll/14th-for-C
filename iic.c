#include "header.h"

static I2C_InitTypeDef I2C_InitStructure;
Gyro_Class gyro;
VL53L0X_Class vl53l0x;
float Acc_X,Acc_Y,Acc_Z,Gyro_X,Gyro_Y,Gyro_Z;
/***************************************************************
	*	@brief	红外初始化
	*	@param	无
	*	@note	无
***************************************************************/
uint8 IIC_Init(void)
{
	I2C_InitStructure.I2C_I2Cx = IIC_I2Cx;
	I2C_InitStructure.I2C_ICR = IIC_SCL_400KHZ;
	I2C_InitStructure.I2C_SclPin = IIC_SCLPIN;
	I2C_InitStructure.I2C_SdaPin = IIC_SDAPIN;
	I2C_InitStructure.I2C_IntEnable = FALSE;
	I2C_InitStructure.I2C_Isr = NULL;
	if(!LPLD_I2C_Init(I2C_InitStructure))
		return STATUS_FAILED;
	return STATUS_OK;
}

/***************************************************************
	*	@brief	读寄存器（8位）
	*	@param	addr：外设地址，reg：寄存器地址
	*	@note	无
***************************************************************/
uint8 IIC_Read_Byte(uint8 addr, uint8 reg)
{
	uint8 result;
	//发送从机地址
	LPLD_I2C_StartTrans(IIC_I2Cx, addr, I2C_MWSR);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//写寄存器地址
	LPLD_I2C_WriteByte(IIC_I2Cx, reg);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//再次产生开始信号
	LPLD_I2C_ReStart(IIC_I2Cx);
	//发送从机地址和读取位
	LPLD_I2C_WriteByte(IIC_I2Cx, (addr << 1 | I2C_MRSW));
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//转换主机模式为读
	LPLD_I2C_SetMasterWR(IIC_I2Cx, I2C_MRSW);
	//关闭应答ACK
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_OFF);//关闭ACK
	//读IIC数据
	result = LPLD_I2C_ReadByte(IIC_I2Cx);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//发送停止信号
	LPLD_I2C_Stop(IIC_I2Cx);
	//读IIC数据
	result = LPLD_I2C_ReadByte(IIC_I2Cx);
	
	IIC_Delay();
	return result;
}

/***************************************************************
	*	@brief	读寄存器（16位）
	*	@param	addr：外设地址，reg：寄存器地址，high_first：第一个字节是否为高8位
	*	@note	未测试
***************************************************************/
int16 IIC_Read_Word(uint8 addr, uint8 reg)
{
	int16 result_H;
	uint8 result_L;
	//发送从机地址
	LPLD_I2C_StartTrans(IIC_I2Cx, addr, I2C_MWSR);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//写寄存器地址
	LPLD_I2C_WriteByte(IIC_I2Cx, reg);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//再次产生开始信号
	LPLD_I2C_ReStart(IIC_I2Cx);
	//发送从机地址和读取位
	LPLD_I2C_WriteByte(IIC_I2Cx, (addr << 1 | I2C_MRSW));
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//转换主机模式为读
	LPLD_I2C_SetMasterWR(IIC_I2Cx, I2C_MRSW);
	//读IIC数据
	result_H = LPLD_I2C_ReadByte(IIC_I2Cx);
	IIC_I2Cx -> C1 &= ~I2C_C1_TXAK_MASK;
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//为什么写两遍
	result_H = LPLD_I2C_ReadByte(IIC_I2Cx);
	IIC_I2Cx -> C1 &= ~I2C_C1_TXAK_MASK;
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	
	result_L = LPLD_I2C_ReadByte(IIC_I2Cx);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_OFF);//关闭ACK
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	LPLD_I2C_Stop(IIC_I2Cx);
	
	IIC_Delay();
	int t = 0;
	while ((IIC_I2Cx -> S & I2C_S_BUSY_MASK))
	{
		LPLD_I2C_Stop(IIC_I2Cx);
		t++;
		if (t > 10)
		{
			LPLD_I2C_Deinit(I2C_InitStructure);
			LPLD_I2C_Init(I2C_InitStructure);
			if (t > 13)
			{
				break;
			}
		}
	}
	result_H = result_H << 8 | result_L;
	return result_H;
}

/***************************************************************
	*	@brief	写寄存器
	*	@param	addr：外设地址，reg：寄存器地址
	*	@note	无
***************************************************************/
void IIC_Write_Byte(uint8 addr, uint8 reg, uint8 data)
{
	//发送从机地址
	LPLD_I2C_StartTrans(IIC_I2Cx, addr, I2C_MWSR);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//写寄存器地址
	LPLD_I2C_WriteByte(IIC_I2Cx, reg);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//向寄存器中写具体数据
	LPLD_I2C_WriteByte(IIC_I2Cx, data);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	LPLD_I2C_Stop(IIC_I2Cx);
	IIC_Delay();
}

/***************************************************************
	*	@brief	IIC延时
	*	@param	无
	*	@note	无
***************************************************************/
void IIC_Delay(void)
{
	int16 i;
	for(i = 0; i < 10000; i++)
	{
		asm("nop");
	}
}

/***************************************************************
	*	@brief	MPU6050初始化
	*	@param	无
	*	@note	无
***************************************************************/
void MPU6050_Init(void)
{
	//配置电源和时钟源。这里是解除休眠状态
	IIC_Write_Byte(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);
	IIC_Delay();
	//SMPLRT_DIV指定陀螺仪采样率，0x00(对应1KHz)
	IIC_Write_Byte(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x00);
	IIC_Delay();
	//CONFIG用于配置数字低通滤波器，配置加速度计和陀螺仪的低通带宽和延时
	IIC_Write_Byte(MPU6050_DEV_ADDR, CONFIG, 0x03);
	IIC_Delay();
	//GYRO_CONFIG配置陀螺仪满量程范围，0x08,对应正负500度每秒
	IIC_Write_Byte(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x00);
	IIC_Delay();
	//ACCEL_CONFIG配置加速度计的输出量程，0x00对应正负2g
	IIC_Write_Byte(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x00);
	IIC_Delay();
}

/***************************************************************
	*	@brief	测零漂
	*	@param	无
	*	@note	无
***************************************************************/
uint8 MPU6050_Zero_Drift(void)
{
	gyro.accx_zero_drift = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_XOUT_H);
	gyro.angy_zero_drift = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_YOUT_H);
	gyro.angz_zero_drift = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_ZOUT_H);
	gyro.accx_zero_drift = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_XOUT_H);
	gyro.accy_zero_drift = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_YOUT_H);
	gyro.accz_zero_drift = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_ZOUT_H);
	return STATUS_OK;
}

/***************************************************************
	*	@brief	MPU6050获取角速度
	*	@param	无
	*	@note	无
***************************************************************/
void Get_Angular_Velocity(void)
{
	gyro.angx = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_XOUT_H) - gyro.accx_zero_drift;
	gyro.angy = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_YOUT_H) - gyro.angy_zero_drift;
	gyro.angz = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_ZOUT_H) - gyro.angz_zero_drift;
}

/***************************************************************
	*	@brief	MPU6050获取加速度
	*	@param	无
	*	@note	无
***************************************************************/
void Get_Acceleration(void)
{
	gyro.accx = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_XOUT_H) - gyro.accx_zero_drift;
	gyro.accy = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_YOUT_H) - gyro.accy_zero_drift;
	gyro.accz = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_ZOUT_H) - gyro.accx_zero_drift;
}

/***************************************************************
	*	@brief	VL53L0X获取距离
	*	@param	无
	*	@note	无
***************************************************************/
void VL53L0X_Get_Distance(void)
{
	int16 temp = IIC_Read_Word(VL53L0X_DEV_ADDR, 0x1e);
	if(temp != 20)
	{
		vl53l0x.distance = temp;
	}
	if(temp > 2500)
	{
		vl53l0x.distance = 2500;
	}
	IIC_Write_Byte(VL53L0X_DEV_ADDR, 0x00, 0x01);
//	vl53l0x.distance = IIC_Read_Byte(VL53L0X_DEV_ADDR, 0x1e) << 8 | IIC_Read_Byte(VL53L0X_DEV_ADDR, 0x1f);
}
