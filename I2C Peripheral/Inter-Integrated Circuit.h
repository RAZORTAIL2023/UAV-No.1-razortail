#ifndef I2C_H
#define I2C_H

#include "INIT.h"
#include "MPU6050.h"

#define release  Bit_SET
#define pullDown Bit_RESET
#define NACK     Bit_SET
#define ACK      Bit_RESET

#define SCL(x) GPIO_WriteBit(GPIOB, GPIO_Pin_6, x)//GPIO_qwrite(PB6,x)
#define SDA(x) GPIO_WriteBit(GPIOB, GPIO_Pin_7, x)//GPIO_qwrite(PB7,x)
#define Read_SDA() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define I2C_delay() {volatile uint8_t i=1; while(i)i--;}

void I2C_Hardware_wiring(void);
uint8_t I2C_sendByte(uint8_t Byte);
uint8_t I2C_recvByte(BitAction ack);
uint8_t I2C_test(uint8_t device);

void I2C_write(uint8_t device, uint8_t _register, uint8_t data);
uint8_t I2C_read(uint8_t device, uint8_t _register);
void I2C_read_current_continuous(uint8_t device, uint8_t* recv, int size);
void I2C_read_continuous(uint8_t device, uint8_t _register, uint8_t* recv, int size);

#endif
