#ifndef _MC60_H_
#define _MC60_H_

#include "main.h"
#include <string.h>
#include "GNSS_data.h"

extern uint8_t	rxbuffer[100];
extern uint8_t rxbuffer_NMEA[700];

// Turn on MC60
void MC60_PowerOn();
// Turn off MC60
void MC60_PowerOff();
// Send AT Command
void MC60_ATCommand_Send(UART_HandleTypeDef *huart,const char * ATCOM);
// Receive data from MC60
void MC60_Receive(UART_HandleTypeDef *huart, char* rx_buffer);
// Send AT Read Command
void MC60_ATCommand_Read(UART_HandleTypeDef * huartz, const char * ATCOM);
// Send AT Write Command
void MC60_ATCommand_Write(UART_HandleTypeDef * huart, const char * ATCOM, const char* parameter);
// Turn on GNSS
void MC60_GNSS_On(UART_HandleTypeDef * huart);
// 
void MC60_View(UART_HandleTypeDef * huart);
// Receive NMEA data
void MC60_Receive_NMEA(UART_HandleTypeDef * huart);

void MC60_View_NMEA(UART_HandleTypeDef * huart);

void MC60_Debug_test(UART_HandleTypeDef * huartz, const char * ATCOM);

void MC60_test_view(UART_HandleTypeDef * huartz);

void MC60_Get_Sentence_By_Header(UART_HandleTypeDef * huart);

#endif