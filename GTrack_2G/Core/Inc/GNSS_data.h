#ifndef _GNSS_DATA_H_
#define _GNSS_DATA_H_

#include "main.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "stdio.h"

#define NUMBER_AFTER_DECIMAL_POINT 6

#define TIME_LENGTH 7
#define LAT_LENGTH 10
#define LONG_LENGTH 11
#define SPEED_LENGTH 5
#define COURSE_LENGTH 7
#define DATE_LENGTH 7

typedef struct {
    char Time[TIME_LENGTH];
    char Lat[LAT_LENGTH];
    char Long[LONG_LENGTH];
    char Speed[SPEED_LENGTH];
    char Course[COURSE_LENGTH];
    char Date[DATE_LENGTH];
} RMC;
// Get sentence by header
char* get_sentence_by_header(char* GNSS_data, const char* header);
// Convert string data to float data
float stringToFloat(char *str);
// Convert float data to string data
void floatToString(float value, char *str, int afterDecimalPoint);
// Decodes RMC data
void decodeRMC(char* RMCbuffer, RMC* rmc);
// Convert NMEA sentence Lat to Decimal Degrees
float decodeLat(char* lat_data_raw);
// Convert NMEA sentence Long to Decimal Degrees
float decodeLong(char* long_data_raw);
// Decode NMEA sentence Time
char* decodeTime(char* time_data_raw);
// Decode NMEA sentence Date
char* decodeDate(char* date_data_raw);
// Format of latitude, longitude to send to the server
char* lat_long_data_format(RMC* rmc);
// Format of time, date to send to the server
char* timedate_data_format(RMC* rmc);
// Format of time, date, latitude, longitude to send to the server
char* timedatelatlong_data_format(RMC* rmc);

#endif