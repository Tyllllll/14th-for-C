#include "header.h"

Line_Class line;
Feature_Class feature;
uint8 TOP_POINT[5];
/***************************************************************
	*	@brief	寻线
	*	@param	无
	*	@note	寻找左右边界及计算中线
***************************************************************/
void Find_Line(void)
{
	uint8 i = 0, j = 0;
	int16 column_start = 80;//基础寻线每行起始搜索列
    line.midline[119] = 80;
	for(i = 118; i > 10; i--)
	{
		line.left_line[i] = 0;
		line.left_line_flag[i] = 0;
		line.right_line[i] = 0;
		line.right_line_flag[i] = 0;
		line.midline[i] = 0;
		//从中线往左扫
		for(j = column_start; j > 2; j--)
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
            if((line.midline[i]-line.midline[i+1])>5)
            {
                line.midline[i] = line.midline[i+1] + 5;
            }
            else if(line.midline[i]-line.midline[i+1]<-5)
            {
                line.midline[i] = line.midline[i+1] - 5;
            }
		}
		else if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 0)
		{
			line.right_line[i] = 159;
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
            if((line.midline[i]-line.midline[i+1])>5)
            {
                line.midline[i] = line.midline[i+1] + 5;
            }
            else if(line.midline[i]-line.midline[i+1]<-5)
            {
                line.midline[i] = line.midline[i+1] - 5;
            }
		}
		else if(line.left_line_flag[i] == 0 && line.right_line_flag[i] == 1)
		{
			line.left_line[i] = 0;
			line.midline[i] = line.right_line[i] / 2;
            if((line.midline[i]-line.midline[i+1])>5)
            {
                line.midline[i] = line.midline[i+1] + 5;
            }
            else if(line.midline[i]-line.midline[i+1]<-5)
            {
                line.midline[i] = line.midline[i+1] - 5;
            }
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
				if(line.midline[i] > 40 && line.midline[i] < 120)
				{
					if(camera.image[i - 1][line.midline[i]] != 0 && camera.image[i - 2][line.midline[i]] == 0 && camera.image[i - 4][line.midline[i]] == 0 && camera.image[i - 6][line.midline[i]] == 0)
					{
						if(camera.image[i - 5][line.midline[i] + 20] != 0)
						{
							line.midline[i] += 20;
						}
						else if(camera.image[i - 5][line.midline[i] - 20] != 0)
						{
							line.midline[i] -= 20;
						}
					}
				}
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
	
	Find_Top_Point();
	Find_Inflection();
	Find_Inflection2();
	
	Judge_Roundabouts();
	Judge_Straight();
	Judge_Curve();
    Judge_breramp();
//	Judge_Breakage();
//    Judge_ramp();
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
		if(camera.image[i][line.midline[i]] != 0 && camera.image[i - 1][line.midline[i]] == 0 && camera.image[i - 3][line.midline[i]] == 0)
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
				if(i > 28 && line.left_line[i] < 155)
				{
					//大小条件
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] > line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5] &&
					   line.left_line[i - 3] - line.left_line[i + 3] < 4)
					{
						//存在性条件
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i + 3] == 1 && line.left_line_flag[i + 5] == 1)
						{
							//斜率条件
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
		//找右拐点
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.right_flection_flag == 0)
			{
				//位置条件
				if(i > 28 && line.right_line[i] > 5)
				{
					//大小条件
					if(line.right_line[i] < line.right_line[i + 3] && line.left_line[i] < line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5] &&
					   line.right_line[i - 3] - line.right_line[i + 3] > -4)
					{
						//存在性条件
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i + 3] == 1 && line.right_line_flag[i + 5] == 1)
						{
							//斜率条件
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
***************************************************************/
void Judge_Straight(void)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	//超长直道 1中线不太偏 2也不丢线 3且不为十字和坡道 4顶点满足直道条件
	//中线要求
	for(i = 90; i > 20; i--)
	{
		if(line.midline[i] >= 88 || line.midline[i] <= 72)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 3)
	{
		//丢线要求
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
			//顶点要求
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
	//短直道判断
	if(feature.straight_state == 0)
	{
		if(feature.top_point < 17)
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
	uint8 cnt_left = 0;
	uint8 cnt_right = 0;
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
	//救火
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
			feature.turn_state = 5;
		}
		else if(cnt_left > 118 - feature.top_point - 2 && cnt_right < 2)
		{
			feature.turn_state = 6;
		}
	}
	else
	{
		feature.turn_state = 0;
	}
	//判深弯
	if(feature.turn_state != 5 && feature.turn_state != 6)
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
			for(i = 50; i > 40; i--)
			{
				//左线50-40行有丢线
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
	uint8 cnt = 0;
	for(i = 0; i < 80; i++)
	{
		if(feature.road_type[i] == 4)
		{
			cnt++;
		}
	}
	if(cnt == 0)
	{
//		Magnetic_Get_Result();
		if((magnetic.onceUni[HLEFT] > 150 || magnetic.onceUni[MIDLEFT] > 180 || magnetic.onceUni[MIDRIGHT] > 180 || magnetic.onceUni[HRIGHT]>150)&& feature.roundabouts_state == 0)
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
		if(line.right_line[feature.right_flection2_row] > 125)
		{
			feature.roundabouts_state = 3;
		}
	}
	if(feature.roundabouts_state == 3)
	{
		if(feature.right_flection_flag == 1)
		{
			if(line.right_line[feature.right_flection_row] > 120)
			{
				feature.roundabouts_state = 5;
			}
		}
	}
	if(feature.roundabouts_state == 5)
	{
		if(feature.top_point < 20 && line.left_line_flag[feature.top_point] == 0 && line.right_line_flag[feature.top_point] == 1
		|| feature.turn_state == 1)
		{
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
	if(feature.roundabouts_state == 2)
	{
		if(feature.left_flection2_flag == 1)
		{
			feature.roundabouts_state = 4;
		}
	}
	if(feature.roundabouts_state == 4)
	{
		if(feature.left_flection_flag == 1)
		{
			if(line.left_line[feature.left_flection_row] < 40)
			{
				feature.roundabouts_state = 6;
			}
		}
	}
	if(feature.roundabouts_state == 6)
	{
		if(feature.top_point < 20 && line.left_line_flag[feature.top_point] == 1 && line.right_line_flag[feature.top_point] == 0
		|| feature.turn_state == 2)
		{
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
}
///***************************************************************
//	*	@brief	判坡道
//	*	@param	无
//	*	@note	无
//***************************************************************/
//void Judge_ramp(void)
//{
//    uint8 cnt1 = 0, cnt2 = 0, i = 0, ramp = 0;
//    uint8 miderrormax = 0;
//    uint8 miderror = 0;
//    for(i = 0; i < 30; i++)
//    {
//        if(feature.road_type[i] == 6)
//        {
//            cnt1++;
//        }
//    }
////    if(feature.ramp_state[1]==0 && feature.roundabouts_state==0 && cnt1==0)        //判断坡道
////    { 
////        Check_Half_Width();
////        cnt1 = 0;
////        cnt2 = 0;
////        for(i=feature.top_point+20; i>feature.top_point; i--)
////        {
////            if(line.left_line_flag[i]==1 && line.right_line_flag[i]==1)
////            {
////                cnt2++;
////                if(line.half_width_test[i] > (half_width[i]+10))
////                {
////                    cnt1++;
////                }                
////            }
////
////        }
////        if((cnt2-cnt1)<3 && cnt2>10)
////        {
////            feature.ramp_state[1] = 1;      //车身处于赛道中间上坡
////            feature.ramp_state[0] = 0;
////            servo.which = 1;
////        }
////        else
////        {
////            cnt1 = 0;
////            cnt2 = 0;
////            for(uint8 i=40;i<100;i++)
////            {
////                cnt1 += line.left_line_flag[i];
////                cnt2 += line.right_line_flag[i];
////            }
////            if(fabs(cnt1-cnt2)>=55 && feature.ramp_state[0]>0)
////            {
////                if(cnt1 > cnt2)
////                {
////                    if((line.left_line[80]-line.left_line[90])<10 && (line.left_line[50]-line.left_line[60])<10)
////                    {
////                        feature.ramp_state[1] = 1;
////                        feature.ramp_state[0] = 0;
////                        servo.which = 1;
////                    }
////                }
////                else
////                {
////                    if((line.right_line[90]-line.right_line[80])<10 && (line.right_line[60]-line.right_line[50])<10)
////                    {
////                        feature.ramp_state[1] = 1;
////                        feature.ramp_state[0] = 0;
////                        servo.which = 1;
////                    }
////                }
////            }
////        }
////    }
////    
//    if(feature.ramp_state[1]==0 && cnt1==0)
//    {
//        if(feature.top_point > 30)
//        {
//            if(Midline_Std_Deviation((uint8)(feature.top_point+30),(uint8)(feature.top_point+5)) < 7)
//            {
//                ramp = 1;
//            }
//            else
//            {
//                for(uint8 i=feature.top_point + 30 ; i > feature.top_point ; i--)
//                {
//                    miderror = (uint8)fabs(line.midline[i] - 80);
//                    if(miderror > miderrormax)
//                    {
//                        miderrormax = miderror;
//                    }
//                }
//                if(miderrormax > 50)
//                {
//                    ramp = 0;
//                    feature.ramp_state[0] = 0;
//                    feature.ramp_state[1] = 0;
//                }
//                else
//                {
//                    ramp = 1;
//                }
//            }
//
//        }
//        feature.ramp_state[0] += ramp;
//        if(feature.ramp_state[0] > 3)
//        {
//            feature.ramp_state[1] = 1;
//            feature.ramp_state[0] = 0;
//        }
//        else
//        {
//            feature.ramp_state[1] = 0;
//        }
//    }
//    else if(feature.ramp_state[1]==1)       //上坡过程切电磁巡线 检测是否到达坡顶  上坡过程中toppoint先减后增 结合toppoint范围 
//    {
//        if(TOP_POINT[0] != feature.top_point)
//        {
//            for(i = 4; i > 0; i--)
//            {
//                TOP_POINT[i] = TOP_POINT[i-1];
//            }
//            TOP_POINT[0] = feature.top_point;
//        }
//        if((TOP_POINT[0] - TOP_POINT[2]) < 0)
//        {
//            feature.ramp_state[0]++;
//        }
//        else
//        {
//            feature.ramp_state[0] = 0;
//        }
// 
//        if(TOP_POINT[0] < 40 && TOP_POINT[0]>5 && feature.ramp_state[0]>5)
//        {
//            feature.ramp_state[1] = 2;
//            feature.ramp_state[0] = 0;
//            servo.which = 0;
//        }
//
//    }
//    else if(feature.ramp_state[1]==2 && feature.top_point < 15)       //下坡处理 暂无
//    {
//        Check_Half_Width();
//        cnt1 = 0;
//        for(i=feature.top_point+30; i>feature.top_point+5; i--)
//        {
//            if(line.half_width_test[i] > (half_width[i]+25))
//            {
//                cnt1++;
//            }
//        }
//        if(cnt1 >= 20)
//        {
//            feature.ramp_state[0] = 0;
//            feature.ramp_state[1] = 0;
//            servo.which = 0;
//        }
//    }
//}
///***************************************************************
//	*	@brief	判断路
//	*	@param	无
//	*	@note	无
//***************************************************************/
//void Judge_Breakage(void)
//{
//	uint8 i;
//	uint8 top_row;
//	uint8 bottom_row;
//	feature.breakage_radius_curvature = 0;
//	//判断路
//	if(feature.breakage_state == 0 && feature.ramp_state[1] == 0)
//	{
//		if(feature.top_point > 50)
//		{
//			for(i = 100; i > feature.top_point; i--)
//			{
//				if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 1)
//				{
//					top_row = i;
//					break;
//				}
//			}
//			//直入
//			if(line.midline[top_row] >= 75 && line.midline[top_row] <= 85)
//			{
//				feature.breakage_state = 1;
//			}
//			else
//			{
//				//左断
//				if(line.midline[top_row] < 75)
//				{
//					for(i = 110; i > feature.top_point; i--)
//					{
//						if(line.left_line_flag[i] == 1)
//						{
//							bottom_row = i;
//							break;
//						}
//					}
//					feature.breakage_radius_curvature = (int16)Get_Radius_Curvature(line.left_line[feature.top_point + 2], feature.top_point + 2, line.left_line[(feature.top_point + bottom_row) / 2], (feature.top_point + bottom_row) / 2, line.left_line[bottom_row], bottom_row);
//					if(feature.breakage_radius_curvature > 1700)
//					{
//						feature.breakage_state = 2;
//					}
//				}
//				//右断
//				else if(line.midline[top_row] > 85)
//				{
//					for(i = 110; i > feature.top_point; i--)
//					{
//						if(line.right_line_flag[i] == 1)
//						{
//							bottom_row = i;
//							break;
//						}
//					}
//					feature.breakage_radius_curvature = (int16)Get_Radius_Curvature(line.right_line[feature.top_point + 2], feature.top_point + 2, line.right_line[(feature.top_point + bottom_row) / 2], (feature.top_point + bottom_row) / 2, line.right_line[bottom_row], bottom_row);
//					if(feature.breakage_radius_curvature > 1700)
//					{
//						feature.breakage_state = 3;
//					}
//				}
//			}
//		}
//	}
//	//进断路
//	else if(feature.breakage_state == 1 || feature.breakage_state == 2 || feature.breakage_state == 3)
//	{
//		if(is_Lose_All(105) == 1)
//		{
//			servo.which = 1;
//			feature.breakage_state = 4;
//		}
//	}
//	//切回摄像头
//	else if(feature.breakage_state == 4)
//	{
//		if(is_Lose_All(35) == 0)
//		{
//			servo.which = 0;
//			feature.breakage_state = 5;
//		}
//	}
//	//出断路
//	else if(feature.breakage_state == 5)
//	{
//		if(is_Lose_All(105) == 0)
//		{
//			feature.breakage_state = 0;
//		}
//	}
//}

void Judge_breramp(void)
{
	static uint8 cnt1 = 0, cnt2 = 0;
    uint8 i, miderror, miderrormax = 0;
    
	//feature.breramp (1=断路坡道未知)
	if(feature.breramp==0 && feature.top_point>40)
	{
		if(line.midline[feature.top_point]>=75 && line.midline[feature.top_point]<=85)
		{
			feature.breramp = 1; //断路 坡道未知
		}
		else
		{
			for(i = feature.top_point+30; i > feature.top_point; i--)
			{
				miderror = (uint8)fabs(line.midline[i]-80);
				if(miderror > miderrormax)
				{
					miderrormax = miderror;
				}
			}
			if(miderrormax < 40)
			{
				feature.breramp = 1;
			}
		}
		if(feature.breramp != 0)
		{
			servo.which = 1;
            motor.alldist = 0;
            
             				BUZZER_ON;
				LPLD_LPTMR_DelayMs(50);
				BUZZER_OFF;   
		}
	}
	else if(feature.breramp == 1) // 断路 坡道未知
	{
        if(motor.alldist < 70)
        {
            if(is_Lose_All(100) == 1)
            {
                feature.breramp = 2;	//半米内有丢线确定为断路 否则为坡道
             				BUZZER_ON;
				LPLD_LPTMR_DelayMs(50);
				BUZZER_OFF;   
            }

        }
        else if(feature.top_point < 40)
        {
            feature.breramp = 4;
            servo.which = 0;
        }
        if(feature.breramp != 1)
        {
            cnt1 = 0;
            cnt2 = 0;
            motor.alldist = 0;
        }
	}
	else if(feature.breramp == 2)   //进入断路 检测前方赛道宽度是否正常
	{
		if(line.half_width_test[60]<=(half_width[60]+5) && line.half_width_test[60]>=(half_width[60]-5))
        {
            cnt1++;
        }
        else
        {
            cnt1 = 0;
        }
        if(cnt1 > 3)
        {
            feature.breramp = 3; 
            cnt1 = 0;
            cnt2 = 0;
        }
	}
	else if(feature.breramp == 3) //检测不丢线则出断路
	{
		if(is_Lose_All(105) == 0)
		{
			feature.breramp = 0;		//驶出断路
            servo.which = 0;
		}
	}
	else if(feature.breramp == 4)  //检测赛道半宽明显大于正常值 
    {
        cnt2 = 0;
		if(feature.top_point>5 && feature.top_point<50)
        {
            for(i = feature.top_point+30; i>feature.top_point; i--)
            {
                if(line.half_width_test[i] > (half_width[i]+20))
                {
                    cnt2++;
                }
                if(cnt2 >= 20 || feature.straight_state!=0)
                {
                    feature.breramp = 0;	//驶出坡道
                    servo.which = 0;
                }
            }     
                        
        }
    }
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
	*	@brief	判断某一行附近行是否丢线
	*	@param	row：行数
	*	@note	上下10行判断
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
	*	@note	上下7行判断
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
	*	@brief	测赛道半宽
	*	@param	无
	*	@note	无
***************************************************************/
void Check_Half_Width(void)
{
	for(uint8 i = 118; i > 10; i --)
	{
		line.half_width_test[i] = (line.right_line[i] - line.left_line[i]) / 2;
	}
}