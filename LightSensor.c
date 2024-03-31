#include "stm32f10x.h"                  // Device header
#define LightSensor_Port 		GPIOB
#define LightSensor_Pin 		GPIO_Pin_13
#define LightSensor_Clock		RCC_APB2Periph_GPIOB
/**
  * 函    数：光敏传感器初始化
  * 参    数：无
  * 返 回 值：无
  */
void LightSensor_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(LightSensor_Clock, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = LightSensor_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LightSensor_Port, &GPIO_InitStructure);						//将PB13引脚初始化为上拉输入
}

/**
  * 函    数：获取当前光敏传感器输出的高低电平
  * 参    数：无
  * 返 回 值：光敏传感器输出的高低电平，范围：0/1
  */
uint8_t LightSensor_Get(void)
{
	return GPIO_ReadInputDataBit(LightSensor_Port, LightSensor_Pin);			//返回PB13输入寄存器的状态
}
