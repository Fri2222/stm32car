#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "L298N.h"
#include "Delay.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "PWM.h"
#include "Motor.h"
#include "AD.h"
int16_t Speed;
int main(void)
{
	
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	//AD_Init();					//AD初始化
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Spd:");
	OLED_ShowString(2, 1, "AD0:");
	OLED_ShowString(3, 1, "AD4:");
	OLED_ShowString(4, 1, "AD2:");
	
	PWM_Init();
	AD_Init();					//AD初始化
	Speed = 1;
	Motor_Init();
	//Motor_Left_Forward_SetSpeed(10);
	//Motor_Right_Forward_SetSpeed(10);
	uint16_t i;
	while (1)
	{
		OLED_ShowNum(1, 5, Speed, 4);		//显示转换结果第4个数据
		OLED_ShowNum(2, 5, Quantize(AD_Value[0])*100, 5);		//显示转换结果第1个数据
		OLED_ShowNum(3, 5, Quantize(AD_Value[4])*100, 5);		//显示转换结果第2个数据
		OLED_ShowNum(4, 5, Quantize(AD_Value[2])*100, 5);		//显示转换结果第3个数据


		Speed = adjustMotorSpeed();		
		Delay_ms(10);							//延时100ms，手动增加一些转换的间隔时间
	}

}
