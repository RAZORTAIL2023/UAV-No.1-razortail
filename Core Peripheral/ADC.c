#include "ADC.h"

void ADC_set()
{
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;        //扫描模式
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;  //连续模式
	ADC_InitStruct.ADC_NbrOfChannel = 4;             //通道数目
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //触发源
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; //对齐
	ADC_Init(ADC1, &ADC_InitStruct);
}

uint16_t ADC_result[4] = {0,0,0,0};

void ADC_config()
{
	ADC_set();
	
	//上电
	ADC_Cmd(ADC1, ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	//连接DMA
	ADC_DMACmd(ADC1, ENABLE);
	//配置DMA
	DMA_config((uint32_t)&ADC1->DR, (uint32_t)ADC_result,4); //'&'不可省略
	
	//端口
	GPIO_qset(PA0, GPIO_Mode_AIN); //ADC12_IN0
	GPIO_qset(PA1, GPIO_Mode_AIN); //ADC12_IN0
	GPIO_qset(PA2, GPIO_Mode_AIN); //ADC12_IN0
	GPIO_qset(PA3, GPIO_Mode_AIN); //ADC12_IN0
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0,1,ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1,2,ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2,3,ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3,4,ADC_SampleTime_28Cycles5);
	
	//启动！
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
