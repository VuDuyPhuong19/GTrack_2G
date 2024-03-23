#include "MQTT.h"
#include "main.h"
#define NUMBER_OF_PUB_BUFFER 992

uint8_t isPublish = 1;
//uint8_t isPublishforLowPower =1;
uint8_t pub_buffer[NUMBER_OF_PUB_BUFFER];
uint8_t pub_buffer_Flash[NUMBER_OF_PUB_BUFFER];

uint16_t pub_buffer_index = 0;
uint16_t pub_buffer_Flash_index = 0;

char buffer[16];
void MQTT_Server_Open(){
	sendCommandToMC60("AT+QMTOPEN=0,\"demo.thingsboard.io\",1883\r\n");
}

void MQTT_Server_Conn(){
	sendCommandToMC60("AT+QMTCONN=0,\"demo.thingsboard.io\",\"JK5BGn0bQCr9PJM3EXq0\"\r\n");
}
// JK5BGn0bQCr9PJM3EXq0
//bt2lJCRQsLBAq9E9m0IR
//EY170L0YV1Ds2RnDvyoA

void publishHandle(uint8_t *rxbuffer) {
    size_t rxbuffer_length = 34; // 
    
    if (pub_buffer_index + rxbuffer_length + 1 + 1 > NUMBER_OF_PUB_BUFFER) { 
				W25Q16_Write_Byte_to_Sector_Start(COUNTRESETFROMCREG,0);
       //	SaveReadSectorIndexToFlash() ; // cap nhat WriteSectorIndex
				//SaveWriteSectorIndexToFlash() ; // cap nhat WriteSectorIndex
//				Save_SECTORFORloop_Read_Index();
//				Save_SECTORFORloop_Write_Index();
				pub_buffer[pub_buffer_index++] = '\'';
        pub_buffer[pub_buffer_index] = '}';
        pub_buffer_index++;

        // G?i d? li?u trong pub_buffer
        ATCommand_Send("AT+QMTPUB=0,0,0,0,\"v1/devices/me/telemetry\",989\r\n");
        HAL_Delay(200);
        HAL_UART_Transmit(&huart3, pub_buffer, 989, HAL_MAX_DELAY);
        HAL_Delay(200);

        // Debug g?i d? li?u ra UART1
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, pub_buffer, 989, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_Delay(200);

        pub_buffer_index = 0;
        isPublish = 1;
				//isPublishforLowPower=1;
			 // Kiểm tra kết quả publish
	
//        if (publish_success) {
//				publish_success==0;
//         //publishLedSuccess();
//				// Debug g?i d? li?u ra UART1
//        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
//        HAL_UART_Transmit(&huart1, " pub thanh cong ", sizeof(" pub thanh cong "), HAL_MAX_DELAY);
//        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
// 
//				
//						Pub_Data_From_Flash();
//					
//						
//        } else {
//				
//				HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
//        HAL_UART_Transmit(&huart1, " pub khong thanh cong ", sizeof(" pub khong thanh cong "), HAL_MAX_DELAY);
//        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
//					
//			WriteDataGPStoFLASH();
//					

//        }
				
    }
		
    
    if (isPublish) {
        // Kh?i t?o l?i pub_buffer v?i chu?i m? d?u
        strcpy(pub_buffer, "{data:'");
        pub_buffer_index = strlen("{data:'");
        isPublish = 0;
    }
    
  
    if (pub_buffer_index > 0 && pub_buffer[pub_buffer_index - 1] == '}') {
        
        pub_buffer_index--;
    }

    
    memcpy(pub_buffer + pub_buffer_index, rxbuffer, rxbuffer_length);
    pub_buffer_index += rxbuffer_length;

    
    pub_buffer[pub_buffer_index++] = '\n';
}

void MQTT_Server_Pub(){
    sendCommandToMC60("AT+QGNSSRD?\r\n");
    if (rmc_data != NULL) {
        strcpy(rxbuffer_temporary, rmc_data);
        decodeRMC(rxbuffer_temporary, &rmc);
        free(rmc_data);
    }
    char* timedatelatlong_str = timedatelatlong_data_format(&rmc);
		sprintf ((char*)currentKnownLocation, timedatelatlong_str);
		//if (strstr(timedatelatlong_str, "00000")||strstr(timedatelatlong_str, "7853,")){ 		HAL_IWDG_Refresh(&hiwdg);	}

		//	else{	
		publishHandle(timedatelatlong_str); 
		//	}
//				
				
				free(timedatelatlong_str);
	}

void MQTT_Server_Disc(){
	ATCommand_Send("AT+QMTDISC=0\r\n");
}

// MQTT process
void MQTT_Process(){
		MQTT_Server_Open();
		MQTT_Server_Conn();
	//JK5BGn0bQCr9PJM3EXq0 //server Do
	//bt2lJCRQsLBAq9E9m0IR // server Hung
	// Server Phuong
		GNSS_Power_On();
		HAL_Delay(4000);
		while(1){
			
			sendCommandToMC60("AT+QMTPUB=0,0,0,0,\"v1\/devices\/me\/telemetry\",31\r\n");
			if((HAL_GetTick()- last_uart3_time_Rx)> TIME_OUT_UART3){
					HAL_UART_Transmit(&huart1, "n\n check time out \n\n", sizeof ("n\n check time out \n\n"), HAL_MAX_DELAY); 
					NVIC_SystemReset();
			}
			HAL_IWDG_Refresh(&hiwdg);
		if ( ((HAL_GetTick() - lastMotionTime) > INACTIVITY_THRESHOLD)   )  //&& (isPublishforLowPower==1)
				{		W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeup_From_Ext, 0);// neu wakeup tu watchdog thi  wakeup_From_Ex=0//
						HAL_Delay(50);
						HAL_UART_Transmit(&huart1, "stop mode ne \n\n", sizeof ("stop mode ne \n\n"), HAL_MAX_DELAY); 
						Entry_StopMode();
						// neu wakeup tu EXT thi  wakeup_From_Ex=1//
						if ( wakeup_From_Ext ==1) {	
							W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeup_From_Ext, 1);
						}
						//Exit_StopMode();
						HAL_UART_Transmit(&huart1, "wake up vcl \n\n", sizeof ("ao ma vcl \n\n"), HAL_MAX_DELAY); 
						
						
			}
		//	isPublishforLowPower=0;
		}
		W25Q16_Write_Byte_to_Sector_Start(COUNTRESETFROMCREG,0);
}
// Resolve MQTT publish error
void HandlePublishError(){
		MQTT_Server_Disc();
		sendCommandToMC60("AT+CREG?;+CGREG?\r\n"); 
		MQTT_Process();
}
// ghi gu lieu khong gui thanh cong vao flash 

void WriteDataGPStoFLASH() {
    size_t dataSize = 989; // Lấy kích thước thực tế của dữ liệu
    size_t bytesToWrite = 989; // Số byte cần ghi
    size_t offset = 0; // Vị trí bắt đầu ghi

    while (bytesToWrite > 0) {
        size_t chunkSize = (bytesToWrite < 250) ? bytesToWrite : 250; // Điều chỉnh để ghi tối đa 250 bytes mỗi lần
        FIFO_Write(pub_buffer + offset, chunkSize); // Ghi dữ liệu vào flash

        // Gửi thông báo ghi dữ liệu
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, " ghi vao du lieu ", strlen(" ghi vao du lieu "), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
			  HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, pub_buffer + offset, chunkSize, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_Delay(200);
			

        offset += chunkSize; // Cập nhật vị trí bắt đầu cho lần ghi tiếp theo
        bytesToWrite -= chunkSize; // Giảm số byte còn lại cần ghi

        HAL_IWDG_Refresh(&hiwdg); // Refresh watchdog timer để tránh reset do watchdog timeout
    }
}


void Pub_Data_From_Flash_test() {
    uint16_t totalBytesToRead = 989; // Tổng số byte cần đọc
    uint16_t bytesRead = 0; // Số byte đã đọc
    uint8_t tempBuffer[250]; // Điều chỉnh buffer tạm thời cho mỗi lần đọc xuống còn 250 byte
    pub_buffer_Flash_index = 0; // Reset index của buffer chính
while ((writeSectorIndex>readSectorIndex)||(loop_Write_Index>loop_Read_Index))
	{
    // Trong khi còn dữ liệu cần đọc và buffer chính chưa đầy
    while ((bytesRead < totalBytesToRead) && (pub_buffer_Flash_index < sizeof(pub_buffer_Flash))) {
        uint16_t bytesToRead = (totalBytesToRead - bytesRead > 250) ? 250 : (totalBytesToRead - bytesRead);
        FIFO_Read(tempBuffer, bytesToRead); // Đọc dữ liệu từ FIFO
        HAL_Delay(100);
        // Sao chép dữ liệu từ tempBuffer vào pub_buffer_Flash
        memcpy(pub_buffer_Flash + pub_buffer_Flash_index, tempBuffer, bytesToRead);
        pub_buffer_Flash_index += bytesToRead; // Cập nhật chỉ số của buffer chính
        bytesRead += bytesToRead; // Cập nhật số byte đã đọc
        
        // Optional: Gửi dữ liệu ra UART để debug
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "Lay du lieu tu flash: ", strlen("Lay du lieu tu flash: "), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, tempBuffer, bytesToRead, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
    }
		}
		HAL_Delay(1000);

    // Gửi dữ liệu đã đọc được qua UART
    ATCommand_Send("AT+QMTPUB=0,0,0,0,\"v1/devices/me/telemetry\",989\r\n");
    HAL_Delay(200);
    HAL_UART_Transmit(&huart3, pub_buffer_Flash, bytesRead, HAL_MAX_DELAY); // Sử dụng bytesRead thay vì 989 để gửi chính xác số byte đã đọc
    HAL_Delay(200);

    // Reset pub_buffer_Flash_index sau khi gửi dữ liệu
    pub_buffer_Flash_index = 0;
}

void Pub_Data_From_Flash() {
    uint16_t totalBytesToRead = 989; // Tổng số byte cần đọc
    uint16_t bytesRead = 0; // Số byte đã đọc
    uint8_t tempBuffer[250]; // Buffer tạm thời cho mỗi lần đọc

    // Reset buffer index
    pub_buffer_Flash_index = 0;

    // Kiểm tra điều kiện và đọc từ FIFO
    while (((writeSectorIndex > readSectorIndex) || (loop_Write_Index > loop_Read_Index)) && (bytesRead < totalBytesToRead)) {
        uint16_t bytesToRead = ((totalBytesToRead - bytesRead) > 250) ? 250 : (totalBytesToRead - bytesRead);
        FIFO_Read(tempBuffer, bytesToRead); // Đọc dữ liệu từ FIFO
        HAL_Delay(100);

        // Sao chép dữ liệu từ tempBuffer vào pub_buffer_Flash
        memcpy(pub_buffer_Flash + pub_buffer_Flash_index, tempBuffer, bytesToRead);
        pub_buffer_Flash_index += bytesToRead; // Cập nhật chỉ số của buffer chính
        bytesRead += bytesToRead; // Cập nhật số byte đã đọc

        // Optional: Gửi dữ liệu ra UART để debug
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "Lay du lieu tu flash: ", strlen("Lay du lieu tu flash: "), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, tempBuffer, bytesToRead, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);

        // Kiểm tra điều kiện dừng
        if (bytesRead >= totalBytesToRead) {
            break; // Thoát vòng lặp khi đã đọc đủ byte
        }
    }
		  // Debug g?i d? li?u ra UART1
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, pub_buffer_Flash, 989, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, "\n\n", 2, HAL_MAX_DELAY);
        HAL_Delay(200);

    // Gửi dữ liệu đã đọc được qua UART
    HAL_Delay(1000);
    ATCommand_Send("AT+QMTPUB=0,0,0,0,\"v1/devices/me/telemetry\",989\r\n");
    HAL_Delay(200);
    HAL_UART_Transmit(&huart3, pub_buffer_Flash, bytesRead, HAL_MAX_DELAY); // Sử dụng bytesRead để gửi số byte đã đọc thực tế
    HAL_Delay(200);

    // Reset pub_buffer_Flash_index sau khi gửi dữ liệu
    pub_buffer_Flash_index = 0;
}
