#ifndef _IDENTIFY_H_
#define _IDENTIFY_H_

extern uint8 flag;

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
	uint8 right_flection2_row;
	uint8 right_flection2_flag;
	//�������ͼ�¼ 1Ϊ��ֱ����2Ϊ��ֱ����3Ϊ�䣬4Ϊ����5Ϊʮ�֣�6Ϊ�µ���7Ϊ������8Ϊ�ϰ�
	uint8 road_type[200];
	//ֱ����־
	uint8 straight_state;
	//����ת��־��12С��34���䣩
	uint8 turn_state;
	uint8 turn_row;
	//ʮ�ֱ�־
	uint8 cross_state[2];
	//������־
	uint8 roundabouts_state;
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

float Midline_Std_Deviation(void);
uint8 is_Left_Lose_Line(uint8 row);
uint8 is_Right_Lose_Line(uint8 row);
uint8 is_Left_Point_Lose_Line(uint8 row);
uint8 is_Right_Point_Lose_Line(uint8 row);

#endif