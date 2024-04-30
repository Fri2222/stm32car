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
int16_t Target_Left_Speed = -10;
int16_t Target_Right_Speed = -10;
int16_t Left_Speed;
int16_t Right_Speed;
int8_t Left_Flag;
int8_t Right_Flag;
int8_t Target_Yaw_Position = 30;
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
	Timer_Init();
	Encoder_Init(100,720,100,720);	//传入参数为自动重装值和预分频数
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "yaw");
	OLED_ShowString(2, 1, "En1:");
	OLED_ShowString(3, 1, "En2:");
	OLED_ShowString(4, 1, "pos:");
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
		//OLED_ShowNum(4, 5, (int32_t)AD_Value_Number, 5);	

		
		Set_Left_Speed = Incremental_PI(Left_Speed,Target_Left_Speed) ;
		Set_Right_Speed = Incremental_PI(Right_Speed,Target_Right_Speed);
		Motor_Left_Back_SetSpeed(50);
		Motor_Right_Back_SetSpeed(50);

		OLED_ShowSignedNum(2, 5, (int32_t)Left_Speed, 5);	
		OLED_ShowSignedNum(3, 5, (int32_t)Right_Speed, 5);	
		
		int16_t Position_PWM = Position_PID(yaw,Target_Yaw_Position);
		OLED_ShowSignedNum(4, 5, (int32_t)Position_PWM, 5);	
		delay_ms(50);	

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
		Left_Speed = Read_Encoder_TIM4();			//每隔固定时间段读取一次编码器计数增量值，减速比1：20，13轴的电机
		Right_Speed = Read_Encoder_TIM3() ;			//每隔固定时间段读取一次编码器计数增量值，减速比1：20，13轴的电机
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);			//清除TIM1更新事件的中断标志位
															//中断标志位必须清除
		Left_Flag = 1;													//否则中断将连续不断地触发，导致主程序卡死
		Right_Flag = 1;												//否则中断将连续不断地触发，导致主程序卡死
	}
}
///*TIM4初始化为编码器接口*/
//void Encoder_Init_TIM4(u16 arr,u16 psc)
//{
//	/*开启时钟*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//开启TIM4的时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//开启GPIOB的时钟
//	
//	/*GPIO初始化*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PB6和PB7引脚初始化为上拉输入
//	
//	/*时基单元初始化*/
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
//	TIM_TimeBaseInitStructure.TIM_Period = arr - 1;              	 //计数周期，即ARR的值
//	TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;                //预分频器，即PSC的值
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM4的时基单元
//	
//	/*输入捕获初始化*/
//	TIM_ICInitTypeDef TIM_ICInitStructure;							//定义结构体变量
//	TIM_ICStructInit(&TIM_ICInitStructure);							//结构体初始化，若结构体没有完整赋值
//																	//则最好执行此函数，给结构体所有成员都赋一个默认值
//																	//避免结构体初值不确定的问题
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//选择配置定时器通道1
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
//	TIM_ICInit(TIM4, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//选择配置定时器通道2
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
//	TIM_ICInit(TIM4, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
//	
//	/*编码器接口配置*/
//	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//																	//配置编码器模式以及两个输入通道是否反相
//																	//注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表是否反相
//																	//此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
//	
//	/*TIM使能*/
//	TIM_Cmd(TIM4, ENABLE);			//使能TIM3，定时器开始运行
//}





///*TIM3初始化为编码器接口*/
//void Encoder_Init_TIM3(u16 arr,u16 psc)
//{
//	/*开启时钟*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
//	
//	/*GPIO初始化*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA6和PA7引脚初始化为上拉输入
//	
//	/*时基单元初始化*/
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
//	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //计数周期，即ARR的值
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //预分频器，即PSC的值
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元
//	
//	/*输入捕获初始化*/
//	TIM_ICInitTypeDef TIM_ICInitStructure;							//定义结构体变量
//	TIM_ICStructInit(&TIM_ICInitStructure);							//结构体初始化，若结构体没有完整赋值
//																	//则最好执行此函数，给结构体所有成员都赋一个默认值
//																	//避免结构体初值不确定的问题
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//选择配置定时器通道1
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
//	TIM_ICInit(TIM3, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//选择配置定时器通道2
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
//	TIM_ICInit(TIM3, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
//	
//	/*编码器接口配置*/
//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//																	//配置编码器模式以及两个输入通道是否反相
//																	//注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表是否反相
//																	//此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
//	
//	/*TIM使能*/
//	TIM_Cmd(TIM3, ENABLE);			//使能TIM3，定时器开始运行
//}