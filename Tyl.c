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
            feature.ramp_state[1] = 0;
            servo.which = 0;
		}
        //电感找最大值 跑前必须测试
        if(SWITCH1 == 0 && SWITCH2 == 0 && SWITCH3 == 1)
        {
            Magnetic_findMax();
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
                feature.breramp = 0;
                servo.which = 0;
				BUZZER_OFF;
			}
		}
		if(camera.ready_read == 1)
//		if(0)
		{
			Img_Extract();
			Find_Line();
			Check_Half_Width();
			Judge_Feature();
			All_Fill();
			if(servo.enable == 1)
			{
				Servo_Control();
			}
			Magnetic_Solution();
			Magnetic_Error_Mapping();
			Speed_Set();
			if(motor.start != 0)
			{
				if(is_Lose_All(105) == 1)
				{
					servo.counter++;
					if(servo.counter == 5)
					{
						motor.stop = 5;
						servo.counter = 0;
					}
				}
				else
				{
					servo.counter = 0;
				}
			}
			if(SWITCH1 == 1 && SWITCH2 == 1 && SWITCH3 == 0)
			{
//				Magnetic_Solution();
//				OLED_PrintIntValue(40, 3, servo.error[0]);
//				OLED_PrintIntValue(10, 4, magnetic.left_equivalent);
//				OLED_PrintIntValue(70, 4, magnetic.right_equivalent);
//				OLED_PrintFloatValue(60, 5, magnetic.angle * 57.3);
				OLED_ShowImage();
			}
		}
//		{
//			Magnetic_Solution();
//			servo.error[0] = (int16)(0.689 * (magnetic.right_outside_mag - magnetic.left_outside_mag) - 1.269);
//			Servo_PID();
//			Speed_Set();
//			if(motor.start != 0)
//			{
//				if(is_Lose_All() == 1)
//				{
//					servo.counter++;
//					if(servo.counter == 5)
//					{
//						motor.stop = 5;
//						servo.counter = 0;
//					}
//				}
//				else
//				{
//					servo.counter = 0;
//				}
//			}
//			if(SWITCH1 == 1 && SWITCH2 == 1 && SWITCH3 == 0)
//			{
//				OLED_ShowImage();
//			}
//		}
//		Magnetic_Solution();
//		OLED_PrintIntValue(10, 1, magnetic.middle_left_mag);
//		OLED_PrintIntValue(70, 1, magnetic.middle_right_mag);
//		OLED_PrintIntValue(10, 2, magnetic.left_horizontal_mag);
//		OLED_PrintIntValue(70, 2, magnetic.right_horizontal_mag);
//		OLED_PrintIntValue(10, 3, magnetic.left_vertical_mag);
//		OLED_PrintIntValue(70, 3, magnetic.right_vertical_mag);
//		OLED_PrintIntValue(10, 4, magnetic.left_equivalent);
//		OLED_PrintIntValue(70, 4, magnetic.right_equivalent);
//		OLED_PrintFloatValue(60, 5, magnetic.angle * 57.3);
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
