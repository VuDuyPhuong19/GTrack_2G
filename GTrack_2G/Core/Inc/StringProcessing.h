#ifndef _STRINGPROCESSING_H_
#define _STRINGPROCESSING_H_

#include "main.h"
#include "ModeLightIndicator.h"
#include <string.h>
#include "MC60.h"
#include "UART.h"
extern uint8_t count_creg_resetMCU ;

void rxProcess(char* response, const char* expectedResponse, const char* ATCommand);

#endif /*_STRINGPROCESSING_H_*/