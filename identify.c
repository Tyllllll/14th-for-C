#include "header.h"

Line_Class line;
Feature_Class feature;

/***************************************************************
	*	@brief	寻线
	*	@param	无
	*	@note	寻找左右边界及计算中线
***************************************************************/
void Find_Line(void)
{
	uint8 i = 0, j = 0;
	int16 column_start = 80;//基础寻线每行起始搜索列
	for(i = 118; i > 10; i--)
	{
		line.left_line[i] = 0;
		line.left_line_flag[i] = 0;
		line.right_line[i] = 0;
		line.right_line_flag[i] = 0;
		line.midline[i] = 0;
		//从中线往左扫
		for(j = column_start ; j > 2; j--)
		{
			//寻找左跳变点
			if(camera.image[i][j] != 0 && camera.image[i][j - 2] == 0)
			{
				line.left_line[i] = j - 1;
				line.left_line_flag[i] = 1;
				break;
			}
		}
		//从中线往右扫
		for(j = column_start; j < 157; j++)
		{
			//寻找右跳变点
			if(camera.image[i][j] != 0 && camera.image[i][j + 2] == 0)
			{
				line.right_line[i] = j + 1;
				line.right_line_flag[i] = 1;
				break;
			}
		}
		//计算中线
		if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 1)
		{
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
		else if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 0)
		{
			line.right_line[i] = 159;
			line.midline[i] = (int16)(line.left_line[i] + RoadWide(i));
		}
		else if(line.left_line_flag[i] == 0 && line.right_line_flag[i] == 1)
		{
			line.left_line[i] = 0;
			line.midline[i] = (int16)(line.right_line[i] - RoadWide(i));
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
				if(line.midline[i + 1] == 80)
				{
					line.midline[i] = line.midline[i + 1];
				}
				else
				{
					line.midline[i] = (line.midline[i + 1] + line.midline[i + 2] + line.midline[i + 3] + line.midline[i + 4] + line.midline[i + 5]) / 5;
				}
//				if(line.midline[i] > 40 && line.midline[i] < 120)
//				{
//					if(camera.image[i - 1][line.midline[i]] != 0 && camera.image[i - 2][line.midline[i]] == 0 && camera.image[i - 4][line.midline[i]] == 0 && camera.image[i - 6][line.midline[i]] == 0)
//					{
//						if(camera.image[i - 5][line.midline[i] + 20] != 0)
//						{
//							line.midline[i] += 20;
//						}
//						else if(camera.image[i - 5][line.midline[i] - 20] != 0)
//						{
//							line.midline[i] -= 20;
//						}
//					}
//				}
			}
		}
		if(i != 118)
		{
			if(line.midline[i] - line.midline[i + 1] > 10)
			{
				line.midline[i] = line.midline[i + 1] + 10;
			}
			else if(line.midline[i] - line.midline[i + 1] < -10)
			{
				line.midline[i] = line.midline[i + 1] - 10;
			}
		}
		column_start = line.midline[i];
	}
}

/***************************************************************
	*	@brief	判断特征
	*	@param	无
	*	@note	无
***************************************************************/
void Judge_Feature(void)
{
	//特征清零
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

//	feature.linecnt = Judge_StartLine();	
	Find_Top_Point();
	Find_Inflection();
	Find_Inflection2();
	Judge_Roundabouts();
    Judge_Breakage();
    Judge_Ramp();
    Judge_Roadblock();
	Judge_Straight();
	Judge_Curve();
	
//	Judge_Cross();
}

/***************************************************************
	*	@brief	找顶点
	*	@param	无
	*	@note	顶点的定义为某一行的中线上的为白色点，但是该中线列的后面的点为黑点，即赛道边界
***************************************************************/
void Find_Top_Point(void)
{
	uint8 i = 0;
	feature.top_point = 0;
	for(i = 100; i > 10; i--)
	{
		if(camera.image[i][line.midline[i]] != 0 && camera.image[i - 1][line.midline[i]] == 0 && camera.image[i - 3][line.midline[i]] == 0 && camera.image[i - 5][line.midline[i]] == 0)
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
	*	@brief	找拐点
	*	@param	无
	*	@note	十字近点处拐点（透视图中最大最小处）
***************************************************************/
void Find_Inflection(void)
{
	uint8 i = 0;
	if(feature.top_point < 100)
	{
		//找左拐点
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.left_flection_flag == 0)
			{
				//位置条件
				if(i > 28 && line.left_line[i] < 130)
				{
					//大小条件
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] > line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5] &&
					   line.left_line[i - 3] - line.left_line[i + 3] < 4)
					{
						//存在性条件
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i + 3] == 1 && line.left_line_flag[i + 5] == 1)
						{
							//斜率条件
							if(line.left_line[i] - line.left_line[i - 4] - (line.left_line[i + 4] - line.left_line[i]) > 4)
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
		//找右拐点
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.right_flection_flag == 0)
			{
				//位置条件
				if(i > 28 && line.right_line[i] > 30)
				{
					//大小条件
					if(line.right_line[i] < line.right_line[i + 3] && line.left_line[i] < line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5] &&
					   line.right_line[i - 3] - line.right_line[i + 3] > -4)
					{
						//存在性条件
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i + 3] == 1 && line.right_line_flag[i + 5] == 1)
						{
							//斜率条件
							if(line.right_line[i + 4] - line.right_line[i] - (line.right_line[i] - line.right_line[i - 4]) > 4)
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
	*	@brief	找拐点2
	*	@param	无
	*	@note	十字远点处拐点（透视图中中间值）有待改进，未测试
***************************************************************/
void Find_Inflection2(void)
{
	uint8 i = 0;
	//找左2拐点
	if(feature.top_point < 100)
	{
		for(i = 100; i > feature.top_point - 2; i--)
		{
			if(feature.left_flection2_flag == 0)
			{
				//位置条件 
				if(line.left_line[i] > 15)
				{
					//大小条件
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] < line.left_line[i - 3])
					{
						//存在性条件
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i - 3] == 1 && line.left_line_flag[i - 5] == 1 && line.left_line_flag[i + 5] == 0)
						{
							//斜率条件
							if(line.left_line[i] - line.left_line[i + 2] > 20)
							{
								feature.left_flection2_row = i;
								feature.left_flection2_flag = 1;
								if(line.left_line[i - 4] - line.left_line[i - 1] >= 10 && line.left_line[feature.left_flection2_row] > 10 && feature.left_flection2_row > 35)
								{
									//反拐点标记
									feature.left_flection2_antiflag = 1;
								}
								break;
							}
						}
					}
				}
			}
		}
		//找右2拐点
		for(i = 100; i > feature.top_point - 2; i--)
		{
			if(feature.right_flection2_flag == 0)
			{
				//位置条件
				if(line.right_line[i] < 145)
				{
					//大小条件
					if(line.right_line[i] < line.right_line[i + 3] && line.right_line[i] > line.right_line[i - 3])
					{
						//存在性条件
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i - 3] == 1 && line.right_line_flag[i - 5] == 1 && line.right_line_flag[i + 5] == 0)
						{
							//斜率条件
							if(line.right_line[i] - line.right_line[i + 2] < -20)
							{
								feature.right_flection2_row = i;
								feature.right_flection2_flag = 1;
								if(line.right_line[i - 4] - line.right_line[i - 1] <= -10 && line.right_line[feature.right_flection2_row] < 150 && feature.right_flection2_row > 35)
								{
									//反拐点标记
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
	*	@brief	判直道
	*	@param	无
	*	@note	无
2019年3月16日09:44:38 长直道取消丢线要求
***************************************************************/
void Judge_Straight(void)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	//超长直道 1中线不太偏 2也不丢线 3且不为十字和坡道 4顶点满足直道条件
	//中线要求
	for(i = 80; i > 25; i--)
	{
		if(line.midline[i] >= 88 || line.midline[i] <= 72)
		{
			lose_cnt++;
		}
	}
//	if(lose_cnt < 3)
//	{
//		//丢线要求
//		lose_cnt = 0;
//		for(i = 50; i > 20; i--)
//		{
//			if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 0)
//			{
//				lose_cnt++;
//			}
//		}
//		if(lose_cnt < 3)
//		{
//			//顶点要求
//			if(feature.top_point <= 25)
//			{
//				feature.straight_state = 1;
//			}
//			else
//			{
//				feature.straight_state = 0;
//			}
//		}
//	}
    if(lose_cnt < 5 && feature.top_point < 25)
    {
        feature.straight_state = 1;
    }
	else if(line_Slope(2,20,60) < 0.5)
	{
        feature.straight_state = 1;
    }
    else
    {
		feature.straight_state = 0;
	}
	//短直道判断
	if(feature.straight_state == 0)
	{
		if(feature.top_point < 27)
		{
			if(Midline_Std_Deviation(60, 30) < 7)
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
	*	@brief	判弯
	*	@param	无
	*	@note	无（十字flag=1情况未写）
***************************************************************/
void Judge_Curve(void)
{
	uint8 i = 0;
	uint8 is_left_turn = 0;
	uint8 is_right_turn = 0;
//	uint8 cnt_left = 0;
//	uint8 cnt_right = 0;
	//判远小弯
	if(feature.turn_state == 0)
	{	
		for(i = 100; i > 30; i--)
		{
			if(line.left_line_flag[i] == 0 && line.left_line_flag[i + 1] == 1 && line.left_line_flag[i + 2] == 1 && line.right_line_flag[i] == 1)
			{
				if(line.right_line_flag[i - 4] == 1 && i > 20)
				{
					feature.pre_turn_state = 1;
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
					feature.pre_turn_state = 2;
					break;
				}
			}
		}
	}
	if(feature.turn_state != 0)
	{
		feature.pre_turn_state = 0;
	}
//	//救火
//	if(feature.top_point > 70)
//	{
//		for(i = 118; i > feature.top_point; i--)
//		{
//			if(line.left_line_flag[i] == 1)
//			{
//				cnt_left++;
//			}
//			if(line.right_line_flag[i] == 1)
//			{
//				cnt_right++;
//			}
//		}
//		if(cnt_left < 2 && cnt_right > 118 - feature.top_point - 2)
//		{
//			feature.turn_state = 5;
//		}
//		else if(cnt_left > 118 - feature.top_point - 2 && cnt_right < 2)
//		{
//			feature.turn_state = 6;
//		}
//	}
//	else
//	{
//		feature.turn_state = 0;
//	}
	//判深弯
	if(feature.turn_state != 5 && feature.turn_state != 6)
	{
//		cnt_left = 0;
//		cnt_right = 0;
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
				feature.turn_state = 3;
			}
			else if(is_left_turn == 0 && is_right_turn == 1)
			{
				feature.turn_state = 4;
			}
			else
			{
				feature.turn_state = 0;
			}
		}
	}
	if(feature.turn_state != 3 && feature.turn_state != 4 && feature.turn_state != 5 && feature.turn_state != 6)
	{
		if(feature.top_point < 70)
		{
			for(i = 80; i > 70; i--)
			{
				//左线70-80行有丢线
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
				for(i = 70; i > feature.top_point + 7; i--)
				{
					if(line.left_line_flag[i] == 1 || line.right_line_flag[i] == 0)
					{
						is_left_turn = 0;
					}
				}
			}
			if(is_right_turn == 1)
			{
				for(i = 70; i > feature.top_point + 7; i--)
				{
					if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 1)
					{
						is_right_turn = 0;
					}
				}
			}
			if(is_left_turn == 1 && is_right_turn == 0)
			{
				feature.turn_state = 1;
			}
			else if(is_left_turn == 0 && is_right_turn == 1)
			{
				feature.turn_state = 2;
			}
			else
			{
				feature.turn_state = 0;
			}
		}
	}
}

/***************************************************************
	*	@brief	判十字
	*	@param	无
	*	@note	拐点是否不应该取消，其他路况使用（重新写）
***************************************************************/
void Judge_Cross(void)
{
	if(feature.left_flection_flag == 1 || feature.right_flection_flag == 1)
	{
		if(feature.left_flection_flag == 1)
		{
			//在左边拐弯的标志为1的前提下，左拐列数太大，不宜再进行左拐，应将左拐标志取消
			if(line.left_line[feature.left_flection_row] > 140)
			{
				feature.left_flection_flag = 0;
				feature.left_flection_row = 0;
			}
			//或者在此时右边没有丢线的情况下，左拐标志应为零（十字时候，才存在既有左拐点，同时在左拐点对应的行数上面判断右边线丢线）
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
			//左拐点在右拐点右边时 保留近处拐点
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
			//如果左或右拐点有丢线，原有拐点标志不变，如果没有丢线，原有拐点标志变为0
			if(is_Left_Point_Lose_Line(feature.left_flection_row) == 0 || is_Right_Point_Lose_Line(feature.left_flection_row) == 0)
			{
				feature.left_flection_flag = 0;
			}
		}
		if(feature.right_flection_flag == 1)
		{
			//如果左或右拐点有丢线，原有拐点标志不变，如果没有丢线，原有拐点标志变为0
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
			//拐点存在，销去转弯标志位
			if(feature.pre_turn_state != 0)
			{
				feature.pre_turn_state = 0;
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
	*	@brief	判环
	*	@param	无
	*	@note	无
***************************************************************/
void Judge_Roundabouts(void)
{
	uint8 i = 0;
	if(feature.roundabouts_state == 0)
	{
		uint8 cnt = 0;
		for(i = 0; i < 40; i++)
		{
			if(feature.road_type[i] == 4)
			{
				cnt++;
			}
		}
		if(cnt == 0)
		{
			if((magnetic.value[MID_L] > 150 || magnetic.value[MID_R] > 150 || magnetic.value[HORIZONTAL_L] > 140 || magnetic.value[HORIZONTAL_R] > 140) && feature.roundabouts_state == 0)
			{
				if(feature.left_flection2_flag == 1 && feature.right_flection2_flag == 0)
				{
					if(is_Right_Lose_Line(feature.left_flection2_row + 10) == 0)
					{
						Bee();
						feature.roundabouts_state = 1;
					}
				}
				if(feature.left_flection2_flag == 0 && feature.right_flection2_flag == 1)
				{
					if(is_Left_Lose_Line(feature.right_flection2_row + 10) == 0)
					{
						Bee();
						feature.roundabouts_state = 2;
					}
				}
			}
		}
	}
	if(feature.roundabouts_state == 1)
	{
		if(feature.right_flection2_flag == 1 && line.right_line[feature.right_flection2_row] > 85)
		{
			servo.enable = 1;
			feature.roundabouts_state = 3;
		}
	}
	if(feature.roundabouts_state == 3)
	{
		if(feature.right_flection_flag == 1)
		{
			if(line.right_line[feature.right_flection_row] > 120)
			{
				int16 sum = 0;
				servo.enable = 0;
				feature.roundabouts_state = 5;
				for(i = 0; i < 10; i++)
				{
					sum += servo.duty_record[i];
				}
				servo.duty = sum / 10;
			}			
		}
	}
	if(feature.roundabouts_state == 5)
	{
		if(feature.top_point < 16 && line.left_line_flag[feature.top_point] == 0 && line.right_line_flag[feature.top_point] == 1
		|| feature.turn_state == 1)
		{
			Bee();
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
	if(feature.roundabouts_state == 2)
	{
		if(feature.left_flection2_flag == 1 && line.left_line[feature.left_flection2_row] < 75)
		{
			servo.enable = 1;
			feature.roundabouts_state = 4;
		}
	}
	if(feature.roundabouts_state == 4)
	{
		if(feature.left_flection_flag == 1)
		{
			if(line.left_line[feature.left_flection_row] < 40)
			{
				int16 sum = 0;
				servo.enable = 0;
				feature.roundabouts_state = 6;
				for(i = 0; i < 10; i++)
				{
					sum += servo.duty_record[i];
				}
				servo.duty = sum / 10;
			}
		}
	}
	if(feature.roundabouts_state == 6)
	{
		if(feature.top_point < 16 && line.left_line_flag[feature.top_point] == 1 && line.right_line_flag[feature.top_point] == 0
		|| feature.turn_state == 2)
		{
			Bee();
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
}
//void Judge_Roundabouts(void)
//{
//	uint8 i = 0;
//	if(feature.roundabouts_state == 0)
//	{
//		uint8 cnt = 0;
//		for(i = 0; i < 40; i++)
//		{
//			if(feature.road_type[i] == 4)
//			{
//				cnt++;
//			}
//		}
//		if(cnt == 0)
//		{
//			if((magnetic.value[MID_L] > 170 || magnetic.value[MID_R] > 170 || magnetic.value[HORIZONTAL_L] > 140 || magnetic.value[HORIZONTAL_R] > 140) && feature.roundabouts_state == 0)
//			{
//				if(feature.left_flection2_flag == 1 && feature.right_flection2_flag == 0)
//				{
//					if(is_Right_Lose_Line(feature.left_flection2_row + 10) == 0)
//					{
//						Bee();
//						feature.roundabouts_state = 1;
//					}
//				}
//				if(feature.left_flection2_flag == 0 && feature.right_flection2_flag == 1)
//				{
//					if(is_Left_Lose_Line(feature.right_flection2_row + 10) == 0)
//					{
//						Bee();
//						feature.roundabouts_state = 2;
//					}
//				}
//			}
//		}
//	}
//	else if(feature.roundabouts_state == 1)
//	{
//		if(feature.right_flection2_flag == 1 && line.right_line[feature.right_flection2_row] > 85)
//		{
//			servo.enable = 1;
//			feature.roundabouts_state = 3;
//			motor.distance_temp = 80;
//		}
//	}
//	else if(feature.roundabouts_state == 3)
//	{
//		if(feature.right_flection_flag == 1)
//		{
//			if(motor.distance_temp <= 0)
//			{
//				int16 sum = 0;
//				servo.enable = 0;
//				feature.roundabouts_state = 5;
//				for(i = 5; i < 10; i++)
//				{
//					sum += servo.duty_record[i];
//				}
//				servo.duty = sum / 5 + 20;
//			}			
//		}
//		if(servo.error[0] >= 0)
//		{
//			Bee();
//			feature.roundabouts_state = 0;
//		}
//	}
//	else if(feature.roundabouts_state == 5)
//	{
//		if(feature.top_point < 16 && line.left_line_flag[feature.top_point] == 0 && line.right_line_flag[feature.top_point] == 1
//		|| feature.turn_state == 1 || magnetic.value[MID_L] > 160 || magnetic.value[MID_R] > 160 || magnetic.value[HORIZONTAL_L] > 130 || magnetic.value[HORIZONTAL_R] > 130)
//		{
//			Bee();
//			servo.enable = 1;
//			feature.roundabouts_state = 0;
//		}
//	}
//	else if(feature.roundabouts_state == 2)
//	{
//		if(feature.left_flection2_flag == 1 && line.left_line[feature.left_flection2_row] < 75)
//		{
//			servo.enable = 1;
//			feature.roundabouts_state = 4;
//			motor.distance_temp = 80;
//		}
//	}
//	else if(feature.roundabouts_state == 4)
//	{
//		if(feature.left_flection_flag == 1)
//		{
//			if(motor.distance_temp <= 0)
//			{
//				int16 sum = 0;
//				servo.enable = 0;
//				feature.roundabouts_state = 6;
//				for(i = 5; i < 10; i++)
//				{
//					sum += servo.duty_record[i];
//				}
//				servo.duty = sum / 5 - 20;
//			}
//		}
//		if(servo.error[0] <= 0)
//		{
//			Bee();
//			feature.roundabouts_state = 0;
//		}
//	}
//	else if(feature.roundabouts_state == 6)
//	{
//		if(feature.top_point < 16 && line.left_line_flag[feature.top_point] == 1 && line.right_line_flag[feature.top_point] == 0
//		|| feature.turn_state == 2 || magnetic.value[MID_L] > 160 || magnetic.value[MID_R] > 160 || magnetic.value[HORIZONTAL_L] > 130 || magnetic.value[HORIZONTAL_R] > 130)
//		{
//			Bee();
//			servo.enable = 1;
//			feature.roundabouts_state = 0;
//		}
//	}
//}

/***************************************************************
	*	@brief	判断路
	*	@param	无
	*	@note	顶点大于50判断
***************************************************************/
void Judge_Breakage(void)
{
	uint8 i;
	uint8 miderror;
	uint8 miderrormax = 0;
    static uint8 cnt = 0;
	//判断路
	if(feature.ramp_state == 0 && feature.roadblock_state == 0 && feature.breakage_state == 0 && feature.top_point > (laser.distance < 100.0 ? 30 : 50) && feature.linecnt == 0)
	{
		if(laser.distance > 100.0)
		{
			feature.roadblock_state = -1;
		}
		else
		{
			feature.roadblock_state = 0;
		}
		if(line.midline[feature.top_point] >= 75 && line.midline[feature.top_point] <= 85)
		{
			//Bee();
			feature.breakage_state = 1;
		}
		else
		{
			for(i = feature.top_point+10; i > feature.top_point; i--)
			{
				miderror = (uint8)fabs(line.midline[i] - 80);
				if(miderror > miderrormax)
				{
					miderrormax = miderror;
				}
			}
//			if(miderrormax < 40)
            if(fabs(line.midline[feature.top_point + 2] - 80) < 40)
			{
				//Bee();
				feature.breakage_state = 1;
			}
		}		
		if(feature.breakage_state != 0)
		{
			servo.which = 1;
		}
	}
	//准备出断路
    else if(feature.breakage_state == 1)
    {
        if(is_Left_Point_Lose_All_Line(90, 25) == 1 && is_Right_Point_Lose_All_Line(90, 25) == 1)
        {
            cnt ++;
        }
        else
        {
            cnt = 0;
        }
        if(cnt > 5)
        {
            feature.breakage_state = 2;
            cnt = 0;
        }
    }
    else if(feature.breakage_state == 2)
    {
        if(is_Lose_All(110) == 0)
        {
            feature.breakage_state = 0;
            servo.which = 0;
        }
    }
}

/***************************************************************
	*	@brief	判坡道
	*	@param	无
	*	@note	无
***************************************************************/
void Judge_Ramp(void)
{   
//	if(feature.breakage_state == 1)
//	{
//		if(line.right_line[80] - line.left_line[80] < 1.5 * half_width[80])
//		{
//			Bee();
//			servo.which = 1;
//			feature.ramp_state = 1;
//		}
//	}
//	if(feature.ramp_state == 1)
//	{
//		if(infrared.distance > 1500)
//		{
//			//下坡
//			Bee();
//			feature.breakage_state = 0;
//			feature.ramp_state = 2;
//			motor.distance_temp = 0;
//			motor.distance_cnt_en = 1;
//		}
//	}
//	else if(feature.ramp_state == 2)
//	{
//		if(motor.distance_temp >= 60)
//		{
//			Bee();
//			servo.which = 0;
//			motor.distance_cnt_en = 0;
//			motor.distance_temp = 0;
//			feature.ramp_state = 0;
//		}
//	}
    if(feature.ramp_state == 0)
    {
        if(feature.top_point < 15 && fabs(servo.error[0]) < 20 && laser.distance < 50 && (magnetic.value[MID_L] + magnetic.value[MID_R]) > 100 && feature.breakage_state == 0 && feature.roadblock_state == 0) //前方可能有坡道 如果一米内没有检测到坡道 清标志
        {
            //Bee();
            feature.ramp_state = 1;
            servo.which = 1;
            motor.distance_temp = 100;
            gyro.Car_Angle = 0;
        }        
    }
    if(feature.ramp_state == 1)
    {
       if(gyro.Car_Angle > 10 && feature.top_point > 40 && fabs(servo.error[0]) < 45)   //上坡状态 电磁巡线
        {
            //Bee();
            servo.which = 1;
            feature.breakage_state = 0;
            feature.ramp_state = 2;
            motor.distance_temp = 200;      //2m距离辅助清坡道标志
        } 
       if(motor.distance_temp <= 0) 
       {
           servo.which = 0;
           feature.ramp_state = 0;
       }
    }
    
    else if(feature.ramp_state == 2)     //下坡摄像头巡线 否则下坡弯道拐不过来
    {
        if(gyro.Car_Angle < -12)
        {
            //Bee();
            servo.which = 0;
            feature.ramp_state = 3;
        }
        if(motor.distance_temp <= 0)
        {
            servo.which = 0;
            feature.ramp_state = 0;
        }
    }
    else if(feature.ramp_state == 3)
    {
        if(gyro.Car_Angle > -7)
        {
            Bee();
            feature.ramp_state = 0;
        }
        if(motor.distance_temp <= 0)
        {
            servo.which = 0;
            feature.ramp_state = 0;
        }
    }
}

/***************************************************************
	*	@brief	判路障
	*	@param	无
	*	@note	回到赛道前，左下角的夹角越大则车与赛道中线的夹角就越大
***************************************************************/
void Judge_Roadblock(void)
{
    if(feature.roadblock_state == 0 || feature.roadblock_state == -1)
    {
        if(laser.distance > 60.0 && laser.distance < 90.0 )
        {
            if((magnetic.value[MID_L] + magnetic.value[MID_R]) > 100    //车身处于中间
               &&  feature.top_point > 20 && feature.top_point < 48    //顶点要求
                &&  (feature.road_type[0] == 1 || feature.road_type[0] == 2 || feature.road_type[0] == 0)    //路况为短直道或长直道
                  &&    fabs(servo.error[0]) < 18               //舵机打脚小
                    )
            {
                //Bee();
                feature.breakage_state = 0;
                feature.roadblock_state = 1;
                motor.distance_temp =  laser.distance - 20;
                servo.enable = 0;
                servo.which = 0;
            }
        }
        else if(feature.breakage_state == 1 && laser.distance < 80.0 && feature.roadblock_state == 0)
        {
            //Bee();
            feature.breakage_state = 0;
            feature.roadblock_state = 1;
            motor.distance_temp =  laser.distance - 20;
            servo.enable = 0;
            servo.which = 0;
        }
    }
    else if(feature.roadblock_state == 1)
    {
        servo.error[0] = (int16)(gyro.TurnAngle_Integral >= 6 ? 5 * (10 - gyro.TurnAngle_Integral) : 50);
        if(laser.distance < 30)
        {
            servo.error[0] = 70;
        }
        if(motor.distance_temp <= 0 && gyro.TurnAngle_Integral > 5.0)
        {
            feature.roadblock_state = 2;
        }
    }
    else if(feature.roadblock_state == 2)
    {
        servo.error[0] = (int16)(gyro.TurnAngle_Integral <= -4 ? 4 * (-6 - gyro.TurnAngle_Integral) : -50);
        if(magnetic.value[MID_L] > 10 || magnetic.value[MID_R] > 10)   
		{
			feature.roadblock_state = 0;
			servo.enable = 1;
            servo.which = 0;
		}
    }
//	if(feature.roadblock_state == 0)
//	{
//		if(feature.breakage_state == 1)
//		{
//			if(laser.distance < 100.0)
//			{
//				Bee();
//				feature.breakage_state = 0;
//				feature.ramp_state = 0;
//				feature.roadblock_state = 1;
//				motor.distance_cnt_en = 1;
//				motor.distance_temp = 0;
//				servo.enable = 0;
//				servo.which = 0;	//断路电磁巡线，需要切换回来
//				servo.duty = (int16)DEG_MAX;
//			}
//		}
//	}
//	else if(feature.roadblock_state == 1)
//	{
//		//左转出赛道，直到右边丢线
////		if(line.midline[80] < 80 || line.midline[80] > 140)
//        if(is_Left_Point_Lose_All_Line(70, 7) == 1 && motor.distance_temp >= 30.0)
//		{
//			feature.roadblock_state = 2;
//			motor.distance_temp = 0;
//			servo.duty = (int16)DEG_MID;
//		}
//	}
//	else if(feature.roadblock_state == 2)
//	{
//		//直走偏离赛道，定距离
//		if(motor.distance_temp >= 25)
//		{
//			feature.roadblock_state = 3;
//			motor.distance_temp = 0;
//			servo.duty = (int16)(DEG_MID - 115);
//		}
//	}
//	else if(feature.roadblock_state == 3)
//	{
//		//右转回赛道
//		if(motor.distance_temp >= 40)
//		{
//			if(is_Left_Point_Lose_All_Line(100, 7) == 0)
//			{
//				if(line_Slope(1, 94, 106) != 160)
//				{
//					feature.roadblock_state = 4;
//					motor.distance_temp = 0;
//				}
//			}
//		}
//	}
//	else if(feature.roadblock_state == 4)
//	{
//		if(line.left_line[110] > 25 && line.left_line[110] < 100)
//		{   
//			servo.duty = DEG_MID + 5 * (line.left_line[110]-line.left_line[90]);
//		}
//		if(Adc_Magnetic_Lose_Line() == 0)   
//		{
//			feature.roadblock_state = 0;
//			servo.enable = 1;
//			motor.distance_cnt_en = 0;
//			motor.distance_temp = 0;
//		}
//	}
}

uint8 Judge_StartLine(void)
{
    uint8 LINECNT = 0, CNTREG = 0, i, j, row;
    for(i = 40; i < 101; i += 2)
    {
        for(j = 10; j < 150; j++)
        {
            if(camera.image[i][j] != 0 && camera.image[i][j + 2] == 0)
            {
                CNTREG ++;
            }
        }
        if(CNTREG > LINECNT)
        {
            LINECNT = CNTREG;
            row = i;
        }
        CNTREG = 0;
    }
    if(row != 40)
    {
        for(i = row - 10; i < row + 10; i++)
        {
            line.left_line_flag[i] = 0;
            line.right_line_flag[i] = 0;
            for(j = 0; j < 159; j++)
            {
                camera.image[i][j] = 1;
            }
        }
    }
    return row;
}
/**********************a little funcitons**********************/
/***************************************************************
	*	@brief	求范围内中线标准差
	*	@param	row_max：行数最大值，row_min：行数最小值
	*	@note	无
***************************************************************/
float Midline_Std_Deviation(uint8 row_max, uint8 row_min)
{
	uint8 i = 0;
	uint16 sum1 = 0;
	float32 ave = 0;
	float32 sum2 = 0;
	for(i = row_max; i > row_min; i--)
	{
		sum1 += line.midline[i];
	}
	ave = sum1 / (float32)(row_max - row_min);
	for(i = row_max; i > row_min; i--)
	{
		sum2 += (line.midline[i] * line.midline[i] - ave * ave);
	}
	return sqrt(sum2 / (float32)(row_max - row_min));
}

/***************************************************************
	*	@brief	判断左右边线是否丢线
	*	@param	row：行数
	*	@note	从行数到顶点判断
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
	*	@brief	判断某一行附近行是否有丢线
	*	@param	row：行数
	*	@note	上下10行判断，两行丢线即返回1
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
	*	@brief	判断某一行附近行是否全丢线
	*	@param	row：行数
	*	@note	上下7行判断，14行全丢线才返回1
***************************************************************/
uint8 is_Left_Point_Lose_All_Line(uint8 row, uint8 area)
{
	int16 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + area; i > row - area; i--)
	{
		if(line.left_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt >= (2 * area - 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8 is_Right_Point_Lose_All_Line(uint8 row, uint8 area)
{
	int16 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + area; i > row - area; i--)
	{
		if(line.right_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt >= (2 * area - 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/***************************************************************
	*	@brief	算三点曲率半径
	*	@param	三点坐标
	*	@note	无
***************************************************************/
float32 Get_Radius_Curvature(int16 point_Ax, int16 point_Ay, int16 point_Bx, int16 point_By, int16 point_Cx, int16 point_Cy)
{
	float32 a = 0, b = 0, c = 0, curvature = 0;
	int32 PF_ab = 0, PF_ac = 0, PF_bc = 0;
	PF_ab = (point_Ax - point_Bx) * (point_Ax - point_Bx) + (point_Ay - point_By) * (point_Ay - point_By);//直线AB的距离平方项
	c = sqrt(PF_ab);//直线AB的距离
	PF_ac = (point_Ax - point_Cx) * (point_Ax - point_Cx) + (point_Ay - point_Cy) * (point_Ay - point_Cy);//直线AC的距离平方项
	b = sqrt(PF_ac);//直线AC的距离
	PF_bc = (point_Bx - point_Cx) * (point_Bx - point_Cx) + (point_By - point_Cy) * (point_By - point_Cy);//直线BC的距离平方项
	a = sqrt(PF_bc);//直线BC的距离
	if(c * b * a == 0)
	{
		return -1;
	}
	curvature = a / 2 / sin(acos((b * b + c * c - a * a) / 2 / b / c));
	if(curvature > 9999)
	{
		curvature = 9999;
	}
	return curvature;
}

/***************************************************************
	*	@brief	算前后5行中线偏差平均值
	*	@param	row：行数
	*	@note	无
***************************************************************/
int16 Get_Mid_Average(uint8 row)
{
	uint8 i;
	int16 sum = 0;
	for(i = row - 5; i < row + 5; i++)
	{
		sum += line.midline[i] - 80;
	}
	return sum / 10;
}

/***************************************************************
	*	@brief	测赛道半宽
	*	@param	无
	*	@note	无
***************************************************************/
void Check_Half_Width(void)
{
	for(uint8 i = 118; i > 5; i --)
	{
		line.half_width_test[i] = (line.right_line[i] - line.left_line[i]) / 2;
	}
}

/***************************************************************
	*	@brief	计算边线斜率
	*	@param	type：1左线 2右线 3中线，row_beg行数小，row_end：行数大
	*	@note	看起来没有写完
***************************************************************/
float32 line_Slope(uint8 type, uint8 row_beg, uint8 row_end)
{
    uint8 i;
    float32 K = 0, sum = 0;
    if(type == 1)
    {//求左边线斜率
        K = ((float32)(line.left_line[row_end] - line.left_line[row_beg])) / ((float32)(row_end - row_beg));
        for(i = row_beg; i <= row_end; i++)
        {
            sum += ((line.left_line[row_beg] + K * (i - row_beg)) - line.left_line[i]) * ((line.left_line[row_beg] + K * (i - row_beg)) - line.left_line[i]);
        }
        if(sum > 4)
        {
            return 160;
        }
        else
        {
            return K;
        }
    }
    if(type == 2)
    {//求左边线斜率
        K = ((float32)(line.midline[row_end] - line.midline[row_beg])) / ((float32)(row_end - row_beg));
        for(i = row_beg; i <= row_end; i+=3)
        {
            sum += ((line.midline[row_beg] + K * (i - row_beg)) - line.midline[i]) * ((line.midline[row_beg] + K * (i - row_beg)) - line.midline[i]);
        }
        if(sum > 10)
        {
            return 160;
        }
        else
        {
            return K;
        }
    }
	return 160;
}

/***************************************************************
	*	@brief	计算赛道宽度
	*	@param	row：行数
	*	@note	无
***************************************************************/
float32 RoadWide(uint8 row)
{
	float Wide;
	int16 WideStandPoint = 90;
	float WideStand = 90;
	float Slope = 0.8;
	Wide = WideStand + Slope * (row - WideStandPoint);
	return Wide;
}