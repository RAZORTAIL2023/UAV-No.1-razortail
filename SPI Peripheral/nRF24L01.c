#include "nRF24L01.h"


/*****************************************************************************/
void nRF24L01_write(uint8_t instruction, uint8_t data){
	CSN(low);
	SPI_swapByte(instruction);
	SPI_swapByte(data);
	CSN(high);
}

uint8_t nRF24L01_read(uint8_t instruction){
	CSN(low);
	SPI_swapByte(instruction);
	uint8_t recv = SPI_swapByte(0xFF);
	CSN(high);
	return recv;
}

void nRF24L01_write_continuous(uint8_t instruction, uint8_t* data, int size){
	CSN(low);
	SPI_swapByte(instruction);
	for(int i=0; i<size; i++)
	SPI_swapByte(data[i]);
	CSN(high);
}

//内含malloc函数
uint8_t* nRF24L01_read_continuous (uint8_t instruction, int size){
	CSN(low);
	SPI_swapByte(instruction);
	uint8_t* recv = malloc(size * sizeof(uint8_t));
	for(int i=0; i<size; i++)
	recv[i] = SPI_swapByte(0xFF);
	CSN(high);
	return recv;
}

uint8_t nRF24L01_check1(){
	nRF24L01_write(0x10|0x20,0xBB);
	return nRF24L01_read(0x10|0x00);
}

int nRF24L01_check2(){
	uint8_t arr1[4] = {0xAA,0xAA,0xAA,0xAA};
	nRF24L01_write_continuous(0x10|0x20, arr1,4);
	uint8_t* arr2 = nRF24L01_read_continuous(0x10|0x00, 4);
	int i=0;
	for(; i<=3; i++)
	if(arr2[i]!=0xAA)break;
	if(i==4) return 1; else return 0;
}
/*****************************************************************************/

void nRF24L01_Hardware_wiring()
{
	GPIO_qset(PB3, GPIO_Mode_Out_PP);
	GPIO_qset(PB4, GPIO_Mode_Out_PP);
	GPIO_qset(PB5, GPIO_Mode_IPU);
	SPI_Hardware_wiring();
	CE(low);
	CSN(high);
}

void RX_mode()
{
	//接收模式
	nRF24L01_write(0x00|0x20,0x0f);
	//自动应答 关
	nRF24L01_write(0x01|0x20,0x00);
	//接收数据通道0 允许
	nRF24L01_write(0x02|0x20,0x01);
	//工作通道频率
	nRF24L01_write(0x05|0x20,0x01);
	//1Mbps
	nRF24L01_write(0x06|0x20,0x07);
	//数据通道0 接收地址
	const uint8_t ADDRESS[] = {0xE1,0xE2,0xE3,0xE4,0xE5};
	nRF24L01_write_continuous(0x0A|0x20, (uint8_t*)ADDRESS,5);
	//有效数据宽度 32
	nRF24L01_write(0x11|0x20,32);
	CE(high);
}

void nRF24L01_config()
{
	nRF24L01_Hardware_wiring();
	RX_mode();
}

int nRF24L01_getData()
{
	uint8_t temp = nRF24L01_read(0x07|0x00); //获取状态寄存器标志位
	nRF24L01_write(0x07|0x20,temp);          //清除状态寄存器标志位
	if(temp & RX_OK)                         //判断状态寄存器标志位
	{
		//转换
		uint8_t* recv = nRF24L01_read_continuous(0x61,32);
		ROLL   = (uint16_t)recv[4]  << 8 | recv[5];
		PITCH  = (uint16_t)recv[6]  << 8 | recv[7];
		THRUST = (uint16_t)recv[8]  << 8 | recv[9];
		YAW    = (uint16_t)recv[10] << 8 | recv[11];
		AUX1   = (uint16_t)recv[12] << 8 | recv[13];
		AUX2   = (uint16_t)recv[14] << 8 | recv[15];
		AUX3   = (uint16_t)recv[16] << 8 | recv[17];
		AUX4   = (uint16_t)recv[18] << 8 | recv[19];
		//nRF24L01_write(0xE2,0xFF); //清除RX FIFO寄存器
		free(recv);
		
		//换算
		pitch.desired = (float)-(PITCH-1500) * 0.04f; //1000-2000 -> +-20
		roll .desired = (float)-(ROLL -1500) * 0.04f; //1000-2000 -> +-20
		if     (YAW>1820)yaw.desired -= 0.45f;
		else if(YAW<1180)yaw.desired += 0.45f;
		
		//解锁判断
//		static int lockedFlag = 1;
//		if(fabsf(ADpitch)>50 || fabsf(ADroll)>50)
//		{
//			isLocked = locked;
//			lockedFlag = 0;
//		}
//		if(lockedFlag == 0 && fabsf(ADpitch)<5 && fabsf(ADroll)<5) lockedFlag = 1;
		
		if(YAW<1150 && THRUST<1050) isLocked = locked;
		if(THRUST>1050) isLocked = unlocked;
		
		return 1;
	}
	return 0;
}
