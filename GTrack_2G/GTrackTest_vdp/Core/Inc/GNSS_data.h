#ifndef _GNSS_DATA_H_
#define _GNSS_DATA_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char Time[7];
	char Lat_Long[25];
	char Date[7];
}RMC;


char* get_sentence_by_header(char* GNSS_data, const char* header);
// Decodes RMC data
void decodeRMC(char* RMCbuffer, RMC* rmc);
#endif