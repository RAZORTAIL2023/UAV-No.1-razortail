#include "MPU6050.h"


/********************************************************************/
int16_t calibration[6] = {0};
int16_t error      [6] = {0};
int check(){
	for(int i=0; i<6; i++) if(abs(error[i]) > 3) return 0;
	return 1;
}
/********************************************************************/

uint8_t data[14];
void MPU6050_getData_PREPOSITION()
{
	I2C_read_continuous(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, data,14);
	
	ACCEL_X = ((int16_t)data[0]  << 8 | data[1]) ;
	ACCEL_Y = ((int16_t)data[2]  << 8 | data[3]) ;
	ACCEL_Z = ((int16_t)data[4]  << 8 | data[5]) ;
	//6,7是温度
	GYRO_X  = ((int16_t)data[8]  << 8 | data[9]) ;
	GYRO_Y  = ((int16_t)data[10] << 8 | data[11]);
	GYRO_Z  = ((int16_t)data[12] << 8 | data[13]);
}

void Get_calibration()
{
	int16_t temp        [6] = {0};
	int     accumulation[6] = {0};
	
	//任一10ms后数据误差大于10ms前3点，视为不水平
	do
	{
		delay_ms(10);
		MPU6050_getData_PREPOSITION();
		for(int i=0; i<6; i++)
		{
			error[i] = MPU6050_data[i] - temp[i];
			temp [i] = MPU6050_data[i];
		}
	}while(!check());
	
	/****************************************************************/
	
	//先来100组去抖，等待滤波完成
	for(int i=1; i<=100; i++)
		MPU6050_getData_PREPOSITION();
	
	memset(calibration,0,12); //清零，可省略
	//累加256次
	for(int count=1; count<=256; count++)
	{
		MPU6050_getData_PREPOSITION();
		for(int i=0; i<6; i++)
		{
			accumulation[i] += MPU6050_data[i];
			if(i==2)accumulation[i]-=16384; //取偏移
		}
	}
	//取平均
	for(int i=0; i<6; i++)
		calibration [i]  = accumulation[i] >> 8;
}

struct KalmanFilterState
{
	float LastP; //上一状态量方差
	float Now_P; //当前状态量方差
	float KG;    //卡尔曼增益
	float Q;     //过程噪声的方差
	float R;     //观测噪声的方差
	float out;
};
void kalman_filter(struct KalmanFilterState* STATE, float input)
{
	//先验方差，忽略状态转移方程
	STATE->Now_P = STATE->LastP + STATE->Q;
	//KG
	STATE->KG    = STATE->Now_P / (STATE->Now_P + STATE->R);
	//没有先验估计，先验估计就是上一次的值
	STATE->out   = STATE->out + STATE->KG * (input - STATE->out);
	//后验方差
	STATE->LastP = (1-STATE->KG) * STATE->Now_P ;
}

void MPU6050_getData()
{
	I2C_read_continuous(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, data,14);
	
	ACCEL_X = ((int16_t)data[0]  << 8 | data[1])  - calibration[0];
	ACCEL_Y = ((int16_t)data[2]  << 8 | data[3])  - calibration[1];
	ACCEL_Z = ((int16_t)data[4]  << 8 | data[5])  - calibration[2];
	//6,7是温度
	GYRO_X  = ((int16_t)data[8]  << 8 | data[9])  - calibration[3];
	GYRO_Y  = ((int16_t)data[10] << 8 | data[11]) - calibration[4];
	GYRO_Z  = ((int16_t)data[12] << 8 | data[13]) - calibration[5];
	
	static struct KalmanFilterState STATE[3]=
	{ {0,0,0,0.001,0.5,0}, {0,0,0,0.001,0.5,0}, {0,0,0,0.001,0.5,16384} };
	kalman_filter(&STATE[0], (float)ACCEL_X); ACCEL_X = (int16_t)STATE[0].out;
	kalman_filter(&STATE[1], (float)ACCEL_Y); ACCEL_Y = (int16_t)STATE[1].out;
	kalman_filter(&STATE[2], (float)ACCEL_Z); ACCEL_Z = (int16_t)STATE[2].out;
}

void MPU6050_config()
{
	I2C_Hardware_wiring();
	I2C_write(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1,  0x80); //复位
	delay_ms(30);
	I2C_write(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1,  0x01); //时钟源
	I2C_write(MPU6050_ADDRESS, MPU6050_SMPLRT_DIV,  0x02); //分频
	I2C_write(MPU6050_ADDRESS, MPU6050_CONFIG,      0x06); //低通滤波最高
	I2C_write(MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG,0b00000001); //量程+-2g、高通滤波最高
	I2C_write(MPU6050_ADDRESS, MPU6050_GYRO_CONFIG ,0b00011000); //量程+-2000°/s
	
	Get_calibration();
}

/***************************************************************************************************
FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	//擦除一页，不成功进入死循环
	if(FLASH_ErasePage      (0x08007C00)                       != FLASH_COMPLETE)while(1){}
	//循环写入，不成功进入死循环
	for(int i=0; i<6; i++)
	if(FLASH_ProgramHalfWord(0x08007C00 + 2*i, calibration[i]) != FLASH_COMPLETE)while(1){}
FLASH_Lock();

for(int i=0; i<6; i++)
calibration[i] = *(volatile int16_t*)(0x08007C00 + 2*i);
***************************************************************************************************/
