#ifndef LED_H
#define LED_H

#include "INIT.h"

#define head_LED1 PB0
#define head_LED2 PB1
#define rear_LED1 PA6
#define rear_LED2 PA7

void LED_Hardware_wiring(void);
void LED_control(enum GPIO gpio, FunctionalState able);
void LED_control_ALL(FunctionalState able);
void LED_ENABLE_ALL(void);
void LED_DISABLE_ALL(void);
void LED_TURN(enum GPIO gpio);
void LED_TURN_ALL(void);

#endif
