#include "header.h"

Line_Class line;
Feature_Class feature;

/***************************************************************
	*	@brief	Ѱ��
	*	@param	��
	*	@note	Ѱ�����ұ߽缰��������
***************************************************************/
void Find_Line(void)
{
	uint8 i = 0, j = 0;
	uint8 column_start = 80;//����Ѱ��ÿ����ʼ������
	for(i = 100; i >5; i--)
	{
		//����������ɨ
		for(j = column_start; j > 2; j--)
		{
			//Ѱ���������
			if(camera.image[i][j] != 0 && camera.image[i][j - 2] == 0)
			{
				line.leftline[i] = j - 1;
				line.leftlineflag[i] = 1;
				break;
			}
		}
		//����������ɨ
		for(j = column_start; j < 157; j++)
		{
			//Ѱ���������
			if(camera.image[i][j] != 0 && camera.image[i][j + 2] == 0)
			{
				line.rightline[i] = j + 1;
				line.rightlineflag[i] = 1;
				break;
			}
		}
		//��������
		if(line.leftlineflag[i] && line.rightlineflag[i])
		{
			line.midline[i] = (line.leftline[i] + line.rightline[i]) / 2;
		}
		else if(line.leftlineflag[i] && !line.rightlineflag[i])
		{
			line.rightline[i] = 159;
			line.midline[i] = (line.leftline[i] + line.rightline[i]) / 2;
		}
		else if(!line.leftlineflag[i] && line.rightlineflag[i])
		{
			line.leftline[i] = 0;
			line.midline[i] = line.rightline[i] / 2;
		}
		else if(!line.leftlineflag[i] && !line.rightlineflag[i])
		{
			line.leftline[i] = 0;
			line.rightline[i] = 159;
			if(100 == i)
			{
				line.midline[i] = 80;
			}
			else
			{
				line.midline[i] = line.midline[i + 1];
			}
		}
		column_start = line.midline[i];
	}
}

/***************************************************************
	*	@brief	�ж�����
	*	@param	��
	*	@note	��
***************************************************************/
void Judge_Feature(void)
{
	Find_Toppoint();
	Find_Inflection();
}

/***************************************************************
	*	@brief	�Ҷ���
	*	@param	��
	*	@note	��
***************************************************************/
void Find_Toppoint(void)
{
	uint8 i = 0;
	feature.toppoint = 0;
	for(i = 100; i > 5; i--)
	{
		if(camera.image[i][line.midline[i]] != 0 && (camera.image[i - 1][line.midline[i]] == 0 || camera.image[i - 3][line.midline[i]] == 0 || camera.image[i - 5][line.midline[i]] == 0))
		{
			feature.toppoint = i;
			break;
		}
	}
	if(feature.toppoint == 0)
	{
		for(i = 118; i > 2; i--)
		{
			if(line.midline[i] > 150 || line.midline[i] < 10)
			{
				feature.toppoint = i;
				break;
			}
		}
	}
}

/***************************************************************
	*	@brief	�ҹյ�
	*	@param	��
	*	@note	��
***************************************************************/
void Find_Inflection(void)
{
	uint8 i = 0;
	if(feature.toppoint < 70 && feature.toppoint > 5)
	{
		for(i = 108; i > feature.toppoint + 4; i--)
		{
			
		}
	}
}
