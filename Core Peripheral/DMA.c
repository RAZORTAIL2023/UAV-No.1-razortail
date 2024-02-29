#include "DMA.h"

void DMA_set(uint32_t Addr1, uint32_t Addr2, uint32_t Buffer)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = Addr1;                           //外设地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度
	DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;       //是否自增
	
	DMA_InitStruct.DMA_MemoryBaseAddr     = Addr2;                           //存储地址
	DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;     //数据宽度
	DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;            //是否自增
	
	DMA_InitStruct.DMA_BufferSize = Buffer;      //传输计数器
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular; //是否自动重装传输计数器
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;    //是否使用软件触发
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);    //ADC1硬件触发只能是通道1，软件触发任意通道均可
}

void DMA_config(uint32_t Addr1, uint32_t Addr2, uint32_t Buffer)
{
	DMA_set(Addr1, Addr2, Buffer);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
