//#include "MC60_UART.h"

//static uint8_t rx_data[12];
//static Fifo_t Fifo1;
//static Fifo_t Fifo3;

//UART_HandleTypeDef huart1;
//UART_HandleTypeDef huart3;

//Fifo_t* Fifo_array[] = {&Fifo1, NULL, &Fifo3};
//UART_HandleTypeDef* huart_array[] = {&huart1, NULL, &huart3};

//// UART Receive Callback
////void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
////	uint8_t i = 0;
////	for(i = 0; i < (sizeof(huart_array) / sizeof(huart_array[0])); i++){
////		if(huart == huart_array[i]){
////			//UART_ProcessReceivedData(&huart1, rx_data);
////			UART_ProcessReceivedString(&huart1, rx_data);
////			break;
////		}
////	}
////	UART_Receive(&huart);
////	HAL_GPIO_TogglePin(GPIOA, pa11_Pin);
////}

////void UART_Init(UART_HandleTypeDef* huart, uint32_t size){
////	Fifo_t* Fifo = NULL;
////	if (huart == &huart1) Fifo = &Fifo1;
////	if (huart == &huart3) Fifo = &Fifo3;
////	Fifo_Init(Fifo,size);
////}

////char UART_Inchar(UART_HandleTypeDef* huart){
////	Fifo_t* Fifo = NULL;
////	if (huart == &huart1) Fifo = &Fifo1;
////	if (huart == &huart3) Fifo = &Fifo3;
////	while(isEmpty(Fifo)); // wait until the FIFO not empty
////	return Fifo_Get(Fifo);
////}

//void UART_OutChar(UART_HandleTypeDef* huart, uint8_t data){
//	HAL_UART_Transmit(huart, &data, 1, 1);
//}

//void UART_OutString(UART_HandleTypeDef* huart, char* str){
////	while(*str != NULL){
////		UART_OutChar(huart, (uint8_t)*str++);
////	}
//	HAL_UART_Transmit(huart, (uint8_t*)str, strlen(str), 100);
//}

////void UART_Receive(UART_HandleTypeDef* huart){
////	HAL_UART_Receive_IT(huart, &rx_data, 1);
////}

//void UART_Receive(UART_HandleTypeDef* huart){
//	HAL_UART_Receive_IT(huart, rx_data, sizeof(rx_data));
//}

////void UART_ProcessReceivedData(UART_HandleTypeDef* huart, Fifo_t* Fifo, uint8_t data){
////	Fifo_Init(Fifo, 1);
////	Fifo_Put(Fifo, data);
////	uint8_t Data = Fifo_Get(Fifo);
////	UART_OutChar(huart, Data);
////}

//void UART_ProcessReceivedData(UART_HandleTypeDef* huart, uint8_t data){
//	UART_OutChar(huart, data);
//}

//void UART_ProcessReceivedString(UART_HandleTypeDef* huart, char* string){
//	UART_OutString(huart, string);
//}

//	

