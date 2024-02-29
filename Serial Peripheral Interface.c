#include "Serial Peripheral Interface.h"


void SPI_Hardware_wiring()
{
	GPIO_qset(PB13, GPIO_Mode_Out_PP);      //SCK
	GPIO_qset(PB15, GPIO_Mode_Out_PP);      //MOSI
	GPIO_qset(PB14, GPIO_Mode_IN_FLOATING); //MISO
	
	SCK(low); //默认为低，下降沿准备，上升沿采样
	GPIO_qwrite(PB12, Bit_SET);
}

uint8_t SPI_swapByte(uint8_t Byte)
{
	uint8_t recv = 0x00;
	for(int i=7;i>=0;i--)
	{
		if(Byte&0x01<<i) MOSI(high); else MOSI(low); //移出
		SCK(high);
		recv |= Read_MISO()<<i; //移入
		SCK(low);
	}
	return recv;
}
