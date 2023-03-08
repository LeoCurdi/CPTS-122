#pragma once

#include "Node.h"

class Queue {

public: // Member functions

	// constructor for the q (don't need the boilerplate constructor here)
	// we also dont need to define this constructor since it has the initializer list
	Queue(Node* pNewHead = nullptr, Node* pNewTail = nullptr) : pHead(pNewHead), pTail(pNewTail) {}; // if we use a constructor with boilerplate arguments, we dont need the boilerplate constructor (if we did both we would probably get an error). also the : is an initializer list
	
	// copy constructor
	Queue(const Queue& copy);

	// destructor
	~Queue();

	// setters
	void setpHead(Node* pNewHead);
	void setpTail(Node* pNewTail);

	// getters
	Node* getpHead() const; // why const function? idk
	Node* getpTail() const;


	// enqueue
	bool enqueue(const Data* newData);
	
	// dequeue. precondition: q must not be empty. returns the data being deleted
	Data* dequeue(); // returns the data being deleted

	// tells you whether the q is empty
	bool isEmpty() const; 


	// print queue
	void printQueue();

	// run simulation
	void runSimulation(int n);

private:
	Node* pHead;
	Node* pTail;

	// now we're making a private member function. this function is not used by the programmer because it helps the destructor
	void destroyQueue();
};

// nonmember functions

// generates a random integer within a given range
int randomIntInRange(int lower, int upper);
