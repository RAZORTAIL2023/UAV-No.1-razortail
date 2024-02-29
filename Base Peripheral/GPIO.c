#include "GPIO.h"


void GPIO_qset(enum GPIO gpio, GPIOMode_TypeDef GPIO_Mode)
{
	if(gpio<=15)      GPIO_set(GPIOA,0x0001<<gpio-00,GPIO_Mode);
	else if(gpio>=16) GPIO_set(GPIOB,0x0001<<gpio-16,GPIO_Mode);
}

void GPIO_qwrite(enum GPIO gpio, BitAction action)
{
	if(gpio<=15)      GPIO_WriteBit(GPIOA,0x0001<<gpio-00,action);
	else if(gpio>=16) GPIO_WriteBit(GPIOB,0x0001<<gpio-16,action);
}

uint8_t GPIO_qread_output(enum GPIO gpio)
{
	if(gpio<=15) return GPIO_ReadOutputDataBit(GPIOA,0x0001<<gpio-00);
	else         return GPIO_ReadOutputDataBit(GPIOB,0x0001<<gpio-16);
}

void GPIO_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**************************
	写
	GPIO_SetBits(GPIOA, GPIO_Pin_0);             //高电平
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);           //低电平
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);   //高电平
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET); //低电平
	GPIO_Write(GPIOC, 0x00000000);               //全低电平
	GPIOC->ODR = 0x00002000;                     //GPIOC P13 高电平
	
	读
	GPIO_ReadInputDataBit (GPIOA, GPIO_Pin_0);
	GPIO_ReadInputData (GPIOA); //读取输入端口寄存器
	GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0);
	GPIO_ReadOutputData(GPIOA); //读取输出端口寄存器
**************************/
