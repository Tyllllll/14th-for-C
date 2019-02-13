#ifndef _CONTROL_H_
#define _CONTROL_H_

typedef struct
{
	int16 straight;
	int16 long_straight;
	int16 curve_high;
	int16 curve_low;
	int16 cross;
	int16 roundabouts;
}Speed_Class;
extern Speed_Class speed;

typedef struct
{
	int8 page_num;
	int8 string[8][8][30];
	float32 data[8][8];
	int8 course;
}Setting_Class;
extern Setting_Class setting;

void Speed_Init(void);
void Curve_Fill(void);
void Cross_Fill(void);
void Roundabouts_Fill(void);
void Speed_Set(void);
uint8 is_Lose_All(void);
void Parameter_Setting(void);
void Parameter_Setting_Init(void);
void Setting_Paint(void);
int8 Setting_Key_Scan(void);
void Save_Data(void);

#endif