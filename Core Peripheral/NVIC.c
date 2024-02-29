#include "NVIC.h"

void NVIC_set(uint8_t IRQChannel,
			  uint8_t NVIC_IRQChannelPreemptionPriority,
			  uint8_t NVIC_IRQChannelSubPriority)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=IRQChannel;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=NVIC_IRQChannelPreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=NVIC_IRQChannelSubPriority;
	NVIC_Init(&NVIC_InitStruct);
}
