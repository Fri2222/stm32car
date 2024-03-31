#include "stm32f10x.h"                  // Device header
#include "Motor.h"
extern uint16_t AD_Value[5];

/**
  * 函    数：AD初始化
  * 参    数：无
  * 返 回 值：无
  */
void AD_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//开启ADC1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//开启DMA1的时钟
	
	/*设置ADC时钟*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);						//选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA0、PA1、PA2、PA4和PA5引脚初始化为模拟输入
	
	/*规则组通道配置*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);	//规则组序列1的位置，配置为通道1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);	//规则组序列2的位置，配置为通道2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_55Cycles5);	//规则组序列3的位置，配置为通道3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_55Cycles5);	//规则组序列4的位置，配置为通道0
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_55Cycles5);	//规则组序列5的位置，配置为通道0

	/*ADC初始化*/
	ADC_InitTypeDef ADC_InitStructure;											//定义结构体变量
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;							//模式，选择独立模式，即单独使用ADC1
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//数据对齐，选择右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			//外部触发，使用软件触发，不需要外部触发
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//连续转换，使能，每转换一次规则组序列后立刻开始下一次转换
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;								//扫描模式，使能，扫描规则组的序列，扫描数量由ADC_NbrOfChannel确定
	ADC_InitStructure.ADC_NbrOfChannel = 5;										//通道数，为5，扫描规则组的前5个通道
	ADC_Init(ADC1, &ADC_InitStructure);											//将结构体变量交给ADC_Init，配置ADC1
	
	/*DMA初始化*/
	DMA_InitTypeDef DMA_InitStructure;											//定义结构体变量
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				//外设基地址，给定形参AddrA
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设数据宽度，选择半字，对应16为的ADC数据寄存器
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址自增，选择失能，始终以ADC数据寄存器为源
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;					//存储器基地址，给定存放AD转换结果的全局数组AD_Value
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//存储器数据宽度，选择半字，与源数据宽度对应
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//存储器地址自增，选择使能，每次转运后，数组移到下一个位置
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//数据传输方向，选择由外设到存储器，ADC数据寄存器转到数组
	DMA_InitStructure.DMA_BufferSize = 5;										//转运的数据大小（转运次数），与ADC通道数一致
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//模式，选择循环模式，与ADC的连续转换一致
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//存储器到存储器，选择失能，数据由ADC外设触发转运到存储器
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//优先级，选择中等
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);								//将结构体变量交给DMA_Init，配置DMA1的通道1
	
	/*DMA和ADC使能*/
	DMA_Cmd(DMA1_Channel1, ENABLE);							//DMA1的通道1使能
	ADC_DMACmd(ADC1, ENABLE);								//ADC1触发DMA1的信号使能
	ADC_Cmd(ADC1, ENABLE);									//ADC1使能
	
	/*ADC校准*/
	ADC_ResetCalibration(ADC1);								//固定流程，内部有电路会自动执行校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	/*ADC触发*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//软件触发ADC开始工作，由于ADC处于连续转换模式，故触发一次后ADC就可以一直连续不断地工作
}
/**
  * 函    数：量化ADC转化后的模拟量的值
  * 参    数：ADC的值
  * 返 回 值：量化后的值
  * 注意 事项 :最大值为10，最小值为0，步长为0.01
  */
float Quantize(uint16_t adc_value)
{
    const float QUANTIZATION_RANGE_MIN = 0;   // 量化范围最小值
    const float QUANTIZATION_RANGE_MAX = 10;  // 量化范围最大值
   // const float ADC_MAX_VALUE = 4095;        // ADC 转换结果的最大值（假设为 12 位 ADC）
    const float STEP_SIZE = 0.01;            // 步长

    float quantized_value = (adc_value * STEP_SIZE) + QUANTIZATION_RANGE_MIN;

    // 确保量化值在指定范围内
    if (quantized_value > QUANTIZATION_RANGE_MAX) {
        quantized_value = QUANTIZATION_RANGE_MAX;
    } else if (quantized_value < QUANTIZATION_RANGE_MIN) {
        quantized_value = QUANTIZATION_RANGE_MIN;
    }

    return quantized_value;
}
/**
  * 函    数：调整电机速度
  * 参    数：无（AD_Value是全局函数）
  * 返 回 值：无
  * 注意 事项 :阈值为500
  */
int16_t adjustMotorSpeed(void) 
{
	int Speed;
    const float THRESHOLD = 500;  // 阈值

    if (AD_Value[0] > THRESHOLD&& AD_Value[4] <= THRESHOLD) 
	{ 
		Speed = AD_Value[0]/1000;
        Motor_Left_Forward_SetSpeed(Speed);
		Motor_Right_Forward_SetSpeed(Speed+10);

    }
    if (AD_Value[4] > THRESHOLD&& AD_Value[0] <= THRESHOLD) 
	{
		Speed = AD_Value[4]/1000;
		Motor_Right_Forward_SetSpeed(Speed);
		Motor_Left_Forward_SetSpeed(Speed+10);
    }
	if(AD_Value[0] > THRESHOLD && AD_Value[4] > THRESHOLD)
	{
		if(AD_Value[0] > AD_Value[4]) 
		{
			Speed =(AD_Value[0] - AD_Value[4])/50;
			Motor_Left_Forward_SetSpeed(Speed);
			Motor_Right_Forward_SetSpeed(Speed);
		}
		else 
		{
			Speed =(AD_Value[4] - AD_Value[0])/500;
			Motor_Left_Forward_SetSpeed(Speed);
			Motor_Right_Forward_SetSpeed(Speed);

		}
	}
	if(AD_Value[0] <= THRESHOLD && AD_Value[4] <= THRESHOLD)
	{
		if(AD_Value[0] > AD_Value[4]) 
		{
			Speed =(AD_Value[0] - AD_Value[4])*3;
			Motor_Left_Forward_SetSpeed(Speed);
			Motor_Right_Forward_SetSpeed(Speed - 50);
		}
		else 
		{
			Speed =(AD_Value[4] - AD_Value[0])*3;
			
			Motor_Left_Forward_SetSpeed(Speed);
			Motor_Right_Forward_SetSpeed(Speed - 50);

		}

	}
	return Speed;
}