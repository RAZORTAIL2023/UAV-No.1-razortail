#ifndef TIMER_H
#define TIMER_H

#include "INIT.h"

typedef enum
{
	OutputCompare1,
	OutputCompare2,
	OutputCompare3,
	OutputCompare4,
	OutputCompare_ALL,
	InputCapture,
	Interrupt,
	None
}TIM_pattern;

void timer_set_base(TIM_TypeDef* TIMx, uint16_t ARR, uint16_t TIM_Prescaler);
void GPtimer_config(TIM_TypeDef* TIMx, uint8_t clock_source, uint16_t ARR, uint16_t TIM_Prescaler,
					TIM_pattern pattern, ...);

#endif
