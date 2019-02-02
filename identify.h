#ifndef _IDENTIFY_H_
#define _IDENTIFY_H_

typedef struct
{
	uint8 left_line[120];
	uint8 left_line_flag[120];
	uint8 right_line[120];
	uint8 right_line_flag[120];
	uint8 midline[120];
}Line_Class;
extern Line_Class line;

typedef struct
{
	//顶点
	uint8 top_point;
	//锐角拐点（十字近拐点）
	uint8 left_flection_row;
	uint8 left_flection_flag;
	uint8 right_flection_row;
	uint8 right_flection_flag;
	//钝角拐点（十字远拐点）
	uint8 left_flection2_row;
	uint8 left_flection2_flag;
	uint8 right_flection2_row;
	uint8 right_flection2_flag;
	//左右转标志（12小弯34深弯）
	uint8 turn_flag;
}Feature_Class;
extern Feature_Class feature;

void Find_Line(void);
void Judge_Feature(void);
void Find_Top_Point(void);
void Find_Inflection(void);
void Find_Inflection2(void);
void Judge_Curve(void);

#endif