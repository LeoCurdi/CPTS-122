#pragma once

#include "Data.h"


class Node {

public: // Member functions
	
	// constructor
	Node(const Data* pNewData, Node* pNewNext = nullptr);

	// copy constructor
	Node(const Node& copy);

	// destructor
	~Node();

	// setters
	void setpData(Data* newpData);
	void setpNext(Node* newpNext);

	// getters
	Data* getpData() const; // why const function? idk
	Node* getpNext() const;

private:
	Data* pData;    // The memory for Data will need to be allocated on the heap as well!
	Node* pNext;
};

// need an overloaded thing for outputting the node to the console
ostream& operator<< (ostream& lhs, Node*& rhs);