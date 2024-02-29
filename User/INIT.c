#include "INIT.h"


volatile enum isLocked isLocked; //全局锁定/解锁标志
void INIT()
{
	//NVIC优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,     ENABLE); //AFIO
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,    ENABLE); //GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,    ENABLE); //GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,    ENABLE); //GPIOC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,     ENABLE); //TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,     ENABLE); //TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,     ENABLE); //TIM4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,     ENABLE); //ADC
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC最大频率14Mhz
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,       ENABLE); //DMA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,   ENABLE); //USART1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,     ENABLE); //I2C2
	//调试
	OLED_init();
}

//数学

//0x0001移位数量计算
uint8_t findShiftAmount(uint32_t target)
{
	uint32_t base = 0x00000001; // 需要匹配的结果
	uint8_t shiftAmount =0; // 记录位移数量
	
	while(base != target)
	{
		base <<= 1;
		shiftAmount++;
		
		if(shiftAmount > 31)
		{
			shiftAmount = 32;
			break;
		}
	}
	
	return shiftAmount;
}

//TRANSFORM

//GPIOx转换为GPIO_PortSourceGPIOx
uint8_t GPIO_PortSource_transform(GPIO_TypeDef* GPIOx)
{
	return ((uint32_t)GPIOx - APB2PERIPH_BASE) / 0x0400 - 2;
}

//GPIO_Pin_x转换为GPIO_PinSourcex
uint8_t GPIO_PinSource_transform(uint16_t GPIO_Pin)
{
	return findShiftAmount(GPIO_Pin);
}

//EXTI_Linex对应的EXTIx_IRQn，仅限0~4
uint8_t EXTI_IRQn_transform(uint32_t EXTI_Line)
{
	return findShiftAmount(EXTI_Line) + 6;
}

//仅限TIM2 TIM3 TIM4
uint8_t TIM2_4_IRQn_transform(TIM_TypeDef* TIMx)
{
	return ((uint32_t)TIMx - APB1PERIPH_BASE) / 0x0400 + 28;
}


//0~233015
void delay_us(uint32_t us)
{
	SysTick->LOAD = 72 * us;
	SysTick->CTRL = 0x00000005;
	while((SysTick->CTRL & 0x00010000) == 0);
	SysTick->CTRL = 0x00000004;
}

//n：0~4294967295
void delay_ms(uint32_t ms)
{
	while(ms--)delay_us(1000);
}
 
//n：0~4294967295
void delay_s(uint32_t s)
{
	while(s--)delay_ms(1000);
}

float square_root_reciprocal(float a)
{
	float half = 0.5f * a;
	
	int temp = *(int*)&a;
	temp = 0x5F3759Df - (temp>>1);
	float x = *(float*)&temp;
	
	return x*(1.5f - half * x * x);
}

int16_t limit(int16_t x, int16_t min, int16_t max)
{
	int16_t temp = x > max ? max : x;
	return x < min ? min : temp;
}

float limitf(float x,    float min,   float max)
{
	float   temp = x > max ? max : x;
	return x < min ? min : temp;
}

void timeout(uint16_t parameter, uint16_t threshold, uint32_t time)
{
	static int count = 0;
	if(parameter<threshold)
	{
		count++;
		if(count>time)
		isLocked = locked; //超时处理
	}
	else
		count=0;
}
