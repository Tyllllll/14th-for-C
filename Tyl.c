/**
 * --------------����"��������K60�ײ��V3"�Ĺ��̣�Tyl��-----------------
 * @file Tyl.c
 * @version 0.0
 * @date 2013-9-29
 * @brief ���ڸù��̵�����
 *
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * http://www.lpld.cn
 * mail:support@lpld.cn
 * Ӳ��ƽ̨:  LPLD K60 Card / LPLD K60 Nano
 *
 * �����̻���"��������K60�ײ��V3"������
 * ���п�Դ�������"lib"�ļ����£��û����ظ��ĸ�Ŀ¼�´��룬
 * �����û������豣����"project"�ļ����£��Թ����������ļ�������
 * �ײ��ʹ�÷���������ĵ��� 
 *
 */
#include "header.h"

void main(void)
{
    Init_All();
    while(1)
    {
		//��������
		if(SWITCH1 == 0 && SWITCH2 == 0 && SWITCH3 == 0)
		{
			Parameter_Setting();
			feature.straight_state = 0;
			feature.turn_state = 0;
			feature.cross_state[0] = 0;
			feature.cross_state[1] = 0;
			feature.roundabouts_state = 0;
			servo.enable = 1;
		}
		if(SWITCH4 == 1)
		{
			Send_Data_to_FreeCars();
		}
		//��������
		if(KEY1 == 0 && SWITCH1 == 1 && SWITCH2 == 0 && SWITCH3 == 0)
		{
			Key_Delay();
			if(KEY1 == 0)
			{
				BUZZER_ON;
                                LPLD_LPTMR_DelayMs(200);
				BUZZER_OFF;
				if(motor.start == 0)
				{
					motor.start = 50;
					motor.error_integral = 0;
					motor.error_integral_L = 0;
					motor.error_integral_R = 0;
				}
			}
		}
		//����ͣ��
		if(KEY2 == 0)
		{
			Key_Delay();
			if(KEY2 == 0)
			{
				motor.stop = 5;
			}
		}
		if(camera.ready_read == 1)
		{
			Img_Extract();
			Find_Line();
			Judge_Feature();
			All_Fill();
			if(servo.enable == 1)
			{
				Servo_Control();
			}
			if(motor.start != 0)
			{
				if(is_Lose_All() == 1 && feature.breakage_state[1] == 0)
				{
					servo.stop++;
					if(servo.stop == 5)
					{
						motor.stop = 5;
						servo.stop = 0;
					}
				}
				else
				{
					servo.stop = 0;
				}
			}
			if(SWITCH1 == 1 && SWITCH2 == 1 && SWITCH3 == 0)
			{
				OLED_ShowImage();
			}
		}
		Magnetic_GetAdc();
//		OLED_PrintFloatValue(40, 3, magnetic.left_mag);
//		OLED_PrintFloatValue(40, 6, magnetic.right_mag);
//		if(KEY1 == 0)
//		{
//			Key_Delay();
//			if(KEY1 == 0)
//			{
//				servo_up1();
//			}
//		}
//		if(KEY2 == 0)
//		{
//			Key_Delay();
//			if(KEY2 == 0)
//			{
//				servo_up5();
//			}
//		}
//		if(KEY3 == 0)
//		{
//			Key_Delay();
//			if(KEY3 == 0)
//			{
//				servo_up10();
//			}
//		}
//		if(KEY4 == 0)
//		{
//			Key_Delay();
//			if(KEY4 == 0)
//			{
//				servo_down10();
//			}
//		}
//		if(KEY5 == 0)
//		{
//			Key_Delay();
//			if(KEY5 == 0)
//			{
//				servo_down5();
//			}
//		}
//		if(KEY6 == 0)
//		{
//			Key_Delay();
//			if(KEY6 == 0)
//			{
//				servo_down1();
//			}
//		}
    }
}
