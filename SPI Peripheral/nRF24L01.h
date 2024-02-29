#ifndef _nRF24L01_
#define _nRF24L01_

#include "INIT.h"

//CE  Chip Enable 片选使能
//CSN Chip Select Not 对应SPI的SS
//IRQ Interrupt request

#define CE(x)  GPIO_qwrite(PB3, x) //nRF24L01有些操作需在CE低时操作
#define CSN(x) GPIO_qwrite(PB4, x)
#define Read_IRQ() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);

#define RX_OK	0x40
#define TX_OK	0x20
#define MAX_RT	0x10

void nRF24L01_config(void);
int nRF24L01_getData(void);

#endif
