#include "header.h"

/***************************************************************
	*	@brief	弯道补线
	*	@param	无
	*	@note	限幅有改动
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
	if(feature.cross_state[1] == 1)
	{
		if(feature.left_flection_flag == 1 && feature.right_flection_row == 1)
		{
			
		}
	}
}

/***************************************************************
	*	@brief	环岛补线
	*	@param	无
	*	@note	无
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
	
}