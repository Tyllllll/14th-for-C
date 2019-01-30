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
				line.leftline[i] = j - 1;
				line.leftlineflag[i] = 1;
				break;
			}
		}
		//从中线往右扫
		for(j = column_start; j < 157; j++)
		{
			//寻找右跳变点
			if(camera.image[i][j] != 0 && camera.image[i][j + 2] == 0)
			{
				line.rightline[i] = j + 1;
				line.rightlineflag[i] = 1;
				break;
			}
		}
		//计算中线
		if(line.leftlineflag[i] && line.rightlineflag[i])
		{
			line.midline[i] = (line.leftline[i] + line.rightline[i]) / 2;
		}
		else if(line.leftlineflag[i] && !line.rightlineflag[i])
		{
			line.rightline[i] = 159;
			line.midline[i] = (line.leftline[i] + line.rightline[i]) / 2;
		}
		else if(!line.leftlineflag[i] && line.rightlineflag[i])
		{
			line.leftline[i] = 0;
			line.midline[i] = line.rightline[i] / 2;
		}
		else if(!line.leftlineflag[i] && !line.rightlineflag[i])
		{
			line.leftline[i] = 0;
			line.rightline[i] = 159;
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
	Find_Toppoint();
	Find_Inflection();
}

/***************************************************************
	*	@brief	找顶点
	*	@param	无
	*	@note	无
***************************************************************/
void Find_Toppoint(void)
{
	uint8 i = 0;
	feature.toppoint = 0;
	for(i = 100; i > 5; i--)
	{
		if(camera.image[i][line.midline[i]] != 0 && (camera.image[i - 1][line.midline[i]] == 0 || camera.image[i - 3][line.midline[i]] == 0 || camera.image[i - 5][line.midline[i]] == 0))
		{
			feature.toppoint = i;
			break;
		}
	}
	if(feature.toppoint == 0)
	{
		for(i = 118; i > 2; i--)
		{
			if(line.midline[i] > 150 || line.midline[i] < 10)
			{
				feature.toppoint = i;
				break;
			}
		}
	}
}

/***************************************************************
	*	@brief	找拐点
	*	@param	无
	*	@note	无
***************************************************************/
void Find_Inflection(void)
{
	uint8 i = 0;
	if(feature.toppoint < 70 && feature.toppoint > 5)
	{
		for(i = 108; i > feature.toppoint + 4; i--)
		{
			
		}
	}
}
