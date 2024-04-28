#ifndef __TIMER_H
#define __TIMER_H	
#include "sys.h"
#include "stm32f10x_tim.h"
void Encoder_Init(u16 arr1,u16 psc1,u16 arr2,u16 psc2);
void Encoder_Init_TIM4(u16 arr,u16 psc);
int Read_Encoder_TIM4(void);
void Encoder_Init_TIM3(u16 arr,u16 psc);
int Read_Encoder_TIM3(void);

#endif
