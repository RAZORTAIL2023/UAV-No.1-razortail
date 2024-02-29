#ifndef SERIAL_PERIPHERAL_INTERFACE_
#define SERIAL_PERIPHERAL_INTERFACE_

#include "INIT.h"
#include "nRF24L01.h"

//同步全双工
//MSB First

//SCK	Serial Clock				//串行时钟
//MOSI	Master Output Slave Input	//主输出从输入
//MISO	Master Input Slave Output	//主输入从输出
//SS	Slave Select				//从机选择

//CPOL	Clock Polarity
//CPHA	Clock Phase

#define high  Bit_SET
#define low Bit_RESET

#define	SCK(x)  GPIO_qwrite(PB13, x)
#define	MOSI(x)	GPIO_qwrite(PB15, x)
#define	Read_MISO()	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)

void SPI_Hardware_wiring(void);
uint8_t SPI_swapByte(uint8_t Byte);

#endif
