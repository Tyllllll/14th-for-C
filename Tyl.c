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
    if(!Init_All())
	{
		while(1);
	}
    while(1)
    {
//		VL53L0X_Get_Distance();
		//��������		
        if(SWITCH1 == 0 && SWITCH2 == 0 && SWITCH3 == 0)
		{
			Parameter_Setting();
			flag_clean();
		}
        //����λ����������
		if(SWITCH4 == 1)
		{
			Send_Data_to_FreeCars();
		}
		//��������oled
		if(KEY5 == 0)
		{
			Key_Delay();
			if(KEY5 == 0)
			{
				Oled_Init();
			}
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
					motor.distance_all = 0;
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
				flag_clean();
                motor.stop = 1;
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
			Adc_Magnetic_Get_Result();
            Data_analyse();
			Adc_Measure_Distance();
			Judge_Feature();
			if(servo.which == 0)
			{
				//����ͷ
//				Check_Half_Width();
				All_Fill();
			}
            Adc_Magnetic_Solution();
			if(servo.enable == 1)
			{
				Servo_Control();
			}
			Speed_Set();
			if(motor.start != 0)
			{
				if(feature.roadblock_state <= 0 && servo.which == 0)
				{
                    if(Adc_Magnetic_Lose_Line() == 1)
                    {
                        motor.stop = 1;
                    }
				}
				if(motor.distance_all > 100 * (motor.distance_set) && motor.distance_set != 0)
				{
					motor.stop = 1;
				}
			}
//			if(motor.start == 0 && motor.distance_set == 0)
//			{
//				if(Adc_Magnetic_Lose_Line() == 0)
//				{
//					motor.start = 1;
//					if(motor.distance_set != 0)
//					{
//						motor.distance_all = 0;
//					}
//					motor.error_integral_left = 0;
//					motor.error_integral_right = 0;
//				}
//			}
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
				OLED_PrintIntValue(60, 5, (int32)(100.0f*magnetic.error[0]));
                OLED_PrintUintValue(60, 6, Position);
//				Get_Angular_Velocity();
//				Get_Acceleration();
//				OLED_PrintIntValue(10, 1, (int32)gyro.accx);
//				OLED_PrintIntValue(70, 1, (int32)gyro.accy);
//				OLED_PrintIntValue(10, 2, (int32)gyro.accz);
//				OLED_PrintIntValue(10, 3, (int32)gyro.angx);
//				OLED_PrintIntValue(70, 3, (int32)gyro.angy);
//				OLED_PrintIntValue(10, 4, (int32)gyro.angz);
			}
		}
   }
}
