#include "stm32f10x.h"                  // Device header
/*
 *PWM1使用通用定时器TIM2通道3，总线为APB1引脚为PA0
 *PWM2使用通用定时器TIM3通道3，总线为APB1引脚为PB0
*/

#define PWM_1_Port 				GPIOA
#define PWM_1_Pin			 	GPIO_Pin_0
#define PWM_1_Clock				RCC_APB2Periph_GPIOA
#define PWM_2_Port 				GPIOB
#define PWM_2_Pin			 	GPIO_Pin_0
#define PWM_2_Clock				RCC_APB2Periph_GPIOB
void PWM_1_Init(void)
{	
	//通用计时器2总线为APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 输出 PWM 波时使用的是内部时钟
    TIM_InternalClockConfig(TIM2);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;    // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    // 初始化 GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    // 配置 PWM_1_Pin 引脚
    GPIO_InitStruct.GPIO_Pin = PWM_1_Pin;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    
    // 初始化比较通道
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 50; // CCR 要结合 ARR 计算占空比
    
    // 配置 TIM2 的第一个输出通道（TIM_OC1）到 PA0 引脚
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);
    
    TIM_Cmd(TIM2, ENABLE);
	
}
void PWM_2_Init(void)
{	
	//通用计时器2总线为APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 输出 PWM 波时使用的是内部时钟
    TIM_InternalClockConfig(TIM2);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;    // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    // 初始化 GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    // 配置 PWM_1_Pin 引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    
    // 初始化比较通道
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 50; // CCR 要结合 ARR 计算占空比
    
    // 配置 TIM2 的第3个输出通道（TIM_OC3）到 PA2 引脚
    TIM_OC3Init(TIM2, &TIM_OCInitStruct);
    
    TIM_Cmd(TIM2, ENABLE);
	
}
//void PWM_2_Init(void)
//{
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    
//    // 输出 PWM 波时使用的是内部时钟
//    TIM_InternalClockConfig(TIM3);
//    
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;    // ARR
//    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
//    
//    // 初始化 GPIO
//    GPIO_InitTypeDef GPIO_InitStruct;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    
//    // 配置 PWM_2_Pin 引脚
//    GPIO_InitStruct.GPIO_Pin = PWM_2_Pin;
//    GPIO_Init(GPIOB, &GPIO_InitStruct);
//    
//    
//    // 初始化比较通道
//    TIM_OCInitTypeDef TIM_OCInitStruct;
//    TIM_OCStructInit(&TIM_OCInitStruct);
//    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStruct.TIM_Pulse = 50; // CCR 要结合 ARR 计算占空比
//    
//    // 配置 TIM3 的第3个输出通道（TIM_OC3）到 PWM_2_Pin 引脚
//    TIM_OC3Init(TIM3, &TIM_OCInitStruct);
//    
//    TIM_Cmd(TIM3, ENABLE);
//}
/**                             
  * 函    数：PWM初始化
  * 参    数：无
  * 返 回 值：无
  */
void PWM_Init(void)
{
	PWM_1_Init();
	PWM_2_Init();
}

/**
  * 函    数：PWM1设置CCR
  * 参    数：Compare 要写入的CCR的值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
  *           占空比Duty = CCR / (ARR + 1)
  */
void PWM_1_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);		//设置CCR3的值

}
/**
  * 函    数：PWM2设置CCR
  * 参    数：Compare 要写入的CCR的值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
  *           占空比Duty = CCR / (ARR + 1)
  */
void PWM_2_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);		//设置CCR3的值
}


