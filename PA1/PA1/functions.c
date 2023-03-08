/*
	Author: Leo Curdi
	Date: 1-16
	Class: cpts 122, lab section 2
	TA: Harry
	Assignment: PA 1
	Description:
		This program traverses through 24 hours worth of fit bit data, cleans it, and calculates values from it
*/
#include "header.h"

/*
	reads all data from the input file, one line at a time, and processes it. 
	processing includes: removing lines with wrong patient id, removing duplicate lines, and cleaning the remaining lines by inserting invalid data where data is missing
	precondition: infile must be open and the title line(s) must have already been read
*/
void processData(char* fileLine2, FitbitData FitBitData[]) {
	// open the data file with mode read
	FILE* infile = fopen("FitBitData.csv", "r");

	char fileLine[100], fileLine1[100]; // for reading a line from the file, and for saving the first 2 lines from the file
	int usableLine = 1, lineCount = 0, dupeLine = 1;
	// checks if the file opened successfully
	if (infile != NULL) {

		// read the first 2 lines individually. (this assumes the first 2 lines dont contain actual data)
		fgets(fileLine1, 100, infile);
		fgets(fileLine2, 100, infile);

		// get the target ID from the first line
		strtok(fileLine1, ",");
		char* targetID = strtok(NULL, ",");

	// use end of file loop to read and process all the data
		while (!feof(infile)) {
			// read a line
			fgets(fileLine, 100, infile);

		// remove lines with the wrong patient ID
			for (int i = 0; i < strlen(targetID); i++) {
				if (*(fileLine + i) != *(targetID + i)) { // if a char in the file line doesnt equal the corresponding char in the target, we know the ID doesnt match
					usableLine = 0; // this means the id is incorrect and the line us un usable
				}
			}
			// ensure it is in fact the same id and not a longer id that starts the same
			if (*(fileLine + strlen(targetID)) != ',') {
				usableLine = 0;
			}

		// remove duplicate lines: access the minute stored in the struct for the previous line and check if it matches
			// get current minute
			int t = 0;
			while (*(fileLine + t) != ',') { // get to the first comma
				t++;
			}
			t++; // the first cell after the first comma
			int k = 0;
			while (*(fileLine + t) != ',') { // get to the second comma
				if (*(fileLine + t) != FitBitData[lineCount - 1].minute[k]) { // if a char in the time doesn't match the corresponding char of the previous line's time, then the row is not a duplicate
					dupeLine = 0; // line is not a duplicate
				}
				t++, k++;
			}

		// after flagging invalid and dupe lines, continue on to process the remaining lines
			if (usableLine == 1 && dupeLine == 0) { // only continue with the processing if it is a usable line

				// clean and store the data with the function below
				cleanAndStoreData(FitBitData, fileLine, lineCount);

				lineCount++; // keep track of how many lines have been used 
			}
			usableLine = 1, dupeLine = 1; // make sure to reset these variables
		}
	}
	// close the data file when done
	fclose(infile);
}

/*
	takes in the valid lines of data, cleans them by filling missing fields with '0', and stores them in the data array
	'0' works for the invalid data because most calculations are sums and 0 contributes nothing to sums. the average heartrate calculation compensates by counting the amount of 0 values and excluding them from average
*/
void cleanAndStoreData(FitbitData FitBitData[], char fileLine[], int lineCount) {
// clean the data. Shift everything over and insert invalid data (this data will not be included in calculations)
	for (int i = 0; i < strlen(fileLine); i++) { // string length will increase each time invalid data is inserted, so you need a dynamic variable aka plugging in strlen directly
		if (*(fileLine + i) == ',' && *(fileLine + i + 1) == ',') { // then there are 2 commas in a row, meaning we have missing data and need to insert something
			for (int j = strlen(fileLine) + 1; j > i + 1; j--) { // this will shift everything starting 1 cell past the null char, and going until 1 cell past the second comma, leaving a blank space where the second comma was
				*(fileLine + j) = *(fileLine + j - 1);
			}
			*(fileLine + i + 1) = '0'; // place the invalid data in the empty cell (for most calculations, 0 wont affect the result as they are sums and 0 will contribute nothing to a sum)
		}
	}
	// if theres missing data at the end, the above for loop wont catch it, so we need to check for a "," at the end of the string
	int lineLength = strlen(fileLine);
	if (*(fileLine + lineLength - 2) == ',') { // so turns out theres two characters after a string in a file: a new line and a null char. So to access the last char and see if its a comma, you actually have to access the second to last char thus doing length - 2
		*(fileLine + lineLength + 1) = *(fileLine + lineLength); // move the null char over 1
		*(fileLine + lineLength) = *(fileLine + lineLength - 1); // move the new line char over 1
		*(fileLine + lineLength - 1) = '0';
	}

// parce the data and store it in the fitbit array
	char* ptr; // this variable is never used, we just need something to pass in to strtod for the second argument
	strcpy(FitBitData[lineCount].patient, strtok(fileLine, ","));
	strcpy(FitBitData[lineCount].minute, strtok(NULL, ","));
	FitBitData[lineCount].calories = strtod(strtok(NULL, ","), &ptr);// have to convert calories to a double
	FitBitData[lineCount].distance = strtod(strtok(NULL, ","), &ptr);
	FitBitData[lineCount].floors = atoi(strtok(NULL, ","));
	FitBitData[lineCount].heartRate = atoi(strtok(NULL, ","));
	FitBitData[lineCount].steps = atoi(strtok(NULL, ","));
	FitBitData[lineCount].sleepLevel = atoi(strtok(NULL, "\n")); // the character at the end of the line is a "\n", not a ","
}

/*
	recieves pointers to all variables representing required calculations on the data, performs said calculations, and returns the computed results through pointers
*/
void calculations(double* totalCalories, double* totalDistance, int* totalFloors, int* totalSteps, int* maxSteps, double* averageHeartrate, char* sleepStart, char* sleepEnd, FitbitData FitBitData[]) {
	int totalHeartRate = 0, lenPoorSleep = 0, longestLenPoorSleep = 0, heartRateInvalidDataCounter = 0; // heart rate is the only one we need to keep track of invalid data since i used '0' to represent invalid data and that wont affect sums but will affect averages
	for (int i = 0; i < NUMLINES; i++) {
		// compute total cals burned, distance walked in miles, floors walked, avg heartrate, and steps taken
		*totalCalories += FitBitData[i].calories;
		*totalDistance += FitBitData[i].distance;
		*totalFloors += FitBitData[i].floors;
		totalHeartRate += FitBitData[i].heartRate; // use total heart rate to calculate average
		if (FitBitData[i].heartRate == 0) { // heart rate is the only one we need to keep track of invalid data since i used '0' to represent invalid data and that wont affect sums but will affect averages
			heartRateInvalidDataCounter++;
		}
		*totalSteps += FitBitData[i].steps;

		// find max steps taken in 1 min, if there are multiple instances of the max, report the most recent one
		if (FitBitData[i].steps >= *maxSteps) { // the greater than or EQUAL TO will allow for overwriting the max steps value with a more recent one in the event there are multiple instances of max
			*maxSteps = FitBitData[i].steps;
		}

		// find longest range of poor sleep, report starting and ending minues
		if (FitBitData[i].sleepLevel > 1) { // keep incrementing length of poor sleep
			lenPoorSleep++;
		}
		else { // once there is a gap in the poor sleep, reset to 0
			lenPoorSleep = 0;
		}
		if (lenPoorSleep > longestLenPoorSleep) {
			longestLenPoorSleep = lenPoorSleep;
			strcpy(sleepStart, FitBitData[i - lenPoorSleep + 1].minute);
			strcpy(sleepEnd, FitBitData[i].minute);
		}
	}
	*averageHeartrate = (double)totalHeartRate / (NUMLINES - heartRateInvalidDataCounter); // exclude invalid data from heart rate average
}

/*
	writes all the calculated values to the output file, as well as all 1440 lines of processed data from the original fit bit data file
*/
void writeToOutfile(FILE* outfile, double totalCalories, double totalDistance, int totalFloors, int totalSteps, double averageHeartrate, int maxSteps, char* sleepStart, char* sleepEnd, char* fileLine2, FitbitData FitBitData[]) {
	// write the calculated values to the output file 
	fprintf(outfile, "Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep");
	fprintf(outfile, "\n%lf,%lf,%d,%d,%lf,%d,%s : %s\n\n", totalCalories, totalDistance, totalFloors, totalSteps, averageHeartrate, maxSteps, sleepStart, sleepEnd);
	// print the cleaned and valid data to the output file
	fprintf(outfile, "%s", fileLine2);
	for (int i = 0; i < NUMLINES; i++) {
		fprintf(outfile, "%s,", FitBitData[i].patient);
		fprintf(outfile, "%s,", FitBitData[i].minute);
		fprintf(outfile, "%.9lf,", FitBitData[i].calories);
		fprintf(outfile, "%.9lf,", FitBitData[i].distance);
		fprintf(outfile, "%d,", FitBitData[i].floors);
		fprintf(outfile, "%d,", FitBitData[i].heartRate);
		fprintf(outfile, "%d,", FitBitData[i].steps);
		fprintf(outfile, "%d\n", FitBitData[i].sleepLevel);
	}
}

/*
	writes all the calculated values to the console, as well as all 1440 lines of processed data from the original fit bit data file (not required so code has been commented out)
*/
void printToConsole(double totalCalories, double totalDistance, int totalFloors, int totalSteps, double averageHeartrate, int maxSteps, char* sleepStart, char* sleepEnd, char* fileLine2, FitbitData FitBitData[]) {
	// print the calculated values to the console
	printf("Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep");
	printf("\n%lf,%lf,%d,%d,%lf,%d,%s : %s\n\n", totalCalories, totalDistance, totalFloors, totalSteps, averageHeartrate, maxSteps, sleepStart, sleepEnd);
	// print the cleaned and valid data to the console (not required)
	//printf("%s", fileLine2);
	//for (int i = 0; i < NUMLINES; i++) {
	//	printf("%s,", FitBitData[i].patient);
	//	printf("%s,", FitBitData[i].minute);
	//	printf("%.9lf,", FitBitData[i].calories);
	//	printf("%.9lf,", FitBitData[i].distance);
	//	printf("%d,", FitBitData[i].floors);
	//	printf("%d,", FitBitData[i].heartRate);
	//	printf("%d,", FitBitData[i].steps);
	//	printf("%d\n", FitBitData[i].sleepLevel);
	//}
}