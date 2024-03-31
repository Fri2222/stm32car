#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_Left_Forward_SetSpeed(int8_t Speed);
void Motor_Left_Back_SetSpeed(int8_t Speed);
void Motor_Right_Forward_SetSpeed(int8_t Speed);
void Motor_Right_Back_SetSpeed(int8_t Speed);
void Motor_Left_Stop(void);
void Motor_Right_Stop(void);

#endif
