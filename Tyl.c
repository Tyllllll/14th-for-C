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

/**************************
换车需修改 
1. servo.h 舵机中值
2. encoder.c Encoder_FTM_Clear（）
3. adc.h 6个电感的宏定义序号
*************************/
#include "header.h"
void main(void)
{
    if(!Init_All())
	{
		while(1);
	}
    while(1)
    {
		VL53L0X_Get_Distance();
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
			feature.block_state = 0;
			servo.enable = 1;
			servo.which = 0;
		}
		if(SWITCH4 == 1)
		{
			Send_Data_to_FreeCars();
		}
		//按键重置oled
		if(KEY5 == 0)
		{
			Key_Delay();
			if(KEY5 == 0)
			{
				Oled_Init();
			}
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
					motor.alldist = 0;
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
				feature.block_state = 0;
				servo.enable = 1;
				servo.which = 0;
				BUZZER_OFF;
				encoder.left_num_sum = 0;
				encoder.right_num_sum = 0;
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
			Adc_Magnetic_Get_Result();
			Adc_Measure_Distance();
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
				Adc_Magnetic_Solution();
			}
			if(servo.enable == 1)
			{
				Servo_Control();
			}
			Speed_Set();
			if(motor.start != 0)
			{
				if(Adc_Magnetic_Lose_Line() == 1 && feature.block_state == 0)
				{
					motor.stop = 1;
				}
				if(motor.alldist > motor.distance_set && motor.distance_set != 0)
				{
					motor.stop = 1;
				}
			}
			if(motor.start == 0)
			{
				if(Adc_Magnetic_Lose_Line() == 0)
				{
					motor.start = 1;
					//motor.alldist = 0;
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
				Adc_Magnetic_Solution();
				OLED_PrintIntValue(10, 1, (int32)magnetic.value[MID_L]);
				OLED_PrintIntValue(70, 1, (int32)magnetic.value[MID_R]);
				OLED_PrintIntValue(10, 2, (int32)magnetic.value[HORIZONTAL_L]);
				OLED_PrintIntValue(70, 2, (int32)magnetic.value[HORIZONTAL_R]);
				OLED_PrintIntValue(10, 3, (int32)magnetic.value[VERTICAL_L]);
				OLED_PrintIntValue(70, 3, (int32)magnetic.value[VERTICAL_R]);
				OLED_PrintIntValue(10, 4, (int32)magnetic.value[EQUIVALENT_L]);
				OLED_PrintIntValue(70, 4, (int32)magnetic.value[EQUIVALENT_R]);
				OLED_PrintIntValue(60, 5, (int32)infrared.distance);
			}
		}
    }
}
