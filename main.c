#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "L298N.h"
#include "Delay.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "PWM.h"
#include "Motor.h"
int16_t Speed;
uint8_t KeyNum;
int main(void)
{
	//OLED_Init();
	PWM_Init();
	//text();
	uint16_t i;
//	Buzzer_Init();
//	Buzzer_OFF();
//Motor_Left_Init();
	
	//Motor_Right_Init();
	Motor_Init();
	Motor_Left_SetSpeed(10);
	while(1)
	{
		for (i = 0; i <= 100; i++)
		{
			Motor_Left_SetSpeed(i);
			Motor_Right_SetSpeed(i);
			//TIM_SetCompare1(TIM2,i);		
			//TIM_SetCompare1(TIM3,i);	

	
			Delay_ms(10);				//延时10ms
		}
	}
}
