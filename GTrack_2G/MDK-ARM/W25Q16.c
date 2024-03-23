#include "main.h"
#include "W25Q16.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_spi.h"
extern SPI_HandleTypeDef hspi1;
 uint16_t writeSectorIndex=8 ; // bien dem hien tai de ghi
 uint16_t readSectorIndex=0 ;  // bien dem hien tai de doc
 uint8_t loop_Write_Index=0; // Biến này tăng lên mỗi khi writeSectorIndex vòng lại từ 510 về 0
 uint8_t loop_Read_Index=0; // Biến này tăng lên mỗi khi writeSectorIndex vòng lại từ 510 về 0
#define W25Q_SPI hspi1

#define csLow()   HAL_GPIO_WritePin(GPIOA, CS1_Pin, GPIO_PIN_RESET); // KÃ­ch hoáº¡t chÃ¢n SS (Slave Select) cá»§a W25Q16
#define csHigh() HAL_GPIO_WritePin(GPIOA, CS1_Pin, GPIO_PIN_SET);

#define numBlOCK 32 // number of total blocks for 16mb flash, 32x16X16 pagex 256 bytes
void SPI_Write(uint8_t *data, uint8_t len)
{
		HAL_SPI_Transmit_IT(&hspi1, data, len);
}
void SPI_Read(uint8_t *data, uint32_t len)
{
		HAL_SPI_Receive_IT(&hspi1, data, len);
}
void W25Q16_Reset(void)
{
    uint8_t data[2];
    data[0] = 0x66; // Enable Reset command for W25Q
    data[1] = 0x99; // Reset command for W25Q

    // Pull the CS (Chip Select) line low to begin the SPI transaction
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // Assuming PA4 is the CS pin

    // Transmit the reset commands
    HAL_SPI_Transmit_IT(&hspi1, data, 2); // Assuming hspi1 is the SPI handle

    // Pull the CS line high to end the SPI transaction
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    // Delay to ensure the reset command has time to process
    HAL_Delay(100); // Delay 100 ms, adjust as needed
}
uint32_t W25Q16_ReadID(void)
{
	uint8_t tData = 0x9F;
	uint8_t rData[3];
  // Gá»­i lá»‡nh JEDEC-ID Read (0x9F)
  HAL_GPIO_WritePin(GPIOA, CS1_Pin, GPIO_PIN_RESET); // KÃ­ch hoáº¡t chÃ¢n SS (Slave Select) cá»§a W25Q16


  // Gá»­i lá»‡nh qua SPI vÃ  nháº­n JEDEC ID cÃ¹ng lÃºc
 // HAL_SPI_TransmitReceive_IT(&hspi1, command, receiveBuffer, sizeof(command));
	HAL_SPI_Transmit_IT(&hspi1,&tData,1);
	HAL_SPI_Receive_IT(&hspi1,&rData,3);


  HAL_GPIO_WritePin(GPIOA, CS1_Pin, GPIO_PIN_SET); // VÃ´ hiá»‡u hÃ³a chÃ¢n SS (Slave Select)
	return ((rData[0]<<16)|(rData[1]<<8)|(rData[2]));
}
void W25Q_Read( uint32_t startSector, uint8_t offset, uint32_t size, uint8_t *rData)
{
		uint8_t tData[5];
		uint32_t memAddr = (startSector*256*16) +offset;
		if( numBlOCK<512) // size <512mb 
			{
		tData[0]= 0x03; //enable Read
		tData[1]= (memAddr>>16)& 0xFF;// MSB of the memory Address
		tData[2]= (memAddr>>8)& 0xFF;
		tData[3]= (memAddr>>0)& 0xFF; // LSB of the memory address
} else 
	{
		tData[0] = 0x13;  // Read Data with 4-Byte Address
		tData[1]= (memAddr>>24)& 0xFF;// MSB of the memory Address
		tData[2]= (memAddr>>16)& 0xFF;
		tData[3]= (memAddr>>8)& 0xFF;
		tData[4]= (memAddr>>0)& 0xFF; // LSB of the memory address
		}
	HAL_GPIO_WritePin(GPIOA, CS1_Pin, GPIO_PIN_RESET);
	if( numBlOCK<512) {
		HAL_SPI_Transmit_IT(&hspi1, tData, 4);
		} 
	else {
			HAL_SPI_Transmit_IT(&hspi1, tData, 5);
		}
	HAL_SPI_Receive_IT(&hspi1, rData, size);
	HAL_GPIO_WritePin(GPIOA, CS1_Pin, GPIO_PIN_SET);
}
void W25Q16_FastRead( uint32_t startSector, uint8_t offset, uint32_t size, uint8_t *rData)
{
		uint8_t tData[6];
		uint32_t memAddr = (startSector*256*16) +offset;
		if( numBlOCK<512) // size <512mb 
			{
		tData[0]= 0x0B; //enable Fast Read
		tData[1]= (memAddr>>16)& 0xFF;// MSB of the memory Address
		tData[2]= (memAddr>>8)& 0xFF;
		tData[3]= (memAddr>>0)& 0xFF; // LSB of the memory address
		tData[4]=0; // dummy clock
} else 
	{
		tData[0] = 0x0C;  // Fast Read with 4-Byte Address
		tData[1]= (memAddr>>24)& 0xFF;// MSB of the memory Address
		tData[2]= (memAddr>>16)& 0xFF;
		tData[3]= (memAddr>>8)& 0xFF;
		tData[4]= (memAddr>>0)& 0xFF; // LSB of the memory address
		tData[5]=0; // dummy clock

		}
	csLow();  // pull the CS Low
	if( numBlOCK<512) {
		HAL_SPI_Transmit_IT(&hspi1, tData, 5);
		} 
	else {
		HAL_SPI_Transmit_IT(&hspi1, tData, 6);
		}
	HAL_SPI_Receive_IT(&hspi1, rData, size);
	csHigh();
}
void write_enable(void) 
{
	uint8_t tData=0x06; // enable write
	csLow();
	SPI_Write(&tData, 1);
	csHigh();
	HAL_Delay(5);
}
void write_disable(void) 
{
	uint8_t tData=0x04; // disable write
	csLow();
	SPI_Write(&tData, 1);
	csHigh();
	HAL_Delay(5);

}
void W25Q16_Erase_Sector(uint16_t numsector)
{
    uint8_t tData[6];
    uint32_t memAddr = numsector * (16 * 256); // Má»—i sector cÃ³ 16 trang * 256 byte

    write_enable(); // Cho phÃ©p ghi dá»¯ liá»‡u vÃ o chip

    if (numBlOCK<512) // Náº¿u numBLOCK512 Ä‘Æ°á»£c Ä‘á»‹nh nghÄ©a á»Ÿ Ä‘Ã¢u Ä‘Ã³ trong mÃ£ nguá»“n cá»§a báº¡n
    {
        tData[0] = 0x20; // Lá»‡nh xÃ³a sector
        tData[1] = (memAddr >> 16) & 0xFF; // MSB cá»§a Ä‘á»‹a chá»‰ memory
        tData[2] = (memAddr >> 8) & 0xFF;
        tData[3] = memAddr & 0xFF; // LSB cá»§a Ä‘á»‹a chá»‰ memory
        csLow();  
				SPI_Write(tData, 4); // Gá»­i lá»‡nh xÃ³a Ä‘áº¿n chip qua SPI
        csHigh();
    }
    else
    {
        tData[0] = 0x21; // Lá»‡nh xÃ³a sector
        tData[1] = (memAddr >> 24) & 0xFF; // MSB cá»§a Ä‘á»‹a chá»‰ memory
        tData[2] = (memAddr >> 16) & 0xFF;
        tData[3] = (memAddr >> 8) & 0xFF;
        tData[4] = memAddr & 0xFF; // LSB cá»§a Ä‘á»‹a chá»‰ memory
        csLow();    
				SPI_Write(tData, 4); // Gá»­i lá»‡nh xÃ³a Ä‘áº¿n chip qua SPI
				csHigh(); 
		}
		HAL_Delay(450);
		write_disable();
}
uint32_t bytestowrite(uint32_t size, uint16_t offset)
{
	if ((size+offset)<256) return size;
	else return 256-offset;
}
uint32_t bytestomodify(uint32_t size, uint16_t offset)
{
	if ((size+offset)<4096) return size;
	else return 4096 -offset;
}
void W25Q16_Write_Sector(uint32_t sector, uint16_t offset, uint32_t size, uint8_t *data) {
    uint8_t tData[256]; // Tăng kích thước buffer nếu cần
    uint32_t memAddr = sector * 4096 + offset; // Tính địa chỉ bộ nhớ dựa trên sector

    // Xóa sector trước khi ghi
    W25Q16_Erase_Sector(sector); // Giả định đã có hàm này

    // Chuẩn bị dữ liệu để ghi vào sector
    if (numBlOCK < 512) { // Nếu Chip Size < 512Mb, sử dụng địa chỉ 3-byte
        tData[0] = 0x02; // Lệnh ghi trang
        tData[1] = (memAddr >> 16) & 0xFF; // MSB của địa chỉ bộ nhớ
        tData[2] = (memAddr >> 8) & 0xFF;
        tData[3] = memAddr & 0xFF; // LSB của địa chỉ bộ nhớ
    } else { // Đối với chip từ 512Mb trở lên, sử dụng địa chỉ 4-byte
        tData[0] = 0x12; // Lệnh ghi trang với địa chỉ 4-byte
        tData[1] = (memAddr >> 24) & 0xFF; // MSB của địa chỉ bộ nhớ
        tData[2] = (memAddr >> 16) & 0xFF;
        tData[3] = (memAddr >> 8) & 0xFF;
        tData[4] = memAddr & 0xFF; // LSB của địa chỉ bộ nhớ
    }

    // Ghi dữ liệu
    uint16_t bytesToWrite = (size > 4096) ? 4096 : size; // Không ghi quá kích thước sector
    for (uint16_t i = 0; i < bytesToWrite; i++) {
        tData[4 + i] = data[i]; // Điều chỉnh nếu cần để phản ánh đúng vị trí dữ liệu
    }

    write_enable(); // Cho phép ghi
    csLow(); // Kích hoạt truyền thông SPI
    SPI_Write(tData, 4 + bytesToWrite); // Gửi lệnh và dữ liệu
    csHigh(); // Ngừng truyền thông SPI
		HAL_Delay(5); // Chờ cho đến khi ghi hoàn tất
    write_disable(); // Vô hiệu hóa ghi
}


void W25Q16_Write_Clean (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)
{
	uint8_t tData[266];
	uint32_t startPage = page;
	uint32_t endPage  = startPage + ((size+offset-1)/256);
	uint32_t numPages = endPage-startPage+1;

	uint16_t startSector  = startPage/16;
	uint16_t endSector  = endPage/16;
	uint16_t numSectors = endSector-startSector+1;
	for (uint16_t i=0; i<numSectors; i++)
	{
		W25Q16_Erase_Sector(startSector++);
	}
	uint32_t dataPosition = 0;

	// write the data
	for (uint32_t i=0; i<numPages; i++)
	{
		uint32_t memAddr = (startPage*256)+offset;
		uint16_t bytesremaining  = bytestowrite(size, offset);
		uint32_t indx = 0;
		write_enable();

		if (numBlOCK<512)   // Chip Size<256Mb
		{
			tData[0] = 0x02;  // page program
			tData[1] = (memAddr>>16)&0xFF;  // MSB of the memory Address
			tData[2] = (memAddr>>8)&0xFF;
			tData[3] = (memAddr)&0xFF; // LSB of the memory Address

			indx = 4;
		}
		else
		{
			tData[0] = 0x12;  // page program with 4-Byte Address
			tData[1] = (memAddr>>24)&0xFF;  // MSB of the memory Address
			tData[2] = (memAddr>>16)&0xFF;
			tData[3] = (memAddr>>8)&0xFF;
			tData[4] = (memAddr)&0xFF; // LSB of the memory Address

			indx = 5;
		}
		uint16_t bytestosend  = bytesremaining + indx;

		for (uint16_t i=0; i<bytesremaining; i++)
		{
			tData[indx++] = data[i+dataPosition];
		}
	if(bytestosend >250)
		{
		csLow();
		SPI_Write(tData, 100);
		SPI_Write(tData+100,bytestosend- 100);	
		csHigh();
			} else 
		{
		csLow();
		SPI_Write(tData,bytestosend);	
		csHigh();
			}
			
		startPage++;
		offset = 0;
		size = size-bytesremaining;
		dataPosition = dataPosition+bytesremaining;
		HAL_Delay(5);
		write_disable();
	}

}



void W25Q16_Write (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)
{
	uint16_t startSector  = page/16;
	uint16_t endSector  = (page + ((size+offset-1)/256))/16;
	uint16_t numSectors = endSector-startSector+1;

	uint8_t previousData[4096];

	uint32_t sectorOffset = ((page%16)*256)+offset;
	uint32_t dataindx = 0;
	for (uint16_t i=0; i<numSectors; i++)
	{
		uint32_t startPage = startSector*16;
		W25Q16_FastRead(startPage, 0, 2048, previousData);
	
		uint16_t bytesRemaining = bytestomodify(size, sectorOffset);
		for (uint16_t i=0; i<bytesRemaining; i++)
		{
			previousData[i+sectorOffset] = data[i+dataindx];
		}
				W25Q16_Write_Clean(startPage, 0, 4096, previousData);
		startSector++;
		sectorOffset = 0;
		dataindx = dataindx+bytesRemaining;
		size = size-bytesRemaining;
	}
}
uint8_t W25Q16_Read_Byte( uint32_t Addr)
{
	uint8_t tData[5];
	uint8_t rData;

		if( numBlOCK<512) // size <512mb 
			{
		tData[0]= 0x03; //enable Read
		tData[1]= (Addr>>16)& 0xFF;// MSB of the memory Address
		tData[2]= (Addr>>8)& 0xFF;
		tData[3]= (Addr>>0)& 0xFF; // LSB of the memory address
} else 
	{
		tData[0]= 0x13; //enable Read
		tData[1]= (Addr>>24)& 0xFF;// MSB of the memory Address
		tData[2]= (Addr>>16)& 0xFF;
		tData[3]= (Addr>>8)& 0xFF;
		tData[4]= (Addr>>0)& 0xFF; // LSB of the memory address
		}
	csLow();
	if( numBlOCK<512) {
		SPI_Write(tData,4);
		} 
	else {
		SPI_Write(tData,5);
		}
	SPI_Read(&rData,1);// read the data
		csHigh();
		
		return rData;
	}
void W25Q16_Write_Byte_to_Sector_Start(uint32_t sector, uint8_t data) {
    uint8_t tData[6];
    uint8_t indx;

    // Tính địa chỉ của byte đầu tiên trong sector
    uint32_t Addr = sector * 4096; // 4096 bytes mỗi sector

    // Xóa sector trước khi ghi
    W25Q16_Erase_Sector(sector);

    if (numBlOCK < 512) { // Chip Size < 256Mb, sử dụng địa chỉ 3-byte
        tData[0] = 0x02; // page program
        tData[1] = (Addr >> 16) & 0xFF; // MSB of the memory Address
        tData[2] = (Addr >> 8) & 0xFF;
        tData[3] = (Addr) & 0xFF; // LSB of the memory Address
        tData[4] = data; // Dữ liệu cần ghi
        indx = 5;
    } else { // Đối với chips >= 256Mb, sử dụng địa chỉ 4-byte
        tData[0] = 0x12; // Write Data with 4-Byte Address
        tData[1] = (Addr >> 24) & 0xFF; // MSB of the memory Address
        tData[2] = (Addr >> 16) & 0xFF;
        tData[3] = (Addr >> 8) & 0xFF;
        tData[4] = (Addr) & 0xFF; // LSB of the memory Address
        tData[5] = data; // Dữ liệu cần ghi
        indx = 6;
    }

    write_enable(); // Kích hoạt trạng thái ghi
    csLow(); // Kích hoạt truyền thông SPI
    SPI_Write(tData, indx); // Ghi lệnh và dữ liệu
    csHigh(); // Ngừng truyền thông SPI

    HAL_Delay(5); // Đợi ghi hoàn tất
    write_disable(); // Vô hiệu hóa trạng thái ghi
}

//void W25Q_Write(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data) {
//    uint32_t sectorNumber = page / 16; // Calculate the sector number
//    uint32_t sectorStartPage = sectorNumber * 16; // Calculate the starting page of the sector
//    uint32_t sectorOffset = (page % 16) * 256 + offset; // Calculate the offset within the sector
//    uint8_t sectorData[4096]; // Buffer to hold the entire sector's data

//    // Read the entire sector into the buffer
//    W25Q16_FastRead(sectorStartPage, 0, 4096, sectorData);

//    // Update the sector data with the new data
//    for (uint32_t i = 0; i < size; i++) {
//        sectorData[sectorOffset + i] = data[i];
//    }

//    // Erase the sector
//    W25Q16_Erase_Sector(sectorNumber);

//    // Write the updated data back to the sector, page by page
//    for (uint32_t i = 0; i < 16; i++) {
//        uint32_t pageAddress = sectorStartPage + i;
//        W25Q16_Write_Page(pageAddress, 0, 256, &sectorData[i * 256]);
//    }
//}


void FIFO_Write(uint8_t *data, uint16_t size) {
   
   if (size > BUFFER_SIZE) {
       size = BUFFER_SIZE;
   }

   
   uint32_t startAddress = writeSectorIndex ;

   
   W25Q16_Erase_Sector(writeSectorIndex);

   
    W25Q16_Write_Sector(startAddress,0, size, data);

  
   writeSectorIndex++;
   if (writeSectorIndex >= TOTAL_SECTORS) {
       writeSectorIndex = 0;
			loop_Write_Index++;
	
   }
}
void FIFO_Read(uint8_t *data, uint16_t size) {
   if (size > BUFFER_SIZE) {
       size = BUFFER_SIZE;
   }

   uint32_t startAddress = readSectorIndex ;

   W25Q16_FastRead(startAddress,0, size, data);

   readSectorIndex++;
   if (readSectorIndex >= TOTAL_SECTORS) {
       readSectorIndex = 0;
		 loop_Read_Index++;
   }
	 if ((loop_Write_Index<=loop_Read_Index)==1) { 
		loop_Write_Index=0;
		loop_Read_Index=0;
	
	 }
}

void W25Q16_DeepPowerDown(void) {
    uint8_t cmd = 0xB9; // Lệnh Deep Power-down
    csLow();
    SPI_Write(&cmd, 1);
    csHigh();
}
void W25Q16_ReleaseFromDeepPowerDown(void) {
    uint8_t cmd = 0xAB; // Lệnh Release from Deep Power-down
    csLow();
    SPI_Write(&cmd, 1);
    csHigh();
    HAL_Delay(3); // Thời gian chờ sau khi đánh thức là khoảng 3ms
}
void SaveWriteSectorIndexToFlash(void) {
    uint8_t dataToWrite[2]; // Chỉ cần 2 byte cho writeSectorIndex

    // Chuyển đổi writeSectorIndex thành mảng byte
    dataToWrite[0] = writeSectorIndex >> 8;
    dataToWrite[1] = writeSectorIndex & 0xFF;

    // Ghi vào một sector cố định, ví dụ: sector cuối cùng - 1
   // W25Q16_Write_Sector(SECTORFORwriteSectorIndex, 0, sizeof(dataToWrite), dataToWrite);
}
void LoadWriteSectorIndexFromFlash(void) {
    uint8_t dataRead[2]; // Chỉ cần 2 byte

    // Đọc từ cùng một sector
    W25Q16_FastRead(SECTORFORwriteSectorIndex, 0, sizeof(dataRead),& dataRead);

    // Chuyển đổi trở lại thành writeSectorIndex
    writeSectorIndex = (dataRead[0] << 8) | dataRead[1];
}
void SaveReadSectorIndexToFlash(void) {
    uint8_t dataToWrite[2]; // Chỉ cần 2 byte cho readSectorIndex

    // Chuyển đổi readSectorIndex thành mảng byte
    dataToWrite[0] = readSectorIndex >> 8;
    dataToWrite[1] = readSectorIndex & 0xFF;
   // W25Q16_Write_Sector(SECTORFORreadSectorIndex, 0, sizeof(dataToWrite), dataToWrite);
}
void LoadReadSectorIndexFromFlash(void) {
    uint8_t dataRead[2]; // Chỉ cần 2 byte

    // Đọc từ cùng một sector
    W25Q16_FastRead(SECTORFORreadSectorIndex, 0, sizeof(dataRead),& dataRead);

    // Chuyển đổi trở lại thành readSectorIndex
    readSectorIndex = (dataRead[0] << 8) | dataRead[1];
	
}
void Save_SECTORFORloop_Write_Index(void) {
    uint8_t dataToWrite[2]; // Chỉ cần 2 byte cho writeSectorIndex



    // Ghi vào một sector cố định, ví dụ: sector cuối cùng - 1
    W25Q16_Write_Byte_to_Sector_Start(SECTORFORloop_Write_Index, loop_Write_Index);

}
void Load_SECTORFORloop_Write_Index(void) {

    // Đọc từ cùng một sector
    W25Q16_FastRead(SECTORFORloop_Write_Index, 0,1, &loop_Write_Index);


   
}
void Save_SECTORFORloop_Read_Index(void) {

    // Ghi vào một sector cố định, ví dụ: sector cuối cùng - 1
    W25Q16_Write_Byte_to_Sector_Start(SECTORFORloop_Read_Index, loop_Read_Index);
}
void Load_SECTORFORloop_Read_Index(void) {

    W25Q16_FastRead(SECTORFORloop_Read_Index, 0,1, &loop_Read_Index);

   
}
