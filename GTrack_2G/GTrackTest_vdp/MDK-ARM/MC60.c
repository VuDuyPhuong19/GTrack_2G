#include "MC60.h"

uint8_t GNSS_ON[]="AT+QGNSSC";
uint8_t	rxbuffer[100];
uint8_t rxbuffer_NMEA[700];

// Turn on MC60
void MC60_PowerOn(){
	HAL_GPIO_WritePin(GPIOB, pb12_Pin, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, pb14_Pin, 1);	
	HAL_Delay(2000);
}
// Turn off MC60
void MC60_PowerOff(){
	HAL_GPIO_WritePin(GPIOB, pb14_Pin, 1);
	HAL_Delay(900);
	HAL_GPIO_WritePin(GPIOB, pb14_Pin, 0);	
	
	// Wait for the module to log out of the network
	HAL_Delay(12000);
	
	HAL_GPIO_WritePin(GPIOB, pb12_Pin, 1);	
}
// Send AT Command
void MC60_ATCommand_Send(UART_HandleTypeDef *huart, const char * ATCOM){
	HAL_UART_Transmit_IT(huart, (uint8_t*)ATCOM, strlen(ATCOM));
}
// Receive data from MC60
void MC60_Receive(UART_HandleTypeDef *huart, char* rx_buffer){
	HAL_UART_Receive_IT(huart, (uint8_t*)rxbuffer, sizeof(rxbuffer));
}
// Send AT Read Command
void MC60_ATCommand_Read(UART_HandleTypeDef * huart, const char * ATCOM){
	char ATCOM_Read[50];
	strcpy(ATCOM_Read, ATCOM);
	strcpy(ATCOM_Read, "?");
	strcpy(ATCOM_Read, "\r\n");
	HAL_UART_Transmit_IT(huart, (uint8_t*)ATCOM_Read, strlen(ATCOM_Read));
}
// Send AT Write Command
void MC60_ATCommand_Write(UART_HandleTypeDef * huart, const char * ATCOM,const char* parameter){
	char ATCOM_Write[50];
	strcpy(ATCOM_Write, ATCOM);
	strcpy(ATCOM_Write, "=");
	strcpy(ATCOM_Write, parameter);
	strcpy(ATCOM_Write, "\r\n");
	HAL_UART_Transmit_IT(huart, (uint8_t*)ATCOM_Write, strlen(ATCOM_Write));
}
// Turn on GNSS
void MC60_GNSS_On(UART_HandleTypeDef * huart){
	MC60_ATCommand_Write(huart, (uint8_t*)GNSS_ON, "1");
}

void MC60_View(UART_HandleTypeDef * huart){
	HAL_UART_Transmit_IT(huart, (uint8_t*)rxbuffer, sizeof(rxbuffer));
}

void MC60_Receive_NMEA(UART_HandleTypeDef * huart){
	MC60_ATCommand_Send(huart, "AT+QGNSSRD?\r\n");
	HAL_Delay(200);
	HAL_UART_Receive_IT(huart,(uint8_t *)rxbuffer_NMEA,sizeof(rxbuffer_NMEA));
}

void MC60_View_NMEA(UART_HandleTypeDef * huart){
	HAL_UART_Transmit_IT(huart, (uint8_t*)rxbuffer_NMEA, sizeof(rxbuffer_NMEA));
//	HAL_UART_Transmit_IT(huart, "123456\n", sizeof("123456\n"));
}

void MC60_Debug_test(UART_HandleTypeDef * huartz, const char * ATCOM){
	MC60_ATCommand_Send(huartz, ATCOM);
	HAL_Delay(200);
	HAL_UART_Receive_IT(huartz,(uint8_t *)rxbuffer,sizeof(rxbuffer));
}

void MC60_test_view(UART_HandleTypeDef * huartz){
	HAL_UART_Transmit_IT(huartz,(uint8_t *)rxbuffer,sizeof(rxbuffer));
}

void MC60_Get_Sentence_By_Header(UART_HandleTypeDef * huart){
	HAL_UART_Transmit_IT(huart, (uint8_t*)get_sentence_by_header(rxbuffer_NMEA, "RMC"), strlen(get_sentence_by_header(rxbuffer_NMEA, "RMC")));
}


	