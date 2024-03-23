#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "ModeLightIndicator.h"
#include "stm32f0xx_hal_tim.h" 

#define COUNTER 7998

extern volatile uint32_t timer3_cycles; // Ð?m s? chu k? c?a timer3
extern volatile uint8_t ledState; // Tr?ng thái hi?n t?i c?a LED

void delay(uint32_t time);

#endif /*_TIMER_H_*/