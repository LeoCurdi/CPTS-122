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
#include "header.h"

#ifndef TEST_H
#define TEST_H

/*
	For the insert test function, you must provide a test case with the following test point: artist name = “Perry, Katy”, 
	album title = “Witness”, song title = “Chained to the Rhythm”, genre = “pop”, song length = “4:36”, times played = -1, rating = 6. 
	List state = initially empty. You must think about what is your expected result? 
	Should you able to insert a song with -1 times played? Should you able to add a song with rating 6? 
	Is the head pointer of the list updated?
*/
void insertTest(void);

/*
	For the delete test function, you must provide a test case with the following test point: song title to 
	delete = “Chained to the Rhythm”. List state = artist name = “Perry, Katy”, album title = “Witness”, 
	song title = “Chained to the Rhythm”, genre = “pop”, song length = “4:36”, times played = 3, 
	rating = 5 (the only song in the list). You must think about what is your expected result? 
	Has the head pointer been updated? Is it NULL? Did the memory get released?
*/
void deleteTest(void);

/*
	For the shuffle test function, you must provide a test case with the following test point: play order = 3, 1, 2. 
	List state = you provide 3 songs. Does the shuffle play in the correct order?
*/
void shuffleTest(void);


#endif