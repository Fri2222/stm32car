#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "PWM.h"
/**
  * L298N_Enable_A：使能全速运行或者PWM调速，注意与PWM文件保持一致
  * L298N_Input_1：	逻辑输入端1
  * L298N_Input_2：	逻辑输入端2  
  *	L298N_Input_1、L298N_Input_2控制轮子正反转
  */
/*
#define L298N_Enable_A_Port			GPIOA
#define L298N_Enable_A_Pin			GPIO_Pin_0
#define L298N_Enable_A_Clock		RCC_APB2Periph_GPIOA

#define L298N_Enable_B_Port			GPIOA
#define L298N_Enable_B_Pin			GPIO_Pin_6
#define L298N_Enable_B_Clock		PWM_2_Clock

#define L298N_Input_1_Port 			GPIOA
#define L298N_Input_1_Pin			GPIO_Pin_1
#define L298N_Input_1_Clock			RCC_APB2Periph_GPIOA
                                    
#define L298N_Input_2_Port 			GPIOA
#define L298N_Input_2_Pin			GPIO_Pin_2
#define L298N_Input_2_Clock			RCC_APB2Periph_GPIOA
                                    
#define L298N_Input_3_Port 			GPIOA
#define L298N_Input_3_Pin			GPIO_Pin_4
#define L298N_Input_3_Clock			RCC_APB2Periph_GPIOA
                                    
#define L298N_Input_4_Port 			GPIOA
#define L298N_Input_4_Pin			GPIO_Pin_5
#define L298N_Input_4_Clock			RCC_APB2Periph_GPIOA
*/

/**
  * 函    数：L298N初始化
  * 参    数：无
  * 返 回 值：无
  * 注意事项：PWM和Motor的引脚定义
  *           
  */
void L298N_Init(void)
{
	PWM_Init();			//初始化PWM，PWM1引脚为PA0，PWM2引脚为PA6
	Motor_Left_Init();	//左轮初始化，Pin_1,Pin_2
	Motor_Right_Init();	//右轮初始化, Pin_4,Pin_5
}
/**
  * 函    数：L298N设置左转速度
  * 参    数：速度
  * 返 回 值：无
  * 注意事项：函数实现在Motor.c中,速度大于0轮子正转，速度小于0轮子反转
  *           
  */
void L298N_Left_SetSpeed(int8_t Speed)
{
	Motor_Left_SetSpeed(Speed);
}
/**
  * 函    数：L298N设置右转速度
  * 参    数：速度
  * 返 回 值：无
  * 注意事项：函数实现在Motor.c中
  *           
  */
void L298N_Right_SetSpeed(int8_t Speed)
{
	Motor_Right_SetSpeed(Speed);
}
/**
  * 函    数：L298N制动
  * 参    数：无
  * 返 回 值：无
* 注意事项：函数实现在Motor.c中,L298N_Input_1_Pin和L298N_Input_2_Pin均为1
  *           
  */
void L298N_Stop(void)
{
	Motor_Left_Stop();
	Motor_Right_Stop();
}
