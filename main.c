#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "L298N.h"
#include "Delay.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "PWM.h"
#include "Motor.h"
#include "AD.h"
#include "Encoder.h"
#include "Control.h"
int16_t Left_Speed;
int16_t Right_Speed;
int main(void)
{
	
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Read1");
	OLED_ShowString(2, 1, "Read2:");
	OLED_ShowString(3, 1, "AD3:");
	OLED_ShowString(4, 1, "AD4:");
	
	PWM_Init();
	AD_Init();					//AD初始化
	Encoder_Init(1040 - 1,1 - 1,1040 - 1,1 - 1);	//传入参数为自动重装值和预分频数
													//	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; 计数周期，即ARR的值
													//	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;预分频器，即PSC的值
	Motor_Init();
	int PWM_2 = Position_PID(AD_Value[2], 1);

	//uint16_t i;
	while (1)
	{
//		Speed = (Read_Encoder_TIM4()/1.04);
		OLED_ShowNum(1, 7, Quantize_0_1(AD_Value[2]), 5);			//显示转换结果第4个数据
		OLED_ShowNum(2, 7, Quantize_0_1(AD_Value[1]), 5);		//显示转换结果第1个数据
		OLED_ShowNum(3, 7, Quantize_0_1(AD_Value[3]), 5);		//显示转换结果第2个数据
		OLED_ShowNum(4, 8, Quantize_0_1(AD_Value[4]), 5);		//显示转换结果第3个数据


		//adjustMotorSpeed();		
		Motor_Left_Forward_SetSpeed(15);
        Motor_Right_Forward_SetSpeed(15);
			//Left_Speed = Read_Encoder_TIM4();
		OLED_ShowNum(1, 8, Left_Speed, 5);	
			//Right_Speed = Read_Encoder_TIM3();
		OLED_ShowNum(2, 8, Right_Speed, 5);		//显示转换结果第3个数据


		Delay_us(100);							//延时100ms，手动增加一些转换的间隔时间
	}

}
/**
  * 函    数：TIM4中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)		//判断是否是TIM4的更新事件触发的中断
	{
		Left_Speed = Read_Encoder_TIM4();								//每隔固定时间段读取一次编码器计数增量值，即为速度值
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);			//清除TIM4更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}
/**
  * 函    数：TIM3中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)		//判断是否是TIM3的更新事件触发的中断
	{
		Right_Speed = Read_Encoder_TIM3();								//每隔固定时间段读取一次编码器计数增量值，即为速度值
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			//清除TIM3更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}
