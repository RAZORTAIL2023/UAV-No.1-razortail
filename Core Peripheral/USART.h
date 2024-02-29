#ifndef USART_H
#define USART_H

#include "INIT.h"

void USART_config(void);

void USART_send_data(uint16_t data);
void USART_send_array(uint8_t* array, int length);
void USART_send_char(char* String);
void USART_send_number(uint32_t number);
void USART_vsprintf(char* format,...);

#endif
