#include "I2C interface.h"


void I2C_set()
{
	I2C_InitTypeDef I2C_InitStruct;
	I2C_StructInit(&I2C_InitStruct);
	I2C_InitStruct.I2C_ClockSpeed = 400 * 1000;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_Init(I2C2,&I2C_InitStruct);
}

void I2C_config()
{
	I2C_set();
	GPIO_qset(PB10, GPIO_Mode_AF_OD);
	GPIO_qset(PB11, GPIO_Mode_AF_OD);
	I2C_Cmd(I2C2, ENABLE);
}

void I2C_waitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS);
}

void I2C_IF_write(uint8_t ADDRESS, uint8_t _register, uint8_t data)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2,ADDRESS, I2C_Direction_Transmitter);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, _register);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2, data);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t I2C_IF_read(uint8_t ADDRESS, uint8_t _register)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2,ADDRESS, I2C_Direction_Transmitter);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, _register);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2,ADDRESS, I2C_Direction_Receiver);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	I2C_waitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	
	uint8_t rec = I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return rec;
}
