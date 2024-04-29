#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "L298N.h"
#include "Delay.h"
#include "Buzzer.h"
#include "MyDelay.h"
#include "LightSensor.h"
#include "PWM.h"
#include "Motor.h"
#include "AD.h"
#include "Encoder.h"
#include "Control.h"
#include "delay.h"
#include "sys.h"
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "PID.h"
int16_t Target_Left_Speed = -30;
int16_t Target_Right_Speed = -30;
int16_t Left_Speed;
int16_t Right_Speed;
int8_t Left_Flag;
int8_t Right_Flag;
int main(void)
{	
	int16_t Set_Left_Speed;
	int16_t Set_Right_Speed;
	float pitch,roll,yaw; 		//欧拉角
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	//延时初始化 
	MPU_Init();					//初始化MPU6050
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	PWM_Init();
	AD_Init();					//AD初始化
	Motor_Init();
	Encoder_Init(520 - 1,1 - 1,520 - 1,1 - 1);	//传入参数为自动重装值和预分频数
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "yaw");
	OLED_ShowString(2, 1, "En1:");
	OLED_ShowString(3, 1, "En2:");
	OLED_ShowString(4, 1, "AD:");
	while(mpu_dmp_init())
	{
		delay_ms(10);
	}
	while(1)
	{	
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			OLED_ShowSignedNum(1, 1, yaw, 8);
		}
		Quantize_AD_Value_Number();
		OLED_ShowNum(4, 5, (int32_t)AD_Value_Number, 5);	

		Set_Left_Speed = Incremental_PI(Left_Speed,Target_Left_Speed) ;
		Set_Right_Speed = Incremental_PI(Right_Speed,Target_Right_Speed);
		Motor_Left_Back_SetSpeed(Set_Left_Speed);
		Motor_Right_Back_SetSpeed(Set_Right_Speed);

		OLED_ShowSignedNum(2, 5, (int32_t)Set_Left_Speed, 5);	
		OLED_ShowSignedNum(3, 5, (int32_t)Right_Speed, 5);	
		delay_ms(50);	

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
		Left_Flag = 1;													//否则中断将连续不断地触发，导致主程序卡死
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
		Right_Flag = 1;
	}
}
