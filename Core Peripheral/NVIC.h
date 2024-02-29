#ifndef NVIC_H
#define NVIC_H

#include "INIT.h"

void NVIC_set(uint8_t IRQChannel,
			  uint8_t NVIC_IRQChannelPreemptionPriority,
			  uint8_t NVIC_IRQChannelSubPriority);

#endif
