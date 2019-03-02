#include "header.h"

uint8 half_width[120] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
						0, 19, 20, 21, 22, 24, 25, 26, 27, 28,//1
						29, 30, 31, 32, 33, 34, 35, 37, 38, 39,//2
						39, 40, 42, 43, 44, 44, 45, 46, 47, 48,//3
						49, 50, 51, 52, 53, 54, 55, 56, 57, 58,//4
						58, 59, 60, 61, 62, 63, 63, 64, 66, 67,//5
						68, 69, 70, 71, 72, 73, 73, 74, 75, 76,//6
						77, 78, 79, 79, 79, 79, 79, 79, 79, 79,//7
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//8
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//9
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//10
						79, 79, 79, 79, 79, 79, 79, 79, 79, 0,//11
};


Speed_Class speed;
Setting_Class setting;

//int16 error_ave=0;
//int16 error_last=0;
//int16 error_pre=0;

/***************************************************************
	*	@brief	速度数据初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Speed_Init(void)
{
	speed.straight = 220;
	speed.long_straight = 250;
	speed.curve_high = 200;
	speed.curve_low = 170;
	speed.cross = 220;
	speed.roundabouts = 190;
	
//	speed.straight = 260;
//	speed.long_straight = 300;
//	speed.curve_high = 240;
//	speed.curve_low = 220;
//	speed.cross = 170;
//	speed.roundabouts = 220;
	
//	speed.straight = 290;
//	speed.long_straight = 330;
//	speed.curve_high = 260;
//	speed.curve_low = 230;
//	speed.cross = 220;
//	speed.roundabouts = 240;
}

/***************************************************************
	*	@brief	总补线
	*	@param	无
	*	@note	无
***************************************************************/
void All_Fill(void)
{
	Roundabouts_Fill();
	Breakage_Fill();
	if(feature.roundabouts_state != 1 && feature.roundabouts_state != 2)
	{
		Cross_Fill();
	}
//	Curve_Fill();
}

/***************************************************************
	*	@brief	弯道补线
	*	@param	无
	*	@note	无
***************************************************************/
void Curve_Fill(void)
{
	uint8 i = 0;
	//左小弯
//	if(feature.turn_state == 1)
//	{
//		for(i = 60; i > feature.top_point; i--)
//		{
//			if(line.right_line[i] != 159)
//			{
//				//丢线部分中线逐渐往左偏
//				line.midline[i] = line.midline[i + 1] - 1;
//			}
//			else
//			{
//				//丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
//				line.midline[i] = line.midline[i + 1] - (line.right_line[i + 1] - line.right_line[i]) / 2;
//			}
//			if(line.midline[i] <= 0)
//			{
////					line.left_line[i] = 0;
////					line.right_line[i] = 0;
//				line.midline[i] = 0;
//			}
//		}
//	}
//	//右小弯
//	else if(feature.turn_state == 2)
//	{
//		for(i = 60; i > feature.top_point; i--)
//		{
//			if(line.left_line[i] == 0)
//			{
//				//丢线部分中线逐渐往右偏
//				line.midline[i] = line.midline[i + 1] + 1;
//			}
//			else
//			{
//				//丢线部分中线逐渐往右偏，且偏离的弧度跟左边界向右偏离程度相同
//				line.midline[i] = line.midline[i + 1] + (line.left_line[i] - line.left_line[i + 1]) / 2;
//			}
//			if(line.midline[i] >= 159)
//			{
////					line.left_line[i] = 159;
////					line.right_line[i] = 159;
//				line.midline[i] = 159;
//			}
//		}
//	}
	//左深弯
	if(feature.turn_state == 3)
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
		if(feature.top_point > 30)
		{
			for(i = feature.top_point; i > 30; i--)
			{
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
		if(feature.top_point > 30)
		{
			for(i = feature.top_point; i > 30; i--)
			{
				line.midline[i] = 159;
			}
		}
	}
	else if(feature.turn_state == 5)
	{
		for(i = 118; i > 35; i--)
		{
			line.midline[i] = 0;
		}
	}
	else if(feature.turn_state == 6)
	{
		for(i = 118; i > 35; i--)
		{
			line.midline[i] = 159;
		}
	}
}

/***************************************************************
	*	@brief	拐点补线
	*	@param	无
	*	@note	无
***************************************************************/
void Cross_Fill(void)
{
	uint8 i = 0;
	uint8 j = 0;
//	if(feature.left_flection_flag == 1 && feature.left_flection2_flag == 1)
//	{
//		feature.k_left_record[0] = (float32)(line.left_line[feature.left_flection2_row] - line.left_line[feature.left_flection_row]) / (feature.left_flection_row - feature.left_flection2_row);
//		feature.k_left = 0.6 * feature.k_left_record[0] + 0.2 * feature.k_left_record[1] + 0.2 * feature.k_left_record[2];
//		for(i = feature.left_flection2_row; i < feature.left_flection_row; i++)
//		{
//			line.left_line[i] = (int16)(line.left_line[feature.left_flection2_row] + feature.k_left * (feature.left_flection2_row - i));
//			if(line.left_line[i] < 0)
//			{
//				line.left_line[i] = 0;
//			}
//			else if(line.left_line[i] > 159)
//			{
//				line.left_line[i] = 159;
//			}
//		}
//	}
//	else if(feature.right_flection_flag == 1 && feature.right_flection2_flag == 1)
//	{
//		feature.k_right_record[0] = (float32)(line.right_line[feature.right_flection2_row] - line.right_line[feature.right_flection_row]) / (feature.right_flection_row - feature.right_flection2_row);
//		feature.k_right = 0.6 * feature.k_right_record[0] + 0.2 * feature.k_right_record[1] + 0.2 * feature.k_right_record[2];
//		for(i = feature.right_flection2_row; i < feature.right_flection_row; i++)
//		{
//			line.right_line[i] = (int16)(line.right_line[feature.right_flection2_row] + feature.k_right * (feature.right_flection2_row - i));
//			if(line.right_line[i] < 0)
//			{
//				line.right_line[i] = 0;
//			}
//			else if(line.right_line[i] > 159)
//			{
//				line.right_line[i] = 159;
//			}
//		}
//	}
	if(feature.left_flection_flag == 1 && line.left_line[feature.left_flection_row + 8] > 0
			|| feature.right_flection_flag == 1 && line.right_line[feature.right_flection_row + 8] < 159)
	{
		if(feature.left_flection_flag == 1)
		{
			if(feature.left_flection_row + 8 < 118)
			{
				feature.k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[feature.left_flection_row + 8]) / 8.0 ;
			}
			else
			{
				feature.k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[118]) / (118.0 - feature.left_flection_row);
			}
			//加权左线斜率
			feature.k_left = 0.6 * feature.k_left_record[0] + 0.2 * feature.k_left_record[1] + 0.2 * feature.k_left_record[2];
			//左补线
			for(i = feature.left_flection_row; i > (feature.top_point > 10 ? feature.top_point - 5 : feature.top_point); i--)
			{
				line.left_line[i] = (int16)(line.left_line[feature.left_flection_row] + feature.k_left * (feature.left_flection_row - i));
				if(line.left_line[i] < 0)
				{
					line.left_line[i] = 0;
				}
				else if(line.left_line[i] > 159)
				{
					line.left_line[i] = 159;
				}
			}
		}
		if(feature.right_flection_flag == 1)
		{
			if(feature.right_flection_row + 8 < 118)
			{
				feature.k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[feature.right_flection_row + 8]) / 8.0;
			}
			else
			{
				feature.k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[118]) / (118.0 - feature.right_flection_row);
			}
			//加权右线斜率
			feature.k_right = 0.6 * feature.k_right_record[0] + 0.2 * feature.k_right_record[1] + 0.2 * feature.k_right_record[2];
			//右补线
			for(i = feature.right_flection_row; i > (feature.top_point > 10 ? feature.top_point - 5 : feature.top_point); i--)
			{
				line.right_line[i] = (int16)(line.right_line[feature.right_flection_row] + feature.k_right * (feature.right_flection_row - i));
				if(line.right_line[i] < 0)
				{
					line.right_line[i] = 0;
				}
				else if(line.right_line[i] > 159)
				{
					line.right_line[i] = 159;
				}
			}
		}
	}
	else if(feature.left_flection2_flag == 1 && feature.left_flection2_row > 20 && line.left_line[feature.left_flection2_row - 1] > 20
			|| feature.right_flection2_flag == 1 && feature.right_flection2_row > 20 && line.right_line[feature.right_flection2_row - 1] < 140)
	{
		if(feature.left_flection2_flag == 1)
		{
			if(feature.left_flection2_row - 9 > 6)
			{
				feature.k_left_record2[0] = (line.left_line[feature.left_flection2_row - 9] - line.left_line[feature.left_flection2_row - 1]) / 8.0;
			}
			else
			{
				feature.k_left_record2[0] = (line.left_line[6] - line.left_line[feature.left_flection2_row - 1]) / (feature.left_flection2_row - 7);
			}
			feature.k_left2 = 0.6 * feature.k_left_record2[0] + 0.2 * feature.k_left_record2[1] + 0.2 * feature.k_left_record2[2];
			if(feature.left_flection2_antiflag == 0)
			{
				for(i = 110; i > feature.left_flection2_row; i--)
				{
					line.left_line[i] = (int16)(line.left_line[feature.left_flection2_row - 2] + feature.k_left2 * (feature.left_flection2_row - 2 - i));
					if(line.left_line[i] < 0)
					{
						line.left_line[i] = 0;
					}
					else if(line.left_line[i] > 159)
					{
						line.left_line[i] = 159;
					}
				}
			}
			else
			{
				for(i = 110; i > 30; i--)
				{
					line.left_line[i] = (int16)(line.left_line[feature.left_flection2_row - 2] - feature.k_left2 * (feature.left_flection2_row - 2 - i));
					if(line.left_line[i] < 0)
					{
						line.left_line[i] = 0;
					}
					else if(line.left_line[i] > 159)
					{
						line.left_line[i] = 159;
					}
				}
			}
		}
		if(feature.right_flection2_flag == 1)
		{
			if(feature.right_flection2_row - 9 > 6)
			{
				feature.k_right_record2[0] = (line.right_line[feature.right_flection2_row - 9] - line.right_line[feature.right_flection2_row - 1]) / 8.0;
			}
			else
			{
				feature.k_right_record2[0] = (line.right_line[6] - line.right_line[feature.right_flection2_row - 1]) / (feature.right_flection2_row - 7);
			}
			feature.k_right2 = 0.6 * feature.k_right_record2[0] + 0.2 * feature.k_right_record2[1] + 0.2 * feature.k_right_record2[2];
			if(feature.right_flection2_antiflag == 0)
			{
				for(i = 110; i > feature.right_flection2_row; i--)
				{
					line.right_line[i] = (int16)(line.right_line[feature.right_flection2_row - 2] + feature.k_right2 * (feature.right_flection2_row - 2 - i));
					if(line.right_line[i] < 0)
					{
						line.right_line[i] = 0;
					}
					else if(line.right_line[i] > 159)
					{
						line.right_line[i] = 159;
					}
				}
			}
			else
			{
				for(i = 110; i > 30; i--)
				{
					line.right_line[i] = (int16)(line.right_line[feature.right_flection2_row - 2] - feature.k_right2 * (feature.right_flection2_row - 2 - i));
					if(line.right_line[i] < 0)
					{
						line.right_line[i] = 0;
					}
					else if(line.right_line[i] > 159)
					{
						line.right_line[i] = 159;
					}
				}
			}
		}
	}
	feature.k_left_record[2] = feature.k_left_record[1];
	feature.k_left_record[1] = feature.k_left_record[0];
	feature.k_right_record[2] = feature.k_right_record[1];
	feature.k_right_record[1] = feature.k_right_record[0];
	feature.k_left_record2[2] = feature.k_left_record2[1];
	feature.k_left_record2[1] = feature.k_left_record2[0];
	feature.k_right_record2[2] = feature.k_right_record2[1];
	feature.k_right_record2[1] = feature.k_right_record2[0];
	//算中线
//	if(feature.left_flection_flag == 1 && feature.left_flection2_flag == 1)
//	{
//		for(i = feature.left_flection2_row; i < feature.left_flection_row; i++)
//		{
//			line.midline[i] = line.left_line[i] + half_width[i];
//			if(line.midline[i] < 0)
//			{
//				line.midline[i] = 0;
//			}
//			else if(line.midline[i] > 159)
//			{
//				line.midline[i] = 159;
//			}
//		}
//	}
	if(feature.left_flection_flag == 1 && line.left_line[feature.left_flection_row] > 20 
	   || feature.right_flection_flag == 1 && line.right_line[feature.right_flection_row] < 140)
	{
		if(feature.left_flection_flag == 1 && feature.right_flection_row == 1)
		{
			for(i = (uint8)fmax(feature.left_flection_row, feature.right_flection_row); i > (feature.top_point > 10 ? feature.top_point - 5 : feature.top_point); i--)
			{
				line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
		}
		else if(feature.left_flection_flag == 1 && feature.right_flection_flag == 0)
		{
			for(i = feature.left_flection_row; i > (feature.top_point > 10 ? feature.top_point - 5 : feature.top_point); i--)
			{
				line.midline[i] = line.left_line[i] + half_width[i];
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
		}
		else if(feature.left_flection_flag == 0 && feature.right_flection_flag == 1)
		{
			for(i = feature.right_flection_row; i > (feature.top_point > 10 ? feature.top_point - 5 : feature.top_point); i--)
			{
				line.midline[i] = line.right_line[i] - half_width[i];
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
		}
	}
	else if(feature.left_flection2_flag == 1 && feature.left_flection2_row > 20 && line.left_line[feature.left_flection2_row - 1] > 20
			|| feature.right_flection2_flag == 1 && feature.right_flection2_row > 20 && line.right_line[feature.right_flection2_row  - 1] < 140)
	{
		if(feature.left_flection2_flag == 1 && feature.right_flection2_flag == 1)
		{
			for(i = 110; i > (uint8)fmin(feature.left_flection2_row, feature.right_flection2_row); i--)
			{
				line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
		}
		else if(feature.left_flection2_flag == 1 && feature.right_flection2_flag == 0)
		{
			for(i = 110; i > feature.left_flection2_row; i--)
			{
				if(feature.left_flection2_antiflag == 0)
				{
					line.midline[i] = line.left_line[i] + half_width[i];
					if(line.midline[i] < 0)
					{
						line.midline[i] = 0;
					}
					else if(line.midline[i] > 159)
					{
						line.midline[i] = 159;
						for(j = i; j > 5; j--)
						{
							line.midline[j] = 159;
						}
						break;
					}
				}
				else
				{
					line.midline[i] = line.left_line[i] - half_width[i];
					if(line.midline[i] < 0)
					{
						line.midline[i] = 0;
					}
					else if(line.midline[i] > 159)
					{
						line.midline[i] = 159;
					}
				}
			}
			if(feature.left_flection2_antiflag == 1)
			{
				for(i = feature.left_flection2_row; i > 5; i--)
				{
					line.midline[i] = 0;
				}
			}
		}
		else if(feature.left_flection2_flag == 0 && feature.right_flection2_flag == 1)
		{
			for(i = 110; i > feature.right_flection2_row; i--)
			{
				if(feature.right_flection2_antiflag == 0)
				{
					line.midline[i] = line.right_line[i] - half_width[i];
					if(line.midline[i] < 0)
					{
						line.midline[i] = 0;
						for(j = i; j > 5; j--)
						{
							line.midline[i] = 0;
						}
						break;
					}
					else if(line.midline[i] > 159)
					{
						line.midline[i] = 159;
					}
				}
				else
				{
					line.midline[i] = line.right_line[i] + half_width[i];
					if(line.midline[i] < 0)
					{
						line.midline[i] = 0;
					}
					else if(line.midline[i] > 159)
					{
						line.midline[i] = 159;
					}
				}
			}
			if(feature.right_flection2_antiflag == 1)
			{
				for(i = feature.right_flection2_row; i > 5; i--)
				{
					line.midline[i] = 159;
				}
			}
		}
	}
	//补完十字后重新定义一个新的顶点
	Find_Top_Point();
}

/***************************************************************
	*	@brief	环岛补线
	*	@param	无
	*	@note	入环补边线，出环打死
***************************************************************/
void Roundabouts_Fill(void)
{
	uint8 i = 0;
	float32 state12_k;
	uint8 state12_fill_row = 80;
	if(feature.roundabouts_state == 1)
	{
		if(feature.left_flection2_flag == 1)
		{
			feature.roundabouts_k_record[0] = (float32)(line.left_line[feature.left_flection2_row] - line.right_line[state12_fill_row]) / (state12_fill_row - feature.left_flection2_row);
			state12_k = 0.6 * feature.roundabouts_k_record[0] + 0.2 * feature.roundabouts_k_record[1] + 0.2 * feature.roundabouts_k_record[2];
			for(i = state12_fill_row; i > feature.left_flection2_row; i--)
			{
				line.right_line[i] = (int16)(line.left_line[feature.left_flection2_row] + state12_k * (feature.left_flection2_row - i));
				if(line.right_line[i] < 0)
				{
					line.right_line[i] = 0;
				}
				else if(line.right_line[i] > 159)
				{
					line.right_line[i] = 159;
				}
				line.midline[i] = line.right_line[i] - half_width[i];
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
			for(i = feature.left_flection2_row; i > 5; i--)
			{
				line.midline[i] = 0;
			}
		}
		else if(feature.right_flection2_flag == 1)
		{
			feature.roundabouts_k_record[0] = (float32)(line.right_line[feature.right_flection2_row] - line.right_line[state12_fill_row]) / (state12_fill_row - feature.right_flection2_row);
			state12_k = 0.6 * feature.roundabouts_k_record[0] + 0.2 * feature.roundabouts_k_record[1] + 0.2 * feature.roundabouts_k_record[2];
			for(i = state12_fill_row; i > feature.right_flection2_row; i--)
			{
				line.right_line[i] = (int16)(line.right_line[feature.right_flection2_row] + state12_k * (feature.right_flection2_row - i));
				if(line.right_line[i] < 0)
				{
					line.right_line[i] = 0;
				}
				else if(line.right_line[i] > 159)
				{
					line.right_line[i] = 159;
				}
				line.midline[i] = line.right_line[i] - half_width[i];
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
		}
		feature.roundabouts_k_record[2] = feature.roundabouts_k_record[1];
		feature.roundabouts_k_record[1] = feature.roundabouts_k_record[0];
		Find_Top_Point();
//		servo.enable = 0;
		if(feature.left_flection2_flag == 1)
		{
			if(feature.left_flection2_row > 16 || feature.left_flection2_row == 0)
			{
				feature.roundabouts_size = 2;
//				servo.duty = DEG_MID + 60 + 60;
			}
			else if(feature.left_flection2_row < 12)
			{
				feature.roundabouts_size = -1;
//				servo.duty = DEG_MID + 60;
			}
			else if(feature.roundabouts_size <= 0 || feature.roundabouts_size >= 1)
			{
				feature.roundabouts_size = (float32)(feature.left_flection2_row - 12) / 4;
//				servo.duty = (int16)(DEG_MID + 60 + feature.roundabouts_size * 60);
			}
		}
	}
	else if(feature.roundabouts_state == 5)
	{
		servo.enable = 0;
		if(feature.roundabouts_size == 2)
		{
			servo.duty = DEG_MID + 70 + 80;
		}
		else if(feature.roundabouts_size == -1)
		{
			servo.duty = DEG_MID + 80;
		}
		else
		{
			servo.duty = (int16)(DEG_MID + 70 + feature.roundabouts_size * 80);
		}
	}
	else if(feature.roundabouts_state == 2)
	{
		if(feature.right_flection2_flag == 1)
		{
			feature.roundabouts_k_record[0] = (float32)(line.right_line[feature.right_flection2_row] - line.left_line[state12_fill_row]) / (state12_fill_row - feature.right_flection2_row);
			state12_k = 0.6 * feature.roundabouts_k_record[0] + 0.2 * feature.roundabouts_k_record[1] + 0.2 * feature.roundabouts_k_record[2];
			for(i = state12_fill_row; i > feature.right_flection2_row; i--)
			{
				line.left_line[i] = (int16)(line.right_line[feature.right_flection2_row] + state12_k * (feature.right_flection2_row - i));
				if(line.left_line[i] < 0)
				{
					line.left_line[i] = 0;
				}
				else if(line.left_line[i] > 159)
				{
					line.left_line[i] = 159;
				}
				line.midline[i] = line.left_line[i] + half_width[i];
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
			for(i = feature.right_flection2_row; i > 5; i--)
			{
				line.midline[i] = 0;
			}
		}
		else if(feature.left_flection2_flag == 1)
		{
			feature.roundabouts_k_record[0] = (float32)(line.left_line[feature.left_flection2_row] - line.left_line[state12_fill_row]) / (state12_fill_row - feature.left_flection2_row);
			state12_k = 0.6 * feature.roundabouts_k_record[0] + 0.2 * feature.roundabouts_k_record[1] + 0.2 * feature.roundabouts_k_record[2];
			for(i = state12_fill_row; i > feature.left_flection2_row; i--)
			{
				line.left_line[i] = (int16)(line.left_line[feature.left_flection2_row] + state12_k * (feature.left_flection2_row - i));
				if(line.left_line[i] < 0)
				{
					line.left_line[i] = 0;
				}
				else if(line.left_line[i] > 159)
				{
					line.left_line[i] = 159;
				}
				line.midline[i] = line.left_line[i] + half_width[i];
				if(line.midline[i] < 0)
				{
					line.midline[i] = 0;
				}
				else if(line.midline[i] > 159)
				{
					line.midline[i] = 159;
				}
			}
		}
		if(feature.right_flection2_flag == 1)
		{
			if(feature.right_flection2_row > 16 || feature.right_flection2_row == 0)
			{
				feature.roundabouts_size = 2;
//				servo.duty = DEG_MID - 60 - 60;
			}
			else if(feature.right_flection2_row < 12)
			{
				feature.roundabouts_size = -1;
//				servo.duty = DEG_MID - 60;
			}
			else if(feature.roundabouts_size <= 0 || feature.roundabouts_size >= 1)
			{
				feature.roundabouts_size = (float32)(feature.right_flection2_row - 12) / 4;
//				servo.duty = (int16)(DEG_MID - 60 - feature.roundabouts_size * 60);
			}
		}
	}
	else if(feature.roundabouts_state == 6)
	{
		servo.enable = 0;
		if(feature.roundabouts_size == 2)
		{
			servo.duty = DEG_MID - 70 - 80;
		}
		else if(feature.roundabouts_size == -1)
		{
			servo.duty = DEG_MID - 80;
		}
		else
		{
			servo.duty = (int16)(DEG_MID - 70 - feature.roundabouts_size * 80);
		}
	}
//	else if(feature.roundabouts_state == 3 || feature.roundabouts_state == 4)
//	{
//		servo.duty_record[0] = servo.duty;
//		for(i = 10; i > 0; i--)
//		{
//			servo.duty_record[i] = servo.duty_record[i - 1];
//		}
//	}
//	else if(feature.roundabouts_state == 5 || feature.roundabouts_state == 6)
//	{
//		servo.enable = 0;
//		servo.duty = Get_Median(servo.duty_record, 19);
//	}
}

/***************************************************************
	*	@brief	断路处理
	*	@param	无
	*	@note	无
***************************************************************/
void Breakage_Fill(void)
{
	uint8 i = 0;
	if(feature.breakage_state == 1)
	{
//		BUZZER_ON;
	}
	else if(feature.breakage_state == 2)
	{
//		BUZZER_ON;
		//左补线
		for(i = 117; i > feature.top_point; i--)
		{
			line.midline[i] = line.midline[i + 1] - (line.right_line[i + 1] - line.right_line[i]);
		}
	}
	else if(feature.breakage_state == 3)
	{
//		BUZZER_ON;
		//右补线
		for(i = 117; i > feature.top_point; i--)
		{
			line.midline[i] = line.midline[i + 1] + (line.left_line[i] - line.left_line[i + 1]);
		}
	}
	else if(feature.breakage_state == 4)
	{
		//切电磁
	}
	else if(feature.breakage_state == 5)
	{
		for(i = 80; i > 30; i--)
		{
			if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 1
			   && line.left_line_flag[i + 1] == 1 && line.right_line_flag[i] == 1)
			{
				servo.fore_max = i;
			}
		}
	}
}

/***************************************************************
	*	@brief	速度设定
	*	@param	无
	*	@note	无
***************************************************************/
void Speed_Set(void)
{
	int16 error_ave;
	if(feature.roundabouts_state != 0)
	{
		if(feature.roundabouts_state == 1 || feature.roundabouts_state == 2)
		{
			motor.speed_set = speed.roundabouts;
		}
		else if(feature.roundabouts_state == 3 || feature.roundabouts_state == 4)
		{
			if(feature.turn_state == 5 || feature.turn_state == 6)
			{
				motor.speed_set = 50;
			}
			else if(feature.turn_state == 3 || feature.turn_state == 4)
			{
				motor.speed_set = speed.curve_low;
			}
			else
			{
				error_ave = (int16)(0.6 * servo.error[0] + 0.2 * servo.error[1] + 0.2 * servo.error[2]);
				motor.speed_set = (int16)(speed.curve_high - (speed.curve_high - speed.curve_low) * error_ave * error_ave / 40.0 / 40);
				if(motor.speed_set > speed.curve_high)
				{
					motor.speed_set = speed.curve_high;
				}
				else if(motor.speed_set < speed.curve_low)
				{
					motor.speed_set = speed.curve_low;
				}
			}
		}
		else if(feature.roundabouts_state == 5 || feature.roundabouts_state == 6)
		{
			motor.speed_set = speed.straight + 50;
		}
		feature.road_type[0] = 4;
	}
    else if(feature.ramp_state[1] == 1)
    {
        feature.road_type[0] = 6;
        if(motor.speed_ave > speed.curve_high)
        {
            motor.speed_set = speed.curve_high;
        }
        else
        {
            motor.speed_set = speed.curve_high + (motor.speed_ave - speed.curve_high);
        }
    }
	else if(feature.cross_state[1] == 1)
	{
		feature.road_type[0] = 5;
		motor.speed_set = speed.cross;
	}
	else if(feature.straight_state == 1)
	{
		feature.road_type[0] = 1;
		if(feature.turn_state == 0)
		{
			motor.speed_set = speed.long_straight;
		}
		else
		{
			motor.speed_set = speed.long_straight - 50;
		}
	}
	else if(feature.straight_state == 2)
	{
		feature.road_type[0] = 2;
		if(feature.turn_state == 0)
		{
			motor.speed_set = speed.straight;
		}
		else
		{
			motor.speed_set = speed.straight - 20;
		}
	}
	else
	{
		feature.road_type[0] = 3;
		if(feature.turn_state == 5 || feature.turn_state == 6)
		{
			motor.speed_set = 50;
		}
		else if(feature.turn_state == 3 || feature.turn_state == 4)
		{
			motor.speed_set = speed.curve_low;
		}
		else
		{
			error_ave = (int16)(0.6 * servo.error[0] + 0.2 * servo.error[1] + 0.2 * servo.error[2]);
			motor.speed_set = (int16)(speed.curve_high - (speed.curve_high - speed.curve_low) * error_ave * error_ave / 40.0 / 40);
			if(motor.speed_set > speed.curve_high)
			{
				motor.speed_set = speed.curve_high;
			}
			else if(motor.speed_set < speed.curve_low)
			{
				motor.speed_set = speed.curve_low;
			}
		}
	}
}

/***************************************************************
	*	@brief	参数设置
	*	@param	无
	*	@note	无
***************************************************************/
void Parameter_Setting(void)
{
	int8 key_state;
	DisableInterrupts;
	Parameter_Setting_Init();
	Setting_Paint();
	while(1)
	{
		key_state = Setting_Key_Scan();
		if(key_state == -1)
		{
			OLED_Fill(0x00);
			break;
		}
		else if(key_state != 0)
		{
			switch(key_state)
			{
				//上切页
			case 1:
				setting.page_num--;
				if(setting.page_num < 0)
				{
					setting.page_num = 4;
				}
				Setting_Paint();
				break;
				//下切页
			case 6:
				setting.page_num++;
				if(setting.page_num > 4)
				{
					setting.page_num = 0;
				}
				Setting_Paint();
				break;
				//上一项
			case 2:
				setting.course--;
				if(setting.course < 1)
				{
					setting.course = 7;
				}
				Setting_Paint();
				break;
				//下一项
			case 5:
				setting.course++;
				if(setting.course > 7)
				{
					setting.course = 1;
				}
				Setting_Paint();
				break;
				//data增加
			case 3:
				switch(setting.page_num)
				{
				case 0:
					setting.data[setting.page_num][setting.course] += 10;
					break;
				case 1:
					setting.data[setting.page_num][setting.course] += 1;
					break;
				case 2:
					setting.data[setting.page_num][setting.course] += 0.1;
					break;
				case 3:
					setting.data[setting.page_num][setting.course] += 1;
					break;
				case 4:
					setting.data[setting.page_num][setting.course] += 0.01;
					break;
				}
				Save_Data();
				OLED_Put6x8Str(70, setting.course, "      ");
				OLED_PrintFloatValue(70, setting.course, setting.data[setting.page_num][setting.course]);
				break;
				//data减小
			case 4:
				switch(setting.page_num)
				{
				case 0:
					setting.data[setting.page_num][setting.course] -= 10;
					break;
				case 1:
					setting.data[setting.page_num][setting.course] -= 1;
					break;
				case 2:
					setting.data[setting.page_num][setting.course] -= 0.1;
					break;
				case 3:
					setting.data[setting.page_num][setting.course] -= 1;
					break;
				case 4:
					setting.data[setting.page_num][setting.course] -= 0.01;
					break;
				}
				Save_Data();
				OLED_Put6x8Str(70, setting.course, "      ");
				OLED_PrintFloatValue(70, setting.course, setting.data[setting.page_num][setting.course]);
				break;
			}
		}
		LPLD_LPTMR_DelayMs(50);
	}
	EnableInterrupts;
}

/***************************************************************
	*	@brief	参数设置初始化
	*	@param	page：页数
	*	@note	无
***************************************************************/
void Parameter_Setting_Init(void)
{
	setting.page_num = 0;
	setting.course = 1;
	sprintf(setting.string[0][0], "SPEED");
	sprintf(setting.string[0][1], "straight");
	setting.data[0][1] = (float32)speed.straight;
	sprintf(setting.string[0][2], "l_straight");
	setting.data[0][2] = (float32)speed.long_straight;
	sprintf(setting.string[0][3], "curve_high");
	setting.data[0][3] = (float32)speed.curve_high;
	sprintf(setting.string[0][4], "curve_low");
	setting.data[0][4] = (float32)speed.curve_low;
	sprintf(setting.string[0][5], "cross");
	setting.data[0][5] = (float32)speed.cross;
	sprintf(setting.string[0][6], "round");
	setting.data[0][6] = (float32)speed.roundabouts;
        
	sprintf(setting.string[1][0], "FORESIGHT");
	sprintf(setting.string[1][1], "fore_min");
	setting.data[1][1] = (float32)servo.fore_min;
	sprintf(setting.string[1][2], "fore_max");
	setting.data[1][2] = (float32)servo.fore_max;
	sprintf(setting.string[1][3], "dead_zone");
	setting.data[1][3] = (float32)servo.dead_zone;
	sprintf(setting.string[1][4], "dyna_zone");
	setting.data[1][4] = (float32)servo.dynamic_zone;
        
	sprintf(setting.string[2][0], "SERVO");
	sprintf(setting.string[2][1], "kp_left");
	setting.data[2][1] = (float32)servo.kp_left;
	sprintf(setting.string[2][2], "kp_right");
	setting.data[2][2] = (float32)servo.kp_right;
	sprintf(setting.string[2][3], "ki");
	setting.data[2][3] = (float32)servo.ki;
	sprintf(setting.string[2][4], "kd");
	setting.data[2][4] = (float32)servo.kd;
	sprintf(setting.string[2][5], "dif_c_l");
	setting.data[2][5] = (float32)servo.dif_const_left;
	sprintf(setting.string[2][6], "dif_c_r");
	setting.data[2][6] = (float32)servo.dif_const_right;
	sprintf(setting.string[2][7], "enable");
	setting.data[2][7] = (float32)servo.enable;
        
	sprintf(setting.string[3][0], "MOTOR");
	sprintf(setting.string[3][1], "kp");
	setting.data[3][1] = (float32)motor.kp;
	sprintf(setting.string[3][2], "ki");
	setting.data[3][2] = (float32)motor.ki;
	sprintf(setting.string[3][3], "kd");
	setting.data[3][3] = (float32)motor.kd;
	sprintf(setting.string[3][4], "openloop");
	setting.data[3][4] = (float32)motor.is_open_loop;
	
	sprintf(setting.string[4][0], "MOTOR_DIF");
	sprintf(setting.string[4][1], "const");
	setting.data[4][1] = (float32)motor.dif_const;
	sprintf(setting.string[4][2], "fore");
	setting.data[4][2] = (float32)motor.dif_fore;
}

/***************************************************************
	*	@brief	绘制页面
	*	@param	无
	*	@note	无
***************************************************************/
void Setting_Paint(void)
{
	uint8 i = 0;
	OLED_Fill(0x00);
	switch(setting.page_num)
	{
	case 0:
		OLED_Put6x8Str(8, 0, setting.string[0][0]);
		OLED_Put6x8Str(115, 0, "P1");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[0][i]);
			OLED_PrintFloatValue(70, i, setting.data[0][i]);
		}
		break;
	case 1:
		OLED_Put6x8Str(8, 0, setting.string[1][0]);
		OLED_Put6x8Str(115, 0, "P2");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[1][i]);
			OLED_PrintFloatValue(70, i, setting.data[1][i]);
		}
		break;
	case 2:
		OLED_Put6x8Str(8, 0, setting.string[2][0]);
		OLED_Put6x8Str(115, 0, "P3");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[2][i]);
			OLED_PrintFloatValue(70, i, setting.data[2][i]);
		}
		break;
	case 3:
		OLED_Put6x8Str(8, 0, setting.string[3][0]);
		OLED_Put6x8Str(115, 0, "P4");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[3][i]);
			OLED_PrintFloatValue(70, i, setting.data[3][i]);
		}
		break;
		break;
	case 4:
		OLED_Put6x8Str(8, 0, setting.string[4][0]);
		OLED_Put6x8Str(115, 0, "P5");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[4][i]);
			OLED_PrintFloatValue(70, i, setting.data[4][i]);
		}
		break;
	}
	OLED_Put6x8Char(0, setting.course, '*');
}

/***************************************************************
	*	@brief	参数设置按键检测
	*	@param	无
	*	@note	无
***************************************************************/
int8 Setting_Key_Scan(void)
{
	if(SWITCH1 == 1 || SWITCH2 == 1 || SWITCH3 == 1)
	{
		Key_Delay();
		if(SWITCH1 == 1 || SWITCH2 == 1 || SWITCH3 == 1)
		{
			return -1;
		}
	}
	else if(KEY1 == 0)
	{
		Key_Delay();
		if(KEY1 == 0)
		{
			BUZZER_ON;
			Key_Delay();
			BUZZER_OFF;
			return 1;
		}
	}
	else if(KEY2 == 0)
	{
		Key_Delay();
		if(KEY2 == 0)
		{
			BUZZER_ON;
			Key_Delay();
			BUZZER_OFF;
			return 2;
		}
	}
	else if(KEY3 == 0)
	{
		Key_Delay();
		if(KEY3 == 0)
		{
			BUZZER_ON;
			Key_Delay();
			BUZZER_OFF;
			return 3;
		}
	}
	else if(KEY4 == 0)
	{
		Key_Delay();
		if(KEY4 == 0)
		{
			BUZZER_ON;
			Key_Delay();
			BUZZER_OFF;
			return 4;
		}
	}
	else if(KEY5 == 0)
	{
		Key_Delay();
		if(KEY5 == 0)
		{
			BUZZER_ON;
			Key_Delay();
			BUZZER_OFF;
			return 5;
		}
	}
	else if(KEY6 == 0)
	{
		Key_Delay();
		if(KEY6 == 0)
		{
			BUZZER_ON;
			Key_Delay();
			BUZZER_OFF;
			return 6;
		}
	}
	return 0;
}

/***************************************************************
	*	@brief	参数保存
	*	@param	无
	*	@note	无
***************************************************************/
void Save_Data(void)
{
	speed.straight = (int16)setting.data[0][1];
	speed.long_straight = (int16)setting.data[0][2];
	speed.curve_high = (int16)setting.data[0][3];
	speed.curve_low = (int16)setting.data[0][4];
	speed.cross = (int16)setting.data[0][5];
	speed.roundabouts = (int16)setting.data[0][6];
	servo.fore_min = (uint8)setting.data[1][1];
	servo.fore_max = (uint8)setting.data[1][2];
	servo.dead_zone = (uint8)setting.data[1][3];
	servo.dynamic_zone = (uint8)setting.data[1][4];
	servo.kp_left = setting.data[2][1];
	servo.kp_right = setting.data[2][2];
	servo.ki = setting.data[2][3];
	servo.kd = setting.data[2][4];
	servo.dif_const_left = setting.data[2][5];
	servo.dif_const_right = setting.data[2][6];
	servo.enable = (uint8)setting.data[2][7];
	motor.kp = setting.data[3][1];
	motor.ki = setting.data[3][2];
	motor.kd = setting.data[3][3];
	motor.is_open_loop = (int8)setting.data[3][4];
	motor.dif_const = setting.data[4][1];
	motor.dif_fore = setting.data[4][2];
}


/**********************a little funcitons**********************/
/***************************************************************
	*	@brief	是否丢线
	*	@param	无
	*	@note	无
***************************************************************/
uint8 is_Lose_All(uint8 row)
{
	uint8 i = 0, j = 0;
	int16 black_count = 0;
	for(i = row; i > row - 3; i--)
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

/***************************************************************
	*	@brief	求数组中的中位数
	*	@param	arr：首地址指针，length：长度
	*	@note	无
***************************************************************/
int16 Get_Median(int16* arr, uint8 length)
{
	uint8 i = 0, j = 0;
	uint8 min;
	int16 temp;
	for(i = 0; i < length / 2; i++)
	{
		min = i;
		for(j = i + 1; j < length; j++)
		{
			if(arr[min] < arr[j])
			{
				min = j;
			}
		}
		temp = arr[i];
		arr[i] = arr[min];
		arr[min] = temp;
	}
	return arr[length / 2];
}