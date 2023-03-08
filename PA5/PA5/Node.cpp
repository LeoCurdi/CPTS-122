

#include "Node.h"



// constructor
Node::Node(const Data* pNewData, Node* pNewNext) {
	this->pData = new Data(pNewData->getCustomerNumber(), pNewData->getServiceTime(), pNewData->getTotalTime());
	this->pNext = pNewNext; // nullptr
}

// copy constructor
Node::Node(const Node& copy) {
	this->pData = copy.pData;
	this->pNext = copy.pNext;
}

// destructor
Node::~Node() { // this is redundant becuase were already deleting the node in the dequeue function
	//delete this;
}

// setters
void Node::setpData(Data* newpData) {
	this->pData = newpData;
}
void Node::setpNext(Node* newpNext) {
	this->pNext = newpNext;
}

// getters
Data* Node::getpData() const {
	return this->pData;
}
Node* Node::getpNext() const {
	return this->pNext;
}


// need an overloaded thing for outputting the node to the console
ostream& operator<< (ostream& lhs, Node*& rhs) {
	cout << "Customer Number: " << rhs->getpData()->getCustomerNumber() << "\tService Time: " << rhs->getpData()->getServiceTime() << "\tTotal Time: " << rhs->getpData()->getTotalTime() << endl;

	return lhs;
}