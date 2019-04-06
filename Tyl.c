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
		Key_Scan();
        Get_Attitude();
        Adc_Magnetic_Get_Result();
        Mag_data_analyse();
        Adc_Measure_Distance();
		if(camera.ready_read == 1)
		{
			if(servo.counter != 0)  //��������10������
			{
				servo.counter--;
			}
			else
			{
				BUZZER_OFF;
			}
			Img_Extract();          //��ѹͼƬ
			Find_Line();            //Ѱ�ұ��ߺ�����

            Judge_Feature();
            if(servo.which == 0)
            {
                //����ͷ
                All_Fill();
            }
		}    
        if(servo.enable == 1 || feature.roadblock_state != 0)
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
        //������ֱ����
        if(motor.start == 0 && motor.distance_set == 0)
        {
            if(Adc_Magnetic_Lose_Line() == 0)
            {
                motor.start = 1;
                if(motor.distance_set != 0)
                {
                    motor.distance_all = 0;
                }
                motor.error_integral_left = 0;
                motor.error_integral_right = 0;
            }
        }
   }
}
