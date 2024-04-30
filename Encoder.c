#include "ENCODER.h"
void Encoder_Init(u16 arr1,u16 psc1,u16 arr2,u16 psc2)
{
	Encoder_Init_TIM3(arr1,psc1);
	Encoder_Init_TIM4(arr2,psc2);		

}
/*TIM4初始化为编码器接口*/
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
	TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能CPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PB6、PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM4, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

	TIM_Cmd(TIM4, ENABLE); //使能定时器4
}

//读取编码器计数
 int Read_Encoder_TIM4(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp;
}
/*TIM3初始化为编码器接口*/
void Encoder_Init_TIM3(u16 arr,u16 psc)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA6和PA7引脚初始化为上拉输入
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元
	
	/*输入捕获初始化*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//定义结构体变量
	TIM_ICStructInit(&TIM_ICInitStructure);							//结构体初始化，若结构体没有完整赋值
																	//则最好执行此函数，给结构体所有成员都赋一个默认值
																	//避免结构体初值不确定的问题
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//选择配置定时器通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
	TIM_ICInit(TIM3, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//选择配置定时器通道2
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
	TIM_ICInit(TIM3, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
	
	/*编码器接口配置*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//配置编码器模式以及两个输入通道是否反相
																	//注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表是否反相
																	//此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);			//使能TIM3，定时器开始运行
}

//读取编码器计数
int Read_Encoder_TIM3(void)
{
	/*使用Temp变量作为中继，目的是返回CNT后将其清零*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}

/*
void Encoder_Init_TIM3(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
	TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能CPIOA时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PA6、PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM3, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

	TIM_Cmd(TIM3, ENABLE); //使能定时器4
}
*/
/*
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
	TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能CPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PB6、PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM4, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

	TIM_Cmd(TIM4, ENABLE); //使能定时器4
}
*/
