#include <stdint.h>
#include "BMA253.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"

void configureBMA253(I2C_HandleTypeDef *hi2c) {
  uint8_t data = 0x03;  // Gia tri da cau hinh pham vi gia toc 2g

  HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, PMU_RANGE_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}
void setPowerMode(I2C_HandleTypeDef *hi2c) {
  uint8_t regValue;
  
 //doc gia tri hien tai cua thanh ghi PMU_LPW
  HAL_I2C_Mem_Read(hi2c, BMA253_ADDRESS, PMU_LPW_REG, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);
  
  // Xoa bit 4-0 (Power Mode) c?a thanh ghi
  regValue &= 0xE0;
  
  // gia tri cho d? tiet kiem nang luong vao thanh ghi
  regValue |=  0x10; // tan so lay mau 1-31// 0x01- 0xFF
  
  // Ghi gia tri moi vao thanh ghi PMU_LPW
  HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, PMU_LPW_REG, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);
}

void checkConfiguration(I2C_HandleTypeDef *hi2c) {
    uint8_t rangeData;
    uint8_t powerModeData;

   
    HAL_I2C_Mem_Read(hi2c, BMA253_ADDRESS, PMU_RANGE_REG, I2C_MEMADD_SIZE_8BIT, &rangeData, 1, HAL_MAX_DELAY);
    // Ki?m tra gia tr? ph?m vi gia t?c
    if (rangeData == 0x03) {
        
        HAL_I2C_Mem_Read(hi2c, BMA253_ADDRESS, PMU_LPW_REG, I2C_MEMADD_SIZE_8BIT, &powerModeData, 1, HAL_MAX_DELAY);
        
        if ((powerModeData & 0x1F) == 0x08) {
            
        } else {
            
        }
    } else {
       
    }
}
// Ham d? c?u hinh ng?t cho c?m bi?n BMA253
void configureInterrupts(I2C_HandleTypeDef *hi2c)
{
    uint8_t regValue;

    // C?u hinh thanh ghi INT_MAP_0
    regValue = 0x00; 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_MAP_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);

    // C?u hinh thanh ghi INT_EN_0
    regValue = 0x00; 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_EN_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);

    // C?u hinh ch? d? Nh?n d?ng Hu?ng
    regValue = 0x40; // 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_MAP_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);
    regValue = 0x40; // 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_EN_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);

    
    regValue = 0x40; 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_MAP_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);
    regValue = 0x40; 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_EN_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);
		
    regValue = 0x40; 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_MAP_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);
    regValue = 0x40; 
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, INT_EN_0_REGISTER, I2C_MEMADD_SIZE_8BIT, &regValue, 1, HAL_MAX_DELAY);
}
void configureMotionDetection(I2C_HandleTypeDef *hi2c, uint8_t thresholdValue, uint8_t durationValue )
{
   // uint8_t thresholdValue = 10;  // gia tri nguong gia toc (0) uint8_t durationValue )
    //uint8_t durationValue = 5;    // Gia tri thoi gian (5)

    // Ghi gia tri nguong gia tốc vao thanh ghi THRESHOLD
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, THRESHOLD_REGISTER, I2C_MEMADD_SIZE_8BIT, &thresholdValue, 1, HAL_MAX_DELAY);

    // Ghi gia tr? th?i gian vao thanh ghi DURATION
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, DURATION_REGISTER, I2C_MEMADD_SIZE_8BIT, &durationValue, 1, HAL_MAX_DELAY);
}
void read_xyz_values(I2C_HandleTypeDef *hi2c, Acc_Data *Acc_BMA253)
{
  uint8_t data[6];
	int16_t x,y,z;
  
  // Kh?i d?ng giao th?c I2C d?c d? li?u t? c?m bi?n
  HAL_I2C_Mem_Read_IT(hi2c, 0x18 << 1, 0x02, 1, data, 6);
  
  // cho qu
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
  
  // chuyển đổi dữ liệu raw thanh cac số nguyen 16-bit
  x = ((int16_t)((data[1] << 8) | data[0]));
  y = ((int16_t)((data[3] << 8) | data[2]));
  z = ((int16_t)((data[5] << 8) | data[4]));
	
	Acc_BMA253->x =x/1024;
  Acc_BMA253->y =y/1024;
  Acc_BMA253->z =z/1024;
}
void initQueue(Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

int isFull(Queue* queue) {
    return (queue->rear == MAX_SIZE - 1);
}

int isEmpty(Queue* queue) {
    return (queue->front == -1 || queue->front > queue->rear);
}

void enqueue(Queue* queue, float item) {
    if (isFull(queue)) {
    
        return;
    }
    if (isEmpty(queue)) {
        queue->front = 0;
    }
    queue->rear++;
    queue->data[queue->rear] = item;
}

float dequeue(Queue* queue) {
    if (isEmpty(queue)) {
      
        return 0;
    }
    float item = queue->data[queue->front];
    queue->front++;
    return item;
}

int detectMotion(Queue* queue) {
    if (queue->rear - queue->front < 1) {
        return 0; // khong du du lieu
    }

    float gia_toc_toan_hop1 = queue->data[queue->front];
    float gia_toc_toan_hop2 = queue->data[queue->front + 1];
    float delta = gia_toc_toan_hop2 - gia_toc_toan_hop1;

    if (delta > 0.1) {
        return 1; // chuyen dong tang
    } else if (delta < -0.1) {
        return -1; // chuyen dong giam
    } else {
        return 0; // khong co chuyen dong
    }
}
char motionDetection(Acc_Data Acc_BMA253) {
    Queue queue;
    initQueue(&queue);

    //them gia toc vao queue
    enqueue(&queue, Acc_BMA253.a);
    enqueue(&queue, Acc_BMA253.a);

    // phat hien chuyen dong
    return (char) detectMotion(&queue);
   
}