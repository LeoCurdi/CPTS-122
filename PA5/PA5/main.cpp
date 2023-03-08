

/*
	to do
		- run simulation
		- comments
*/
#include "Queue.h"
#include "Test.h"



int main(void) {
	

	Test test;
	test.testEnqueueEmpty();
	test.testEnqueueNonEmpty();
	test.testDequeueOneNode();
	test.testDequeueTwoNodes();
	test.testRunSimulation();


	return 0;
}