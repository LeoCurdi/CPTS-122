#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

// dont use using namespace becuase if u have a lot of stuff going on you could accidentally do something bad like naming a class the same thing as something in standard namespace

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::ostream;


class Data {

public: // Member functions

	// constructor
	Data(int newCustomerNumber = 0, int newServiceTime = 0, int newTotalTime = 0); // boilerplate arguments

	// copy constructor
	Data(const Data& copy);

	// destructor
	~Data();

	// setters
	void setCustomerNumber(int newCustomerNumber);
	void setServiceTime(int newServiceTime);
	void setTotalTime(int newTotalTime);

	// getters
	int getCustomerNumber() const; // why const function? idk
	int getServiceTime() const;
	int getTotalTime() const;

private:
	int customerNumber; // Unique identifier; starts at 1; after 24 hours should be reset to 1
	int serviceTime; // Random time; varies between express and normal lanes; units in minutes
	int totalTime; // totalTime = serviceTime + sum of serviceTimes of customers in line before this customer; units in minutes
}; // This memory needs to be allocated on the heap!
