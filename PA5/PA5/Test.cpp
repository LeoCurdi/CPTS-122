

#include "Test.h"



// test functions
void Test::testEnqueueEmpty(void) {
	// initialize a q and some data for testing
	Queue testQ;
	Data* pNewData = new Data(1, 5, 0);

	// enqueue
	cout << "Test: enqueue on empty . . ." << endl;
	bool success = testQ.enqueue(pNewData);

	// check if memory failed
	if (!success) {
		cout << "\tAllocate memory failed!" << endl;
		return;
	}
	cout << "\tAllocate memory passed!" << endl;

	// check if setting head pointer failed
	if (testQ.getpHead() == nullptr) cout << "\tSet head pointer failed!" << endl;
	else cout << "\tSet head pointer passed!" << endl;

	// check if setting tail pointer failed
	if (testQ.getpTail() == nullptr) cout << "\tSet tail pointer failed!" << endl;
	else cout << "\tSet tail pointer passed!" << endl;

	// check if the q is empty
	if (testQ.isEmpty()) cout << "\tQueue is not empty failed!" << endl;
	else cout << "\tQueue is not empty passed!" << endl;
}

void Test::testEnqueueNonEmpty(void) {
	// initialize a q and some data for testing
	Queue testQ;
	Data* pNewData = new Data(1, 5, 0);
	testQ.enqueue(pNewData);
	pNewData = new Data(2, 3, 5);

	// enqueue
	cout << "\nTest: enqueue on non empty . . ." << endl;
	bool success = testQ.enqueue(pNewData);

	// check if memory failed
	if (!success) {
		cout << "\tAllocate memory failed!" << endl;
		return;
	}
	cout << "\tAllocate memory passed!" << endl;

	// check if setting tail pointer failed
	if (testQ.getpTail()->getpData()->getCustomerNumber() != pNewData->getCustomerNumber()) cout << "\tSet tail pointer failed!" << endl;
	else cout << "\tSet tail pointer passed!" << endl;

	// check if the new node is linked up
	if (testQ.getpHead()->getpNext()->getpData()->getCustomerNumber() != pNewData->getCustomerNumber()) cout << "\tNew node linked in q failed!" << endl;
	else cout << "\tNew node linked in q passed!" << endl;
}

void Test::testDequeueOneNode(void) {
	// initialize a q and enqueue a node for testing
	Queue testQ;
	Data* pNewData = new Data(1, 5, 0);
	testQ.enqueue(pNewData);

	// dequeue
	cout << "\nTest: dequeue with one node . . ." << endl;
	Data* dequeuedData = testQ.dequeue();

	// check if head pointer is null
	if (testQ.getpHead() != nullptr) cout << "\tSet head pointer to null failed!" << endl;
	else cout << "\tSet head pointer to null passed!" << endl;

	// check if tail pointer is null
	if (testQ.getpTail() != nullptr) cout << "\tSet tail pointer to null failed!" << endl;
	else cout << "\tSet tail pointer to null passed!" << endl;

	// check if q is empty
	if (!testQ.isEmpty()) cout << "\tTest queue is empty failed!" << endl;
	else cout << "\tTest queue is empty passed!" << endl;

	// check if the data returned matches
	if (!(dequeuedData->getCustomerNumber() == 1 && dequeuedData->getServiceTime() == 5 && dequeuedData->getTotalTime() == 0)) cout << "\tReturned correct data failed!" << endl;
	else cout << "\tReturned correct data passed!" << endl;
}

void Test::testDequeueTwoNodes(void) {
	// initialize a q and enqueue 2 nodes for testing
	Queue testQ;
	Data* pNewData = new Data(1, 5, 0);
	testQ.enqueue(pNewData);
	pNewData = new Data(2, 3, 5);
	testQ.enqueue(pNewData);

	// test print
	cout << "\nTest: try to print the q . . ." << endl;
	testQ.printQueue();

	// dequeue
	cout << "\nTest: dequeue with two nodes . . ." << endl;
	Data* dequeuedData = testQ.dequeue();

	// check if head pointer = tail pointer != null
	if (!(testQ.getpHead() == testQ.getpTail() && testQ.getpTail() != nullptr)) cout << "\tHead pointer equals Tail pointer && tail pointer does not equal null failed!" << endl;
	cout << "\tHead pointer equals Tail pointer && tail pointer does not equal null passed!" << endl;

	// check if head pointer points to (2, 3, 5)
	if (!(testQ.getpHead()->getpData()->getCustomerNumber() == 2 && testQ.getpHead()->getpData()->getServiceTime() == 3 && testQ.getpHead()->getpData()->getTotalTime() == 5)) cout << "\tHead points to correct node failed!" << endl;
	else cout << "\tHead points to correct node passed!" << endl;

	// check if the data returned matches
	if (!(dequeuedData->getCustomerNumber() == 1 && dequeuedData->getServiceTime() == 5 && dequeuedData->getTotalTime() == 0)) cout << "\tReturned correct data failed!" << endl;
	else cout << "\tReturned correct data passed!" << endl;
}

void Test::testRunSimulation(void) {
	// initialize a q
	Queue testQ;
	int n;

	cout << "\n\nReady to run simulation. How long do you want? (enter minutes): ";
	cin >> n;

	// run the test
	testQ.runSimulation(n);
}