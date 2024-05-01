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
#include "Timer.h"
int16_t Left_PWM;
int16_t Right_PWM;
int8_t Target_Yaw_Position = 10;
int main(void)
{	
	int16_t difference_1_3 ;
	int16_t difference_0_4 ;

	float pitch,roll,yaw; 		//欧拉角
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	//延时初始化 
	MPU_Init();					//初始化MPU6050
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	PWM_Init();
	AD_Init();					//AD初始化
	Motor_Init();
	Timer_Init();
	Encoder_Init(100,720,100,720);	//传入参数为自动重装值和预分频数
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "yaw");
	OLED_ShowString(2, 1, "En1:");
	OLED_ShowString(3, 1, "En2");
	OLED_ShowString(4, 1, "AD:");
//	while(mpu_dmp_init())
//	{
//		delay_ms(10);
//	}
	while(1)
	{	
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			//OLED_ShowSignedNum(1, 1, yaw, 8);
		}
		Quantize_AD_Value_Number();
		OLED_ShowNum(4, 5, (int32_t)AD_Value_Number, 5);	
		int16_t Position_PWM = Position_PID_Position(yaw,Target_Yaw_Position);
		Trace(AD_Value_Number,AD_Value);
		difference_1_3 = AD_Value_Difference_1_3();
		difference_0_4 = AD_Value_Difference_0_4();
		Trace_Line(difference_1_3,difference_0_4);
		Trace_Set_Speed();
		delay_ms(1);
	} 	

}


/**
  * 函    数：TIM1中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)		//判断是否是TIM1的更新事件触发的中断
	{
		Left_PWM = Read_Encoder_TIM4() / 80;			//每隔固定时间段读取一次编码器计数增量值，减速比1：20，13轴的电机
		Right_PWM = Read_Encoder_TIM3() / 80;			//每隔固定时间段读取一次编码器计数增量值，减速比1：20，13轴的电机
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);			//清除TIM1更新事件的中断标志位
															//中断标志位必须清除
	}
}
