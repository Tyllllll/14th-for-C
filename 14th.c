#include "head_file.h"

int time_flag=0;
int t=0 ;
//int time;
//int half[120]={0};


void main ()
{  
  DisableInterrupts;
    NVIC_Init();
    pit0_init();
    led_init();  
    UART_Init();//���ڳ�ʼ��
    gpio_init();//����ͷGPIO����
    dma_init();//dma�жϳ�ʼ��
    Servo_gpio_init();//  �����ʼ��
    Motor_pwm_init();//�����ʼ��
    Encoder_QD_init();//��ʼ�������� 	
    key_init_gpio();//������ʼ��
    BatteryVoltage_Init();
    boma_init_gpio();//���س�ʼ��
    Oled_GPIO_Init();
    Welcome_page();
     Buzzer_gpio_init();

    
    
    if(ov7725_init()==1)
    {
        LED_Fill(0x00);
        LED_P6x8Str(19,1, " Congulation");
        LED_P6x8Str(19,3, " Init success");
        LPLD_LPTMR_DelayMs(100);
        camera_delay();
    }
    else
    {
        LED_P6x8Str(19,1, " go die ");
        LED_P6x8Str(19,3, " go die ");
        LED_P6x8Str(19,5, " go die ");
        LED_P6x8Str(19,7, " go die ");
    }
    
    BUZZER_OFF;
    EnableInterrupts;
    LED_Fill(0x00);
    
    
    while(1)
    {
        
        if(SWITCH1==0&&SWITCH2==0&&SWITCH3==0&&SWITCH4==0)
        {//���Բ���
            key_process();
            if(times>0)
            {
                time_flag=50*times;
            }
            if(times<=0)
            {
                time_flag=50;
            }
        }
        
        if(key1==0&&SWITCH1==1&&SWITCH2==0&&SWITCH3==0&&SWITCH4==0)
        {//��������
            key_delay();    
            if(key1==0)
            {
                BUZZER_ON;
                LedOff(1);
                LedOff(3);
                LedOff(2);
                LPLD_LPTMR_DelayMs(200);
                BUZZER_OFF;
                               
                if(motor_flag==0)
                {
                    motor_flag=50;//���ת����־
                    MotorPID.IntSum_all=0;//�������I������
                    if(times>0)
                    {
                        time_flag=50*times;
                    }
                }
                
            }
            
        }
        
        if(key2 == 0)
        {
          key_delay(); 
          if(key2 == 0)
            stop_flag=100;
        }
        if(sampleover ==1)  //��ɲ�ͼ
        {
            //      LPLD_PIT_Time_Start(PIT2); // ��ʱ��ʼ
            
            LedOff(1);
            LedOff(3);
            LedOff(2);
            sampleover = 0;
            img_extract();//��ѹͼƬ
            
            //      time = LPLD_PIT_Time_Get(PIT2);
            //      LPLD_PIT_Close(PIT2);
            //      time *= (1000000.0 / g_bus_clock); // ��ʱ����
            
            findline1();//����Ѱ��
            findline2();
            
            //      int i;
            //      for(i=119;i>=0;i--)
            //      {//����������
            //        half[i]=(f1.rightline[i]-f1.leftline[i]);//2;
            //      }
            
            Sevor_control();//���������
            if( SWITCH1==1&&SWITCH3==0)
            {//��ʱͣ��
                if(time_flag>0&&times>0)
                {
                    if(time_flag!=0)
                    {
                        time_flag=time_flag-1;
                    }
                    if(time_flag==0)
                    {
                        stop_flag=100;        	   
                    }
                }
            }
            if(motor_flag!=0)
            {//����ͣ��
                
                if(set_dist>=1)
                {
                    if(All_distance>=set_dist)
                    {
                        stop_flag=100;
                    } 
                }
                
                Emergency_judge();
                if( Emergencystop_flag==1)
                {
                    stop_flag=100;
                }
            }
            
            
                  sendData_to_PC();
                  sendDataToScope();
            //blueteeth low
            
            if(FM_mode==1)
            {
                FM_Start_di();
            }
            if(FM_mode==2)
            {
                FM_Start_didi();
            }
            if(FM_mode==3)
            {
                FM_Start_dididi();
            }
            
            if(SWITCH1==1&&SWITCH2==1&&SWITCH3==0&&SWITCH4==0)
            {     
                showimage();  
            }  
            
        }
        
        //    sendData_to_PC();
        //    sendDataToScope();
        //blueteeth high
        
        
        if(SWITCH1==1&&SWITCH2==1&&SWITCH3==1&&SWITCH4==1)
        {//��ͼ�������
            static int8 Image_Show_sele = 4;    
            PutcharCamera(Image_Show_sele);
        }
    }
    
}
