#include "header.h"

int16 v_straight = 280;
int16 v_long_straight = 330;
int16 v_curve_high = 240;
int16 v_curve_low = 220;
int16 v_cross = 220;
int16 v_roundabouts = 230;
//�������ͼ�¼ 1Ϊ��ֱ����2Ϊ��ֱ����3Ϊ�䣬4Ϊ����5Ϊʮ�֣�6Ϊ�µ���7Ϊ������8Ϊ�ϰ�
uint8 road_type[200];
uint8 half_width[120];

/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	Ϊ��������޷��иĶ�
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
	uint8 i = 0;
	float32 k_left_record[3];
	float32 k_right_record[3];
	float32 k_left;
	float32 k_right;
	if(feature.left_flection_flag == 1)
	{
		if(feature.left_flection_row + 8 < 118)
		{
			k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[feature.left_flection_row + 8]) / 8.0;
		}
		else
		{
			k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[118]) / (118.0 - feature.left_flection_row);
		}
		//��Ȩ����б��
		k_left = 0.6 * k_left_record[0] + 0.2 * k_left_record[1] + 0.2 * k_left_record[2];
		//����
		for(i = feature.left_flection_row; i > feature.left_flection2_row; i--)
		{
			line.left_line[i] = (uint8)(line.left_line[feature.left_flection_row] + k_left * (feature.left_flection_row - i));
			if(line.left_line[i] < 0)
			{
				line.left_line[i] = 0;
			}
			if(line.left_line[i] > 159)
			{
				line.left_line[i] = 159;
			}
		}
	}
	if(feature.right_flection_flag == 1)
	{
		if(feature.right_flection_row + 8 < 118)
		{
			k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[feature.right_flection_row + 8]) / 8.0;
		}
		else
		{
			k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[118]) / (118.0 - feature.right_flection_row);
		}
		//��Ȩ����б��
		k_right = 0.6 * k_right_record[0] + 0.2 * k_right_record[1] + 0.2 * k_right_record[2];
		//�Ҳ���
		for(i = feature.right_flection_row; i > feature.right_flection2_row; i--)
		{
			line.right_line[i] = (uint8)(line.right_line[feature.right_flection_row] + k_right * (feature.right_flection_row - i));
			if(line.right_line[i] < 0)
			{
				line.right_line[i] = 0;
			}
			if(line.right_line[i] > 159)
			{
				line.right_line[i] = 159;
			}
		}
	}
	k_left_record[2] = k_left_record[1];
	k_left_record[1] = k_left_record[0];
	k_right_record[2] = k_right_record[1];
	k_right_record[1] = k_right_record[0];
	//������
	if(feature.left_flection_flag == 1 && feature.right_flection_row == 1)
	{
		for(i = (uint8)fmax(feature.left_flection_row, feature.right_flection_row); i > (uint8)fmin(feature.left_flection2_row, feature.right_flection2_row); i--)
		{
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
	}
	if(feature.left_flection_flag == 1 && feature.right_flection_flag == 0)
	{
		for(i = feature.left_flection_row; i > feature.left_flection2_row; i--)
		{
			line.midline[i] = line.left_line[i] + half_width[i] + 10;
		}
	}
	if(feature.left_flection_flag == 0 && feature.right_flection_flag == 1)
	{
		for(i = feature.left_flection_row; i > feature.left_flection2_row; i--)
		{
			line.midline[i] = line.right_line[i] - half_width[i] - 10;
		}
	}
	//����ʮ�ֺ����¶���һ���µĶ���
	Find_Top_Point();
}

/***************************************************************
	*	@brief	��������
	*	@param	��
	*	@note	������
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
	int16 error_ave;
	//������������
//	if(feature.roundabouts_state != 0 && feature.roundabouts_state != 5 && feature.roundabouts_state != 6)
	if(feature.roundabouts_state != 0)
	{
		road_type[0] = 4;
		motor.speed_set = v_roundabouts;
	}
	else if(feature.cross_state[1] == 1)
	{
		road_type[0] = 5;
		motor.speed_set = v_cross;
	}
	else if(feature.straight_state == 1)
	{
		road_type[0] = 2;
		motor.speed_set = v_straight;
	}
	else if(feature.straight_state == 2)
	{
		road_type[0] = 1;
		motor.speed_set = v_long_straight;
	}
	else if(feature.turn_state != 0)
	{
		road_type[0] = 3;
		error_ave = (int16)(0.6 * servo.error[0] + 0.2 * servo.error[1] + 0.2 * servo.error[2]);
		motor.speed_set = (int16)(v_curve_high - (v_curve_high - v_curve_low) * error_ave * error_ave / 40.0 / 40);
		if(motor.speed_set > v_curve_high)
		{
			motor.speed_set = v_curve_high;
		}
		if(motor.speed_set < v_curve_low)
		{
			motor.speed_set = v_curve_low;
		}
	}
}

/***************************************************************
	*	@brief	�Ƿ���
	*	@param	��
	*	@note	��
***************************************************************/
uint8 is_Lose_All(void)
{
	uint8 i = 0, j = 0;
	int16 black_count = 0;
	for(i = 118; i > 115; i--)
	{
		for(j = 130; j >= 30; j--)
		{
			if(camera.image[i][j] == 0)
			{
				black_count++;
			}
		}
	}
	if(black_count > 250)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}