
#include "header.h"

int main(void) {
	//FILE* infile = fopen("FitBitData.csv", "r");
	//char line[100];
	//while (!feof(infile)) {
	//	fgets(line, 100, infile);
	//	printf("%s", line);
	//}

	FILE* infile = fopen("test.csv", "r");
	char line[100];
	fgets(line, 100, infile);
	printf("%d", strlen(line));
	if (*(line + strlen(line) - 1) == '\n') {
		printf("\nTrue");
	}

	printf("\n\n\n");
	return 0;
}