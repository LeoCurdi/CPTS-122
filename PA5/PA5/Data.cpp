

#include "Data.h"


// constructor
Data::Data(int newCustomerNumber, int newServiceTime, int newTotalTime) {
	this->customerNumber = newCustomerNumber;
	this->serviceTime = newServiceTime;
	this->totalTime = newTotalTime;
}

// copy constructor
Data::Data(const Data& copy) {
	this->customerNumber = copy.customerNumber;
	this->serviceTime = copy.serviceTime;
	this->totalTime = copy.totalTime;
}

// destructor
Data::~Data() {
	// skip because we've already deleted the data in dequeue
}

// setters
void Data::setCustomerNumber(int newCustomerNumber) {
	this->customerNumber = newCustomerNumber;
}
void Data::setServiceTime(int newServiceTime) {
	this->serviceTime = newServiceTime;
}
void Data::setTotalTime(int newTotalTime) {
	this->totalTime = newTotalTime;
}

// getters
int Data::getCustomerNumber() const {
	return this->customerNumber;
}
int Data::getServiceTime() const {
	return this->serviceTime;
}
int Data::getTotalTime() const {
	return this->totalTime;
}
