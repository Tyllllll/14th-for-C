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
	uint8 column_start = 80;//基础寻线每行起始搜索列
	for(i = 100; i >5; i--)
	{
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
	
	Find_Top_Point();
	Find_Inflection();
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
	*	@brief	找拐点
	*	@param	无
	*	@note	十字近点处拐点（透视图中最大最小处）
***************************************************************/
void Find_Inflection(void)
{
	uint8 i = 0;
	if(feature.top_point < 100 && feature.top_point > 10)
	{
		//找左拐点
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.left_flection_flag == 0)
			{
				//位置条件
				if(i > 35 && line.left_line[i] < 140)
				{
					//大小条件
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] > line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5] &&
					   line.left_line[i - 3] - line.left_line[i + 3] < 4)
						//最后一个条件？？？
					{
						//存在性条件
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i + 3] == 1 && line.left_line_flag[i + 5] == 1)
						{
							//斜率条件
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
		//找右拐点
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.right_flection_flag == 0)
			{
				//位置条件
				if(i > 35 && line.right_line[i] > 20)
				{
					//大小条件
					if(line.right_line[i] < line.right_line[i + 3] && line.left_line[i] < line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5] &&
					   line.right_line[i - 3] - line.right_line[i + 3] > -4)
						//最后一个条件？？？
					{
						//存在性条件
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i + 3] == 1 && line.right_line_flag[i + 5] == 1)
						{
							//斜率条件
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
	*	@brief	找拐点2
	*	@param	无
	*	@note	十字远点处拐点（透视图中中间值）有待改进，未测试
***************************************************************/
void Find_Flection2(void)
{
	uint8 i = 0;
	//找左拐点2
	if(feature.top_point < 100 && feature.top_point > 10)
	{
		for(i = 100; i > feature.top_point + 4; i--)
		{
			if(feature.left_flection2_flag == 0)
			{
				//位置条件
				if(line.left_line[i] < 140)
				{
					//大小条件
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] < line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5])
					{
						//存在性条件
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i - 3] == 1 && line.left_line_flag[i - 5] == 1)
						{
							//斜率条件
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
		//找右拐点2
		for(i = 100; i > feature.top_point + 4; i--)
		{
			if(feature.right_flection2_flag == 0)
			{
				//位置条件
				if(line.right_line[i] > 20)
				{
					//大小条件
					if(line.right_line[i] < line.right_line[i + 3] && line.right_line[i] > line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5])
					{
						//存在性条件
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i - 3] == 1 && line.right_line_flag[i - 5] == 1)
						{
							//斜率条件
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