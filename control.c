#include "header.h"

//赛道类型记录 1为长直道，2为短直道，3为弯，4为环，5为十字，6为坡道，7为颠簸，8为障碍
uint8 road_type[200];
uint8 half_width[120];
Speed_Class speed;
Setting_Class setting;

/***************************************************************
	*	@brief	速度数据初始化
	*	@param	无
	*	@note	无
***************************************************************/
void Speed_Init(void)
{
	speed.straight = 280;
	speed.long_straight = 330;
	speed.curve_high = 240;
	speed.curve_low = 220;
	speed.cross = 220;
	speed.roundabouts = 230;
}

/***************************************************************
	*	@brief	弯道补线
	*	@param	无
	*	@note	为环放最后，限幅有改动
***************************************************************/
void Curve_Fill(void)
{
	uint8 i = 0;
	if(feature.turn_state != 0)
	{
		//左小弯
		if(feature.turn_state == 1)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.right_line[i] == 159)
				{
					//丢线部分中线逐渐往左偏
					line.midline[i] = line.midline[i + 1] - 1;
				}
				else
				{
					//丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
					line.midline[i] = line.midline[i + 1] - (line.right_line[i + 1] - line.right_line[i]);
				}
				if(line.midline[i] <= 0)
				{
//					line.left_line[i] = 0;
//					line.right_line[i] = 0;
					line.midline[i] = 0;
				}
			}
		}
		//右小弯
		else if(feature.turn_state == 2)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.left_line[i] == 0)
				{
					//丢线部分中线逐渐往左偏
					line.midline[i] = line.midline[i + 1] + 1;
				}
				else
				{
					//丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
					line.midline[i] = line.midline[i + 1] - (line.left_line[i] - line.left_line[i + 1]);
				}
				if(line.midline[i] >= 159)
				{
//					line.left_line[i] = 159;
//					line.right_line[i] = 159;
					line.midline[i] = 159;
				}
			}
		}
		//左深弯
		else if(feature.turn_state == 3)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//直接以右边界减去80，将中线轨迹向左偏离幅度更大
				line.midline[i] = line.right_line[i] - 80;
				if(line.midline[i] <= 0)
				{
//					line.left_line[i] = 0;
//					line.right_line[i] = 0;
					line.midline[i] = 0;
				}
			}
		}
		//右深弯
		else if(feature.turn_state == 4)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//直接以左边界加80，将中线轨迹向右偏离幅度更大
				line.midline[i] = line.left_line[i] + 80;
				if(line.midline[i] >= 159)
				{
//					line.left_line[i] = 159;
//					line.right_line[i] = 159;
					line.midline[i] = 159;
				}
			}
		}
	}
}

/***************************************************************
	*	@brief	十字补线
	*	@param	无
	*	@note	无
***************************************************************/
void Cross_Fill(void)
{
	uint8 i = 0;
	float32 k_left_record[3];
	float32 k_right_record[3];
	float32 k_left;
	float32 k_right;
	if(feature.left_flection_flag == 1)
	{
		if(feature.left_flection_row + 8 < 118)
		{
			k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[feature.left_flection_row + 8]) / 8.0;
		}
		else
		{
			k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[118]) / (118.0 - feature.left_flection_row);
		}
		//加权左线斜率
		k_left = 0.6 * k_left_record[0] + 0.2 * k_left_record[1] + 0.2 * k_left_record[2];
		//左补线
		for(i = feature.left_flection_row; i > feature.left_flection2_row; i--)
		{
			line.left_line[i] = (uint8)(line.left_line[feature.left_flection_row] + k_left * (feature.left_flection_row - i));
			if(line.left_line[i] < 0)
			{
				line.left_line[i] = 0;
			}
			if(line.left_line[i] > 159)
			{
				line.left_line[i] = 159;
			}
		}
	}
	if(feature.right_flection_flag == 1)
	{
		if(feature.right_flection_row + 8 < 118)
		{
			k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[feature.right_flection_row + 8]) / 8.0;
		}
		else
		{
			k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[118]) / (118.0 - feature.right_flection_row);
		}
		//加权右线斜率
		k_right = 0.6 * k_right_record[0] + 0.2 * k_right_record[1] + 0.2 * k_right_record[2];
		//右补线
		for(i = feature.right_flection_row; i > feature.right_flection2_row; i--)
		{
			line.right_line[i] = (uint8)(line.right_line[feature.right_flection_row] + k_right * (feature.right_flection_row - i));
			if(line.right_line[i] < 0)
			{
				line.right_line[i] = 0;
			}
			if(line.right_line[i] > 159)
			{
				line.right_line[i] = 159;
			}
		}
	}
	k_left_record[2] = k_left_record[1];
	k_left_record[1] = k_left_record[0];
	k_right_record[2] = k_right_record[1];
	k_right_record[1] = k_right_record[0];
	//算中线
	if(feature.left_flection_flag == 1 && feature.right_flection_row == 1)
	{
		for(i = (uint8)fmax(feature.left_flection_row, feature.right_flection_row); i > (uint8)fmin(feature.left_flection2_row, feature.right_flection2_row); i--)
		{
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
	}
	if(feature.left_flection_flag == 1 && feature.right_flection_flag == 0)
	{
		for(i = feature.left_flection_row; i > feature.left_flection2_row; i--)
		{
			line.midline[i] = line.left_line[i] + half_width[i] + 10;
		}
	}
	if(feature.left_flection_flag == 0 && feature.right_flection_flag == 1)
	{
		for(i = feature.left_flection_row; i > feature.left_flection2_row; i--)
		{
			line.midline[i] = line.right_line[i] - half_width[i] - 10;
		}
	}
	//补完十字后重新定义一个新的顶点
	Find_Top_Point();
}

/***************************************************************
	*	@brief	环岛补线
	*	@param	无
	*	@note	补边线
***************************************************************/
void Roundabouts_Fill(void)
{
	
}

/***************************************************************
	*	@brief	速度设定
	*	@param	无
	*	@note	无
***************************************************************/
void Speed_Set(void)
{
	int16 error_ave;
	//保留出环加速
//	if(feature.roundabouts_state != 0 && feature.roundabouts_state != 5 && feature.roundabouts_state != 6)
	if(feature.roundabouts_state != 0)
	{
		road_type[0] = 4;
		motor.speed_set = speed.roundabouts;
	}
	else if(feature.cross_state[1] == 1)
	{
		road_type[0] = 5;
		motor.speed_set = speed.cross;
	}
	else if(feature.straight_state == 1)
	{
		road_type[0] = 1;
		motor.speed_set = speed.long_straight;
	}
	else if(feature.straight_state == 2)
	{
		road_type[0] = 2;
		motor.speed_set = speed.straight;
	}
	else if(feature.turn_state != 0)
	{
		road_type[0] = 3;
		error_ave = (int16)(0.6 * servo.error[0] + 0.2 * servo.error[1] + 0.2 * servo.error[2]);
		motor.speed_set = (int16)(speed.curve_high - (speed.curve_high - speed.curve_low) * error_ave * error_ave / 40.0 / 40);
		if(motor.speed_set > speed.curve_high)
		{
			motor.speed_set = speed.curve_high;
		}
		if(motor.speed_set < speed.curve_low)
		{
			motor.speed_set = speed.curve_low;
		}
	}
}

/***************************************************************
	*	@brief	是否丢线
	*	@param	无
	*	@note	无
***************************************************************/
uint8 is_Lose_All(void)
{
	uint8 i = 0, j = 0;
	int16 black_count = 0;
	for(i = 118; i > 115; i--)
	{
		for(j = 130; j >= 30; j--)
		{
			if(camera.image[i][j] == 0)
			{
				black_count++;
			}
		}
	}
	if(black_count > 250)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/***************************************************************
	*	@brief	参数设置
	*	@param	无
	*	@note	无
***************************************************************/
void Parameter_Setting(void)
{
	int8 key_state;
	DisableInterrupts;
	Parameter_Setting_Init();
	while(1)
	{
		key_state = Setting_Key_Scan();
		if(key_state == -1)
		{
			OLED_Fill(0x00);
			break;
		}
		else if(key_state != 0)
		{
			switch(key_state)
			{
				//上切页
			case 1:
				if(setting.page_num == 0)
				{
					setting.page_num = 3;
				}
				Setting_Paint();
				break;
				//下切页
			case 6:
				if(setting.page_num == 3)
				{
					setting.page_num = 0;
				}
				Setting_Paint();
				break;
				//上一项
			case 2:
				if(setting.course == 0)
				{
					setting.course = 7;
				}
				Setting_Paint();
				break;
				//下一项
			case 5:
				if(setting.course == 7)
				{
					setting.course = 0;
				}
				Setting_Paint();
				break;
				//data增加
			case 3:
				switch(setting.page_num)
				{
				case 0:
					setting.data[setting.page_num][setting.course] += 10;
					break;
				case 1:
					setting.data[setting.page_num][setting.course] += 1;
					break;
				case 2:
					setting.data[setting.page_num][setting.course] += 0.1;
					break;
				case 3:
					setting.data[setting.page_num][setting.course] += 1;
					break;
				}
				Save_Data();
				OLED_Put6x8Str(70, setting.course, "      ");
				OLED_PrintFloatValue(70, setting.course, setting.data[setting.page_num][setting.course]);
				break;
				//data减小
			case 4:
				switch(setting.page_num)
				{
				case 0:
					setting.data[setting.page_num][setting.course] -= 10;
					break;
				case 1:
					setting.data[setting.page_num][setting.course] -= 1;
					break;
				case 2:
					setting.data[setting.page_num][setting.course] -= 0.1;
					break;
				case 3:
					setting.data[setting.page_num][setting.course] -= 1;
					break;
				}
				Save_Data();
				OLED_Put6x8Str(70, setting.course, "      ");
				OLED_PrintFloatValue(70, setting.course, setting.data[setting.page_num][setting.course]);
				break;
			}
		}
	}
}

/***************************************************************
	*	@brief	参数设置初始化
	*	@param	page：页数
	*	@note	无
***************************************************************/
void Parameter_Setting_Init(void)
{
	sprintf(setting.string[0][0], "SPEED");
	sprintf(setting.string[0][1], "straight");
	setting.data[0][1] = (float32)speed.straight;
	sprintf(setting.string[0][2], "l_straight");
	setting.data[0][2] = (float32)speed.long_straight;
	sprintf(setting.string[0][3], "curve_high");
	setting.data[0][3] = (float32)speed.curve_high;
	sprintf(setting.string[0][4], "curve_low");
	setting.data[0][4] = (float32)speed.curve_low;
	sprintf(setting.string[0][5], "cross");
	setting.data[0][5] = (float32)speed.cross;
	sprintf(setting.string[0][6], "round");
	setting.data[0][6] = (float32)speed.roundabouts;
	sprintf(setting.string[1][0], "FORESIGHT");
	sprintf(setting.string[1][1], "fore_min");
	setting.data[1][1] = (float32)servo.fore_min;
	sprintf(setting.string[1][2], "fore_max");
	setting.data[1][2] = (float32)servo.fore_max;
	sprintf(setting.string[1][3], "dead_zone");
	setting.data[1][3] = (float32)servo.dead_zone;
	sprintf(setting.string[1][4], "dynamic_zone");
	setting.data[1][4] = (float32)servo.dynamic_zone;
	sprintf(setting.string[2][0], "SERVO");
	sprintf(setting.string[2][1], "kp_left");
	setting.data[2][1] = (float32)servo.kp_left;
	sprintf(setting.string[2][2], "kp_right");
	setting.data[2][2] = (float32)servo.kp_right;
	sprintf(setting.string[2][3], "ki");
	setting.data[2][3] = (float32)servo.ki;
	sprintf(setting.string[2][4], "kd");
	setting.data[2][4] = (float32)servo.kd;
	sprintf(setting.string[3][0], "MOTOR");
	sprintf(setting.string[3][1], "kp");
	setting.data[3][1] = (float32)motor.kp;
	sprintf(setting.string[3][2], "ki");
	setting.data[3][2] = (float32)motor.ki;
	sprintf(setting.string[3][3], "kd");
	setting.data[3][3] = (float32)motor.kd;
}

/***************************************************************
	*	@brief	绘制页面
	*	@param	无
	*	@note	无
***************************************************************/
void Setting_Paint(void)
{
	uint8 i = 0;
	OLED_Fill(0x00);
	switch(setting.page_num)
	{
	case 0:
		OLED_Put6x8Str(8, 0, (uint8*)setting.string[0][0]);
		OLED_Put6x8Str(115, 0, "P1");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, (uint8*)setting.string[0][i]);
			OLED_PrintFloatValue(70, i, setting.data[0][i]);
		}
		break;
	case 1:
		OLED_Put6x8Str(8, 0, (uint8*)setting.string[1][0]);
		OLED_Put6x8Str(115, 0, "P2");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, (uint8*)setting.string[1][i]);
			OLED_PrintFloatValue(70, i, setting.data[1][i]);
		}
		break;
	case 2:
		OLED_Put6x8Str(8, 0, (uint8*)setting.string[2][0]);
		OLED_Put6x8Str(115, 0, "P3");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, (uint8*)setting.string[2][i]);
			OLED_PrintFloatValue(70, i, setting.data[2][i]);
		}
		break;
	case 3:
		OLED_Put6x8Str(8, 0, (uint8*)setting.string[3][0]);
		OLED_Put6x8Str(115, 0, "P4");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, (uint8*)setting.string[3][i]);
			OLED_PrintFloatValue(70, i, setting.data[3][i]);
		}
		break;
	}
	OLED_Put6x8Char(0, setting.course, '*');
}

/***************************************************************
	*	@brief	参数设置按键检测
	*	@param	无
	*	@note	无
***************************************************************/
int8 Setting_Key_Scan(void)
{
	if(SWITCH1 == 1 || SWITCH2 == 1 || SWITCH3 == 1 || SWITCH4 == 1)
	{
		Key_Delay();
		if(SWITCH1 == 1 || SWITCH2 == 1 || SWITCH3 == 1 || SWITCH4 == 1)
		{
			return -1;
		}
	}
	else if(KEY1 == 0)
	{
		Key_Delay();
		if(KEY1 == 0)
		{
			BUZZER_ON;
			return 1;
		}
	}
	else if(KEY2 == 0)
	{
		Key_Delay();
		if(KEY2 == 0)
		{
			BUZZER_ON;
			return 2;
		}
	}
	else if(KEY3 == 0)
	{
		Key_Delay();
		if(KEY3 == 0)
		{
			BUZZER_ON;
			return 3;
		}
	}
	else if(KEY4 == 0)
	{
		Key_Delay();
		if(KEY4 == 0)
		{
			BUZZER_ON;
			return 4;
		}
	}
	else if(KEY5 == 0)
	{
		Key_Delay();
		if(KEY5 == 0)
		{
			BUZZER_ON;
			return 5;
		}
	}
	else if(KEY6 == 0)
	{
		Key_Delay();
		if(KEY6 == 0)
		{
			BUZZER_ON;
			return 6;
		}
	}
	return 0;
}

/***************************************************************
	*	@brief	参数保存
	*	@param	无
	*	@note	无
***************************************************************/
void Save_Data(void)
{
	speed.straight = (int16)setting.data[0][1];
	speed.long_straight = (int16)setting.data[0][2];
	speed.curve_high = (int16)setting.data[0][3];
	speed.curve_low = (int16)setting.data[0][4];
	speed.cross = (int16)setting.data[0][5];
	speed.roundabouts = (int16)setting.data[0][6];
	servo.fore_min = (uint8)setting.data[1][1];
	servo.fore_max = (uint8)setting.data[1][2];
	servo.dead_zone = (uint8)setting.data[1][3];
	servo.dynamic_zone = (uint8)setting.data[1][4];
	servo.kp_left = setting.data[2][1];
	servo.kp_right = setting.data[2][2];
	servo.ki = setting.data[2][3];
	servo.kd = setting.data[2][4];
	motor.kp = setting.data[3][1];
	motor.ki = setting.data[3][2];
	motor.kd = setting.data[3][3];
}