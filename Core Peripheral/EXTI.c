#include "EXTI.h"

void EXTI_set(uint32_t EXTI_Line, EXTIMode_TypeDef mode, EXTITrigger_TypeDef trigger)
{
	//AFIO时钟不可写在此处，我暂时不知道原因
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=mode;
	EXTI_InitStruct.EXTI_Trigger=trigger;
	
	EXTI_Init(&EXTI_InitStruct);
}

void EXTI_config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t EXTI_Line,
				 uint8_t NVIC_IRQChannelPreemptionPriority,
				 uint8_t NVIC_IRQChannelSubPriority)
{
	//设置GPIO
	GPIO_set(GPIOx, GPIO_Pin, GPIO_Mode_IN_FLOATING);
	//将GPIO连接到EXTI
	GPIO_EXTILineConfig(GPIO_PortSource_transform(GPIOx), GPIO_PinSource_transform(GPIO_Pin));
	//设置EXTI
	EXTI_set(EXTI_Line, EXTI_Mode_Interrupt, EXTI_Trigger_Falling);
	//设置NVIC，EXTI0_IRQn=6
	NVIC_set(EXTI_IRQn_transform(EXTI_Line), NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriority);
}
