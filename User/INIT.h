#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#include "stm32f10x.h"
#include "main.h"

#include "Inter-Integrated Circuit.h"
#include "I2C interface.h"
#include "Serial Peripheral Interface.h"

#include "GPIO.h"
#include "LED.h"
#include "OLED.h"

#include "Timer.h"
#include "EXTI.h"
#include "NVIC.h"
#include "ADC.h"
#include "DMA.h"
#include "USART.h"

#include "PID.h"
#include "Attitude Determination.h"
#include "motor.h"
#include "Calibration.h"

/**********************************************************
这里的枚举名称采用了与变量名同名的重复名称。
这是因为枚举类型名受限于我的技术无法省略，
它的作用仅限于定义volatile isLocked.
**********************************************************/
enum isLocked
{
	locked,
	unlocked
}extern volatile isLocked;

void INIT(void);
uint8_t findShiftAmount(uint32_t value);
uint8_t GPIO_PortSource_transform(GPIO_TypeDef* GPIOx);
uint8_t GPIO_PinSource_transform(uint16_t GPIO_Pin);
uint8_t EXTI_IRQn_transform(uint32_t EXTI_Line);
uint8_t TIM2_4_IRQn_transform(TIM_TypeDef* TIMx);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delay_s (uint32_t  s);
float square_root_reciprocal(float a);
int16_t limit(int16_t x, int16_t min, int16_t max);
float limitf(float x, float min, float max);
void timeout(uint16_t parameter, uint16_t threshold, uint32_t time);

#endif
