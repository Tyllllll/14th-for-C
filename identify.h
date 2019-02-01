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
	uint8 toppoint;
	uint8 left_flection_row;
	uint8 left_flection_flag;
	uint8 right_flection_row;
	uint8 right_flection_flag;
}Feature_Class;
extern Feature_Class feature;

void Find_Line(void);
void Judge_Feature(void);
void Find_Toppoint(void);
void Find_Inflection(void);

#endif