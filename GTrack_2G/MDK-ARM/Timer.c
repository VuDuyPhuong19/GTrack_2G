#include "Timer.h"

uint8_t start_led_count = 3;
uint8_t restart_led_count = 5;
uint8_t error2G_led_count = 3;

volatile uint32_t timer3_cycles = 0; // Ð?m s? chu k? c?a timer3
volatile uint8_t ledState = 0; // Tr?ng thaii hi?n t?i c?a LED


void delay_1ms(){
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while(__HAL_TIM_GET_COUNTER(&htim3) <= COUNTER);
}

void delay(uint32_t time_ms){
	uint32_t i = 0;
	for (i = 0; i < time_ms; i++){
			delay_1ms();
	}
}

void TimerCallbackHandle(){
		if(status == Publish){
				HAL_GPIO_WritePin(GPIOA, pa11_Pin, 1); 
				HAL_TIM_Base_Start_IT(&htim3);
		}
		else if (status == Start){
				if(start_led_count--){
						HAL_GPIO_TogglePin(GPIOA, pa11_Pin);
						status = Start;
						HAL_TIM_Base_Start_IT(&htim3);
				}
				else {
					start_led_count = 3;
					HAL_TIM_Base_Stop_IT(&htim3);
				}
		}
		else if (status == Restart){
				if(restart_led_count--){
						HAL_GPIO_TogglePin(GPIOA, pa11_Pin);
						status = Restart;
						HAL_TIM_Base_Start_IT(&htim3);
				}
				else {
					restart_led_count = 5;
					HAL_TIM_Base_Stop_IT(&htim3);
				}
		}
		else if (status == Error2G){
				if(error2G_led_count--){
						HAL_GPIO_TogglePin(GPIOA, pa12_Pin);
						status = Error2G;
						HAL_TIM_Base_Start_IT(&htim3);
				}
				else {
					error2G_led_count = 3;
					HAL_TIM_Base_Stop_IT(&htim3);
				}
		}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {
				TimerCallbackHandle();
		}
}
