#include "Inter-Integrated Circuit.h"


void I2C_write(uint8_t device, uint8_t _register, uint8_t data)
{
	SDA(pullDown);
	
	if( I2C_sendByte(device) != 0)    return;
	if( I2C_sendByte(_register) != 0) return;
	if( I2C_sendByte(data) != 0)      return;
	
	SCL(pullDown);
	I2C_delay();
	SDA(pullDown);
	SCL(release);
	SDA(release);
}

uint8_t I2C_read_current(uint8_t device)
{
	SDA(pullDown);
	
	if(I2C_sendByte(device | 0x01) != 0)  return 255;
	
	uint8_t receive= I2C_recvByte(NACK);
	
	SCL(pullDown);
	SDA(pullDown);
	SCL(release);
	SDA(release);
	
	return receive;
}

uint8_t I2C_read(uint8_t device, uint8_t _register)
{
	SDA(pullDown);
	
	if( I2C_sendByte(device) != 0)    return 255;
	if( I2C_sendByte(_register) != 0) return 255;
	
	//重复开始
	SCL(pullDown);
	I2C_delay();
	SCL(release);
	
	return I2C_read_current(device);
}

void I2C_read_current_continuous(uint8_t device, uint8_t* recv, int size)
{
	SDA(pullDown);
	
	if(I2C_sendByte(device | 0x01) != 0)  return;
	
	for(int i=0; i <= size-2; i++)
	recv[i] = I2C_recvByte(ACK);
	recv[size-1] = I2C_recvByte(NACK);
	
	SCL(pullDown);
	SDA(pullDown);
	SCL(release);
	SDA(release);
}

void I2C_read_continuous(uint8_t device, uint8_t _register, uint8_t* recv, int size)
{
	SDA(pullDown);
	
	if( I2C_sendByte(device) != 0)    return;
	if( I2C_sendByte(_register) != 0) return;
	
	//重复开始
	SCL(pullDown);
	I2C_delay();
	SCL(release);
	
	I2C_read_current_continuous(device, recv, size);
}
