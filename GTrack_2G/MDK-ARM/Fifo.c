#include "Fifo.h"

// Initialize the Fifo
void Fifo_Init(Fifo_t* Fifo, uint32_t size){
	Fifo->PutI = Fifo->GetI = 0;
	Fifo->Size = size;
}

// Put data into the Fifo
uint8_t Fifo_Put(Fifo_t* Fifo, char data){
	if(Fifo_isFull(Fifo)) return 0;
	Fifo->Fifo[Fifo->PutI] = data;
	Fifo->PutI = (Fifo->PutI + 1) % Fifo->Size;
	return 1;
}

// Get data from the Fifo
uint8_t Fifo_Get(Fifo_t* Fifo){
	if(Fifo_isEmpty(Fifo)) return 0;
	char data = Fifo->Fifo[Fifo->GetI];
	Fifo->GetI = (Fifo->GetI + 1) % Fifo->Size;
	return (uint8_t)data;
}

// Check if the Fifo is empty
uint8_t Fifo_isEmpty(Fifo_t* Fifo){
	return (Fifo->GetI == Fifo->PutI);
}

// Check if the Fifo is full
uint8_t Fifo_isFull(Fifo_t* Fifo){
	return (((Fifo->PutI + 1) % Fifo->Size) == Fifo->GetI);
}