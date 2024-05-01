#include "Control.h"
#include "PID.h"
#include "OLED.h"
#define Left_Flag 1
#define Right_Flag 2
int16_t Flag = 0;
int16_t Target_Left_PWM = -12;
int16_t Target_Right_PWM = -14;
extern int16_t Left_PWM;
extern int16_t Right_PWM;
uint16_t Trace_AD_Value[5];
void Trace_Line(int16_t difference_1_3,int16_t difference_0_4)
{
	if(difference_1_3 > 1000)
	{
		Target_Right_PWM += 3;
	}
	else if (difference_1_3 < -1000)
	{
		Target_Left_PWM += 3;
	}
	if(difference_0_4 > 1000)
	{
		Target_Right_PWM += 5;
	}
	else if (difference_0_4 < -1000)
	{
		Target_Left_PWM += 5;
	}
}
void Trace(uint16_t AD_Value_Number,uint16_t AD_Value[5])
{
	int i;
    
    for(i = 0; i < 5; i++)
    {
        Trace_AD_Value[i] = AD_Value[i];
    }

    if (Flag == 1)
    {
        Target_Left_PWM = 10;
        Target_Right_PWM = 0;
        if (Trace_AD_Value[2] > 2000 )
        {
            Flag = 0;
        }
    }
    else if (Flag == 0)
    {
	switch (AD_Value_Number) 
		{
			case 0:
				Target_Left_PWM = 0;
				Target_Right_PWM = 0;
				Flag = 1;
				break;
			case 111:
			Target_Left_PWM = 0;
			Target_Right_PWM = 0;
						break;
		
			default:
				Trace_Straight();
				break;
		}
    }

    
	

}
void Trace_Set_Speed(void)
{
	int16_t Set_Left_PWM;
	int16_t Set_Right_PWM;

		
	Set_Left_PWM = Incremental_PI_Left(Left_PWM,Target_Left_PWM) + Target_Left_PWM;
	Set_Right_PWM = Incremental_PI_Right(Right_PWM,Target_Right_PWM) + Target_Right_PWM;
	Motor_Left_SetSpeed(Set_Left_PWM);
	Motor_Right_SetSpeed(Set_Right_PWM);
	
	OLED_ShowSignedNum(1, 1, Flag, 8);
	OLED_ShowSignedNum(2, 5, (int32_t)Set_Left_PWM, 5);	
	OLED_ShowSignedNum(3, 5, (int32_t)Set_Right_PWM, 5);	
}
void Trace_Turn_Left(void)
{	
	Target_Left_PWM = 20;
	Target_Right_PWM = -30;
	Trace_Set_Speed();
}
void Trace_Turn_Right(void)
{		
	Target_Left_PWM = -30;
	Target_Right_PWM = 20;
	Trace_Set_Speed();
}
void Trace_Straight(void)
{	
	Target_Left_PWM = -23;
	Target_Right_PWM = -23;
}
