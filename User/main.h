#ifndef MAIN_H
#define MAIN_H

#include "INIT.h"

extern volatile int16_t  MPU6050_data[6];
#define ACCEL_X   MPU6050_data[0]
#define ACCEL_Y   MPU6050_data[1]
#define ACCEL_Z   MPU6050_data[2]
#define GYRO_X	  MPU6050_data[3]
#define GYRO_Y	  MPU6050_data[4]
#define GYRO_Z	  MPU6050_data[5]

extern volatile uint16_t nRF24L01_data[8];
#define ROLL     nRF24L01_data[0]
#define PITCH    nRF24L01_data[1]
#define THRUST   nRF24L01_data[2]
#define YAW      nRF24L01_data[3]
#define AUX1     nRF24L01_data[4]
#define AUX2     nRF24L01_data[5]
#define AUX3     nRF24L01_data[6]
#define AUX4     nRF24L01_data[7]

extern volatile float    AD_eulerAngle[3];
#define AD_roll  AD_eulerAngle[0]
#define AD_pitch AD_eulerAngle[1]
#define AD_yaw   AD_eulerAngle[2]

extern volatile int16_t  motor        [4];

extern float dt;

#endif
