#include "USART.h"
#include "math.h"

void USART_set()
{
	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 9600;
	USART_Init(USART1,&USART_InitStruct);
}

void USART_config()
{
	USART_set();
	
	//端口
	GPIO_qset(PA9, GPIO_Mode_AF_PP);
	GPIO_qset(PA10, GPIO_Mode_IN_FLOATING);
	
	//使能
	USART_Cmd(USART1, ENABLE);
	
	//中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_set(USART1_IRQn,0,0);
}

void USART_send_data(uint16_t data)
{
	USART_SendData(USART1, data); //发送
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //发送ing
	//发送自动清除标志位
}
void USART_send_array(uint8_t* array,   int length)
{
	for(int i=1; i<=length; i++)USART_send_data(array[i-1]);
}
void USART_send_char(char* String)
{
	for(int i=1; String[i-1] != '\0'; i++)USART_send_data(String[i-1]);
}
void USART_send_number(uint32_t number)
{
	for(int countDownDigits = floor(log10(number)) + 1; countDownDigits>=1; countDownDigits--)USART_send_data((uint16_t)(number % (uint32_t)pow(10,countDownDigits) / pow(10,countDownDigits-1)) + 0x30); //注意偏移不可省略
}

//printf()函数
int fputc(int ch, FILE *F)
{
	USART_send_data(ch);
	return ch;
}

//vsprintf()函数
void USART_vsprintf(char* format,...)
{
	va_list ap;
	__va_start(ap, format);
	char String[100]; vsprintf(String, format, ap);
	va_end(ap);
	USART_send_char(String);
}
