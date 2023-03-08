

#include "Queue.h"


// copy constructor. this sucks because it has to be a deep copy
Queue::Queue(const Queue& copy) { // how the fckk do u copy a q?? probably need some kind of while loop
	Node* pCur = copy.getpHead(); // get the first node of the q to be copied
	Node* pCopyCur = nullptr;

	// check if the q to be copied is empty
	if (copy.getpHead() == nullptr) {
		return;
	}

	// looks like it's not empty
	// while loop to repeat the process until the whole thing is copied
	while (pCur != nullptr) {
		// figure out pNew
		Node* pNew = new Node(pCur->getpData()); // allocate space for a new node with (a pointer to) the data from the node to be copied
		pNew->setpData(pCur->getpData());
		pNew->setpNext(nullptr); // dont really need to do this though since it should already = nullptr by the constructor with boilerplate arguments
		
		// hook it up to the list
		// if first node
		if (pCur == copy.getpHead()) { // if this is the first node being copied, gotta update the head pointer for the new q
			this->pHead = pNew; // point the new q to the first new node
		}
		// if any other node
		else {
			pCopyCur->setpNext(pNew); // hook it up
		}
		
		// shift both Qs
		pCur = pCur->getpNext();
		pCopyCur = pNew;
	}
}

// destructor
Queue::~Queue() { // this gets called when q goes out of scope (when main returns)
	destroyQueue();
}
void Queue::destroyQueue() {
	while (!isEmpty()) {
		cout << "deleting " << dequeue() << endl;
	}
}

// setters
void Queue::setpHead(Node* pNewHead) {
	this->pHead = pNewHead;
}
void Queue::setpTail(Node* pNewTail) {
	this->pTail = pNewTail;
}

// getters
Node* Queue::getpHead() const {
	return this->pHead;
}
Node* Queue::getpTail() const {
	return this->pTail;
}


// enqueue
bool Queue::enqueue(const Data* newData) {
	// allocate space for a new node
	Node* pNew = new Node(newData);

	// check for failure
	if (pNew == nullptr) {
		return false;
	}

	// two cases: empty q or nonempty q

	// case: empty q. update pHead and pTail to point to pNew, pNew->pNext to NULL (which it already is)
	if (this->pHead == nullptr) {
		this->pHead = this->pTail = pNew; // update pHead and pTail to point to pNew
	}

	// case: nonempty q. update pTail to point to pNew, pPrev->pNext to pNew, pNew->pNext to NULL (which it already is)
	else {
		// the q does not have access to a node's private data members, so we cant just do ptail->pnext, we need builders
		this->pTail->setpNext(pNew); // pPrev->pNext to pNew
		pTail = pNew; // update pTail to point to pNew
	}

	return true;
}

// dequeue. precondition: q must not be empty. returns the data being deleted
Data* Queue::dequeue() { // returns the data being deleted
	Node* pTemp = this->pHead;

	// 2 cases: 1 node in q or multiple

	// case: 1 node. set head and tail to null
	if (this->pHead == this->pTail) {
		this->pHead = this->pTail = nullptr;
	}

	// case: multiple nodes. set head to the next node in the list
	else {
		this->pHead = this->pHead->getpNext();
	}

	// copy over the data
	Data* pNew = new Data;
	pNew->setCustomerNumber(pTemp->getpData()->getCustomerNumber()); // jesus
	pNew->setServiceTime(pTemp->getpData()->getServiceTime());
	pNew->setTotalTime(pTemp->getpData()->getTotalTime());

	// either case: free the deleted node
	delete pTemp; // this will invoke the destructor for a node

	return pNew;
}

// tells you whether the q is empty. returns true for empty, false for nonempty
bool Queue::isEmpty() const { 
	return this->pHead == nullptr;
}


// print queue. You don't want a parameter for the q becuase it is a member function so you can literally use "this->"
void Queue::printQueue() { // if you pass in a q, and not a pointer to a q, it will call the copy function and pass in a copy of the q, then destroy the copy after printQ is done
	// create a pointer to the first node in the list
	Node* pCur = this->getpHead();

	if (pCur == nullptr) cout << "Empty!" << endl;

	// traverse the list
	while (pCur != nullptr) {
		cout << pCur; // print the node data
		pCur = pCur->getpNext(); // shift
	}
}

// run simulation
void Queue::runSimulation(int n) {

	// seed for random number generation
	srand((unsigned int)time(NULL));

	int expCustomerNumber = 1, stdCustomerNumber = 1, currentTime = 0, expWaitTime = 0, stdWaitTime = 0, serviceTime; // customer number resets to 1 after 24 hours

	// initialize the cues
	Queue standardQ, expressQ;
	Data* customerData;

	// generate arrival time of first customer per lane
	int expNextArrival = randomIntInRange(1, 5), stdNextArrival = randomIntInRange(3, 8);
	int expNextServed = 0, stdNextServed = 0;

	// 24 hour simulation loop
	for (; currentTime < n; currentTime++) {

		// case: express customer arrives. queue customer, calculate service time, calcuate next customer arrival
		if (expNextArrival == 0) {
			// queue the customer with a randomly generated service time
			serviceTime = randomIntInRange(1, 5);
			expWaitTime += serviceTime;
			customerData = new Data(expCustomerNumber, serviceTime, expWaitTime); // the 3rd argument is the time until the customer will be serviced

			expressQ.enqueue(customerData);

			// print out a message indicating in which line each customer arrives, along with the overall corresponding arrival time and customer number.
			cout << "Express lane arrival.\t\tTime: " << currentTime << " Customer number: " << expCustomerNumber << " Will be serviced in: " << expWaitTime << endl;

			// calculate the next arrival with a randomly generated arrival time
			expNextArrival = randomIntInRange(1, 5);

			// if its the only customer in the q, it will start processing immediately
			if (expressQ.getpHead() == expressQ.getpTail()) {
				expNextServed = expressQ.getpHead()->getpData()->getServiceTime();
			}

			// note that a customer has arrived
			expCustomerNumber++;
		}

		// case: express customer finished service. q cannot be empty
		if (expNextServed == 0 && !expressQ.isEmpty()) { // dequeue the customer
			customerData = expressQ.dequeue();

			// print out a message indicating which line the customer was in, along the corresponding customer numberand totalTime in the line.
			cout << "Express customer served.\tTime: " << currentTime << " Customer number: " << customerData->getCustomerNumber() << " Total time in line: " << customerData->getTotalTime() << endl;

			// set the time till the next customer is served (if there is a next customer)
			if (!expressQ.isEmpty()) {
				expNextServed = expressQ.getpHead()->getpData()->getServiceTime();
			}
		}


		// case: standard customer arrives. queue customer, calculate service time, calcuate next customer arrival
		if (stdNextArrival == 0) {
			// queue the customer with a randomly generated service time
			serviceTime = randomIntInRange(3, 8);
			stdWaitTime += serviceTime;
			customerData = new Data(stdCustomerNumber, serviceTime, stdWaitTime); // the 3rd argument is the time until the customer will be serviced

			standardQ.enqueue(customerData);

			// print out a message indicating in which line each customer arrives, along with the overall corresponding arrival time and customer number.
			cout << "Standard lane arrival.\t\tTime: " << currentTime << " Customer number: " << stdCustomerNumber << " Will be serviced in: " << stdWaitTime << endl;

			// calculate the next arrival with a randomly generated arrival time
			stdNextArrival = randomIntInRange(3, 8);

			// if its the only customer in the q, it will start processing immediately
			if (standardQ.getpHead() == standardQ.getpTail()) {
				stdNextServed = standardQ.getpHead()->getpData()->getServiceTime();
			}

			// note that a customer has arrived
			stdCustomerNumber++;
		}

		// case: standard customer finished service. q cannot be empty
		if (stdNextServed == 0 && !standardQ.isEmpty()) { // dequeue the customer
			customerData = standardQ.dequeue();

			// print out a message indicating which line the customer was in, along the corresponding customer numberand totalTime in the line.
			cout << "Standard customer served.\tTime: " << currentTime << " Customer number: " << customerData->getCustomerNumber() << " Total time in line: " << customerData->getTotalTime() << endl;

			// set the time till the next customer is served (if there is a next customer)
			if (!standardQ.isEmpty()) {
				stdNextServed = standardQ.getpHead()->getpData()->getServiceTime();
			}
		}


		// print q every 10 minutes
		if (currentTime % 10 == 0) {
			cout << "\nPrinting Standard Q . . ." << endl;
			standardQ.printQueue();

			cout << "Printing Express Q . . ." << endl;
			expressQ.printQueue();
			putchar('\n');
		}

		// reset customer number every 24 hours
		if (currentTime == 1440) {
			expCustomerNumber = 1, stdCustomerNumber = 1;
		}

		// always count down until the next customer arrives, is serviced, and track current wait time
		expNextArrival--, stdNextArrival--;
		expNextServed--, stdNextServed--;
		if (expWaitTime > 0) expWaitTime--;
		if (stdWaitTime > 0) stdWaitTime--;
	}

	cout << "\nEnd of simulation. Destroying Q . . .\n" << endl;
}


// nonmember functions

// generates a random integer within a given range
int randomIntInRange(int lower, int upper) {
	return (rand() % (upper + 1 - lower)) + lower;
}
