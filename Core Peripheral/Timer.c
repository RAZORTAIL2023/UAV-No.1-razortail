#include "timer.h"


void clock_select(TIM_TypeDef* TIMx, uint8_t x)
{
	/*注意：2、3时钟源中主定时器的时钟源不能为2、3，否则可能间接调用自身，无限递归！*/
	switch(x){
		case 1:
			//内部时钟
			TIM_InternalClockConfig(TIMx);
			break;
		
		case 2:
			//定时器同步（级联）
			//需改动参数：TIM_TS_ITRx	TIM2的ITR2为TIM3
			GPtimer_config(TIM3,1,2000,0,None);
			TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update); //设置主模式
			TIM_ITRxExternalClockConfig(TIMx, TIM_TS_ITR2);       //设置从模式
			break;
		
		case 3:
			//使用一个定时器OC产生的TI信号，作为另一个定时器的外部时钟
			GPtimer_config(TIM3,1,2000,0,OutputCompare1,1000);
			TIM_TIxExternalClockConfig(TIMx, TIM_TIxExternalCLK1Source_TI1,
			TIM_ICPolarity_Rising,0x0); GPIO_qset(PA3, GPIO_Mode_IPU); //设置从模式             
			break;
		
		case 4:
			//External Trigger Input (ETRF) TRGI通道
			TIM_ETRClockMode1Config(TIMx, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted,0x00);
			break;
		
		case 5:
			//External Trigger Input (ETRF) ETRF通道
			TIM_ETRClockMode2Config(TIMx, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted,0x00);
			break;
}}

void timer_set_base(TIM_TypeDef* TIMx, uint16_t ARR, uint16_t TIM_Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Period=ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler=TIM_Prescaler; //计数速度，反比
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStruct);
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
}

void timer_set_OC(TIM_TypeDef* TIMx, uint8_t OCx, uint16_t CCR)
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=CCR;
	/*TIMx_CCER CC1P位 对于向上计数，PWM1模式，
	高极性：不到CCR为高电平，之后为低电平
	低极性：不到CCR为低电平，之后为高电平*/
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ARR预装载使能
	
	if(TIMx == TIM2)
	{
		if(OCx==1)GPIO_qset(PA0, GPIO_Mode_AF_PP);
		if(OCx==2)GPIO_qset(PA1, GPIO_Mode_AF_PP);
		if(OCx==3)GPIO_qset(PA2, GPIO_Mode_AF_PP);
		if(OCx==4)GPIO_qset(PA3, GPIO_Mode_AF_PP);
	}
	else if(TIMx == TIM3)
	{
		if(OCx==1)GPIO_qset(PA6, GPIO_Mode_AF_PP);
		if(OCx==2)GPIO_qset(PA7, GPIO_Mode_AF_PP);
		if(OCx==3)GPIO_qset(PB0, GPIO_Mode_AF_PP);
		if(OCx==4)GPIO_qset(PB1, GPIO_Mode_AF_PP);
	}
	else if(TIMx == TIM4)
	{
		if(OCx==1)GPIO_qset(PB6, GPIO_Mode_AF_PP);
		if(OCx==2)GPIO_qset(PB7, GPIO_Mode_AF_PP);
		if(OCx==3)GPIO_qset(PB8, GPIO_Mode_AF_PP);
		if(OCx==4)GPIO_qset(PB9, GPIO_Mode_AF_PP);
	}
	
	switch(OCx){
	case 1: 
		TIM_OC1Init(TIMx, &TIM_OCInitStruct);
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable); //OC1预装载使能
		break;
	case 2:
		TIM_OC2Init(TIMx, &TIM_OCInitStruct);
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable); //OC2预装载使能
		break;
	case 3:
		TIM_OC3Init(TIMx, &TIM_OCInitStruct);
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable); //OC3预装载使能
		break;
	case 4:
		TIM_OC4Init(TIMx, &TIM_OCInitStruct);
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable); //OC4预装载使能
		break;
}}

/*作为从定时器：
在TI1FP1周期结束时的上升沿，
TIM3的计数器将自动锁存（覆写）至TIM3_CH1的CCR
在TI1FP1周期的下降沿，
TIM3的计数器将自动锁存（覆写）至TIM3_CH2的CCR*/
void timer_set_IC(TIM_TypeDef* TIMx)
{
	GPIO_set(GPIOA, GPIO_Pin_6, GPIO_Mode_IPU); //TIM3的CH1
	//通道一，锁存CCR
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	//通道二，锁存CCR'
	TIM_PWMIConfig(TIMx, &TIM_ICInitStruct);
	/*清零CCR
	通过从模式设置，在TI1FP1周期结束时的上升沿清零CCR
	注意：
	1,这个极性是内定的，不可更改
	2,系统会自动先将Counter锁存至CCR，再清零
	*/
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
}

//General-purpose timer
void GPtimer_config(TIM_TypeDef* TIMx, uint8_t clock_source, uint16_t ARR, uint16_t TIM_Prescaler,
					TIM_pattern pattern, ...)
{
	//可选参数CCR，仅配置OC模式需要
	va_list ap;
	va_start(ap, pattern);
	int CCR = va_arg(ap, int);
	va_end(ap);
	//配置时钟源
	clock_select(TIMx, clock_source);
	//配置定时器
	timer_set_base(TIMx, ARR, TIM_Prescaler);
	//工种选择
	switch(pattern)
	{
		case OutputCompare1:
			timer_set_OC(TIMx, 1, CCR); //配置OC1
			break;
		case OutputCompare2:
			timer_set_OC(TIMx, 2, CCR); //配置OC2
			break;
		case OutputCompare3:
			timer_set_OC(TIMx, 3, CCR); //配置OC3
			break;
		case OutputCompare4:
			timer_set_OC(TIMx, 4, CCR); //配置OC4
			break;
		case OutputCompare_ALL:
			timer_set_OC(TIMx, 1, CCR);
			timer_set_OC(TIMx, 2, CCR);
			timer_set_OC(TIMx, 3, CCR);
			timer_set_OC(TIMx, 4, CCR);
			break;
		case InputCapture:
			timer_set_IC(TIMx);         //配置IC
			break;
		case Interrupt:
			TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); //配置TIM中断
			NVIC_set(TIM2_4_IRQn_transform(TIMx),1,3); //NVIC
			break;
		case None:
			break;
	}
	//使能
	TIM_Cmd(TIMx, ENABLE);
}
