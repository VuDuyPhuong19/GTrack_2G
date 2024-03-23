#include "ModeLightIndicator.h"

enum Status status;

//void startBlinkingWithTimer3(void) {
//    timer3_cycles = 0; // пt l?i s? chu k?
//    ledState = 1; // пm b?o LED b?t d?u trong tr?ng th᩠t?t
//    HAL_TIM_Base_Start_IT(&htim3); // B?t d?u timer v?i ng?t
//}

//void stopBlinkingWithTimer3(void) {
//    HAL_TIM_Base_Stop_IT(&htim3); 
//    timer3_cycles = 0; 
//    HAL_GPIO_WritePin(GPIOA, pa11_Pin, GPIO_PIN_RESET); 
//}

void publishLedSuccess(){
	HAL_GPIO_WritePin(GPIOA, pa11_Pin, 0);
	status = Publish;
	HAL_TIM_Base_Start_IT(&htim3);
}
void publishLedError(){
	HAL_GPIO_WritePin(GPIOA, pa11_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, pa12_Pin, 0);
	status = ErrorPublish;
	HAL_TIM_Base_Start_IT(&htim3);
}
void StartLedStatus(){
	HAL_GPIO_WritePin(GPIOA, pa11_Pin, 0);
	status = Start;
	HAL_TIM_Base_Start_IT(&htim3);
}

void RestartLedStatus(){
	HAL_GPIO_WritePin(GPIOA, pa11_Pin, 0);
	status = Restart;
	HAL_TIM_Base_Start_IT(&htim3);
}

void Error2GLed(){
	HAL_GPIO_WritePin(GPIOA, pa12_Pin, 0);
	status = Error2G;
	HAL_TIM_Base_Start_IT(&htim3);
}

void ErrorPublishLed(){
	HAL_GPIO_WritePin(GPIOA, pa12_Pin, 0);
	status = ErrorPublish;
	HAL_TIM_Base_Start_IT(&htim3);
}