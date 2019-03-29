#ifndef _IDENTIFY_H_
#define _IDENTIFY_H_

typedef struct
{
	int16 left_line[120];
	uint8 left_line_flag[120];
	int16 right_line[120];
	uint8 right_line_flag[120];
	int16 midline[120];
	int16 half_width_test[120];
}Line_Class;
extern Line_Class line;

typedef struct
{
	//顶点
	uint8 top_point;
    uint8 fullline;
	//锐角拐点（十字近拐点）
	uint8 left_flection_row;
	int8 left_flection_flag;
	uint8 right_flection_row;
	int8 right_flection_flag;
	//钝角拐点（十字远拐点）
	uint8 left_flection2_row;
	int8 left_flection2_flag;
	int8 left_flection2_antiflag;
	uint8 right_flection2_row;
	int8 right_flection2_flag;
	int8 right_flection2_antiflag;
	//斜率
	float32 k_left;
	float32 k_left2;
	float32 k_right;
	float32 k_right2;
	float32 k_mid;
	float32 k_left_record[3];
	float32 k_right_record[3];
	float32 k_left_record2[3];
	float32 k_right_record2[3];
	float32 k_mid_record[3];
	//赛道类型记录 1为长直道，2为短直道，3为弯，4为环，5为断路，6为坡道，7为颠簸，8为障碍
	int8 road_type[200];
	//直道标志
	int8 straight_state;
	//左右转标志
	int8 pre_turn_state;
	int8 turn_state;
	uint8 turn_row;
	//十字标志
	int8 cross_state[2];
	//环岛标志
	int8 roundabouts_state;
	float32 roundabouts_k_record[3];
	//断路标志
	int8 breakage_state;
	int16 breakage_radius_curvature;
	float32 breakage_k_record[3];
	//坡道标志
	int8 ramp_state;
	//路障标志
	int8 roadblock_state;
}Feature_Class;
extern Feature_Class feature;

void Find_Line(void);
void Judge_Feature(void);
void Find_Top_Point(void);
void Find_Inflection(void);
void Find_Inflection2(void);
void Judge_Straight(void);
void Judge_Curve(void);
void Judge_Cross(void);
void Judge_Roundabouts(void);
void Judge_Breakage(void);
void Judge_Ramp(void);
void Judge_Roadblock(void);

float Midline_Std_Deviation(uint8 row_max, uint8 row_min);
uint8 is_Left_Lose_Line(uint8 row);
uint8 is_Right_Lose_Line(uint8 row);
uint8 is_Left_Point_Lose_Line(uint8 row);
uint8 is_Right_Point_Lose_Line(uint8 row);
uint8 is_Left_Point_Lose_All_Line(uint8 row, uint8 area);
uint8 is_Right_Point_Lose_All_Line(uint8 row, uint8 area);
float32 Get_Radius_Curvature(int16 point_Ax, int16 point_Ay, int16 point_Bx, int16 point_By, int16 point_Cx, int16 point_Cy);
int16 Get_Mid_Average(uint8 row);
void Check_Half_Width(void);
float32 line_Slope(uint8 type, uint8 row_beg, uint8 row_end);

#endif