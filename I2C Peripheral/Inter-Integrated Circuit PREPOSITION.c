#include "Inter-Integrated Circuit.h"


void I2C_Hardware_wiring()
{
	GPIO_qset(PB6, GPIO_Mode_Out_OD);
	GPIO_qset(PB7, GPIO_Mode_Out_OD);
	SCL(release);
	SDA(release);
}

uint8_t I2C_sendByte(uint8_t Byte)
{
	/*循环8次*/
	for(int i=0;i<8;i++)
	{
		SCL(pullDown);					  //主 拉低SCL
		SDA((BitAction)(Byte&0x80>>i));   //主 准备SDA
		SCL(release);                     //主 释放SCL
        I2C_delay();                      //从 读取SDA
	}
	/*应答*/
	SCL(pullDown); SDA(release);		  //主 拉低SCL、释放SDA
	I2C_delay();						  //从 准备ACK
	SCL(release); return Read_SDA();	  //主 释放SCL、读取ACK
}

//如果仅使用一次就将结束I2C，那么拉低SCL之后的释放SDA就不是必要的，因为在那之前SDA由从设备控制
uint8_t I2C_recvByte(BitAction ack)
{
	uint8_t recv = 0x00;
	/*循环8次*/
	for(int i=7;i>=0;i--)
	{
		SCL(pullDown); SDA(release);      //主 拉低SCL
		I2C_delay();					  //从 准备SDA
		SCL(release);					  //主 释放SCL
		recv |= Read_SDA()<<i;		      //主 读取SDA
	}
	/*应答*/
	SCL(pullDown);                        //主 拉低SCL
	SDA(ack);                             //主 NACK/ACK
	SCL(release);                         //主 释放SCL
	I2C_delay();                          //从 读取NACK
	return recv;
}

uint8_t I2C_test(uint8_t device)
{
	I2C_Hardware_wiring();
	SDA(pullDown);
	uint8_t ack = I2C_sendByte(device);
	
	SCL(pullDown);
	I2C_delay();
	SDA(pullDown);
	SCL(release);
	SDA(release);
	
	return ack;
}
