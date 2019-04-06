#include "header.h"

/***************************************************************
	*	@brief	拨码开关始化
	*	@param	无
	*	@note	开关拨上去为0
***************************************************************/
uint8 Switch_Gpio_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PTx = SWITCH1_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH1_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = SWITCH2_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH2_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = SWITCH3_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH3_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	GPIO_InitStructure.GPIO_PTx = SWITCH4_PTx;
	GPIO_InitStructure.GPIO_Pins = SWITCH4_Pinx;
	GPIO_InitStructure.GPIO_Dir = DIR_INPUT;
	GPIO_InitStructure.GPIO_PinControl = INPUT_PULL_UP;
	if(!LPLD_GPIO_Init(GPIO_InitStructure))
		return STATUS_FAILED;
	
	return STATUS_OK;
}
void Flag_Clean(void)
{
    feature.straight_state = 0;
    feature.pre_turn_state = 0;
    feature.turn_state = 0;
    feature.cross_state[0] = 0;
    feature.cross_state[1] = 0;
    feature.roundabouts_state = 0;
    feature.breakage_state = 0;
    feature.ramp_state = 0;
    feature.roadblock_state = 0;
    servo.enable = 1;
    servo.which = 0;    
}

void Key_Scan(void)
{
    //参数设置		
    if(SWITCH1 == 0 && SWITCH2 == 0 && SWITCH3 == 0)
    {
        Parameter_Setting();
        Flag_Clean();
    }
    //向上位机发送数据
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
    if(SWITCH1 == 1 && SWITCH2 == 0 && SWITCH3 == 0)
    {
        if(KEY1 == 0)
        {
            Key_Delay();
            if(KEY1 == 0)
            {
                BUZZER_ON;
                LPLD_LPTMR_DelayMs(200);
                BUZZER_OFF;
                servo.which = 1;
                if(motor.start == 0)
                {
                    motor.start = 50;
                    motor.distance_all = 0;
                    motor.error_integral_left = 0;
                    motor.error_integral_right = 0;
                }
            }
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
        OLED_PrintIntValue(60, 5, (int32)(laser.distance));
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
    if(SWITCH1 == 0 && SWITCH2 == 1 && SWITCH3 == 1)
    {
        //Get_Attitude();
        OLED_Put6x8Str(10, 1, "GYRO");
        OLED_PrintFloatValue(10, 2, (float)gyro.Turn_Speed);
        OLED_PrintFloatValue(10, 3, (float)gyro.ACC_Angle);
        OLED_PrintFloatValue(10, 4, (float)gyro.Gravity_Angle);
        OLED_PrintFloatValue(10, 5, (float)gyro.Angle_Speed);
        OLED_PrintFloatValue(10, 6, (float)gyro.TurnAngle_Integral);
        OLED_PrintFloatValue(10, 7, (float)gyro.CarY_Angle);
        OLED_PrintFloatValue(10, 8, (float)gyro.Car_Angle);
    }
    //按键停车
    if(KEY2 == 0)
    {
        Key_Delay();
        if(KEY2 == 0)
        {
            Flag_Clean();
            motor.stop = 1;
            BUZZER_OFF;
        }
    }
    if(KEY3 == 0)
    {
        Key_Delay();
        if(KEY3 == 0)
        {
            servo_up5();
        }
    }
    if(KEY4 == 0)
    {
        Key_Delay();
        if(KEY4 == 0)
        {
            servo_down5();
        }
    }
}