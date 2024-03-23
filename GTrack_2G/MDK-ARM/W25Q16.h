/* W25Q16.h
*/
#include <stdint.h>

#ifndef W25Q16_H
#define W25Q16_H
#define SECTOR_SIZE 4096 // Kích thước mỗi sector là 4KB
#define PAGE_IN_SECTOR 16 // Kích thước mỗi sector là 4KB
#define TOTAL_SECTORS 500 // Tổng số sectors
#define BUFFER_SIZE 256   // Kích thước buffer
extern uint16_t writeSectorIndex ; // bien dem hien tai de ghi
extern uint16_t readSectorIndex ;  // bien dem hien tai de doc
extern uint8_t loop_Write_Index; // Biến này tăng lên mỗi khi writeSectorIndex vòng lại từ TOTAL_SECTORS về 0
extern uint8_t loop_Read_Index ; // Biến này tăng lên mỗi khi writeSectorIndex vòng lại từ TOTAL_SECTORS về 0


void W25Q16_Reset(void);
uint32_t W25Q16_ReadID(void);
void W25Q16_Read( uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData);
void W25Q16_FastRead( uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData);
void W25Q16_Erase_Sector(uint16_t numsector);
void W25Q16_Write_Sector(uint32_t sector, uint16_t offset, uint32_t size, uint8_t *data) ;
void W25Q16_Write(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);
void FIFO_Write(uint8_t *data, uint16_t size);
void FIFO_Read(uint8_t *data, uint16_t size);
void W25Q16_Write_Byte_to_Sector_Start(uint32_t sector, uint8_t data);
void W25Q16_DeepPowerDown(void);
void W25Q16_ReleaseFromDeepPowerDown(void);
//void SaveWriteSectorIndexToFlash(void) ;
//void LoadWriteSectorIndexFromFlash(void);
//void SaveReadSectorIndexToFlash(void) ;;
//void LoadReadSectorIndexFromFlash(void) ;
//void Save_SECTORFORloop_Write_Index(void);
//void Load_SECTORFORloop_Write_Index(void);
//void Save_SECTORFORloop_Read_Index(void);
//void Load_SECTORFORloop_Read_Index(void);
#endif 