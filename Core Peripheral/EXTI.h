#ifndef EXTI_H
#define EXTI_H

#include "INIT.h"

void EXTI_config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t EXTI_Line,
				 uint8_t NVIC_IRQChannelPreemptionPriority,
				 uint8_t NVIC_IRQChannelSubPriority);

#endif
