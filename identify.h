#ifndef _IDENTIFY_H_
#define _IDENTIFY_H_

typedef struct
{
	uint8 leftline[120];
	uint8 leftlineflag[120];
	uint8 rightline[120];
	uint8 rightlineflag[120];
	uint8 midline[120];
}Line_Class;
extern Line_Class line;

typedef struct
{
	uint8 toppoint;
	uint8 leftflection;
	uint8 rightflection;
}Feature_Class;
extern Feature_Class feature;

void Find_Line(void);
void Judge_Feature(void);
void Find_Toppoint(void);
void Find_Inflection(void);

#endif