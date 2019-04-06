#include "header.h"

static I2C_InitTypeDef I2C_InitStructure;
Gyro_Class gyro;
VL53L0X_Class vl53l0x;
float Acc_X,Acc_Y,Acc_Z,Gyro_X,Gyro_Y,Gyro_Z;
/***************************************************************
	*	@brief	�����ʼ��
	*	@param	��
	*	@note	��
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
int16 IIC_Read_Word(uint8 addr, uint8 reg)
{
	int16 result_H;
	uint8 result_L;
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
	for(i = 0; i < 10000; i++)
	{
		asm("nop");
	}
}

/***************************************************************
	*	@brief	MPU6050��ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
void MPU6050_Init(void)
{
	//���õ�Դ��ʱ��Դ�������ǽ������״̬
	IIC_Write_Byte(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);
	IIC_Delay();
	//SMPLRT_DIVָ�������ǲ����ʣ�0x00(��Ӧ1KHz)
	IIC_Write_Byte(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x00);
	IIC_Delay();
	//CONFIG�����������ֵ�ͨ�˲��������ü��ٶȼƺ������ǵĵ�ͨ�������ʱ
	IIC_Write_Byte(MPU6050_DEV_ADDR, CONFIG, 0x03);
	IIC_Delay();
	//GYRO_CONFIG���������������̷�Χ��0x08,��Ӧ����500��ÿ��
	IIC_Write_Byte(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x00);
	IIC_Delay();
	//ACCEL_CONFIG���ü��ٶȼƵ�������̣�0x00��Ӧ����2g
	IIC_Write_Byte(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x00);
	IIC_Delay();
}

/***************************************************************
	*	@brief	����Ư
	*	@param	��
	*	@note	��
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
	*	@brief	MPU6050��ȡ���ٶ�
	*	@param	��
	*	@note	��
***************************************************************/
void Get_Angular_Velocity(void)
{
	gyro.angx = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_XOUT_H) - gyro.accx_zero_drift;
	gyro.angy = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_YOUT_H) - gyro.angy_zero_drift;
	gyro.angz = IIC_Read_Word(MPU6050_DEV_ADDR, GYRO_ZOUT_H) - gyro.angz_zero_drift;
}

/***************************************************************
	*	@brief	MPU6050��ȡ���ٶ�
	*	@param	��
	*	@note	��
***************************************************************/
void Get_Acceleration(void)
{
	gyro.accx = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_XOUT_H) - gyro.accx_zero_drift;
	gyro.accy = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_YOUT_H) - gyro.accy_zero_drift;
	gyro.accz = IIC_Read_Word(MPU6050_DEV_ADDR, ACCEL_ZOUT_H) - gyro.accx_zero_drift;
}

/***************************************************************
	*	@brief	VL53L0X��ȡ����
	*	@param	��
	*	@note	��
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
