/*
	Author: Leo Curdi
	Date: 1-16
	Class: cpts 122, lab section 2
	TA: Harry
	Assignment: PA 1
	Description:
		This program traverses through 24 hours worth of fit bit data, cleans it, and calculates values from it
*/
/*
	to do
 
		- add functions and comment blocks
*/
#include "header.h"

int main(void) {
	// declare fit bit data array
	FitbitData FitBitData[NUMLINES] = { {"", "", 0, 0, 0, 0, 0, NONE}}; // initialize all components of the struct

	// these are all values to print to the output file
	double totalCalories = 0, totalDistance = 0, averageHeartrate;
	unsigned int totalFloors = 0, totalSteps = 0, maxSteps = 0;
	char sleepStart[9], sleepEnd[9];
	char fileLine2[100];

// get the data from the data file, toss any lines that have a different patient ID and duplicate lines, clean, parce, and insert into the struct array
	processData(fileLine2, FitBitData);

// perform the necessary calculations on the data with calculation function. use pointers to retain the changes
	calculations(&totalCalories, &totalDistance, &totalFloors, &totalSteps, &maxSteps, &averageHeartrate, sleepStart, sleepEnd, FitBitData);

// write to the output file and print to console
	// open the output file with mode write
	FILE* outfile = fopen("Results.csv", "w");
	// write everything to the output file using function
	writeToOutfile(outfile, totalCalories, totalDistance, totalFloors, totalSteps, averageHeartrate, maxSteps, sleepStart, sleepEnd, fileLine2, FitBitData);
	// close output file when done writing
	fclose(outfile);

	// call the function to print everything to the console
	printToConsole(totalCalories, totalDistance, totalFloors, totalSteps, averageHeartrate, maxSteps, sleepStart, sleepEnd, fileLine2, FitBitData);

	printf("\n\n\n");
	return 0;
}