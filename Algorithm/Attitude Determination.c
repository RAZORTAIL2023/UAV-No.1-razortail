#include "Attitude Determination.h"


float q0=1, q1=0, q2=0, q3=0;
float gravity_X, gravity_Y, gravity_Z;
void mahony(float kp, float dt)
{
	/*
		该算法使用加速度运算数据来修正角速度，
		然而当飞行器水平时，无论偏航角为多少都不影响观测得到的重力分量，
		此时加速度不能反映出偏航角，故此时偏航角测不准。
	*/
	
	//ki大了干扰kp，ki有助于角速度消噪
	float ki = kp * 0.01f;
	dt *= 0.5f; //提前乘个1/2
	
	static float recipNorm;
	
	static float gx, gy, gz;
	static float coefficient = 0.00106526489f; //°/s → 弧度/s 4000/65536*0.0174533
	gx = GYRO_X * coefficient;
	gy = GYRO_Y * coefficient;
	gz = GYRO_Z * coefficient;
	
	static float ax, ay, az;
	ax = ACCEL_X;
	ay = ACCEL_Y;
	az = ACCEL_Z;
	recipNorm = square_root_reciprocal(ax*ax + ay*ay + az*az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;
	
	q0 = q0 + dt*(-gx*q1 - gy*q2 - gz*q3);
	q1 = q1 + dt*( gx*q0 - gy*q3 + gz*q2);
	q2 = q2 + dt*( gx*q3 + gy*q0 - gz*q1);
	q3 = q3 + dt*(-gx*q2 + gy*q1 + gz*q0);
	recipNorm = square_root_reciprocal(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
	gravity_X =      2.0f*(q1*q3-q0*q2);
	gravity_Y =      2.0f*(q0*q1+q2*q3);
	gravity_Z = 1.0f-2.0f*(q1*q1+q2*q2);
	
	/*****************************************************/
	
	static float error_x, error_y, error_z;
	error_x = (ay * gravity_Z - az * gravity_Y);
	error_y = (az * gravity_X - ax * gravity_Z);
	error_z = (ax * gravity_Y - ay * gravity_X);
	
	static float integral_error_x, integral_error_y, integral_error_z;
	integral_error_x += error_x * ki * dt;
	integral_error_y += error_y * ki * dt;
	integral_error_z += error_z * ki * dt;
	
	gx += kp * error_x + integral_error_x;
	gy += kp * error_y + integral_error_y;
	gz += kp * error_z + integral_error_z;
	
	/*****************************************************/
	
	q0 = q0 + dt*(-gx*q1 - gy*q2 - gz*q3);
	q1 = q1 + dt*( gx*q0 - gy*q3 + gz*q2);
	q2 = q2 + dt*( gx*q3 + gy*q0 - gz*q1);
	q3 = q3 + dt*(-gx*q2 + gy*q1 + gz*q0);
	recipNorm = square_root_reciprocal(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
	
	float g1, g2, g3, g4, g5;
	g1 = 2.0f * (q1*q3 - q0*q2);
	g2 = 2.0f * (q0*q1 + q2*q3);
	g3 = q0*q0 - q1*q1 - q2*q2 + q3*q3;
	g4 = 2.0f * (q1*q2 + q0*q3);
	g5 = q0*q0 + q1*q1 - q2*q2 - q3*q3;
	
	AD_roll  =  atan2f(g2, g3) * 57.29577951308f; //弧度 → 角度
	AD_pitch = -asinf (g1)     * 57.29577951308f; //弧度 → 角度
	AD_yaw   =  atan2f(g4, g5) * 57.29577951308f; //弧度 → 角度
}
