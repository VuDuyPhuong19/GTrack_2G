#include "UART.h"

uint8_t rx_buffer[RX_BUFFER_SIZE]; 
uint16_t rx_index  = 0; 
uint8_t rx_buffer_temp[RX_BUFFER_SIZE];
uint16_t rx_index_temp = 0;
uint32_t last_uart3_time_Rx;

void UART_RxCallback_Process(){
				if (rx_buffer[rx_index] == '\n') {
            if (rx_index > 0 && rx_buffer[rx_index - 1] == '\r') {
                rx_buffer[rx_index - 1] = '\n'; 
								rx_buffer[rx_index] = '\0';
            }	else {
                rx_buffer[rx_index] = '\0';
            }
						memcpy(rx_buffer_temp + rx_index_temp, rx_buffer, strlen((const char *)rx_buffer));
						rx_index_temp += strlen((const char *)rx_buffer);
						rx_index = 0;	// Nhan du lieu moi				
            receive_flag = 1; 
        } else {
            rx_index++; 
						if (rx_index >= RX_BUFFER_SIZE - 1) {
						rx_index = 0; // Tránh tràn buffer
						}
        }
        HAL_UART_Receive_IT(&huart3, &rx_buffer[rx_index], 1); // San sang nhan ky tu tiep theo
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART3) {
        UART_RxCallback_Process();
				last_uart3_time_Rx = HAL_GetTick();
  }
}