#ifndef _CONTROL_H_
#define _CONTROL_H_

#define SPEED_MAX 500
#define SPEED_MIN 100

typedef struct
{
	int16 straight;
	int16 long_straight;
	int16 curve_high;
	int16 curve_low;
	int16 cross;
	int16 roundabouts;
    int16 breakage;
    int16 ramp;
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
extern uint8 half_width[120];
void Speed_Init(void);
void All_Fill(void);
void Curve_Fill(void);
void Cross_Fill(void);
void Roundabouts_Fill(void);
//void Breakage_Fill(void);
void Speed_Set(void);
void Parameter_Setting(void);
void Parameter_Setting_Init(void);
void Setting_Paint(void);
int8 Setting_Key_Scan(void);
void Save_Data(void);
float constrain_32(float *data, float max_out,float min_out);
int16 constrain_16(int16 *data, int16 max_out,int16 min_out);
uint8 is_Lose_All(uint8 row);
int16 Get_Median(int16* aSSrr, uint8 length);

#endif