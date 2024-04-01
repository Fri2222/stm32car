#include "Control.h"	
float Position_KP=0.6,Position_KI=0.001,Position_KD=0.5;  //PID系数
float Velocity_KP=150,Velocity_KI=20,Velocity_KD=150; //PID系数
extern uint16_t AD_Value[5];
/**
  * 函    数：调整电机速度
  * 参    数：无（AD_Value是全局函数）
  * 返 回 值：无
  * 注意 事项 :阈值为500
  */
void adjustMotorSpeed(void) 
{
	int difference_2 = 1000 - AD_Value[2];
	uint16_t AD_2 = AD_Value[2];
	int difference_0_4 = AD_Value[0] - AD_Value[4];
	int THRESHOLD = 500;
	if (difference_2 <= 300)
		{
			Motor_Left_Forward_SetSpeed(15);
			Motor_Right_Forward_SetSpeed(19);
		}
	else if (abs(difference_0_4) >= THRESHOLD)
		{
			if (difference_0_4 > 0 )
			{
				// 左右轮速度根据 PID 控制差值进行调整
				int PWM = Position_PID(AD_2, 1000);
				Motor_Left_Forward_SetSpeed(PWM);
				Motor_Right_Forward_SetSpeed(16);
			}
			else
			{
				// 左右轮速度根据 PID 控制差值进行调整
				int PWM = Position_PID(AD_2, 1000);
				Motor_Right_Forward_SetSpeed(PWM);
				Motor_Left_Forward_SetSpeed(16);
			}
		}
	else if(difference_2 > 300 && (AD_Value[0]>700) && (AD_Value[4]>700))
	{
		Motor_Left_Stop();
		Motor_Right_Forward_SetSpeed(16);
	}
	else 
	{
		int PWM = Position_PID (AD_2,1000);
		Motor_Left_Forward_SetSpeed(PWM);
		Motor_Right_Forward_SetSpeed(16);
	}

}
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm += Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/

int Incremental_PI (int Encoder,int Target)
{ 	
	 static float Bias, Pwm, Last_bias, Integral_bias;
	 Bias = Target - Encoder;				//计算偏差
	 Integral_bias += Bias;
	 Pwm = Velocity_KP * Bias + Velocity_KI * Integral_bias + Velocity_KD * (Bias - Last_bias);   //增量式PI控制器
	 Last_bias = Bias;	                                   //保存上一次偏差 
	if(Pwm>30)
		Pwm=30;
	if(Pwm<-30)
		Pwm=-30;
	 return Pwm;                                           //增量输出
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PID (uint16_t AD_0,uint16_t target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias = AD_0-target;                                  //计算偏差
	 Integral_bias += Bias;	                                 //求出偏差的积分
	 Pwm = Position_KP*Bias + Position_KI*Integral_bias + Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias = Bias;                                       //保存上一次偏差 
	
	if(Pwm>10)
		Pwm=10;
	if(Pwm<-10)
		Pwm=-10;
	 return Pwm;                                            //增量输出
}