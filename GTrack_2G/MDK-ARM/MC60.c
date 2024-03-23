#include "MC60.h"
#include "W25Q16.h"

uint8_t rxbuffer_temporary[100];
char* rmc_data;
RMC rmc;
uint8_t RxData[34];
uint8_t TxData[34];
uint8_t lastKnownLocation[34]; // Giả sử có kích thước đủ lớn để chứa tọa độ
uint8_t currentKnownLocation[34] ;

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
void ATCommand_Send(const char * ATCOM){
	HAL_UART_Transmit(&huart3, (uint8_t *)ATCOM, strlen(ATCOM), HAL_MAX_DELAY);
}

char* expectedResponse(const char* ATCommand){
		char* expected_response = (char*)malloc(20);
		if (strstr(ATCommand, "AT+CPIN?")){
				strcpy(expected_response, "+CPIN: READY");
		}
		else if (strstr(ATCommand, "AT+CREG?;+CGREG?")){
				strcpy(expected_response, "+CGREG: 0,1");
		}
		else if (strstr(ATCommand, "AT+CREG?")){
				strcpy(expected_response, "+CREG: 0,2");
		}
		else if (strstr(ATCommand, "AT+QIFGCNT=2") || strstr(ATCommand, "AT+QICSGP=1") || strstr(ATCommand, "AT+QGNSSEPO=1") || strstr(ATCommand, "AT+QGNSSC=1")){
				strcpy(expected_response, "OK");
		}
		else if (strstr(ATCommand, "AT+QMTOPEN=")){
				strcpy(expected_response, "+QMTOPEN: 0,0");
		}
		else if (strstr(ATCommand, "AT+QMTCONN=")){
				strcpy(expected_response, "+QMTCONN: 0,0,0");
		}
		else if (strstr(ATCommand, "AT+QMTPUB=")){
				strcpy(expected_response, "+QMTPUB: 0,0,0");
			
		}		
		else if (strstr(ATCommand, "AT+QMTDISC=0")){
				strcpy(expected_response, "+CME");
		}
		else strcpy(expected_response, "OK");
		return expected_response;
}

void sendCommandToMC60(const char* ATCommand) {
		if(strstr(ATCommand, "AT+QMTPUB=")){
				MQTT_Server_Pub();
		}
		else {
    HAL_UART_Transmit(&huart3, (uint8_t*)ATCommand, strlen(ATCommand), HAL_MAX_DELAY); // G?i l?nh qua UART3
		}
		if(strstr(ATCommand, "AT+QMTCONN")|| strstr(ATCommand, "AT+CREG")){
				HAL_Delay(2000);
		}		
		else HAL_Delay(550);
		char* expected_response = expectedResponse(ATCommand);
		rxProcess(rx_buffer_temp, expected_response, ATCommand);
		free(expected_response);
}

void GNSS_Power_On(){
	sendCommandToMC60("AT+QGNSSC=1\r\n");
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB, pb13_Pin, 1);
}

void GNSS_Power_Off(){
	sendCommandToMC60("AT+QGNSSC=0\r\n");
}
// Activate AGPS
void AGPS_On(){
		sendCommandToMC60("AT+QIFGCNT=2\r\n");
		sendCommandToMC60("AT+QICSGP=1,\"v-internet\"\r\n");
		HAL_Delay(10000);
		sendCommandToMC60("AT+CREG?;+CGREG?\r\n");
		sendCommandToMC60("AT+QGNSSTS?\r\n");
		sendCommandToMC60("AT+QGREFLOC=21.003189,105.507098\r\n");
		sendCommandToMC60("AT+QGNSSEPO=1\r\n");
		HAL_Delay(2000);	
}