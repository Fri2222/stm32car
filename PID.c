#include "PID.h"	
float Position_KP=0.5,Position_KI=0.01,Position_KD=0;  //PID系数
float Velocity_KP=0.05,Velocity_KI=0.0005,Velocity_KD=0.2; //PID系数
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

int Incremental_PI_Left (int Encoder,int Target)
{ 	
	 static float Bias_Left, Pwm_Left, Last_Bias_Left, Integral_Bias_Left;
	 Bias_Left = Target - Encoder;				//计算偏差
	 Integral_Bias_Left += Bias_Left;
	 Pwm_Left = Velocity_KP * Bias_Left + Velocity_KI * Integral_Bias_Left + Velocity_KD * (Bias_Left - Last_Bias_Left);   //增量式PI控制器
	 Last_Bias_Left = Bias_Left;	                                   //保存上一次偏差 
	if(Pwm_Left>5)
		Pwm_Left=5;
	if(Pwm_Left<-5 )
		Pwm_Left=-5;
	 return Pwm_Left;                                           //增量输出
}
int Incremental_PI_Right (int Encoder,int Target)
{ 	
	 static float Bias_Right, Pwm_Right, Last_Bias_Right, Integral_Bias_Right;
	 Bias_Right = Target - Encoder;				//计算偏差
	 Integral_Bias_Right += Bias_Right;
	 Pwm_Right = Velocity_KP * Bias_Right + Velocity_KI * Integral_Bias_Right + Velocity_KD * (Bias_Right - Last_Bias_Right);   //增量式PI控制器
	 Last_Bias_Right = Bias_Right;	                                   //保存上一次偏差 
	if(Pwm_Right>5)
		Pwm_Right=5;
	if(Pwm_Right<-5 )
		Pwm_Right=-5;
	 return Pwm_Right;                                           //增量输出
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
int Position_PID_Position (uint16_t position,uint16_t target)
{ 	
	 static float Bias_Position,Pwm_Position,Integral_Bias_Position,Last_Bias_Position;
	 Bias_Position = position-target;                                  //计算偏差
	 Integral_Bias_Position += Bias_Position;	                                 //求出偏差的积分
	 Pwm_Position = Position_KP*Bias_Position + Position_KI*Integral_Bias_Position + Position_KD*(Bias_Position-Last_Bias_Position);       //位置式PID控制器
	 Last_Bias_Position = Bias_Position;                                       //保存上一次偏差 
	
	if(Pwm_Position>5)
		Pwm_Position=5;
	if(Pwm_Position<-5)
		Pwm_Position=-5;
	 return Pwm_Position;                                            //增量输出
}
