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

//void main(void)
//{
//	Uart_Init();
//	IIC_Init();
//	MPU6050_Init();
//	while(1)
//	{
//		Get_Angular_Velocity();
//		Get_Acceleration();
//		push(0, (uint16)gyro.angx);
//		push(1, (uint16)gyro.angy);
//		push(2, (uint16)gyro.angz);
//		push(3, (uint16)gyro.accx);
//		push(4, (uint16)gyro.accy);
//		push(5, (uint16)gyro.accz);
//		Send_Data_To_Scope();
//	}
////	uint8 i = 0;
////	uint8 count = 0;
////	for(i = 0; i < 255; i++)
////	{
////		if(IIC_Read_Byte(i, 0) != 255)
////		{
////			OLED_PrintIntValue(20, count, i);
////			count++;
////		}
////		if(count == 7)
////		{
////			break;
////		}
////	}
////	if(i == 255)
////	{
////		OLED_Put6x8Str(100, 7, "fuck");
////	}
//}

void main(void)
{
    Init_All();
    while(1)
    {
		magnetic.hongwaiceju = Magnetic_GetAdc(MAGNETIC_MID_ADCx, HONGWAICEJU_CHx);
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
			feature.ramp_state = 0;
			servo.enable = 1;
			servo.which = 0;
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
				motor.stop = 1;
				feature.straight_state = 0;
				feature.pre_turn_state = 0;
				feature.turn_state = 0;
				feature.cross_state[0] = 0;
				feature.cross_state[1] = 0;
				feature.roundabouts_state = 0;
				feature.breakage_state = 0;
				feature.ramp_state = 0;
				servo.enable = 1;
				servo.which = 0;
				BUZZER_OFF;
			}
		}
		if(camera.ready_read == 1)
		{
			if(servo.counter != 0)
			{
				servo.counter--;
			}
			else
			{
				BUZZER_OFF;
			}
			Img_Extract();
			Find_Line();
			Judge_Feature();
			if(servo.which == 0)
			{
				//摄像头
//				Check_Half_Width();
				All_Fill();
			}
			else if(servo.which == 1)
			{
				//电磁
				Magnetic_Solution();
			}
			if(servo.enable == 1)
			{
				Servo_Control();
			}
			Speed_Set();
			if(motor.start != 0)
			{
//				if(Magnetic_Lose_Line() == 1)
				if(is_Lose_All(105))
				{
					motor.stop = 1;
				}
			}
			if(motor.start == 0)
			{
				if(Magnetic_Lose_Line() == 0)
				{
					motor.start = 20;
					motor.error_integral = 0;
					motor.error_integral_left = 0;
					motor.error_integral_right = 0;
				}
			}
			if(SWITCH1 == 1 && SWITCH2 == 1 && SWITCH3 == 0)
			{
				OLED_ShowImage();
			}
			if(SWITCH1 == 1 && SWITCH2 == 1 && SWITCH3 == 1)
			{
				Magnetic_Solution();
				OLED_PrintIntValue(10, 1, (int32)magnetic.value[MID_L]);
				OLED_PrintIntValue(70, 1, (int32)magnetic.value[MID_R]);
				OLED_PrintIntValue(10, 2, (int32)magnetic.value[HORIZONTAL_L]);
				OLED_PrintIntValue(70, 2, (int32)magnetic.value[HORIZONTAL_R]);
				OLED_PrintIntValue(10, 3, (int32)magnetic.value[VERTICAL_L]);
				OLED_PrintIntValue(70, 3, (int32)magnetic.value[VERTICAL_R]);
				OLED_PrintIntValue(10, 4, (int32)magnetic.value[EQUIVALENT_L]);
				OLED_PrintIntValue(70, 4, (int32)magnetic.value[EQUIVALENT_R]);
				OLED_PrintFloatValue(60, 5, (int32)magnetic.angle * 57.3);
			}
		}
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
