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
	//����
	uint8 top_point;
	//��ǹյ㣨ʮ�ֽ��յ㣩
	uint8 left_flection_row;
	uint8 left_flection_flag;
	uint8 right_flection_row;
	uint8 right_flection_flag;
	//�۽ǹյ㣨ʮ��Զ�յ㣩
	uint8 left_flection2_row;
	uint8 left_flection2_flag;
	uint8 left_flection2_antiflag;
	uint8 right_flection2_row;
	uint8 right_flection2_flag;
	uint8 right_flection2_antiflag;
	//б��
	float32 k_left;
	float32 k_left2;
	float32 k_right;
	float32 k_right2;
	float32 k_left_record[3];
	float32 k_right_record[3];
	float32 k_left_record2[3];
	float32 k_right_record2[3];
	//�������ͼ�¼ 1Ϊ��ֱ����2Ϊ��ֱ����3Ϊ�䣬4Ϊ����5Ϊʮ�֣�6Ϊ�µ���7Ϊ������8Ϊ�ϰ�
	uint8 road_type[200];
	//ֱ����־
	uint8 straight_state;
	//����ת��־
	uint8 pre_turn_state;
	uint8 turn_state;
	uint8 turn_row;
	//ʮ�ֱ�־
	uint8 cross_state[2];
	//������־
	uint8 roundabouts_state;
	float32 roundabouts_k_record[3];
	float32 roundabouts_size;
	//��·��־
	uint8 breakage_state;
    //�µ���־
    uint8 ramp_state;
    //·�ϱ�־
    uint8 block_state;
	int16 breakage_radius_curvature;
}Feature_Class;
extern Feature_Class feature;
extern float32 d1,d2,d3,d4,d5;
void Find_Line(void);
void Judge_Feature(void);
void Find_Top_Point(void);
void Find_Inflection(void);
void Find_Inflection2(void);
void Judge_Straight(void);
void Judge_Curve(void);
void Judge_Cross(void);
void Judge_Roundabouts(void);
void Judge_breakage(void);
void Judge_ramp(void);
void Judge_Block(void);
void Judge_breramp(void);
float Midline_Std_Deviation(uint8 row_max, uint8 row_min);
uint8 is_Left_Lose_Line(uint8 row);
uint8 is_Right_Lose_Line(uint8 row);
uint8 is_Left_Point_Lose_Line(uint8 row);
uint8 is_Right_Point_Lose_Line(uint8 row);
uint8 is_Left_Point_Lose_All_Line(uint8 row);
uint8 is_Right_Point_Lose_All_Line(uint8 row);
float32 Get_Radius_Curvature(int16 point_Ax, int16 point_Ay, int16 point_Bx, int16 point_By, int16 point_Cx, int16 point_Cy);
void Check_Half_Width(void);
void Bee(void);
#endif