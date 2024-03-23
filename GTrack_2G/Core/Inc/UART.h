#ifndef _UART_H_
#define _UART_H_

#include "main.h"
#include <string.h>

#define RX_BUFFER_SIZE 2048	
extern uint8_t rx_buffer[RX_BUFFER_SIZE]; 
extern uint16_t rx_index; 
extern uint8_t rx_buffer_temp[RX_BUFFER_SIZE];
extern uint16_t rx_index_temp;
extern uint32_t last_uart3_time_Rx; // thoi diem lan gan nhat uart3 truyen toi mcu

void UART_RxCallback_Process();

#endif /*_UART_H_*/