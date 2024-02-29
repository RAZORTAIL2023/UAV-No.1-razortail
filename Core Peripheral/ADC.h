#ifndef ADC_H
#define ADC_H

#include "INIT.h"

void ADC_config(void);
uint16_t ADC_convert(void);

extern uint16_t ADC_result[4];

#endif


