#include "97901.gps.z4.h"

int readFile(char* file_name, Record* recordArr) {

	FILE* pFile;
	char fileString[40] = "";
	char year[4] = "";
	char mad[3] = "";
	char id[8] = "";
	char latlon[8] = "";
	int i = 0;

	pFile = fopen(file_name, "r");

	if (pFile) {
		while (fgets(fileString, 40, pFile) != 0) {
			int date = 0;
			strncpy(year, fileString, 4);
			date += atoi(year) * 10000;
			strncpy(mad, fileString + 5, 2);
			date += atoi(mad) * 100;
			strncpy(mad, fileString + 8, 2);
			date += atoi(mad);
			recordArr[i].date = date;
			strncpy(id, fileString + 11, 8);
			recordArr[i].imei = atoi(id);
			strncpy(latlon, fileString + 20, 8);
			recordArr[i].lat = atof(latlon);
			strncpy(latlon, fileString + 29, 8);
			recordArr[i].lon = atof(latlon);
			i++;
		}
	}
	return i;
}

int compare(const void* a, const void* b){

	Record* A = (Record*)a;
	Record* B = (Record*)b;

	return (B->date - A->date);
}

float haversine(float lat1, float lon1, float lat2, float lon2) {

	float dLat = (lat2 - lat1) *
		M_PI / 180.0;
	float dLon = (lon2 - lon1) *
		M_PI / 180.0;

	lat1 = (lat1)*M_PI / 180.0;
	lat2 = (lat2)*M_PI / 180.0;

	float a = pow(sin(dLat / 2), 2) +
		pow(sin(dLon / 2), 2) *
		cos(lat1) * cos(lat2);
	float rad = 6371;
	float c = 2 * asin(sqrt(a));
	return (rad * c) * 1000;
}

void printRec(Record record) {
	int year = record.date / 10000;
	int day = record.date % 100;
	int month = (record.date % 10000 - day) / 100;

	printf("%d-%02d-%02d %d %.5f %.5f\n", year, month, day, record.imei, record.lat, record.lon);
}

void writeF(Record record, FILE *f) {
	int year = record.date / 10000;
	int day = record.date % 100;
	int month = (record.date % 10000 - day) / 100;

	fprintf(f ,"%d-%02d-%02d %d %.5f %.5f\n", year, month, day, record.imei, record.lat, record.lon);
}

int readDate(char array[]) {
	char year[5] = "";
	char mad[3] = "";
	int date = 0;

	strncpy(year, array, 4);
	date += atoi(year) * 10000;
	strncpy(mad, array + 5, 2);
	date += atoi(mad) * 100;
	strncpy(mad, array + 8, 2);
	date += atoi(mad);

	return date;
}


void searchGPS(int args, char* argv[], Record recordArr[], int size) {
	int flagDefault = 0;
	char file_out[256] = { 0 };

		if (args > 3) {

			if (strcmp(argv[3], "-o") == 0) {
				strcpy(file_out, argv[4]);
			}
			// --------------------------------vyhladanie imei
			if (strcmp(argv[3], "-u") == 0 || (args > 5 && strcmp(argv[5], "-u") == 0)) {
				flagDefault = 1;
				if (strlen(file_out) > 0) {
					FILE* f = fopen(file_out, "w");
					for (int i = 0; i < size; i++) {
						if (recordArr[i].imei == atoi(argv[6]))
							writeF(recordArr[i], f);
					}
					fclose(f);
				}
				else {
					for (int i = 0; i < size; i++) {
						if (recordArr[i].imei == atoi(argv[4])) {
							printRec(recordArr[i]);
						}
					}
				}
			}

			// -------------------------------- gps do x metrov

			if (strcmp(argv[3], "-l") == 0 || (args > 5 && strcmp(argv[5], "-l") == 0)) {
				flagDefault = 1;
				int flag = 0;
				if (strcmp(argv[3], "-l") == 0) {
					flag = 3;
				}
				else {
					flag = 5;
				}
				if (strlen(file_out) > 0) {
					FILE* f = fopen(file_out, "w");
					for (int i = 0; i < size; i++) {
						if (haversine(recordArr[i].lat, recordArr[i].lon, atof(argv[flag + 1]), atof(argv[flag + 2])) < atoi(argv[flag + 3]))
							writeF(recordArr[i], f);
					}
					fclose(f);
				}
				else {
					for (int i = 0; i < size; i++) {
						if (haversine(recordArr[i].lat, recordArr[i].lon, atof(argv[flag + 1]), atof(argv[flag + 2])) < atoi(argv[flag + 3])) {
							printRec(recordArr[i]);
						}
					}
				}
			}
			// --------------------------------
			if (strcmp(argv[3], "-t") == 0 || (args > 5 && strcmp(argv[5], "-t") == 0)) {
				flagDefault = 1;
				int flag = 0;
				int dateStart = 0, dateEnd = 0;
				if (strcmp(argv[3], "-t") == 0) {
					flag = 3;
				}
				else {
					flag = 5;
				}
				char year[5] = "";
				char mad[3] = "";

				dateStart = readDate(argv[flag + 1]);

				dateEnd = readDate(argv[flag + 2]);

				if (strlen(file_out) > 0) {
					FILE* f = fopen(file_out, "w");
					for (int i = 0; i < size; i++) {
						if (recordArr[i].date >= dateStart && recordArr[i].date <= dateEnd)
							writeF(recordArr[i], f);
					}
					fclose(f);
				}
				else {
					for (int i = 0; i < size; i++) {
						if (recordArr[i].date >= dateStart && recordArr[i].date <= dateEnd)
							printRec(recordArr[i]);
					}
				}

			}

			if (strcmp(argv[3], "-s") == 0 || (args > 5 && strcmp(argv[5], "-s") == 0)) {
				Record infected[101] = { 0 };
				int j = 0, flag = 0;
				char year[5] = "";
				char mad[3] = "";
				int date = 0;
				flagDefault = 1;
				if (strcmp(argv[3], "-s") == 0) {
					flag = 3;
				}
				else {
					flag = 5;
				}

				strncpy(year, argv[flag + 2], 4);
				date = readDate(argv[flag + 2]);

				for (int i = 0; i < size; i++) {
					if ((recordArr[i].imei == atoi(argv[flag + 1]) && (recordArr[i].date >= date))) {
						infected[j].date = recordArr[i].date;  ///malo by to byt cez memcpy() ale neslo mi to
						infected[j].imei = recordArr[i].imei;
						infected[j].lat = recordArr[i].lat;
						infected[j].lon = recordArr[i].lon;
						j++;
					}
				}

				int pom = j;
				if (strlen(file_out) > 0) {
					FILE* f = fopen(file_out, "w");
					for (int i = 0; i <= pom; i++) {
						for (j = 0; j < size; j++) {
							if ((infected[i].date <= recordArr[j].date) && (haversine(infected[i].lat, infected[i].lon, recordArr[j].lat, recordArr[j].lon) <= atoi(argv[flag + 3])))
								writeF(recordArr[j], f);
						}
					}
					fclose(f);
				}
				else {
					for (int i = 0; i <= pom; i++) {
						for (j = 0; j < size; j++) {
							if ((infected[i].date == recordArr[j].date) && (haversine(infected[i].lat, infected[i].lon, recordArr[j].lat, recordArr[j].lon) <= atoi(argv[flag + 3])))
								printRec(recordArr[j]);
						}
					}
				}
			}
		}


		// -------------------------------- bez parametra
		if (flagDefault == 0) {
			qsort(recordArr, 1000000, sizeof(Record), compare);

			if (strlen(file_out) > 0) {
				FILE* f = fopen(file_out, "w");
				for (int i = 0; i < size; i++) {
					writeF(recordArr[i], f);
				}
				fclose(f);
			}
			else {
				for (int i = 0; i < size; i++) {
					printRec(recordArr[i]);
				}
			}
		}
	}
