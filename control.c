#include "header.h"

uint8 half_width[120] = {14, 17, 19, 20, 22, 23, 24, 24, 25, 25,//0
						27, 28, 29, 30, 31, 32, 33, 35, 36, 36,//1
						37, 39, 40, 41, 42, 43, 44, 45, 46, 47,//2
						48, 49, 50, 51, 52, 53, 53, 54, 55, 56,//3
						57, 58, 59, 60, 61, 62, 63, 64, 65, 65,//4
						66, 67, 68, 69, 70, 71, 72, 72, 73, 74,//5
						75, 76, 76, 76, 77, 77, 77, 78, 78, 79,//6
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//7
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//8
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//9
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79,//10
						79, 79, 79, 79, 79, 79, 79, 79, 79, 79//11
};
Speed_Class speed;
Setting_Class setting;

int16 error_ave=0;
int16 error_last=0;
int16 error_pre=0;

/***************************************************************
	*	@brief	�ٶ����ݳ�ʼ��
	*	@param	��
	*	@note	��
***************************************************************/
void Speed_Init(void)
{
	speed.straight = 200;
	speed.long_straight = 220;
	speed.curve_high = 180;
	speed.curve_low = 170;
	speed.cross = 170;
	speed.roundabouts = 170;
//	speed.straight = 280;
//	speed.long_straight = 330;
//	speed.curve_high = 240;
//	speed.curve_low = 220;
//	speed.cross = 220;
//	speed.roundabouts = 230;
}

/***************************************************************
	*	@brief	�ܲ���
	*	@param	��
	*	@note	��
***************************************************************/
void All_Fill(void)
{
//	Cross_Fill();
//	Curve_Fill();
}

/***************************************************************
	*	@brief	�������
	*	@param	��
	*	@note	Ϊ��������޷��иĶ�
***************************************************************/
void Curve_Fill(void)
{
	uint8 i = 0;
	if(feature.turn_state != 0)
	{
		//��С��
		if(feature.turn_state == 1)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.right_line[i] == 159)
				{
					//���߲�������������ƫ
					line.midline[i] = line.midline[i + 1] - 1;
				}
				else
				{
					//���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
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
		//��С��
		else if(feature.turn_state == 2)
		{
			for(i = feature.turn_row; i > feature.top_point; i--)
			{
				if(line.left_line[i] == 0)
				{
					//���߲�������������ƫ
					line.midline[i] = line.midline[i + 1] + 1;
				}
				else
				{
					//���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
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
		//������
		else if(feature.turn_state == 3)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//ֱ�����ұ߽��ȥ80�������߹켣����ƫ����ȸ���
				line.midline[i] = line.right_line[i] - 80;
				if(line.midline[i] <= 0)
				{
//					line.left_line[i] = 0;
//					line.right_line[i] = 0;
					line.midline[i] = 0;
				}
			}
		}
		//������
		else if(feature.turn_state == 4)
		{
			for(i = 118; i > feature.top_point; i--)
			{
				//ֱ������߽��80�������߹켣����ƫ����ȸ���
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
	*	@brief	ʮ�ֲ���
	*	@param	��
	*	@note	��
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
			k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[feature.left_flection_row + 8]) / (8.0 - 2);
		}
		else
		{
			k_left_record[0] = (line.left_line[feature.left_flection_row] - line.left_line[118]) / (118.0 - feature.left_flection_row - 2);
		}
		//��Ȩ����б��
		k_left = 0.6 * k_left_record[0] + 0.2 * k_left_record[1] + 0.2 * k_left_record[2];
		//����
		for(i = feature.left_flection_row; i > (feature.top_point > 10 ? feature.top_point - 10 : feature.top_point); i--)
		{
			line.left_line[i] = (int16)(line.left_line[feature.left_flection_row] + k_left * (feature.left_flection_row - i));
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
			k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[feature.right_flection_row + 8]) / (8.0 - 2);
		}
		else
		{
			k_right_record[0] = (line.right_line[feature.right_flection_row] - line.right_line[118]) / (118.0 - feature.right_flection_row - 2);
		}
		//��Ȩ����б��
		k_right = 0.6 * k_right_record[0] + 0.2 * k_right_record[1] + 0.2 * k_right_record[2];
		//�Ҳ���
		for(i = feature.right_flection_row; i > (feature.top_point > 10 ? feature.top_point - 10 : feature.top_point); i--)
		{
			line.right_line[i] = (int16)(line.right_line[feature.right_flection_row] + k_right * (feature.right_flection_row - i));
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
	//������
	if(feature.left_flection_flag == 1 && feature.right_flection_row == 1)
	{
		for(i = feature.left_flection_row > feature.right_flection_row ? feature.left_flection_row: feature.right_flection_row; i > (feature.top_point > 10 ? feature.top_point - 10 : feature.top_point); i--)
		{
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
	}
	if(feature.left_flection_flag == 1 && feature.right_flection_flag == 0)
	{
		for(i = feature.left_flection_row; i > (feature.top_point > 10 ? feature.top_point - 10 : feature.top_point); i--)
		{
			line.midline[i] = line.left_line[i] + half_width[i] + 10;
		}
	}
	if(feature.left_flection_flag == 0 && feature.right_flection_flag == 1)
	{
		for(i = feature.right_flection_row; i > (feature.top_point > 10 ? feature.top_point - 10 : feature.top_point); i--)
		{
			line.midline[i] = line.right_line[i] - half_width[i] - 10;
		}
	}
	//����ʮ�ֺ����¶���һ���µĶ���
	Find_Top_Point();
}

/***************************************************************
	*	@brief	��������
	*	@param	��
	*	@note	������
***************************************************************/
void Roundabouts_Fill(void)
{
	
}

/***************************************************************
	*	@brief	�ٶ��趨
	*	@param	��
	*	@note	��
***************************************************************/
void Speed_Set(void)
{
//	int16 i=0;
//	int16 g_HighestSpeed = 0;
//	int16 g_LowestSpeed = 0;
//    error_ave=(int16)(0.6*servo.error[0]+0.2*error_last+0.2*error_pre);//�˲�
//    error_pre=error_last;
//    error_last=error_ave;
////    super_zhidao=0;
////    
////    if(feature.cross_state[1]==0)
////        zhidao();//ֱ���ж�
///**********�ٶȸ���*************/
//    if(feature.cross_state[1]==1)
//    {  
//        feature.road_type[0]=5;
//        motor.speed_set = speed.cross; 
//    }
//    else if(feature.roundabouts_state != 0)
//	{
//		feature.road_type[0] = 4;
//		
//		motor.speed_set = speed.roundabouts;
//	}
////    else if(super_zhidao==1)
////    {
////        feature.road_type[0]=1;
////        if(feature.road_type[40]==3&&feature.road_type[41]==3&&feature.road_type[42]==3&&feature.road_type[43]==3&&feature.road_type[44]==3)
////            motor.speed_set=s_max+100;//������� δ���� �������ע��
////        else
////            motor.speed_set=s_max;
////    }
//    
//    else if(feature.top_point<=37)
//    {
//        feature.road_type[0]=2;
//        if(feature.road_type[6]==1&&feature.road_type[7]==1&&feature.road_type[8]==1&&feature.road_type[9]==1&&feature.road_type[10]==1)
//        {//ǰ8֡�ǳ���ֱ��,�ȼ���һ��
//            motor.speed_set=speed.curve_high-70;
//        }
//        if(feature.road_type[3]==3&&feature.road_type[4]==3&&feature.road_type[5]==3)
//        {//ǰ3֡����� ����ŷķ���ڼ���
//            motor.speed_set=speed.curve_high;
//        }
//        else
//        { 
//            motor.speed_set=(int16)(speed.straight-(speed.straight-speed.curve_high)*(abs(error_ave)/35));//ƫ�����35������ٶ�
//            motor.speed_set=motor.speed_set<speed.curve_high?speed.curve_high:motor.speed_set; 
//        }
//    }
//    else 
//    {
//        feature.road_type[0]=3;
//        if(feature.road_type[9]==1&&feature.road_type[10]==1&&feature.road_type[11]==1&&feature.road_type[12]==1&&feature.road_type[13]==1)
//        {//ǰ6֡�ǳ���ֱ��,�ȼ���һ��
//            motor.speed_set=speed.curve_high-50;
//        }
//        else
//        {
//            g_HighestSpeed=speed.curve_high;
//            g_LowestSpeed=speed.curve_low;    
//            //��̬��������ٶȺ�����ٶȣ���������ֱ����Ǳ��
//            motor.speed_set=g_HighestSpeed-(g_HighestSpeed-g_LowestSpeed)*(error_ave*error_ave)/(40*40);//ƫ�����40������ٶ�
//            motor.speed_set=motor.speed_set<g_LowestSpeed?g_LowestSpeed:motor.speed_set;
//            motor.speed_set=motor.speed_set>g_HighestSpeed?g_HighestSpeed:motor.speed_set;//��Сs_wan2���s_wan1
//        }
//        
//    }
//    for(i=200;i>=1;i--)
//    {
//        feature.road_type[i]=feature.road_type[i-1];
//    }
	int16 error_ave;
	//������������
//	if(feature.roundabouts_state != 0 && feature.roundabouts_state != 5 && feature.roundabouts_state != 6)
	if(feature.roundabouts_state != 0)
	{
		feature.road_type[0] = 4;
		motor.speed_set = speed.roundabouts;
	}
	else if(feature.cross_state[1] == 1)
	{
		feature.road_type[0] = 5;
		motor.speed_set = speed.cross;
	}
	else if(feature.straight_state == 1)
	{
		feature.road_type[0] = 1;
		motor.speed_set = speed.long_straight;
	}
	else if(feature.straight_state == 2)
	{
		feature.road_type[0] = 2;
		motor.speed_set = speed.straight;
	}
	else
	{
		feature.road_type[0] = 3;
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
	*	@brief	�Ƿ���
	*	@param	��
	*	@note	��
***************************************************************/
uint8 is_Lose_All(void)
{
	uint8 i = 0, j = 0;
	int16 black_count = 0;
	for(i = 98; i > 95; i--)
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
	*	@brief	��������
	*	@param	��
	*	@note	��
***************************************************************/
void Parameter_Setting(void)
{
	int8 key_state;
	DisableInterrupts;
	Parameter_Setting_Init();
	Setting_Paint();
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
				//����ҳ
			case 1:
				setting.page_num--;
				if(setting.page_num < 0)
				{
					setting.page_num = 3;
				}
				Setting_Paint();
				break;
				//����ҳ
			case 6:
				setting.page_num++;
				if(setting.page_num > 3)
				{
					setting.page_num = 0;
				}
				Setting_Paint();
				break;
				//��һ��
			case 2:
				setting.course--;
				if(setting.course < 1)
				{
					setting.course = 7;
				}
				Setting_Paint();
				break;
				//��һ��
			case 5:
				setting.course++;
				if(setting.course > 7)
				{
					setting.course = 1;
				}
				Setting_Paint();
				break;
				//data����
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
				//data��С
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
		Key_Delay();
		Key_Delay();
	}
	EnableInterrupts;
}

/***************************************************************
	*	@brief	�������ó�ʼ��
	*	@param	page��ҳ��
	*	@note	��
***************************************************************/
void Parameter_Setting_Init(void)
{
	setting.page_num = 0;
	setting.course = 1;
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
	sprintf(setting.string[1][4], "dyna_zone");
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
	sprintf(setting.string[2][5], "dif_c_l");
	setting.data[2][5] = (float32)servo.dif_const_left;
	sprintf(setting.string[2][6], "dif_c_r");
	setting.data[2][6] = (float32)servo.dif_const_right;
        
	sprintf(setting.string[3][0], "MOTOR");
	sprintf(setting.string[3][1], "kp");
	setting.data[3][1] = (float32)motor.kp;
	sprintf(setting.string[3][2], "ki");
	setting.data[3][2] = (float32)motor.ki;
	sprintf(setting.string[3][3], "kd");
	setting.data[3][3] = (float32)motor.kd;
}

/***************************************************************
	*	@brief	����ҳ��
	*	@param	��
	*	@note	��
***************************************************************/
void Setting_Paint(void)
{
	uint8 i = 0;
	OLED_Fill(0x00);
	switch(setting.page_num)
	{
	case 0:
		OLED_Put6x8Str(8, 0, setting.string[0][0]);
		OLED_Put6x8Str(115, 0, "P1");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[0][i]);
			OLED_PrintFloatValue(70, i, setting.data[0][i]);
		}
		break;
	case 1:
		OLED_Put6x8Str(8, 0, setting.string[1][0]);
		OLED_Put6x8Str(115, 0, "P2");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[1][i]);
			OLED_PrintFloatValue(70, i, setting.data[1][i]);
		}
		break;
	case 2:
		OLED_Put6x8Str(8, 0, setting.string[2][0]);
		OLED_Put6x8Str(115, 0, "P3");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[2][i]);
			OLED_PrintFloatValue(70, i, setting.data[2][i]);
		}
		break;
	case 3:
		OLED_Put6x8Str(8, 0, setting.string[3][0]);
		OLED_Put6x8Str(115, 0, "P4");
		for(i = 1; i < 8; i++)
		{
			OLED_Put6x8Str(8, i, setting.string[3][i]);
			OLED_PrintFloatValue(70, i, setting.data[3][i]);
		}
		break;
	}
	OLED_Put6x8Char(0, setting.course, '*');
}

/***************************************************************
	*	@brief	�������ð������
	*	@param	��
	*	@note	��
***************************************************************/
int8 Setting_Key_Scan(void)
{
	if(SWITCH1 == 1 || SWITCH2 == 1 || SWITCH3 == 1)
	{
		Key_Delay();
		if(SWITCH1 == 1 || SWITCH2 == 1 || SWITCH3 == 1)
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
	BUZZER_OFF;
	return 0;
}

/***************************************************************
	*	@brief	��������
	*	@param	��
	*	@note	��
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
	servo.dif_const_left = setting.data[2][5];
	servo.dif_const_right = setting.data[2][6];
	motor.kp = setting.data[3][1];
	motor.ki = setting.data[3][2];
	motor.kd = setting.data[3][3];
}