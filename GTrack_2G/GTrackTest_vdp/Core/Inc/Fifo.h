#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdint.h>

#define FIFO_SIZE_MAX 300

typedef struct {
	uint32_t PutI;  // index to put new
	uint32_t GetI;  // index of oldest
	uint32_t Size;
	uint8_t Fifo[FIFO_SIZE_MAX];
}Fifo_t;

// Initialize the Fifo
void Fifo_Init(Fifo_t* Fifo, uint32_t size);

// Put data into the Fifo
uint8_t Fifo_Put(Fifo_t* Fifo, char data);

// Get data from the Fifo
uint8_t Fifo_Get(Fifo_t* Fifo);

// Check if the Fifo is empty
uint8_t Fifo_isEmpty(Fifo_t* Fifo);

// Check if the Fifo is full
uint8_t Fifo_isFull(Fifo_t* Fifo);

#endif