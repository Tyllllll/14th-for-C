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
	for(i = 118; i >5; i--)
	{
		line.left_line[i] = 0;
		line.left_line_flag[i] = 0;
		line.right_line[i] = 0;
		line.right_line_flag[i] = 0;
		line.midline[i] = 0;
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
		if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 1)
		{
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
		else if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 0)
		{
			line.right_line[i] = 159;
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
		else if(line.left_line_flag[i] == 0 && line.right_line_flag[i] == 1)
		{
			line.left_line[i] = 0;
			line.midline[i] = line.right_line[i] / 2;
		}
		else if(line.left_line_flag[i] == 0 && line.right_line_flag[i] == 0)
		{
			line.left_line[i] = 0;
			line.right_line[i] = 159;
			if(118 == i)
			{
				line.midline[i] = 80;
			}
			else
			{
				line.midline[i] = line.midline[i + 1];
				if(i > 40 && i < 120)
				{
					if(camera.image[i][line.midline[i]] != 0 && (camera.image[i - 1][line.midline[i]] == 0 || camera.image[i - 3][line.midline[i]] == 0 || camera.image[i - 5][line.midline[i]] == 0))
					{
						if(camera.image[i + 30][line.midline[i]] != 0)
						{
							line.midline[i] += 30;
						}
						else if(camera.image[i - 30][line.midline[i]] != 0)
						{
							line.midline[i] -= 30;
						}
					}
				}
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
	feature.left_flection2_flag = 0;
	feature.left_flection2_antiflag = 0;
	feature.left_flection2_row = 0;
	feature.right_flection2_flag = 0;
	feature.right_flection2_antiflag = 0;
	feature.right_flection2_row = 0;
	
	Find_Top_Point();
	Find_Inflection();
	Find_Inflection2();
	
	Judge_Roundabouts();
	Judge_Straight();
	Judge_Curve();
//	Judge_Cross();
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
	if(feature.top_point < 100)
	{
		//����յ�
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.left_flection_flag == 0)
			{
				//λ������
				if(i > 28 && line.left_line[i] < 155)
				{
					//��С����
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] > line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5] &&
					   line.left_line[i - 3] - line.left_line[i + 3] < 4)
					{
						//����������
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i + 3] == 1 && line.left_line_flag[i + 5] == 1)
						{
							//б������
							if(line.left_line[i] - line.left_line[i - 3] - (line.left_line[i + 3] - line.left_line[i]) > 4)
							{
								feature.left_flection_row = i;
								feature.left_flection_flag = 1;
								break;
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
				if(i > 28 && line.right_line[i] > 5)
				{
					//��С����
					if(line.right_line[i] < line.right_line[i + 3] && line.left_line[i] < line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5] &&
					   line.right_line[i - 3] - line.right_line[i + 3] > -4)
					{
						//����������
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i + 3] == 1 && line.right_line_flag[i + 5] == 1)
						{
							//б������
							if(line.right_line[i + 3] - line.right_line[i] - (line.right_line[i] - line.right_line[i - 3]) > 4)
							{
								feature.right_flection_row = i;
								feature.right_flection_flag = 1;
								break;
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
void Find_Inflection2(void)
{
	uint8 i = 0;
	//����2�յ�
	if(feature.top_point < 100)
	{
		for(i = 100; i > feature.top_point - 2; i--)
		{
			if(feature.left_flection2_flag == 0)
			{
				//λ������
				if(line.left_line[i] > 15)
				{
					//��С����
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] < line.left_line[i - 3])
					{
						//����������
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i - 3] == 1 && line.left_line_flag[i - 5] == 1 && line.left_line_flag[i + 5] == 0)
						{
							//б������
							if(line.left_line[i] - line.left_line[i + 2] > 20)
							{
								feature.left_flection2_row = i;
								feature.left_flection2_flag = 1;
								if(line.left_line[i - 4] - line.left_line[i - 1] >= 10 && line.left_line[feature.left_flection2_row] > 10)
								{
									//���յ���
									feature.left_flection2_antiflag = 1;
								}
								break;
							}
						}
					}
				}
			}
		}
		//����2�յ�
		for(i = 100; i > feature.top_point - 2; i--)
		{
			if(feature.right_flection2_flag == 0)
			{
				//λ������
				if(line.right_line[i] < 145)
				{
					//��С����
					if(line.right_line[i] < line.right_line[i + 3] && line.right_line[i] > line.right_line[i - 3])
					{
						//����������
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i - 3] == 1 && line.right_line_flag[i - 5] == 1 && line.right_line_flag[i + 5] == 0)
						{
							//б������
							if(line.right_line[i] - line.right_line[i + 2] < -20)
							{
								feature.right_flection2_row = i;
								feature.right_flection2_flag = 1;
								if(line.right_line[i - 4] - line.right_line[i - 1] <= -10 && line.right_line[feature.right_flection2_row] < 150)
								{
									//���յ���
									feature.right_flection2_antiflag = 1;
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

/***************************************************************
	*	@brief	��ֱ��
	*	@param	��
	*	@note	��
***************************************************************/
void Judge_Straight(void)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	//����ֱ�� 1���߲�̫ƫ 2Ҳ������ 3�Ҳ�Ϊʮ�ֺ��µ� 4��������ֱ������
	//����Ҫ��
	for(i = 90; i > 20; i--)
	{
		if(line.midline[i] >= 88 || line.midline[i] <= 72)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 3)
	{
		//����Ҫ��
		lose_cnt = 0;
		for(i = 50; i > 20; i--)
		{
			if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 0)
			{
				lose_cnt++;
			}
		}
		if(lose_cnt < 3)
		{
			//����Ҫ��
			if(feature.top_point <= 15)
			{
				feature.straight_state = 1;
			}
			else
			{
				feature.straight_state = 0;
			}
		}
		else
		{
			feature.straight_state = 0;
		}
	}
	else
	{
		feature.straight_state = 0;
	}
	//��ֱ���ж�
	if(feature.straight_state == 0)
	{
		if(feature.top_point < 17)
		{
			if(Midline_Std_Deviation() < 7)
			{
				feature.straight_state = 2;
			}
			else
			{
				feature.straight_state = 0;
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
	uint8 is_left_turn = 0;
	uint8 is_right_turn = 0;
	uint8 cnt_left = 0;
	uint8 cnt_right = 0;
	//��ԶС��
	if(feature.deep_turn_state == 0)
	{	
		for(i = 100; i > 30; i--)
		{
			if(line.left_line_flag[i] == 0 && line.left_line_flag[i + 1] == 1 && line.left_line_flag[i + 2] == 1 && line.right_line_flag[i] == 1)
			{
				if(line.right_line_flag[i - 4] == 1 && i > 20)
				{
					feature.turn_state = 1;
					break;
				}
			}
		}
		for(i = 100; i > 30; i--)
		{
			if(line.right_line_flag[i] == 0 && line.right_line_flag[i + 1] == 1 && line.right_line_flag[i + 2] == 1 && line.left_line_flag[i] == 1)
			{
				if(line.left_line_flag[i - 4] == 1 && i > 20)
				{
					feature.turn_state = 2;
					break;
				}
			}
		}
	}
	if(feature.deep_turn_state != 0)
	{
		feature.turn_state = 0;
	}
	//�Ȼ�
	if(feature.top_point > 70)
	{
		for(i = 118; i > feature.top_point; i--)
		{
			if(line.left_line_flag[i] == 1)
			{
				cnt_left++;
			}
			if(line.right_line_flag[i] == 1)
			{
				cnt_right++;
			}
		}
		if(cnt_left < 2 && cnt_right > 118 - feature.top_point - 2)
		{
			feature.deep_turn_state = 5;
		}
		else if(cnt_left > 118 - feature.top_point - 2 && cnt_right < 2)
		{
			feature.deep_turn_state = 6;
		}
	}
	else
	{
		feature.deep_turn_state = 0;
	}
	//������
	if(feature.deep_turn_state != 5 && feature.deep_turn_state != 6)
	{
		cnt_left = 0;
		cnt_right = 0;
		if(feature.top_point < 105)
		{
			for(i = 118; i > 112; i--)
			{
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
				feature.deep_turn_state = 3;
			}
			else if(is_left_turn == 0 && is_right_turn == 1)
			{
				feature.deep_turn_state = 4;
			}
			else
			{
				feature.deep_turn_state = 0;
			}
		}
	}
	if(feature.deep_turn_state != 3 && feature.deep_turn_state != 4 && feature.deep_turn_state != 5 && feature.deep_turn_state != 6)
	{
		if(feature.top_point < 70)
		{
			for(i = 50; i > 40; i--)
			{
				//����50-40���ж���
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
				for(i = 40; i > feature.top_point + 7; i--)
				{
					if(line.left_line_flag[i] == 1 || line.right_line_flag[i] == 0)
					{
						is_left_turn = 0;
					}
				}
			}
			if(is_right_turn == 1)
			{
				for(i = 40; i > feature.top_point + 7; i--)
				{
					if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 1)
					{
						is_right_turn = 0;
					}
				}
			}
			if(is_left_turn == 1 && is_right_turn == 0)
			{
				feature.deep_turn_state = 1;
			}
			else if(is_left_turn == 0 && is_right_turn == 1)
			{
				feature.deep_turn_state = 2;
			}
			else
			{
				feature.deep_turn_state = 0;
			}
		}
	}
}

/***************************************************************
	*	@brief	��ʮ��
	*	@param	��
	*	@note	�յ��Ƿ�Ӧ��ȡ��������·��ʹ�ã�����д��
***************************************************************/
void Judge_Cross(void)
{
	if(feature.left_flection_flag == 1 || feature.right_flection_flag == 1)
	{
		if(feature.left_flection_flag == 1)
		{
			//����߹���ı�־Ϊ1��ǰ���£��������̫�󣬲����ٽ�����գ�Ӧ����ձ�־ȡ��
			if(line.left_line[feature.left_flection_row] > 140)
			{
				feature.left_flection_flag = 0;
				feature.left_flection_row = 0;
			}
			//�����ڴ�ʱ�ұ�û�ж��ߵ�����£���ձ�־ӦΪ�㣨ʮ��ʱ�򣬲Ŵ��ڼ�����յ㣬ͬʱ����յ��Ӧ�����������ж��ұ��߶��ߣ�
			else if(is_Right_Lose_Line(feature.left_flection_row) == 0)
			{
				feature.left_flection_flag = 0;
				feature.left_flection_row = 0;
			}
		}
		if(feature.right_flection_flag == 1)
		{
			if(feature.right_flection_row < 20)
			{
				feature.right_flection_flag = 0;
				feature.right_flection_row = 0;
			}
			else if(is_Left_Lose_Line(feature.right_flection_row) == 0)
			{
				feature.right_flection_flag = 0;
				feature.right_flection_row = 0;
			}
		}
		if(feature.left_flection_flag == 1 && feature.right_flection_flag == 1)
		{
			//��յ����ҹյ��ұ�ʱ ���������յ�
			if(line.left_line[feature.left_flection_row] > line.right_line[feature.right_flection_row])
			{
				if(feature.left_flection_row > feature.right_flection_row)
				{
					feature.right_flection_flag = 0;
				}
				if(feature.left_flection_row < feature.right_flection_row)
				{
					feature.left_flection_flag = 0;
				}
			}
		}
		if(feature.left_flection_flag == 1)
		{
			//�������ҹյ��ж��ߣ�ԭ�йյ��־���䣬���û�ж��ߣ�ԭ�йյ��־��Ϊ0
			if(is_Left_Point_Lose_Line(feature.left_flection_row) == 0 || is_Right_Point_Lose_Line(feature.left_flection_row) == 0)
			{
				feature.left_flection_flag = 0;
			}
		}
		if(feature.right_flection_flag == 1)
		{
			//�������ҹյ��ж��ߣ�ԭ�йյ��־���䣬���û�ж��ߣ�ԭ�йյ��־��Ϊ0
			if(is_Left_Point_Lose_Line(feature.right_flection_row) == 0 || is_Right_Point_Lose_Line(feature.right_flection_row) == 0)
			{
				feature.right_flection_flag = 0;
			}
		}
		if(feature.left_flection_flag == 1 || feature.right_flection_row == 1)
		{
			if(feature.cross_state[1] == 0)
			{
				feature.cross_state[0]++;
			}
			if(feature.cross_state[0] == 3)
			{
				feature.cross_state[1] = 1;
				feature.cross_state[0] = 0;
			}
			//�յ���ڣ���ȥת���־λ
			if(feature.turn_state != 0)
			{
				feature.turn_state = 0;
			}
		}
		else
		{
			feature.cross_state[1] = 0;
		}
		if(feature.left_flection_flag == 0)
		{
			feature.left_flection_row = 0;
		}
		if(feature.right_flection_flag == 0)
		{
			feature.right_flection_row = 0;
		}
	}
}

/***************************************************************
	*	@brief	�л�
	*	@param	��
	*	@note	��
***************************************************************/
void Judge_Roundabouts(void)
{
	uint8 i = 0;
	uint8 cnt = 0;
	for(i = 0; i < 30; i++)
	{
		if(feature.road_type[i] == 4)
		{
			cnt++;
		}
	}
	if(cnt == 0)
	{
		Magnetic_GetAdc();
		if(magnetic.left_mag > 50 || magnetic.right_mag > 50 && feature.roundabouts_state == 0)
		{
			if(feature.left_flection2_flag == 1 && feature.right_flection2_flag == 0)
			{
				if(is_Right_Lose_Line(feature.left_flection2_row + 10) == 0)
				{
					feature.roundabouts_state = 1;
				}
			}
			if(feature.left_flection2_flag == 0 && feature.right_flection2_flag == 1)
			{
				if(is_Left_Lose_Line(feature.right_flection2_row + 10) == 0)
				{
					feature.roundabouts_state = 2;
				}
			}
		}
	}
	if(feature.roundabouts_state == 1)
	{
		if(feature.right_flection2_flag == 1)
		{
			servo.enable = 1;
			feature.roundabouts_state = 3;
		}
	}
	if(feature.roundabouts_state == 3)
	{
		if(feature.right_flection_flag == 1)
		{
			if(line.right_line[feature.right_flection_row] > 140)
			{
				feature.roundabouts_state = 5;
			}
		}
	}
	if(feature.roundabouts_state == 5)
	{
		if(feature.top_point < 20 && line.left_line_flag[feature.top_point] == 0 && line.right_line_flag[feature.top_point] == 1
		|| feature.deep_turn_state == 1)
		{
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
	if(feature.roundabouts_state == 2)
	{
		if(feature.left_flection2_flag == 1)
		{
			servo.enable = 1;
			feature.roundabouts_state = 4;
		}
	}
	if(feature.roundabouts_state == 4)
	{
		if(feature.left_flection_flag == 1)
		{
			if(line.left_line[feature.left_flection_row] < 20)
			{
				feature.roundabouts_state = 6;
			}
		}
	}
	if(feature.roundabouts_state == 6)
	{
		if(feature.top_point < 20 && line.left_line_flag[feature.top_point] == 1 && line.right_line_flag[feature.top_point] == 0
		|| feature.deep_turn_state == 2)
		{
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
}


/**********************a little funcitons**********************/
/***************************************************************
	*	@brief	��60-30�з�Χ�����߱�׼��
	*	@param	��
	*	@note	��
***************************************************************/
float Midline_Std_Deviation(void)
{
	uint8 i = 0;
	uint16 sum1 = 0;
	float32 ave = 0;
	float32 sum2 = 0;
	for(i = 60; i > 30; i--)
	{
		sum1 += line.midline[i];
	}
	ave = sum1 / 30.0;
	for(i = 60; i > 30; i--)
	{
		sum2 += (line.midline[i] * line.midline[i] - ave * ave);
	}
	return sqrt(sum2 / 30.0);
}

/***************************************************************
	*	@brief	�ж����ұ����Ƿ���
	*	@param	row������
	*	@note	�������������ж�
***************************************************************/
uint8 is_Left_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row; i > feature.top_point; i--)
	{
		if(line.left_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 3)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8 is_Right_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row; i > feature.top_point; i--)
	{
		if(line.right_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 3)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/***************************************************************
	*	@brief	�ж�ĳһ�и������Ƿ���
	*	@param	row������
	*	@note	����10���ж�
***************************************************************/
uint8 is_Left_Point_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 10; i > row - 10; i--)
	{
		if(line.left_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8 is_Right_Point_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 10; i > row - 10; i--)
	{
		if(line.right_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/***************************************************************
	*	@brief	�ж�ĳһ�и������Ƿ�ȫ����
	*	@param	row������
	*	@note	����7���ж�
***************************************************************/
uint8 is_Left_Point_Lose_All_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 7; i > row - 7; i--)
	{
		if(line.left_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 13)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8 is_Right_Point_Lose_All_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 7; i > row - 7; i--)
	{
		if(line.right_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 13)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}