#ifndef __OLED_H
#define __OLED_H

#include "INIT.h"

void OLED_init(void);
void OLED_clear(void);
void OLED_show_char(uint8_t Line, uint8_t Column, char Char);
void OLED_show_string(uint8_t Line, uint8_t Column, char *String);
void OLED_show_number(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_show_signedNumber(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_show_hexNumber(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_show_binNumber(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_show_floatNumber(uint8_t line, uint8_t colume, float number, uint8_t length, uint8_t decimal_places);
void debugging(void);

#endif
