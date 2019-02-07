#include "header.h"

int16 v_straight = 280;
int16 v_long_straight = 330;
int16 v_curve_high = 240;
int16 v_curve_low = 220;
int16 v_cross = 220;
int16 v_roundabouts = 230;
//赛道类型记录 1为长直道，2为短直道，3为弯，4为环，5为十字，6为坡道，7为颠簸，8为障碍
uint8 road_type[200];
uint8 half_width[120];

/***************************************************************
	*	@brief	弯道补线
	*	@param	无
	*	@note	为环放最后，限幅有改动
***************************************************************/
void Curve_Fill(void)
{
	uint8 i = 0;
	if(feature.turn_state != 0)
	{
		//左小弯
		if(feature.turn_state == 1)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.right_line[i] == 159)
				{
					//丢线部分中线逐渐往左偏
					line.midline[i] = line.midline[i + 1] - 1;
				}
				else
				{
					//丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
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
		//右小弯
		else if(feature.turn_state == 2)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.left_line[i] == 0)
				{
					//丢线部分中线逐渐往左偏
					line.midline[i] = line.midline[i + 1] + 1;
				}
				else
				{
					//丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
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
		//左深弯
		else if(feature.turn_state == 3)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//直接以右边界减去80，将中线轨迹向左偏离幅度更大
				line.midline[i] = line.right_line[i] - 80;
				if(line.midline[i] <= 0)
				{
//					line.left_line[i] = 0;
//					line.right_line[i] = 0;
					line.midline[i] = 0;
				}
			}
		}
		//右深弯
		else if(feature.turn_state == 4)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//直接以左边界加80，将中线轨迹向右偏离幅度更大
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
	*	@brief	十字补线
	*	@param	无
	*	@note	无
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
		//加权左线斜率
		k_left = 0.6 * k_left_record[0] + 0.2 * k_left_record[1] + 0.2 * k_left_record[2];
		//左补线
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
		//加权右线斜率
		k_right = 0.6 * k_right_record[0] + 0.2 * k_right_record[1] + 0.2 * k_right_record[2];
		//右补线
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
	//算中线
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
	//补完十字后重新定义一个新的顶点
	Find_Top_Point();
}

/***************************************************************
	*	@brief	环岛补线
	*	@param	无
	*	@note	补边线
***************************************************************/
void Roundabouts_Fill(void)
{
	
}

/***************************************************************
	*	@brief	速度设定
	*	@param	无
	*	@note	无
***************************************************************/
void Speed_Set(void)
{
	int16 error_ave;
	//保留出环加速
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
	*	@brief	是否丢线
	*	@param	无
	*	@note	无
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