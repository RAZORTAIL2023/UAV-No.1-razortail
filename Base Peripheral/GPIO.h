#ifndef GPIO_H
#define GPIO_H

//#include "INIT.h"必须写在下面，否则会提前在LED.h包含使用enum GPIO的函数声明
enum GPIO
{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,
	PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,
	PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15
};

#include "INIT.h"

void GPIO_qset(enum GPIO gpio, GPIOMode_TypeDef GPIO_Mode);
void GPIO_qwrite(enum GPIO gpio, BitAction action);
uint8_t GPIO_qread_output(enum GPIO gpio);
void GPIO_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);

#endif
