#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
void Trace(uint16_t AD_Value_Number,uint16_t AD_Value[5]);
void Trace_Set_Speed(void);
void Trace_Turn_Left(void);
void Trace_Turn_Right(void);
void Trace_Straight(void);
void Trace_Line(int16_t difference_1_3,int16_t difference_0_4);
#endif
