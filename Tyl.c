/**
 * --------------基于"拉普兰德K60底层库V3"的工程（Tyl）-----------------
 * @file Tyl.c
 * @version 0.0
 * @date 2013-9-29
 * @brief 关于该工程的描述
 *
 * 版权所有:北京拉普兰德电子技术有限公司
 * http://www.lpld.cn
 * mail:support@lpld.cn
 * 硬件平台:  LPLD K60 Card / LPLD K60 Nano
 *
 * 本工程基于"拉普兰德K60底层库V3"开发，
 * 所有开源代码均在"lib"文件夹下，用户不必更改该目录下代码，
 * 所有用户工程需保存在"project"文件夹下，以工程名定义文件夹名，
 * 底层库使用方法见相关文档。 
 *
 */
#include "header.h"

void main(void)
{
    Init_All();
    while(1)
    {
		//参数设置
		if(SWITCH1 == 0 && SWITCH2 == 0 && SWITCH3 == 0)
		{
			Parameter_Setting();
			feature.straight_state = 0;
			feature.pre_turn_state = 0;
			feature.turn_state = 0;
			feature.cross_state[0] = 0;
			feature.cross_state[1] = 0;
			feature.roundabouts_state = 0;
	 		feature.breakage_state = 0;
		}
		if(SWITCH4 == 1)
		{
			Send_Data_to_FreeCars();
		}
		//按键发车
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
					motor.error_integral_left = 0;
					motor.error_integral_right = 0;
				}
			}
		}
		//按键停车
		if(KEY2 == 0)
		{
			Key_Delay();
			if(KEY2 == 0)
			{
				motor.stop = 5;
				feature.straight_state = 0;
				feature.pre_turn_state = 0;
				feature.turn_state = 0;
				feature.cross_state[0] = 0;
				feature.cross_state[1] = 0;
				feature.roundabouts_state = 0;
				feature.breakage_state = 0;
				servo.enable = 1;
			}
		}
		if(camera.ready_read == 1)
//		if(0)
		{
			Img_Extract();
			Find_Line();
//			Check_Half_Width();
			Judge_Feature();
			All_Fill();
			if(servo.enable == 1)
			{
				Servo_Control();
			}
			Speed_Set();
			if(motor.start != 0)
			{
				if(is_Lose_All(105) == 1)
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
//		{
//			Magnetic_Get_Result();
//			servo.error[0] = (int16)(0.689 * (magnetic.right_outside_mag - magnetic.left_outside_mag) - 1.269);
//			Servo_PID();
//			Speed_Set();
//			if(motor.start != 0)
//			{
//				if(is_Lose_All() == 1)
//				{
//					servo.stop++;
//					if(servo.stop == 5)
//					{
//						motor.stop = 5;
//						servo.stop = 0;
//					}
//				}
//				else
//				{
//					servo.stop = 0;
//				}
//			}
//			if(SWITCH1 == 1 && SWITCH2 == 1 && SWITCH3 == 0)
//			{
//				OLED_ShowImage();
//			}
//		}
//		Magnetic_Get_Result();
//		OLED_PrintIntValue(40, 2, servo.error[0]);
//		OLED_PrintIntValue(40, 5, magnetic.left_outside_mag);
//		OLED_PrintIntValue(40, 7, magnetic.right_outside_mag);
//		LPLD_LPTMR_DelayMs(100);
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
