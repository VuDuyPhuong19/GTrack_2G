#include "StringProcessing.h"
#include "main.h"
uint8_t creg_flag = 3;
uint8_t open_flag = 3;
uint8_t con_flag =3;
uint8_t count_creg_resetMCU =0 ;
uint8_t VerifyResponseContent(char* response, const char* expectedResponse){
		if (strstr(response,expectedResponse) != NULL) return 1;
		else return 0;
}

void HandleErrorRx(const char* ATCommand){
				if (strstr(ATCommand,"AT+QMTPUB=")){
						//ErrorPublishLed();
						HandlePublishError();
						return;
				}
				else if(strstr(ATCommand,"GNSS")){
						return;
				}
				else if(strstr(ATCommand,"OPEN")){
						open_flag--;
						if(open_flag > 0){
							sendCommandToMC60(ATCommand);
						}
						else {
							open_flag = 3;
							sendCommandToMC60("AT+CREG?;+CGREG?\r\n");
						}		
						return;									
				}
				else if(strstr(ATCommand,"CREG")){
						creg_flag--;
						if(creg_flag > 0){
							//Error2GLed();
							sendCommandToMC60(ATCommand);					
						}
						else {
							creg_flag = 3;
							W25Q16_FastRead(COUNTRESETFROMCREG,0,1,&count_creg_resetMCU);
							count_creg_resetMCU++;
							W25Q16_Write_Byte_to_Sector_Start(COUNTRESETFROMCREG,count_creg_resetMCU);

							NVIC_SystemReset();									
						}
						return ;
				}
				else if(strstr(ATCommand,"CONN")){
						con_flag--;
						if(con_flag > 0){
							sendCommandToMC60(ATCommand);
						}
						else {
							con_flag = 3;
							sendCommandToMC60("AT+CREG?;+CGREG?\r\n");
						}
						return ;
				}
				else{		
						sendCommandToMC60(ATCommand);
						return ;
				}
}

void rxProcess(char* response, const char* expectedResponse, const char* ATCommand) {
    if (receive_flag) {
        HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen((const char *)response), HAL_MAX_DELAY);
				if(strstr(ATCommand, "AT+QGNSSRD?")){
						rmc_data = get_sentence_by_header(response,"RMC");
				}
				else if(strstr(ATCommand, "AT+QMTPUB=")){
						if(strstr(response, expectedResponse)) {
								publish_success = 1;
						} else publish_success=0;
				}				
				uint8_t isValidResponse = VerifyResponseContent(response, expectedResponse);
        memset(response, '\0', RX_BUFFER_SIZE); // Làm s?ch buffer sau khi dã x? lý
        receive_flag = 0; // Reset flag
				rx_index_temp = 0; // Reset index of rx_buffer_temp
				if (!isValidResponse) {
						HandleErrorRx(ATCommand);
				}			
		}
}