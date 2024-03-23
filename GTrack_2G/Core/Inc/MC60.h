#ifndef _MC60_H_
#define _MC60_H_

#include "main.h"
#include <string.h>
#include "GNSS_data.h"
#include "stdio.h"
#include "../../MDK-ARM/W25Q16.h"
#include "ModeLightIndicator.h"
#include "MQTT.h"
#include "UART.h"
#include "StringProcessing.h"

extern uint8_t rxbuffer_temporary[100];
extern char* rmc_data;
extern RMC rmc;
extern uint8_t RxData[34];
extern uint8_t TxData[34];
extern uint8_t lastKnownLocation[34];
extern uint8_t currentKnownLocation[34];

// Turn on MC60
void MC60_PowerOn();
// Turn off MC60
void MC60_PowerOff();
// Send AT Command
void ATCommand_Send(const char * ATCOM);
// Send AT Command to MC60
void sendCommandToMC60(const char* ATCommand);
// Power of GNSS
void GNSS_Power_On();
// Power of GNSS 
void GNSS_Power_Off();
// Activate AGPS
void AGPS_On();

#endif