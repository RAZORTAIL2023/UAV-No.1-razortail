#include "LED.h"


void LED_Hardware_wiring()
{
	GPIO_qset(PB0, GPIO_Mode_Out_PP); //前灯1
	GPIO_qset(PB1, GPIO_Mode_Out_PP); //前灯2
	GPIO_qset(PA6, GPIO_Mode_Out_PP); //后灯1
	GPIO_qset(PA7, GPIO_Mode_Out_PP); //后灯2
	LED_control_ALL(DISABLE);
}

void LED_control(enum GPIO gpio, FunctionalState able)
{
	if(able==ENABLE)
		GPIO_qwrite(gpio, Bit_RESET);
	else if(able==DISABLE)
		GPIO_qwrite(gpio, Bit_SET);
}

void LED_control_ALL(FunctionalState able)
{
	LED_control(head_LED1, able);
	LED_control(head_LED2, able);
	LED_control(rear_LED1, able);
	LED_control(rear_LED2, able);
}

void LED_ENABLE_ALL()
{
	LED_control(head_LED1, ENABLE);
	LED_control(head_LED2, ENABLE);
	LED_control(rear_LED1, ENABLE);
	LED_control(rear_LED2, ENABLE);
}

void LED_DISABLE_ALL()
{
	LED_control(head_LED1, DISABLE);
	LED_control(head_LED2, DISABLE);
	LED_control(rear_LED1, DISABLE);
	LED_control(rear_LED2, DISABLE);
}

void LED_TURN(enum GPIO gpio)
{
	if(GPIO_qread_output(gpio)==0)
		GPIO_qwrite(gpio, Bit_SET);
	else if(GPIO_qread_output(gpio)==1)
		GPIO_qwrite(gpio, Bit_RESET);
}

void LED_TURN_ALL()
{
	LED_TURN(head_LED1);
	LED_TURN(head_LED2);
	LED_TURN(rear_LED1);
	LED_TURN(rear_LED2);
}
