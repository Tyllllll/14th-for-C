#include "header.h"

VL53L0X_Class vl53l0x;

/***************************************************************
	*	@brief	�����ʼ��
	*	@param	��
	*	@note	��
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
	*	@brief	���Ĵ�����8λ��
	*	@param	addr�������ַ��reg���Ĵ�����ַ
	*	@note	��
***************************************************************/
uint8 IIC_Read_Byte(uint8 addr, uint8 reg)
{
	uint8 result;
	//���ʹӻ���ַ
	LPLD_I2C_StartTrans(IIC_I2Cx, addr, I2C_MWSR);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//д�Ĵ�����ַ
	LPLD_I2C_WriteByte(IIC_I2Cx, reg);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//�ٴβ�����ʼ�ź�
	LPLD_I2C_ReStart(IIC_I2Cx);
	//���ʹӻ���ַ�Ͷ�ȡλ
	LPLD_I2C_WriteByte(IIC_I2Cx, (addr << 1 | I2C_MRSW));
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//ת������ģʽΪ��
	LPLD_I2C_SetMasterWR(IIC_I2Cx, I2C_MRSW);
	//�ر�Ӧ��ACK
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_OFF);//�ر�ACK
	//��IIC����
	result = LPLD_I2C_ReadByte(IIC_I2Cx);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//����ֹͣ�ź�
	LPLD_I2C_Stop(IIC_I2Cx);
	//��IIC����
	result = LPLD_I2C_ReadByte(IIC_I2Cx);
	
	IIC_Delay();
	return result;
}

/***************************************************************
	*	@brief	���Ĵ�����16λ��
	*	@param	addr�������ַ��reg���Ĵ�����ַ��high_first����һ���ֽ��Ƿ�Ϊ��8λ
	*	@note	δ����
***************************************************************/
int16 IIC_Read_Word(uint8 addr, uint8 reg, uint8 high_first)
{
	uint8 result_L, result_H;
	//���ʹӻ���ַ
	LPLD_I2C_StartTrans(IIC_I2Cx, addr, I2C_MWSR);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//д�Ĵ�����ַ
	LPLD_I2C_WriteByte(IIC_I2Cx, reg);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//�ٴβ�����ʼ�ź�
	LPLD_I2C_ReStart(IIC_I2Cx);
	//���ʹӻ���ַ�Ͷ�ȡλ
	LPLD_I2C_WriteByte(IIC_I2Cx, (addr << 1 | I2C_MRSW));
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//ת������ģʽΪ��
	LPLD_I2C_SetMasterWR(IIC_I2Cx, I2C_MRSW);
	//��IIC����
	result_H = LPLD_I2C_ReadByte(IIC_I2Cx);
	IIC_I2Cx -> C1 &= ~I2C_C1_TXAK_MASK;
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//Ϊʲôд����
	result_H = LPLD_I2C_ReadByte(IIC_I2Cx);
	IIC_I2Cx -> C1 &= ~I2C_C1_TXAK_MASK;
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	
	result_L = LPLD_I2C_ReadByte(IIC_I2Cx);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_OFF);//�ر�ACK
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
	*	@brief	д�Ĵ���
	*	@param	addr�������ַ��reg���Ĵ�����ַ
	*	@note	��
***************************************************************/
void IIC_Write_Byte(uint8 addr, uint8 reg, uint8 data)
{
	//���ʹӻ���ַ
	LPLD_I2C_StartTrans(IIC_I2Cx, addr, I2C_MWSR);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//д�Ĵ�����ַ
	LPLD_I2C_WriteByte(IIC_I2Cx, reg);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	//��Ĵ�����д��������
	LPLD_I2C_WriteByte(IIC_I2Cx, data);
	LPLD_I2C_WaitAck(IIC_I2Cx, I2C_ACK_ON);
	LPLD_I2C_Stop(IIC_I2Cx);
	IIC_Delay();
}

/***************************************************************
	*	@brief	IIC��ʱ
	*	@param	��
	*	@note	��
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
	*	@brief	VL53L0X�Ĵ�����¼
	*	@param	��
	*	@note	��
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