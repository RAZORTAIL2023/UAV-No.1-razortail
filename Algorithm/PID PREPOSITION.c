#include "PID.h"


PID_arg rateX; PID_arg rateY; PID_arg rateZ;
PID_arg pitch; PID_arg roll ; PID_arg yaw  ;
PID_arg* (PID_Obj_arr[]) = {&rateX, &rateY, &rateZ, &pitch, &roll, &yaw};

//2 2*0.02 2*0.1
void angularVelocity_X_PID_config(float kp)
{
	rateX.p = kp;
	//积分低了导致无法回正，高了随时间累积多了会影响比例
	rateX.i = kp * 0.1f;
	rateX.d = kp * 0.05f;
}

void angularVelocity_Y_PID_config(float kp)
{
	rateY.p = kp;
	//积分低了导致无法回正，高了随时间累积多了会影响比例
	rateY.i = kp * 0.1f;
	rateY.d = kp * 0.05f;
}

void angularVelocity_Z_PID_config(float kp)
{
	rateZ.p = kp;
	//积分低了导致无法回正，高了随时间累积多了会影响比例
	rateZ.i = kp * 0.1f;
	rateZ.d = kp * 0.05f;
}

void rockerExpectation_PID_config(float kp)
{
	roll.p  = kp;
	pitch.p = kp;
	yaw.p   = kp;
}

void PID_reset()
{
	for(int i=0; i<6; i++)
	{
		PID_Obj_arr[i]->Integration = 0;
		PID_Obj_arr[i]->prevError   = 0;
		PID_Obj_arr[i]->out         = 0;
	}
}

void PID_update(PID_arg* st, float dt)
{
	float error = st->desired - st->measured;
	
	st->Proportion       = st->p * error;
	st->Integration     += st->i * error * dt;
	st->Differentiation  = st->d * (error - st->prevError) / dt;
	
	//OLED_show_floatNumber(1,1,st->Proportion,10,5);
	//微积分项调试没有意义，因为调试延时大。
	
	st->out = st->Proportion + st->Integration + st->Differentiation;
	
	st->prevError = error;
}

void PID_cascade_control(PID_arg* st1, PID_arg* st2, float dt)
{
	PID_update(st1, dt);
	st2->desired = st1->out;
	PID_update(st2, dt);
}
