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
				line.left_line[i] = j - 1;
				line.left_line_flag[i] = 1;
				break;
			}
		}
		//����������ɨ
		for(j = column_start; j < 157; j++)
		{
			//Ѱ���������
			if(camera.image[i][j] != 0 && camera.image[i][j + 2] == 0)
			{
				line.right_line[i] = j + 1;
				line.right_line_flag[i] = 1;
				break;
			}
		}
		//��������
		if(line.left_line_flag[i] && line.right_line_flag[i])
		{
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
		else if(line.left_line_flag[i] && !line.right_line_flag[i])
		{
			line.right_line[i] = 159;
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
		else if(!line.left_line_flag[i] && line.right_line_flag[i])
		{
			line.left_line[i] = 0;
			line.midline[i] = line.right_line[i] / 2;
		}
		else if(!line.left_line_flag[i] && !line.right_line_flag[i])
		{
			line.left_line[i] = 0;
			line.right_line[i] = 159;
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
	//��������
	feature.top_point = 0;
	feature.left_flection_flag = 0;
	feature.left_flection_row = 0;
	feature.right_flection_flag = 0;
	feature.right_flection_row = 0;
	
	Find_Top_Point();
	Find_Inflection();
	Find_Inflection2();
	if(feature.top_point != 0)
	{
		Judge_Curve();
	}
}

/***************************************************************
	*	@brief	�Ҷ���
	*	@param	��
	*	@note	����Ķ���Ϊĳһ�е������ϵ�Ϊ��ɫ�㣬���Ǹ������еĺ���ĵ�Ϊ�ڵ㣬�������߽�
***************************************************************/
void Find_Top_Point(void)
{
	uint8 i = 0;
	feature.top_point = 0;
	for(i = 100; i > 5; i--)
	{
		if(camera.image[i][line.midline[i]] != 0 && (camera.image[i - 1][line.midline[i]] == 0 || camera.image[i - 3][line.midline[i]] == 0 || camera.image[i - 5][line.midline[i]] == 0))
		{
			feature.top_point = i;
			break;
		}
	}
	if(feature.top_point == 0)
	{
		for(i = 118; i > 2; i--)
		{
			if(line.midline[i] > 150 || line.midline[i] < 10)
			{
				feature.top_point = i;
				break;
			}
		}
	}
}

/***************************************************************
	*	@brief	�ҹյ�
	*	@param	��
	*	@note	ʮ�ֽ��㴦�յ㣨͸��ͼ�������С����
***************************************************************/
void Find_Inflection(void)
{
	uint8 i = 0;
	if(feature.top_point < 100 && feature.top_point > 10)
	{
		//����յ�
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.left_flection_flag == 0)
			{
				//λ������
				if(i > 35 && line.left_line[i] < 140)
				{
					//��С����
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] > line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5] &&
					   line.left_line[i - 3] - line.left_line[i + 3] < 4)
						//���һ������������
					{
						//����������
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i + 3] == 1 && line.left_line_flag[i + 5] == 1)
						{
							//б������
							if(line.left_line[i] - line.left_line[i - 3] - (line.left_line[i + 3] - line.left_line[i]) > 4)
							{
								feature.left_flection_row = i;
								feature.left_flection_flag = 1;
							}
						}
					}
				}
			}
		}
		//���ҹյ�
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.right_flection_flag == 0)
			{
				//λ������
				if(i > 35 && line.right_line[i] > 20)
				{
					//��С����
					if(line.right_line[i] < line.right_line[i + 3] && line.left_line[i] < line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5] &&
					   line.right_line[i - 3] - line.right_line[i + 3] > -4)
						//���һ������������
					{
						//����������
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i + 3] == 1 && line.right_line_flag[i + 5] == 1)
						{
							//б������
							if(line.right_line[i + 3] - line.right_line[i] - (line.right_line[i] - line.right_line[i - 3]) > 4)
							{
								feature.right_flection_row = i;
								feature.right_flection_flag = 1;
							}
						}
					}
				}
			}
		}
	}
}

/***************************************************************
	*	@brief	�ҹյ�2
	*	@param	��
	*	@note	ʮ��Զ�㴦�յ㣨͸��ͼ���м�ֵ���д��Ľ���δ����
***************************************************************/
void Find_Flection2(void)
{
	uint8 i = 0;
	//����յ�2
	if(feature.top_point < 100 && feature.top_point > 10)
	{
		for(i = 100; i > feature.top_point + 4; i--)
		{
			if(feature.left_flection2_flag == 0)
			{
				//λ������
				if(line.left_line[i] < 140)
				{
					//��С����
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] < line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5])
					{
						//����������
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i - 3] == 1 && line.left_line_flag[i - 5] == 1)
						{
							//б������
							if(line.left_line[i + 3] - line.left_line[i] > 2)
							{
								feature.left_flection2_row = i;
								feature.left_flection2_flag = 1;
							}
						}
					}
				}
			}
		}
		//���ҹյ�2
		for(i = 100; i > feature.top_point + 4; i--)
		{
			if(feature.right_flection2_flag == 0)
			{
				//λ������
				if(line.right_line[i] > 20)
				{
					//��С����
					if(line.right_line[i] < line.right_line[i + 3] && line.right_line[i] > line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5])
					{
						//����������
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i - 3] == 1 && line.right_line_flag[i - 5] == 1)
						{
							//б������
							if(line.right_line[i + 3] - line.right_line[i] < -2)
							{
								feature.right_flection2_row = i;
								feature.right_flection2_flag = 1;
							}
						}
					}
				}
			}
		}
	}
}

/***************************************************************
	*	@brief	����
	*	@param	��
	*	@note	�ޣ�ʮ��flag=1���δд��
***************************************************************/
void Judge_Curve(void)
{
	uint8 i = 0;
	//��С��
	for(i = 100; i > feature.top_point + 3; i--)
	{
		if(feature.turn_flag != 1)
		{
			//���ߵ�һ��
			if(line.left_line_flag[i] == 0 && line.left_line_flag[i + 1] == 1 && line.left_line_flag[i + 2] == 1 && line.right_line_flag[i] == 1)
			{
				if(line.right_line_flag[i - 5] == 1 && i > 30)
				{
					//�����ܿ�ǰʱ��ֹ��ʮ�ֺ�Զ������ȥ��ʱ������
					if(i < 40 && line.right_line[i - 5] > 100)
					{
						if(line.right_line[i - 5] - line.right_line[i] > 10)
						{
							feature.turn_flag = 0;
						}
					}
					else
					{
						feature.turn_flag = 1;
					}
				}
			}
		}
		else if(feature.turn_flag != 2)
		{
			//�Ҷ��ߵ�һ��
			if(line.right_line_flag[i] == 0 && line.right_line_flag[i + 1] == 1 && line.right_line_flag[i + 2] == 1 && line.right_line_flag[i] == 1)
			{
				if(line.left_line_flag[i - 5] == 1 && i > 30)
				{
					//�����ܿ�ǰʱ��ֹ��ʮ�ֺ�Զ������ȥ��ʱ������
					if(i < 40 && line.left_line[i - 5] < 60)
					{
						if(line.left_line[i] - line.left_line[i - 5] > 10)
						{
							feature.turn_flag = 0;
						}
					}
					else
					{
						feature.turn_flag = 2;
					}
				}
			}
		}
	}
	//������
	uint8 is_left_turn = 0;
	uint8 is_right_turn = 0;
	if(feature.top_point < 95 && feature.turn_flag == 0)
	{
		//����������Χ
		for(i = 118; i > 112; i--)
		{
			//����118-112���ж���
			if(line.left_line_flag[i] == 0 && is_left_turn == 0)
			{
				is_left_turn = 1;
			}
			if(line.right_line_flag[i] == 0 && is_right_turn == 0)
			{
				is_right_turn = 1;
			}
		}
		if(is_left_turn == 1)
		{
			//100����90��������������ǰת��
			for(i = 100; i > feature.top_point + 10; i--)
			{
				if(line.left_line_flag[i] == 1 || line.right_line_flag[i] == 0)
				{
					is_left_turn = 0;
				}
			}
		}
		if(is_right_turn == 1)
		{
			//100����90��������������ǰת��
			for(i = 100; i > feature.top_point + 10; i--)
			{
				if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 1)
				{
					is_right_turn = 0;
				}
			}
		}
		if(is_left_turn == 1 && is_right_turn == 0)
		{
			feature.turn_flag = 3;
		}
		if(is_left_turn == 0 && is_right_turn == 1)
		{
			feature.turn_flag = 4;
		}
	}
}
