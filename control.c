#include "header.h"

/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	�޷��иĶ�
***************************************************************/
void Curve_Fill(void)
{
	uint8 i = 0;
	if(feature.turn_state != 0)
	{
		//��С��
		if(feature.turn_state == 1)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.right_line[i] == 159)
				{
					//���߲�������������ƫ
					line.midline[i] = line.midline[i + 1] - 1;
				}
				else
				{
					//���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
					line.midline[i] = line.midline[i + 1] - (line.right_line[i + 1] - line.right_line[i]);
				}
				if(line.midline[i] <= 0)
				{
//					line.left_line[i] = 0;
//					line.right_line[i] = 0;
					line.midline[i] = 0;
				}
			}
		}
		//��С��
		else if(feature.turn_state == 2)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.left_line[i] == 0)
				{
					//���߲�������������ƫ
					line.midline[i] = line.midline[i + 1] + 1;
				}
				else
				{
					//���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
					line.midline[i] = line.midline[i + 1] - (line.left_line[i] - line.left_line[i + 1]);
				}
				if(line.midline[i] >= 159)
				{
//					line.left_line[i] = 159;
//					line.right_line[i] = 159;
					line.midline[i] = 159;
				}
			}
		}
		//������
		else if(feature.turn_state == 3)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//ֱ�����ұ߽��ȥ80�������߹켣����ƫ����ȸ���
				line.midline[i] = line.right_line[i] - 80;
				if(line.midline[i] <= 0)
				{
//					line.left_line[i] = 0;
//					line.right_line[i] = 0;
					line.midline[i] = 0;
				}
			}
		}
		//������
		else if(feature.turn_state == 4)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//ֱ������߽��80�������߹켣����ƫ����ȸ���
				line.midline[i] = line.left_line[i] + 80;
				if(line.midline[i] >= 159)
				{
//					line.left_line[i] = 159;
//					line.right_line[i] = 159;
					line.midline[i] = 159;
				}
			}
		}
	}
}

/***************************************************************
	*	@brief	ʮ�ֲ���
	*	@param	��
	*	@note	��
***************************************************************/
void Cross_Fill(void)
{
	if(feature.cross_state[1] == 1)
	{
		if(feature.left_flection_flag == 1 && feature.right_flection_row == 1)
		{
			
		}
	}
}

/***************************************************************
	*	@brief	��������
	*	@param	��
	*	@note	��
***************************************************************/
void Roundabouts_Fill(void)
{
	
}

/***************************************************************
	*	@brief	�ٶ��趨
	*	@param	��
	*	@note	��
***************************************************************/
void Speed_Set(void)
{
	
}