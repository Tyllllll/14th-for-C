#include "header.h"

VL53L0X_Class vl53l0x;

/***************************************************************
	*	@brief	红外初始化
	*	@param	无
	*	@note	无
***************************************************************/
void IIC_Init(void)
{
	static I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_I2Cx = IIC_I2Cx;
	I2C_InitStructure.I2C_ICR = IIC_SCL_400KHZ;
	I2C_InitStructure.I2C_SclPin = IIC_SCLPIN;
	I2C_InitStructure.I2C_SdaPin = IIC_SDAPIN;
	I2C_InitStructure.I2C_IntEnable = FALSE;
	I2C_InitStructure.I2C_Isr = NULL;
	LPLD_I2C_Init(I2C_InitStructure);
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
int16 IIC_Read_Word(uint8 addr, uint8 reg, uint8 high_first)
{
	uint8 result_L, result_H;
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
			IIC_Init();
			if (t > 13)
			{
				break;
			}
		}
	}
	if(high_first == 1)
		result_H = result_L << 8 | result_H;
	else
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
	for(i = 0; i < 200; i++)
	{
		asm("nop");
	}
}

/***************************************************************
	*	@brief	VL53L0X寄存器记录
	*	@param	无
	*	@note	无
***************************************************************/
void 
VL53L0X_test(void)
{
	int i = 0;
	for(i = 0; i < 0xff; i++)
	{
		vl53l0x.value[i] = IIC_Read_Byte(VL53L0X_DEV_ADDR, i);
	}
	
}