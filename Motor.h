#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Left_Init(void);
void Motor_Right_Init(void);
void Motor_Left_SetSpeed(int8_t Speed);
void Motor_Right_SetSpeed(int8_t Speed);
void Motor_Left_Stop(void);
void Motor_Right_Stop(void);
void Motor_Init(void);
#endif
