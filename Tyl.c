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
        Magnetic_OnceUnion();
		//��������
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
            servo.which = 0;
            servo.enable = 1;
		}
        //��������ֵ ��ǰ�������
        if(SWITCH1 == 0 && SWITCH2 == 0 && SWITCH3 == 1)
        {
            Magnetic_findMax();
            servo.enable = 0;
            OLED_Put6x8Str(20, 2, "find max mag!  ");
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
				Bee();
				if(motor.start == 0)
				{
					motor.start = 50;
					motor.error_integral = 0;
					motor.error_integral_left = 0;
					motor.error_integral_right = 0;
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
				feature.straight_state = 0;
				feature.pre_turn_state = 0;
				feature.turn_state = 0;
				feature.cross_state[0] = 0;
				feature.cross_state[1] = 0;
				feature.roundabouts_state = 0;
                servo.which = 1;
                motor.alldist = 0;
                feature.block_state = 0;
                Bee();
			}
		}
		if(camera.ready_read == 1)
	//	if(0)
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
			Magnetic_Solution();
//			Magnetic_Error_Mapping();
			Speed_Set();
			if(motor.start != 0)
			{
				if(Magnetic_Lose_Line()==1)
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
				OLED_ShowImage();
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
//				//servo_up5();
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
