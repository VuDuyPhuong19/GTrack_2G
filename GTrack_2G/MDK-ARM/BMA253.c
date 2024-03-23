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

void configureNoMotionInterrupt(I2C_HandleTypeDef *hi2c) {
    uint8_t data;

    // Select no-motion mode
	// Gia tri da cau hinh pham vi gia toc 2g
    data = 0x03;
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x0F, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Set 1kHz bandwidth
    data = 0x0F;
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x10, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Set lower mode
    data = 0x56;
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x11, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);


	 data = 0x01;// ngáº¯t nháº£y tá»« 0 lÃªn 1
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x20, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Set no-motion threshold
    //data = 0x10; // Example threshold, needs adjustment based on sensitivity required
		data = 0x19;// 70 in dec-4B  50dec-32
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x29, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Set no-motion duration
    //data = 0x0A; // Example duration, adjust based on the required stationary time
		data = 0xFF;
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x27, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
		data = 0x08; // Map no-motion interrupt to INT1
		HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x19, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
    // Enable no-motion interrupt
	 // Enable axes for no-motion detection
    data = 0x07; // Enable x, y, and z axes
    HAL_I2C_Mem_Write(hi2c, BMA253_ADDRESS, 0x18, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

	
}

void read_xyz_values(I2C_HandleTypeDef *hi2c, Acc_Data *Acc_BMA253)
{
  uint8_t data[6];
	int16_t x,y,z;
  
  // Kh?i d?ng giao th?c I2C d?c d? li?u t? c?m bi?n
  HAL_I2C_Mem_Read_IT(hi2c, 0x18 << 1, 0x02, 1, data, 6);
  
  // cho qu
  while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
  
  // chuyen cac du lieu raw thanh cac so nguyen 16-bit
  x = ((int16_t)((data[1] << 8) | data[0]));
  y = ((int16_t)((data[3] << 8) | data[2]));
  z = ((int16_t)((data[5] << 8) | data[4]));
	
	Acc_BMA253->x =x/1024;
  Acc_BMA253->y =y/1024;
  Acc_BMA253->z =z/1024;
}