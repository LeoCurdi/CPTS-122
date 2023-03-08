/*
	Author: Leo Curdi
	Date: 1-25
	Class: cpts 122, lab section 2
	TA: Harry
	Assignment: PA 3
	Description:
		You must design and implement 3 test functions. These test functions must not accept any arguments or return any values.
		They should be self-sufficient. You should provide function declarations for them that are in a separate header file than
		your utility/application function declarations. Also, the corresponding implementations for them should be placed in a
		separate source file than your utility/applications function definitions and main ().
		You must implement one test function for insert, delete, and shuffle features for a total of 3 functions.
*/
#include "test.h"

/*
	For the insert test function, you must provide a test case with the following test point: artist name = “Perry, Katy”,
	album title = “Witness”, song title = “Chained to the Rhythm”, genre = “pop”, song length = “4:36”, times played = -1, rating = 6.
	List state = initially empty. You must think about what is your expected result?
	Should you able to insert a song with -1 times played? Should you able to add a song with rating 6?
	Is the head pointer of the list updated?
*/
void insertTest(void) {
	Record data = { "Perry, Katy", "Witness", "Chained to the Rhythm", "pop", {4, 36}, -1, 6};
	Node* pHead = NULL;

	int success = insertAtFront(&pHead, data);

	if (success == 1) {
		printf("Test for catching invalid data failed\n");
	
		if (pHead != NULL) printf("Test for updating head pointer for empty list passed!\n");
		else printf("Test for updating head pointer for empty list failed\n");
	
	}
	else printf("Test for catching invalid data passed!\n");
}

/*
	For the delete test function, you must provide a test case with the following test point: song title to
	delete = “Chained to the Rhythm”. List state = artist name = “Perry, Katy”, album title = “Witness”,
	song title = “Chained to the Rhythm”, genre = “pop”, song length = “4:36”, times played = 3,
	rating = 5 (the only song in the list). 
	You must think about what is your expected result.
	Has the head pointer been updated? Is it NULL? Did the memory get released?
*/
void deleteTest(void) {
	Record data = { "Perry, Katy", "Witness", "Chained to the Rhythm", "pop", {4, 36}, 3, 5 };
	
	// create a list with only 1 node (the test node)
	Node* pHead = NULL;
	insertAtFront(&pHead, data);
	Node* pTarget = pHead;

	// delete the node
	int success = deleteNode(&pHead, pTarget);
	
	if (pTarget->record.artist != data.artist) printf("Test for releasing memory when deleting a node passed!\n");
	else printf("Test for releasing memory when when deleting a node failed\n");

	if (pHead == NULL) printf("Test for updating pHead when deleting a node passed!\n");
	else printf("Test for updating pHead when deleting a node failed\n");
}

/*
	For the shuffle test function, you must provide a test case with the following test point: play order = 3, 1, 2.
	List state = you provide 3 songs. Does the shuffle play in the correct order?
*/
void shuffleTest(void) {
	Record data1 = { "Perry, Katy", "Witness", "Chained to the Rhythm", "pop", {4, 36}, 3, 5 }; // song 3
	Record data2 = { "Perry, Katy", "Witness", "D Rose", "pop", {4, 36}, 3, 5 }; // song 2
	Record data3 = { "Perry, Katy", "Witness", "Frozen Burritos", "pop", {4, 36}, 3, 5 }; // song 1

	// create the list
	Node* pHead = NULL;
	insertAtFront(&pHead, data1);
	insertAtFront(&pHead, data2);
	insertAtFront(&pHead, data3);

	int playOrder[] = { 3, 1, 2 };

	shufflePlay(pHead, 3, playOrder);
}