
#ifndef BMA253_H
#define BMA253_H
#include <stdint.h>
#include <stdbool.h>
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"

#define BMA253_ADDRESS (0x18<<1)  // address sensor BMA253
#define PMU_RANGE_REG 0x0F   // address thanh ghi PMU_RANGE
#define PMU_LPW_REG 0x11     // address thanh ghi PMU_LPW
#define INT_MAP_0_REGISTER   0x19
#define INT_MAP_1_REGISTER   0x1A

#define INT_EN_0_REGISTER    0x16
#define INT_EN_1_REGISTER    0x17
#define THRESHOLD_REGISTER   0x24
#define DURATION_REGISTER    0x27
#define BMA253_INT1_REG      0x23  // Register containing the threshold definition for low-g interrupt
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





void configureBMA253(I2C_HandleTypeDef *hi2c);

void setPowerMode(I2C_HandleTypeDef *hi2c);

void configureNoMotionInterrupt(I2C_HandleTypeDef *hi2c);

void read_xyz_values(I2C_HandleTypeDef *hi2c, Acc_Data *Acc_BMA253);

#endif