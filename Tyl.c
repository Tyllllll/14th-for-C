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

void main (void)
{
	DisableInterrupts;
    Init_All();
	EnableInterrupts;
	OLED_Fill(0x00);
    while(1)
    {
//		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 3500);
//		LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 3500);
		if(camera.ready_read)
		{
			Img_Extract();
			OLED_ShowImage();
		}
    }
}
