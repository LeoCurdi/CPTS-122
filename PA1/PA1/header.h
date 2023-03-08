/*
	Author: Leo Curdi
	Date: 1-16
	Class: cpts 122, lab section 2
	TA: Harry
	Assignment: PA 1
	Description:
		This program traverses through 24 hours worth of fit bit data, cleans it, and calculates values from it
*/
#ifndef HEADER_H
#define HEADER_H

#define _CRT_SECURE_NO_WARNINGS
#define NUMLINES 1440

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum sleep {
	NONE = 0, 
	ASLEEP = 1, 
	AWAKE = 2, 
	REALLYAWAKE = 3
} Sleep;

typedef struct fitbit {
	char patient[10]; // this is a string of length 10
	char minute[9]; // this is a string of length 9
	double calories;
	double distance; // in miles
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;
} FitbitData;

/*
	reads all data from the input file, one line at a time, and processes it.
	processing includes: removing lines with wrong patient id, removing duplicate lines, and cleaning the remaining lines by inserting invalid data where data is missing
	precondition: infile must be open and the title line(s) must have already been read
*/
void processData(char* fileLine2, FitbitData FitBitData[]);

/*
	takes in the valid lines of data, cleans them by filling missing fields with '0', and stores them in the data array
	'0' works for the invalid data because most calculations are sums and 0 contributes nothing to sums. the average heartrate calculation compensates by counting the amount of 0 values and excluding them from average
*/
void cleanAndStoreData(FitbitData FitBitData[], char fileLine[], int lineCount);

/*
	recieves pointers to all variables representing required calculations on the data, performs said calculations, and returns the computed results through pointers
*/
void calculations(double* totalCalories, double* totalDistance, int* totalFloors, int* totalSteps, int* maxSteps, double* averageHeartrate, char* sleepStart, char* sleepEnd, FitbitData FitBitData[]);

/*
	writes all the calculated values to the output file, as well as all 1440 lines of processed data from the original fit bit data file
*/
void writeToOutfile(FILE* outfile, double totalCalories, double totalDistance, int totalFloors, int totalSteps, double averageHeartrate, int maxSteps, char* sleepStart, char* sleepEnd, char* fileLine2, FitbitData FitBitData[]);

/*
	writes all the calculated values to the console, as well as all 1440 lines of processed data from the original fit bit data file (not required so code has been commented out)
*/
void printToConsole(double totalCalories, double totalDistance, int totalFloors, int totalSteps, double averageHeartrate, int maxSteps, char* sleepStart, char* sleepEnd, char* fileLine2, FitbitData FitBitData[]);

#endif