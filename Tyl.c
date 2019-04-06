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
			if(servo.counter != 0)  //蜂鸣器响10个周期
			{
				servo.counter--;
			}
			else
			{
				BUZZER_OFF;
			}
			Img_Extract();          //解压图片
			Find_Line();            //寻找边线和中线

            Judge_Feature();
            if(servo.which == 0)
            {
                //摄像头
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
        //开驱动直接跑
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
