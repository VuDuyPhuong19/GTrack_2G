#ifndef _MC60_UART_
#define _MC60_UART_

#include "MC60_UART.h"
#include "main.h"
#include "Fifo.h"
#include <string.h>

void UART_Init(UART_HandleTypeDef* huart, uint32_t size);

char UART_Inchar(UART_HandleTypeDef* huart);

void UART_OutChar(UART_HandleTypeDef* huart, uint8_t data);

void UART_OutString(UART_HandleTypeDef* huart, char* str);

void UART_Receive(UART_HandleTypeDef* huart);

void UART_ProcessReceivedData(UART_HandleTypeDef* huart/*, Fifo_t* Fifo*/, uint8_t data);

void UART_ProcessReceivedString(UART_HandleTypeDef* huart/*, Fifo_t* Fifo*/, char* string);
	
#endif