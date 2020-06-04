#ifndef GPS_Z4_H
#define GPS_Z4_H

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


typedef struct {
	int date;
	int imei;
	float lat;
	float lon;
} Record;

int readFile(char* file_name, Record* recordArr);
int compare(const void* a, const void* b);
float haversine(float lat1, float lon1, float lat2, float lon2);
void printRec(Record record);
void writeF(Record record, FILE* f);
int readDate(char array[]);
void searchGPS(int args, char* argv[], Record recordArr[], int size);






#endif //GPS_Z4_H
