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
