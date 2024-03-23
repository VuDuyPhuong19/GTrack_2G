
#ifndef BMA253_H
#define BMA253_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"
#include <stdio.h>

#define BMA253_REG_ACC_X_LSB 0x02
#define BMA253_ADDRESS 0x18  // address sensor BMA253
#define PMU_RANGE_REG 0x0F   // address thanh ghi PMU_RANGE
#define BMA253_ADDRESS 0x18  // address sensor BMA253
#define PMU_LPW_REG 0x11     // address thanh ghi PMU_LPW
#define INT_MAP_0_REGISTER   0x19
#define INT_EN_0_REGISTER    0x16
#define THRESHOLD_REGISTER   0x24
#define DURATION_REGISTER    0x27

#define MAX_SIZE 2
#define MOTION_THRESHOLD 0.1 
//I2C_HandleTypeDef hi2c1;

typedef struct {
	int16_t x :9;
	int16_t y :9;
	int16_t z :9;
	uint8_t a ;
} Acc_Data;
typedef struct {
    float data[MAX_SIZE];
    int front;
    int rear;
} Queue;

void initQueue(Queue* queue) ;

int isFull(Queue* queue) ;

int isEmpty(Queue* queue) ;

void enqueue(Queue* queue, float item);

float dequeue(Queue* queue) ;

char motionDetection(Acc_Data Acc_BMA253);

int detectMotion(Queue* queue) ;

void configureBMA253(I2C_HandleTypeDef *hi2c);

void setPowerMode(I2C_HandleTypeDef *hi2c);

void configureInterrupts(I2C_HandleTypeDef *hi2c);

void  read_xyz_values(I2C_HandleTypeDef *hi2c, Acc_Data *Acc_BMA253);

void configureMotionDetection(I2C_HandleTypeDef *hi2c, uint8_t thresholdValue, uint8_t durationValue );

void BMA253_read_acc(UART_HandleTypeDef * huartz,I2C_HandleTypeDef *hi2c,Acc_Data *Acc_BMA253);
#endif