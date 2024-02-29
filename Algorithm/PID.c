#include "PID.h"


void PID()
{	
	//4000/65536
	rateX.measured = GYRO_X * 0.06103515625f; //角速度 +-2000°/s
	rateY.measured = GYRO_Y * 0.06103515625f; //角速度 +-2000°/s
	rateZ.measured = GYRO_Z * 0.06103515625f; //角速度 +-2000°/s
	
	roll .measured = AD_roll ;
	pitch.measured = AD_pitch;
	yaw  .measured = AD_yaw;
	
	//内环measured放在接收数据处
	
	PID_cascade_control(&roll , &rateX, dt);
	PID_cascade_control(&pitch, &rateY, dt);
	PID_cascade_control(&yaw  , &rateZ, dt);
}
