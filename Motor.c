#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "PID.h"
#include "Encoder.h"
#define Motor_Left_Pin_1_Port 				GPIOB
#define Motor_Left_Pin_1 					GPIO_Pin_12
#define Motor_Left_Pin_1_Clock				RCC_APB2Periph_GPIOB
                                            
#define Motor_Left_Pin_2_Port 				GPIOB
#define Motor_Left_Pin_2 					GPIO_Pin_13
#define Motor_Left_Pin_2_Clock				RCC_APB2Periph_GPIOB
                                            
#define Motor_Right_Pin_1_Port 				GPIOB
#define Motor_Right_Pin_1 					GPIO_Pin_14
#define Motor_Right_Pin_1_Clock				RCC_APB2Periph_GPIOB
                                            
#define Motor_Right_Pin_2_Port 				GPIOB
#define Motor_Right_Pin_2 					GPIO_Pin_15
#define Motor_Right_Pin_2_Clock				RCC_APB2Periph_GPIOB

void Motor_Init(void)
{
	PWM_Init();
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 开启 GPIOB 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* GPIO 初始化结构体设置 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* 初始化 GPIO_Pin_5 */
	GPIO_InitStructure.GPIO_Pin = Motor_Left_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 初始化 GPIO_Pin_6 */
	GPIO_InitStructure.GPIO_Pin = Motor_Left_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 初始化 GPIO_Pin_7 */
	GPIO_InitStructure.GPIO_Pin = Motor_Right_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 初始化 GPIO_Pin_8 */
	GPIO_InitStructure.GPIO_Pin = Motor_Right_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Motor_SetSpeed(int8_t Speed)
{ 
}
/**
  * 函    数：左轮直流电机正转设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
*注意事项 ：In1设置为1，In2设置为0
  */
void Motor_Left_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(Motor_Left_Pin_1_Port, Motor_Left_Pin_1);	//PA4置高电平
		GPIO_ResetBits(Motor_Left_Pin_2_Port, Motor_Left_Pin_2);	//PA5置低电平，设置方向为正转
		PWM_1_SetCompare1(Speed);				//PWM设置为速度值
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(Motor_Left_Pin_1_Port, Motor_Left_Pin_1);	//PA4置低电平
		GPIO_SetBits(Motor_Left_Pin_2_Port, Motor_Left_Pin_2);	//PA5置高电平，设置方向为反转
		PWM_1_SetCompare1(-Speed);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
}

/**
  * 函    数：右轮直流电机设置正转速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  *注意事项 ：In1设置为1，In2设置为2
  */
void Motor_Right_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(Motor_Right_Pin_1_Port, Motor_Right_Pin_1);	//置高电平
		GPIO_ResetBits(Motor_Right_Pin_2_Port, Motor_Right_Pin_2);	//置低电平，设置方向为正转
		PWM_2_SetCompare1(Speed);				//PWM设置为速度值
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(Motor_Right_Pin_1_Port, Motor_Right_Pin_1);	//置高电平
		GPIO_SetBits(Motor_Right_Pin_2_Port, Motor_Right_Pin_2);	//置低电平，设置方向为正转
		PWM_2_SetCompare1(-Speed);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
}

/**
  * 函    数：左轮制动
  * 参    数：无
  * 返 回 值：无
  * 注意事项：两个引脚均设置为0
  */
void Motor_Left_Stop(void)
{

	GPIO_ResetBits(Motor_Left_Pin_1_Port, Motor_Left_Pin_1);	//PA4置高电平
	GPIO_ResetBits(Motor_Left_Pin_2_Port, Motor_Left_Pin_2);	//PA5置低电平，设置方向为正转

}
/**
  * 函    数：右轮制动
  * 参    数：无
  * 返 回 值：无
  * 注意事项：两个引脚均设置为0
  */
void Motor_Right_Stop(void)
{

	GPIO_ResetBits(Motor_Right_Pin_1_Port, Motor_Right_Pin_1);	//PA4置高电平
	GPIO_ResetBits(Motor_Right_Pin_1_Port, Motor_Right_Pin_2);	//PA5置低电平，设置方向为正转

}
