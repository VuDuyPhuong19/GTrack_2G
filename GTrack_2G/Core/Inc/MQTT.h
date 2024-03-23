#ifndef _MQTT_H_
#define _MQTT_H_

#include "MC60.h"
#include "LowPowerMode.h"
#include "main.h"
#include "UART.h"

extern uint32_t lastMotionTime;
#define INACTIVITY_THRESHOLD  900000 // vao stopmode sau 15p
#define TIME_OUT_UART3 90000 // thoi gian timeout phat hien mc60 bi treo do mat song 
extern uint8_t isPublishforLowPower;

// Open server
void MQTT_Server_Open();
// Connect server
void MQTT_Server_Conn();
// Public message to the server 
void MQTT_Server_Pub();
// Disconnect server
void MQTT_Server_Disc();
// MQTT process
void MQTT_Process();
// Resolve MQTT publish error
void HandlePublishError();
void WriteDataGPStoFLASH();
void Pub_Data_From_Flash();
#endif /*_MQTT_H_*/