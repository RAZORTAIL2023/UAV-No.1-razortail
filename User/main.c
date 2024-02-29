#include "INIT.h"


volatile int16_t  MPU6050_data [6];
volatile uint16_t nRF24L01_data[8];
volatile float    AD_eulerAngle[3];
volatile int16_t  motor        [4];

float    dt  = 0.0006; //十分重要
uint16_t ARR = 3000;   //rateXYZ不便放缩，故放缩ARR以适配rateXYZ数据

int main()
{
	//打底
	INIT();
	
	//配置
	MPU6050_config ();
	nRF24L01_config();
	angularVelocity_X_PID_config(2.5);
	angularVelocity_Y_PID_config(2.5);
	angularVelocity_Z_PID_config(6);
	rockerExpectation_PID_config(3);
	LED_Hardware_wiring();
	GPtimer_config(TIM2,1,ARR-1,0,OutputCompare_ALL,0);
	
	while(1)
	{
		MPU6050_getData ();
		nRF24L01_getData();
		//在角速度误差固定的情况下，kp决定了“解算准确角度”需要的时间
		mahony(2,dt); //数据融合
		switch(isLocked)
		{
			case locked:
				motor[0] = motor[1] = motor[2] = motor[3] = 0;
				PID_reset();
				yaw.desired = 0; //偏航期望置零
				AD_yaw = 0;      //防止解算偏航漂移
				LED_TURN_ALL();
				delay_ms(100);   //调试时请注释掉该语句
				break;
			
			case unlocked:
				PID(); //解锁后再执行，防止解算值漂移被放大
				motor[0] = motor[1] = motor[2] = motor[3] = limit((THRUST-1000)*ARR/1000,0,ARR*0.8);
				//一部分升力是由rateXYZ提供的
				motor[0] += + rateX.out + rateY.out + rateZ.out;
				motor[1] += - rateX.out + rateY.out - rateZ.out;
				motor[2] += + rateX.out - rateY.out - rateZ.out;
				motor[3] += - rateX.out - rateY.out + rateZ.out;
				timeout(THRUST,1010,300); //油门最低一会锁定
				LED_ENABLE_ALL();
				break;
		}
		TIM_SetCompare1(TIM2, limit(motor[0],0,ARR)  );
		TIM_SetCompare2(TIM2, limit(motor[1],0,ARR)  );
		TIM_SetCompare3(TIM2, limit(motor[2],0,ARR)  );
		TIM_SetCompare4(TIM2, limit(motor[3],0,ARR)  );
		
		//time不可过小，否则姿态数据等算法解算结果将不准确
		static int time = 0; time++; if(time == 50){time = 0; debugging();}
	}
}
