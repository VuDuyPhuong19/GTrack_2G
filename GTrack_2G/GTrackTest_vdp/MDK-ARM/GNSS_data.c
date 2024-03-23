#include "GNSS_data.h"

#define TIME_LENGTH 7
#define LAT_LONG_LENGTH 25
#define DATE_LENGTH 7

char* get_sentence_by_header(char* GNSS_data, const char* header) {
    char* token;
    char temp_data[strlen(GNSS_data) + 1];
    strcpy(temp_data, GNSS_data);

    // split string into lines
    token = strtok(temp_data, " \r\n");
    while (token != NULL) {
        if (strstr(token, header) != NULL) {
            char* temp = (char*)malloc((strlen(token) + 1) * sizeof(char)); // dynamic allocation for temp
            strcpy(temp, token);
            return temp;
        }
        token = strtok(NULL, " \r\n");
    }

    return NULL;  // return NULL if the line with the corresponding header is not found
}

void moveToNextComma(char* buffer, int* index) {
    while (buffer[(*index)] != ',') {
        (*index)++;
    }
    (*index)++;
}

void decodeRMC(char* RMCbuffer, RMC* rmc) {
    int buffer_index = 0;
    
    // Get Time
    moveToNextComma(RMCbuffer, &buffer_index);
    memset(rmc->Time, '\0', TIME_LENGTH);
    for (int j = 0; j < TIME_LENGTH - 1; j++) {
        rmc->Time[j] = RMCbuffer[buffer_index++];
    }
    
    // Get Latitude & Longitude
    moveToNextComma(RMCbuffer, &buffer_index);
    moveToNextComma(RMCbuffer, &buffer_index);
    memset(rmc->Lat_Long, '\0', LAT_LONG_LENGTH);
    for (int j = 0; j < LAT_LONG_LENGTH - 1; j++) {
        rmc->Lat_Long[j] = RMCbuffer[buffer_index++];
    }
    
    // Get Date
    moveToNextComma(RMCbuffer, &buffer_index);
    moveToNextComma(RMCbuffer, &buffer_index);
    moveToNextComma(RMCbuffer, &buffer_index);
    memset(rmc->Date, '\0', DATE_LENGTH);
    for (int j = 0; j < DATE_LENGTH - 1; j++) {
        rmc->Date[j] = RMCbuffer[buffer_index++];
    }
}