#ifndef _CONTROL_H_
#define _CONTROL_H_

#define SPEED_MAX 500
#define SPEED_MIN 50

extern uint8 half_width[120];

typedef struct
{
	int16 straight;
	int16 long_straight;
	int16 curve_high;
	int16 curve_low;
	int16 roundabouts;
	int16 breakage;
	int16 ramp;
    int16 block;
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
void All_Fill(void);
void Curve_Fill(void);
void Cross_Fill(void);
void Roundabouts_Fill(void);
void Breakage_Fill(void);
void Speed_Set(void);
void Parameter_Setting(void);
void Parameter_Setting_Init(void);
void Setting_Paint(void);
int8 Setting_Key_Scan(void);
void Save_Data(void);

uint8 is_Lose_All(uint8 row);
int16 Get_Median(int16* arr, uint8 length);
float Constrain_32(float *data, float max_out,float min_out);
int16 Constrain_16(int16 *data, int16 max_out,int16 min_out);
#endif