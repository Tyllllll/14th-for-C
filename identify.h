#ifndef _IDENTIFY_H_
#define _IDENTIFY_H_

typedef struct
{
	int16 left_line[120];
	uint8 left_line_flag[120];
	int16 right_line[120];
	uint8 right_line_flag[120];
	int16 midline[120];
}Line_Class;
extern Line_Class line;

typedef struct
{
	//顶点
	uint8 top_point;
    uint8 toppoint_err;
	//锐角拐点（十字近拐点）
	uint8 left_flection_row;
	uint8 left_flection_flag;
	uint8 right_flection_row;
	uint8 right_flection_flag;
	//钝角拐点（十字远拐点）
	uint8 left_flection2_row;
	uint8 left_flection2_flag;
	uint8 left_flection2_antiflag;
	uint8 right_flection2_row;
	uint8 right_flection2_flag;
	uint8 right_flection2_antiflag;
	//斜率
	float32 k_left_record[3];
	float32 k_right_record[3];
	float32 k_left_record2[3];
	float32 k_right_record2[3];
	//赛道类型记录 1为长直道，2为短直道，3为弯，4为环，5为十字，6为坡道，7为颠簸，8为断路，9为障碍
	uint8 road_type[200];
	//直道标志
	uint8 straight_state;
	//左右转标志
	uint8 turn_state;
	uint8 turn_row;
	uint8 deep_turn_state;
	//十字标志
	uint8 cross_state[2];
	//环岛标志
	uint8 roundabouts_state;
	float32 roundabouts_size;
    //断路标志
    uint8 breakage_state[2]; //[1]:0=无断路 1=即将进入或已进入断路  
    //障碍标志
    uint8 block_state[2];   //
    uint8 ramp_state[2];
    uint8 breakage_row;
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
void Judge_Block(void);
void Judge_ramp(void);
float Midline_Std_Deviation(uint8 row_max,uint8 row_min);
uint8 is_Left_Lose_Line(uint8 row);
uint8 is_Right_Lose_Line(uint8 row);
uint8 is_Left_Point_Lose_Line(uint8 row);
uint8 is_Right_Point_Lose_Line(uint8 row);
uint8 is_Left_Point_Lose_All_Line(uint8 row);
uint8 is_Right_Point_Lose_All_Line(uint8 row);

#endif