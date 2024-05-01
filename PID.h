#ifndef __PID_H
#define __PID_H
#include "sys.h"
#include "Motor.h"
#include <stdlib.h>
#define PI 3.14159265
#define ZHONGZHI 3085
extern	int Balance_Pwm,Velocity_Pwm;
int Incremental_PI_Left (int Encoder,int Target);
int Incremental_PI_Right (int Encoder,int Target);
int Position_PID_Position (uint16_t AD_0,uint16_t target);

#endif
