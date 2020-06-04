#include "97901.gps.z4.h"



int main(int argc, char* argv[]) {

	if (argv[1] == "-i" && argv[strlen(argv[2] - 3)] != ".")
		return 0;

	Record* recordArr = (Record*)malloc(1000000 * sizeof(Record));
	int size = readFile(argv[2], recordArr);
	searchGPS(argc, argv, recordArr, size);

}


