#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#include "Motor.h"
#include <stdlib.h>
#define PI 3.14159265
#define ZHONGZHI 3085
extern	int Balance_Pwm,Velocity_Pwm;
extern uint16_t AD_Value[5];

void adjustMotorSpeed(void);
int Incremental_PI (int Encoder,int Target);
int Position_PID (uint16_t AD_0,uint16_t target);
#endif
