#ifndef _MODE_LIGHT_INDICATOR_H_
#define _MODE_LIGHT_INDICATOR_H_

#include "main.h"
//#include "Timer.h"

enum Status {Start, Restart, Connect, Publish, Error2G, ErrorPublish}; 

extern enum Status status;

void publishLedSuccess();

void publishLedError();

void StartLedStatus();

void RestartLedStatus();

void Error2GLed();

void ErrorPublishLed();

#endif