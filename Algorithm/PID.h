#ifndef PID_H
#define PID_H

#include "INIT.h"

typedef volatile struct
{
	float measured;
	float desired;
	float prevError;
	
	float p;
	float i;
	float d;
	
	float Proportion;
	float Integration;
	float Differentiation;
	
	float out;
}PID_arg;

extern PID_arg rateX;
extern PID_arg rateY;
extern PID_arg rateZ;
extern PID_arg pitch;
extern PID_arg roll;
extern PID_arg yaw;

extern PID_arg* (PID_Obj_arr[]);

void angularVelocity_X_PID_config(float kp);
void angularVelocity_Y_PID_config(float kp);
void angularVelocity_Z_PID_config(float kp);
void rockerExpectation_PID_config(float kp);
void PID_reset(void);
void PID_update(PID_arg* st, float dt);
void PID_cascade_control(PID_arg* st1, PID_arg* st2, float dt);
void PID(void);

#endif
