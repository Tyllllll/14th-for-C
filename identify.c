#include "header.h"

Line_Class line;
Feature_Class feature;
/***************************************************************
	*	@brief	寻线
	*	@param	无
	*	@note	寻找左右边界及计算中线
***************************************************************/
void Find_Line(void)
{
	uint8 i = 0, j = 0;
	uint8 column_start = 80;//基础寻线每行起始搜索列
	for(i = 118; i >5; i--)
	{
		line.left_line[i] = 0;
		line.left_line_flag[i] = 0;
		line.right_line[i] = 0;
		line.right_line_flag[i] = 0;
		line.midline[i] = 0;
		//从中线往左扫
		for(j = column_start; j > 2; j--)
		{
			//寻找左跳变点
			if(camera.image[i][j] != 0 && camera.image[i][j - 2] == 0)
			{
				line.left_line[i] = j - 1;
				line.left_line_flag[i] = 1;
				break;
			}
		}
		//从中线往右扫
		for(j = column_start; j < 157; j++)
		{
			//寻找右跳变点
			if(camera.image[i][j] != 0 && camera.image[i][j + 2] == 0)
			{
				line.right_line[i] = j + 1;
				line.right_line_flag[i] = 1;
				break;
			}
		}
		//计算中线
		if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 1)
		{
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
		else if(line.left_line_flag[i] == 1 && line.right_line_flag[i] == 0)
		{
			line.right_line[i] = 159;
			line.midline[i] = (line.left_line[i] + line.right_line[i]) / 2;
		}
		else if(line.left_line_flag[i] == 0 && line.right_line_flag[i] == 1)
		{
			line.left_line[i] = 0;
			line.midline[i] = line.right_line[i] / 2;
		}
		else if(line.left_line_flag[i] == 0 && line.right_line_flag[i] == 0)
		{
			line.left_line[i] = 0;
			line.right_line[i] = 159;
			if(118 == i)
			{
				line.midline[i] = 80;
			}
			else
			{
				line.midline[i] = line.midline[i + 1];
				if(i > 40 && i < 120)
				{
					if(camera.image[i][line.midline[i]] != 0 && (camera.image[i - 1][line.midline[i]] == 0 || camera.image[i - 3][line.midline[i]] == 0 || camera.image[i - 5][line.midline[i]] == 0))
					{
						if(camera.image[i + 30][line.midline[i]] != 0)
						{
							line.midline[i] += 30;
						}
						else if(camera.image[i - 30][line.midline[i]] != 0)
						{
							line.midline[i] -= 30;
						}
					}
				}
			}
		}
		column_start = line.midline[i];
	}
}

/***************************************************************
	*	@brief	判断特征
	*	@param	无
	*	@note	无
***************************************************************/
void Judge_Feature(void)
{
	//特征清零
	feature.top_point = 0;
	feature.left_flection_flag = 0;
	feature.left_flection_row = 0;
	feature.right_flection_flag = 0;
	feature.right_flection_row = 0;
	feature.left_flection2_flag = 0;
	feature.left_flection2_antiflag = 0;
	feature.left_flection2_row = 0;
	feature.right_flection2_flag = 0;
	feature.right_flection2_antiflag = 0;
	feature.right_flection2_row = 0;
	
	Find_Top_Point();
	Find_Inflection();
	Find_Inflection2();
	
    Judge_Breakage();
  //  Judge_Block();
	Judge_Roundabouts();
	Judge_Straight();
	Judge_Curve();
    Judge_ramp();
//	Judge_Cross();
}

/***************************************************************
	*	@brief	找顶点
	*	@param	无
	*	@note	顶点的定义为某一行的中线上的为白色点，但是该中线列的后面的点为黑点，即赛道边界
***************************************************************/
void Find_Top_Point(void)
{
	uint8 i = 0;
    if(feature.top_point!=0 && feature.top_point!=feature.top_point_last)
    {
        feature.top_point_last = feature.top_point;
    }
    feature.top_point = 0;
	for(i = 100; i > 5; i--)
	{
		if(camera.image[i][line.midline[i]] != 0 && (camera.image[i - 1][line.midline[i]] == 0 && camera.image[i - 3][line.midline[i]] == 0 ))
		{
			feature.top_point = i;
			break;
		}
	}
	if(feature.top_point == 0)
	{
		for(i = 118; i > 2; i--)
		{
			if(line.midline[i] > 150 || line.midline[i] < 10)
			{
                feature.top_point = i;
				break;
			}
		}
	}
}

/***************************************************************
	*	@brief	找拐点
	*	@param	无
	*	@note	十字近点处拐点（透视图中最大最小处）
***************************************************************/
void Find_Inflection(void)
{
	uint8 i = 0;
	if(feature.top_point < 100)
	{
		//找左拐点
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.left_flection_flag == 0)
			{
				//位置条件
				if(i > 28 && line.left_line[i] < 155)
				{
					//大小条件
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] > line.left_line[i - 3] && line.left_line[i] > line.left_line[i + 5] &&
					   line.left_line[i - 3] - line.left_line[i + 3] < 4)
					{
						//存在性条件
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i + 3] == 1 && line.left_line_flag[i + 5] == 1)
						{
							//斜率条件
							if(line.left_line[i] - line.left_line[i - 3] - (line.left_line[i + 3] - line.left_line[i]) > 4)
							{
								feature.left_flection_row = i;
								feature.left_flection_flag = 1;
								break;
							}
						}
					}
				}
			}
		}
		//找右拐点
		for(i = 108; i > feature.top_point + 4; i--)
		{
			if(feature.right_flection_flag == 0)
			{
				//位置条件
				if(i > 28 && line.right_line[i] > 5)
				{
					//大小条件
					if(line.right_line[i] < line.right_line[i + 3] && line.left_line[i] < line.right_line[i - 3] && line.right_line[i] < line.right_line[i + 5] &&
					   line.right_line[i - 3] - line.right_line[i + 3] > -4)
					{
						//存在性条件
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i + 3] == 1 && line.right_line_flag[i + 5] == 1)
						{
							//斜率条件
							if(line.right_line[i + 3] - line.right_line[i] - (line.right_line[i] - line.right_line[i - 3]) > 4)
							{
								feature.right_flection_row = i;
								feature.right_flection_flag = 1;
								break;
							}
						}
					}
				}
			}
		}
	}
}

/***************************************************************
	*	@brief	找拐点2
	*	@param	无
	*	@note	十字远点处拐点（透视图中中间值）有待改进，未测试
***************************************************************/
void Find_Inflection2(void)
{
	uint8 i = 0;
	//找左2拐点
	if(feature.top_point < 100)
	{
		for(i = 100; i > feature.top_point - 2; i--)
		{
			if(feature.left_flection2_flag == 0)
			{
				//位置条件
				if(line.left_line[i] > 15)
				{
					//大小条件
					if(line.left_line[i] > line.left_line[i + 3] && line.left_line[i] < line.left_line[i - 3])
					{
						//存在性条件
						if(line.left_line_flag[i] == 1 && line.left_line_flag[i - 3] == 1 && line.left_line_flag[i - 5] == 1 && line.left_line_flag[i + 5] == 0)
						{
							//斜率条件
							if(line.left_line[i] - line.left_line[i + 2] > 20)
							{
								feature.left_flection2_row = i;
								feature.left_flection2_flag = 1;
								if(line.left_line[i - 4] - line.left_line[i - 1] >= 10 && line.left_line[feature.left_flection2_row] > 10)
								{
									//反拐点标记
									feature.left_flection2_antiflag = 1;
								}
								break;
							}
						}
					}
				}
			}
		}
		//找右2拐点
		for(i = 100; i > feature.top_point - 2; i--)
		{
			if(feature.right_flection2_flag == 0)
			{
				//位置条件
				if(line.right_line[i] < 145)
				{
					//大小条件
					if(line.right_line[i] < line.right_line[i + 3] && line.right_line[i] > line.right_line[i - 3])
					{
						//存在性条件
						if(line.right_line_flag[i] == 1 && line.right_line_flag[i - 3] == 1 && line.right_line_flag[i - 5] == 1 && line.right_line_flag[i + 5] == 0)
						{
							//斜率条件
							if(line.right_line[i] - line.right_line[i + 2] < -20)
							{
								feature.right_flection2_row = i;
								feature.right_flection2_flag = 1;
								if(line.right_line[i - 4] - line.right_line[i - 1] <= -10 && line.right_line[feature.right_flection2_row] < 150)
								{
									//反拐点标记
									feature.right_flection2_antiflag = 1;
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

/***************************************************************
	*	@brief	路障控制
	*	@param	无
	*	@note	无
***************************************************************/
void Judge_Block(void)
{
    int16 servoturn = 130;
    float32 d1 = 20.0, d2 = 20.0, d3 = 20.0; //d1:转弯距离 d2：车身摆直距离 d3：平行于赛道直行距离
    if(feature.block_state[1] == 0)   //检测路障
    {
        //激光测距
        if(motor.alldist > 10.0)
        {
            feature.block_state[1] = 1;
            motor.alldist = 0;
        }
    }
    else if(feature.block_state[1] == 1)        //左转
    {
        servo.enable = 0;
        servo.duty = (int16)(DEG_MID + servoturn);
        if(motor.alldist >= d1)
        {
            feature.block_state[1] = 2;
            motor.alldist = 0;
        }
    }
    else if(feature.block_state[1] == 2)        //摆直
    {
        servo.duty = (int16)(DEG_MID);
        if(motor.alldist >= d2)
        {
            feature.block_state[1] = 3;
            motor.alldist = 0;
        }
    }
    else if(feature.block_state[1] == 3)        //右转
    {
        servo.duty = (int16)(DEG_MID - servoturn);
        if(motor.alldist >= 2*d1)
        {
            feature.block_state[1] = 4;
            motor.alldist = 0;
        }
    }
    else if(feature.block_state[1] == 4)        //摆直
    {
        servo.duty = (int16)(DEG_MID);
        if(motor.alldist >= d2)
        {
            feature.block_state[1] = 5;
            motor.alldist = 0;
        }
    }
    else if(feature.block_state[1] == 5)        //平行于赛道直行
    {
        servo.duty = (int16)(DEG_MID);
        if(motor.alldist >= d3)
        {
            feature.block_state[1] = 6;
            motor.alldist = 0;
        }
    }
    else if(feature.block_state[1] == 6)        //右转 回到赛道
    {
        servo.duty = (int16)(DEG_MID - servoturn);
        if(magnetic.right2_mag >= 10)   
        {
            feature.block_state[1] = 0;
            servo.enable = 1;
        }
    }
    
}
/***************************************************************
	*	@brief	判直道
	*	@param	无
	*	@note	无
***************************************************************/
void Judge_Straight(void)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	//超长直道 1中线不太偏 2也不丢线 3且不为十字和坡道 4顶点满足直道条件
	//中线要求
	for(i = 90; i > 20; i--)
	{
		if(line.midline[i] >= 88 || line.midline[i] <= 72)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 3)
	{
		//丢线要求
		lose_cnt = 0;
		for(i = 50; i > 20; i--)
		{
			if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 0)
			{
				lose_cnt++;
			}
		}
		if(lose_cnt < 3)
		{
			//顶点要求
			if(feature.top_point <= 15)
			{
				feature.straight_state = 1;
			}
			else
			{
				feature.straight_state = 0;
			}
		}
		else
		{
			feature.straight_state = 0;
		}
	}
	else
	{
		feature.straight_state = 0;
	}
	//短直道判断
	if(feature.straight_state == 0)
	{
		if(feature.top_point < 17)
		{
			if(Midline_Std_Deviation(60,30) < 7)
			{
				feature.straight_state = 2;
			}
			else
			{
				feature.straight_state = 0;
			}
		}
	}
}

/***************************************************************
	*	@brief	判弯
	*	@param	无
	*	@note	无（十字flag=1情况未写）
***************************************************************/
void Judge_Curve(void)
{
	uint8 i = 0;
	uint8 is_left_turn = 0;
	uint8 is_right_turn = 0;
	uint8 cnt_left = 0;
	uint8 cnt_right = 0;
	//判远小弯
	if(feature.deep_turn_state == 0)
	{	
		for(i = 100; i > 30; i--)
		{
			if(line.left_line_flag[i] == 0 && line.left_line_flag[i + 1] == 1 && line.left_line_flag[i + 2] == 1 && line.right_line_flag[i] == 1)
			{
				if(line.right_line_flag[i - 4] == 1 && i > 20)
				{
					feature.turn_state = 1;     //左转远弯
					break;
				}
			}
		}
		for(i = 100; i > 30; i--)
		{
			if(line.right_line_flag[i] == 0 && line.right_line_flag[i + 1] == 1 && line.right_line_flag[i + 2] == 1 && line.left_line_flag[i] == 1)
			{
				if(line.left_line_flag[i - 4] == 1 && i > 20)
				{
					feature.turn_state = 2;     //右转远弯
					break;
				}
			}
		}
	}
	if(feature.deep_turn_state != 0)
	{
		feature.turn_state = 0;
	}
	//救火
	if(feature.top_point > 70)
	{
		for(i = 118; i > feature.top_point; i--)
		{
			if(line.left_line_flag[i] == 1)
			{
				cnt_left++;
			}
			if(line.right_line_flag[i] == 1)
			{
				cnt_right++;
			}
		}
		if(cnt_left < 2 && cnt_right > 118 - feature.top_point - 2)
		{
			feature.deep_turn_state = 5;
		}
		else if(cnt_left > 118 - feature.top_point - 2 && cnt_right < 2)
		{
			feature.deep_turn_state = 6;
		}
	}
	else
	{
		feature.deep_turn_state = 0;
	}
	//判深弯
	if(feature.deep_turn_state != 5 && feature.deep_turn_state != 6)
	{
		cnt_left = 0;
		cnt_right = 0;
		if(feature.top_point < 105)
		{
			for(i = 118; i > 112; i--)
			{
				if(line.left_line_flag[i] == 0 && is_left_turn == 0)
				{
					is_left_turn = 1;
				}
				if(line.right_line_flag[i] == 0 && is_right_turn == 0)
				{
					is_right_turn = 1;
				}
			}
			if(is_left_turn == 1)
			{
				for(i = 100; i > feature.top_point + 10; i--)
				{
					if(line.left_line_flag[i] == 1 || line.right_line_flag[i] == 0)
					{
						is_left_turn = 0;
					}
				}
			}
			if(is_right_turn == 1)
			{
				for(i = 100; i > feature.top_point + 10; i--)
				{
					if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 1)
					{
						is_right_turn = 0;
					}
				}
			}
			if(is_left_turn == 1 && is_right_turn == 0)
			{
				feature.deep_turn_state = 3;
			}
			else if(is_left_turn == 0 && is_right_turn == 1)
			{
				feature.deep_turn_state = 4;
			}
			else
			{
				feature.deep_turn_state = 0;
			}
		}
	}
	if(feature.deep_turn_state != 3 && feature.deep_turn_state != 4 && feature.deep_turn_state != 5 && feature.deep_turn_state != 6)
	{
		if(feature.top_point < 70)
		{
			for(i = 50; i > 40; i--)
			{
				//左线50-40行有丢线
				if(line.left_line_flag[i] == 0 && is_left_turn == 0)
				{
					is_left_turn = 1;
				}
				if(line.right_line_flag[i] == 0 && is_right_turn == 0)
				{
					is_right_turn = 1;
				}
			}
			if(is_left_turn == 1)
			{
				for(i = 40; i > feature.top_point + 7; i--)
				{
					if(line.left_line_flag[i] == 1 || line.right_line_flag[i] == 0)
					{
						is_left_turn = 0;
					}
				}
			}
			if(is_right_turn == 1)
			{
				for(i = 40; i > feature.top_point + 7; i--)
				{
					if(line.left_line_flag[i] == 0 || line.right_line_flag[i] == 1)
					{
						is_right_turn = 0;
					}
				}
			}
			if(is_left_turn == 1 && is_right_turn == 0)
			{
				feature.deep_turn_state = 1;
			}
			else if(is_left_turn == 0 && is_right_turn == 1)
			{
				feature.deep_turn_state = 2;
			}
			else
			{
				feature.deep_turn_state = 0;
			}
		}
	}
}

/***************************************************************
	*	@brief	判坡道
	*	@param	无
	*	@note   无
***************************************************************/
void Judge_ramp(void)
{
    uint8 cnt1 = 0, cnt2 =0;
    if(feature.ramp_state[1]==0 && feature.roundabouts_state==0 )        //判断坡道
    { 
        if( motor.add_speed<10 && motor.output_value[0]>=(0.5*motor.output_value[1]+0.2*motor.output_value[2]+0.2*motor.output_value[3]+0.1*motor.output_value[4]))   //加速度小于0 outvalue增加
        { 
            if((line.right_line[30]-line.left_line[30])>120 && line.left_line_flag[30]==1 && line.right_line_flag[30]==1)      //车身处于赛道中间上坡    赛道宽度远大于正常值 
            {
                if(feature.top_point - feature.top_point_last < 0 )
                {
                    feature.ramp_state[0] = 0;
                }
                else if(feature.top_point - feature.top_point_last > 0)     //toppoint连续减小
                {
                    if(Midline_Std_Deviation((uint8)(feature.top_point+35),(uint8)(feature.top_point+5)) < 10)  // 中线较直  
                    {
                        feature.ramp_state[0]++;
                    }
                    else
                    {
                        feature.ramp_state[0] = 0;
                    }                
                }
                if(feature.ramp_state[0] > 2)
                {
                    feature.ramp_state[0] = 0;
                    feature.ramp_state[1] = 1;
                    BUZZER_ON;
                    LPLD_LPTMR_DelayMs(50);
                    BUZZER_OFF;                
                    servo.enable = 0;
                }
            }
            else                             //车身倾斜上赛道  一侧边线完整另一侧完全丢线  边线斜率小         
            {
                for(uint8 i=40;i<100;i++)
                {
                    cnt1 += line.left_line_flag[i];
                    cnt2 += line.right_line_flag[i];
                }
                if(fabs(cnt1-cnt2) >= 55)
                {
                    if(cnt1 > cnt2)
                    {
                        if((line.left_line[80]-line.left_line[90])<10 && (line.left_line[50]-line.left_line[60])<10)
                        {
                            feature.ramp_state[1] = 1;
                            servo.sensor = 2;
                            BUZZER_ON;
                            LPLD_LPTMR_DelayMs(100);
                            BUZZER_OFF;
                        }
                    }
                    else
                    {
                        if((line.right_line[90]-line.right_line[80])<10 && (line.right_line[60]-line.right_line[50])<10)
                        {
                            feature.ramp_state[1] = 1;
                            servo.sensor = 2;
                            BUZZER_ON;
                            LPLD_LPTMR_DelayMs(100);
                            BUZZER_OFF;
                        }
                    }
                }
            }
        }
    }
    else if(feature.ramp_state[1]==1)       //上坡过程切电磁巡线 检测是否到达坡顶  上坡过程中toppoint先减后增 结合toppoint范围 
    {
        if(feature.top_point > 10)
        {
            if((feature.top_point - feature.top_point_last) < 0)
            {
                feature.ramp_state[0]++;
            }
//            else
//            {
//                feature.ramp_state[0] = 0;
//            }

            if(feature.top_point < 40 && feature.ramp_state[0]>2)
            {
                feature.ramp_state[1] = 2;
                feature.ramp_state[0] = 0;
                BUZZER_ON;
                LPLD_LPTMR_DelayMs(200);
                BUZZER_OFF;
            }
        }
    }
    else if(feature.ramp_state[1]==2)       //下坡处理 暂无
    {
        servo.sensor = 1;
        servo.enable = 1;
        feature.ramp_state[1] = 0;
        
    }
}
/***************************************************************
	*	@brief	判断路
	*	@param	无
	*	@note   无
***************************************************************/
void Judge_Breakage(void)
{
    uint8 breakage = 0;
    uint8 miderrormax = 0;
    uint8 miderror = 0;
    uint8 TOP_LINE = 100;
    uint8 cnt=0;
    for(uint8 i=0;i < 100;i++)
    {
        if(feature.road_type[i] == 8)
        {
            cnt++;
        }
    }
    if(feature.breakage_state[1] == 0 && cnt == 0) 
    {
        if(feature.top_point>50 && feature.roundabouts_state==0 && feature.ramp_state[1]==0)
        {
            if(Midline_Std_Deviation((uint8)(feature.top_point+30),(uint8)(feature.top_point)) < 7)
            {
                breakage = 1;
            }
            else
            {
                for(uint8 i=feature.top_point + 20 ; i > 5 ;i--)
                {
                    if(line.left_line_flag[i+1]==1 && line.left_line_flag[i]==1 && line.left_line_flag[i-1]==0 && line.left_line_flag[i-2]==0)
                    {
                        TOP_LINE = i;
                        break;
                    }
                    else if(line.right_line_flag[i+1]==1 && line.right_line_flag[i]==1 && line.right_line_flag[i-1]==0 && line.right_line_flag[i-2]==0)
                    {
                        TOP_LINE = i;
                        break;
                    }
                }
                for(uint8 i=TOP_LINE + 20 ; i > TOP_LINE ; i--)
                {
                    miderror = (uint8)fabs(line.midline[i] - 80);
                    if(miderror > miderrormax)
                    {
                        miderrormax = miderror;
                        feature.breakage_row = i;
                    }
                }
                if(miderrormax > 50)
                {
                    breakage = 0;
                    feature.breakage_state[0] = 0;
                    feature.breakage_state[1] = 0;
                }
                else
                {
                    breakage = 1;
                }
            }

        }
        feature.breakage_state[0] += breakage;
        if(feature.breakage_state[0] > 6)
        {
            feature.breakage_state[1] = 1;
            feature.breakage_state[0] = 0;
            BUZZER_ON;
            LPLD_LPTMR_DelayMs(100);
            BUZZER_OFF;
        }
        else
        {
            feature.breakage_state[1] = 0;
        }
    }
    else if(feature.breakage_state[1] == 1)
    {
        servo.sensor = 2;
        //判断前方赛道宽度是否正常
        int16 width;
        width = line.right_line[40] - line.left_line[40];
        if(width > 110 && width < 140)
        {
            feature.breakage_state[0] +=1;
        }
        else
        {
            feature.breakage_state[0] =0;
        }
        if(feature.breakage_state[0]>3)
        {
            feature.breakage_state[1] = 2;
            feature.breakage_state[0] = 0;
            BUZZER_ON;
            LPLD_LPTMR_DelayMs(100);
            BUZZER_OFF;
        }
    }
    else if(feature.breakage_state[1] == 2)
    {
        //判断摄像头不丢线 走出断路
        if(is_Lose_All() == 0)
        {
            feature.breakage_state[0]++;
        }
        else
        {
            feature.breakage_state[0] = 0;
        }
        if(feature.breakage_state[0] > 3)
        {
            feature.breakage_state[0] = 0;
            feature.breakage_state[1] = 0;
            servo.sensor = 1;
            BUZZER_ON;
            LPLD_LPTMR_DelayMs(100);
            BUZZER_OFF;
        }
    }
}

/***************************************************************
	*	@brief	判十字
	*	@param	无
	*	@note	拐点是否不应该取消，其他路况使用（重新写）
***************************************************************/
void Judge_Cross(void)
{
	if(feature.left_flection_flag == 1 || feature.right_flection_flag == 1)
	{
		if(feature.left_flection_flag == 1)
		{
			//在左边拐弯的标志为1的前提下，左拐列数太大，不宜再进行左拐，应将左拐标志取消
			if(line.left_line[feature.left_flection_row] > 140)
			{
				feature.left_flection_flag = 0;
				feature.left_flection_row = 0;
			}
			//或者在此时右边没有丢线的情况下，左拐标志应为零（十字时候，才存在既有左拐点，同时在左拐点对应的行数上面判断右边线丢线）
			else if(is_Right_Lose_Line(feature.left_flection_row) == 0)
			{
				feature.left_flection_flag = 0;
				feature.left_flection_row = 0;
			}
		}
		if(feature.right_flection_flag == 1)
		{
			if(feature.right_flection_row < 20)
			{
				feature.right_flection_flag = 0;
				feature.right_flection_row = 0;
			}
			else if(is_Left_Lose_Line(feature.right_flection_row) == 0)
			{
				feature.right_flection_flag = 0;
				feature.right_flection_row = 0;
			}
		}
		if(feature.left_flection_flag == 1 && feature.right_flection_flag == 1)
		{
			//左拐点在右拐点右边时 保留近处拐点
			if(line.left_line[feature.left_flection_row] > line.right_line[feature.right_flection_row])
			{
				if(feature.left_flection_row > feature.right_flection_row)
				{
					feature.right_flection_flag = 0;
				}
				if(feature.left_flection_row < feature.right_flection_row)
				{
					feature.left_flection_flag = 0;
				}
			}
		}
		if(feature.left_flection_flag == 1)
		{
			//如果左或右拐点有丢线，原有拐点标志不变，如果没有丢线，原有拐点标志变为0
			if(is_Left_Point_Lose_Line(feature.left_flection_row) == 0 || is_Right_Point_Lose_Line(feature.left_flection_row) == 0)
			{
				feature.left_flection_flag = 0;
			}
		}
		if(feature.right_flection_flag == 1)
		{
			//如果左或右拐点有丢线，原有拐点标志不变，如果没有丢线，原有拐点标志变为0
			if(is_Left_Point_Lose_Line(feature.right_flection_row) == 0 || is_Right_Point_Lose_Line(feature.right_flection_row) == 0)
			{
				feature.right_flection_flag = 0;
			}
		}
		if(feature.left_flection_flag == 1 || feature.right_flection_row == 1)
		{
			if(feature.cross_state[1] == 0)
			{
				feature.cross_state[0]++;
			}
			if(feature.cross_state[0] == 3)
			{
				feature.cross_state[1] = 1;
				feature.cross_state[0] = 0;
			}
			//拐点存在，销去转弯标志位
			if(feature.turn_state != 0)
			{
				feature.turn_state = 0;
			}
		}
		else
		{
			feature.cross_state[1] = 0;
		}
		if(feature.left_flection_flag == 0)
		{
			feature.left_flection_row = 0;
		}
		if(feature.right_flection_flag == 0)
		{
			feature.right_flection_row = 0;
		}
	}
}

/***************************************************************
	*	@brief	判环
	*	@param	无
	*	@note	无
***************************************************************/
void Judge_Roundabouts(void)
{
	uint8 i = 0;
	uint8 cnt = 0;
	for(i = 0; i < 50; i++)
	{
		if(feature.road_type[i] == 4)
		{
			cnt++;
		}
	}
	if(cnt == 0)
	{
		Magnetic_GetAdc();
		if(magnetic.left1_mag > 50 || magnetic.right1_mag > 50 && feature.roundabouts_state == 0)
		{
			if(feature.left_flection2_flag == 1 && feature.right_flection2_flag == 0)
			{
				if(is_Right_Lose_Line(feature.left_flection2_row + 10) == 0)
				{
					feature.roundabouts_state = 1;
				}
			}
			if(feature.left_flection2_flag == 0 && feature.right_flection2_flag == 1)
			{
				if(is_Left_Lose_Line(feature.right_flection2_row + 10) == 0)
				{
					feature.roundabouts_state = 2;
				}
			}
		}
	}
	if(feature.roundabouts_state == 1)
	{
		if(feature.right_flection2_flag == 1)
		{
			servo.enable = 1;
			feature.roundabouts_state = 3;
		}
	}
	if(feature.roundabouts_state == 3)
	{
		if(feature.right_flection_flag == 1)
		{
			if(line.right_line[feature.right_flection_row] > 140)
			{
				feature.roundabouts_state = 5;
			}
		}
	}
	if(feature.roundabouts_state == 5)
	{
		if(feature.top_point < 20 && line.left_line_flag[feature.top_point] == 0 && line.right_line_flag[feature.top_point] == 1
		|| feature.deep_turn_state == 1)
		{
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
	if(feature.roundabouts_state == 2)
	{
		if(feature.left_flection2_flag == 1)
		{
			servo.enable = 1;
			feature.roundabouts_state = 4;
		}
	}
	if(feature.roundabouts_state == 4)
	{
		if(feature.left_flection_flag == 1)
		{
			if(line.left_line[feature.left_flection_row] < 20)
			{
				feature.roundabouts_state = 6;
			}
		}
	}
	if(feature.roundabouts_state == 6)
	{
		if(feature.top_point < 20 && line.left_line_flag[feature.top_point] == 1 && line.right_line_flag[feature.top_point] == 0
		|| feature.deep_turn_state == 2)
		{
			servo.enable = 1;
			feature.roundabouts_state = 0;
		}
	}
}


/**********************a little funcitons**********************/
/***************************************************************
	*	@brief	求60-30行范围内中线标准差
	*	@param	无
	*	@note	无
***************************************************************/
float Midline_Std_Deviation(uint8 row_max,uint8 row_min)
{
	uint8 i = 0;
	uint16 sum1 = 0;
	float32 ave = 0;
	float32 sum2 = 0;
	for(i = row_max; i > row_min; i--)
	{
		sum1 += line.midline[i];
	}
	ave = sum1 / ((float32)(row_max - row_min));
	for(i = row_max; i > row_min; i--)
	{
		sum2 += (line.midline[i] * line.midline[i] - ave * ave);
	}
	return sqrt(sum2 / ((float32)(row_max - row_min)));
}

/***************************************************************
	*	@brief	判断左右边线是否丢线
	*	@param	row：行数
	*	@note	从行数到顶点判断
***************************************************************/
uint8 is_Left_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row; i > feature.top_point; i--)
	{
		if(line.left_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 3)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8 is_Right_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row; i > feature.top_point; i--)
	{
		if(line.right_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 3)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/***************************************************************
	*	@brief	判断某一行附近行是否丢线
	*	@param	row：行数
	*	@note	上下10行判断
***************************************************************/
uint8 is_Left_Point_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 10; i > row - 10; i--)
	{
		if(line.left_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8 is_Right_Point_Lose_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 10; i > row - 10; i--)
	{
		if(line.right_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/***************************************************************
	*	@brief	判断某一行附近行是否全丢线
	*	@param	row：行数
	*	@note	上下7行判断
***************************************************************/
uint8 is_Left_Point_Lose_All_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 7; i > row - 7; i--)
	{
		if(line.left_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 13)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8 is_Right_Point_Lose_All_Line(uint8 row)
{
	uint8 i = 0;
	uint8 lose_cnt = 0;
	for(i = row + 7; i > row - 7; i--)
	{
		if(line.right_line_flag[i] == 0)
		{
			lose_cnt++;
		}
	}
	if(lose_cnt < 13)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
