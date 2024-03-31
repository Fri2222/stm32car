#include "stm32f10x.h"  // Device header
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void LED_ON(uint16_t GPIO_Pin_x)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_x);
}

void LED_OFF(uint16_t GPIO_Pin_x)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_x);
}
void LED_Turn (uint16_t GPIO_Pin_x)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_x,(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_x)+1)%2);	
}


