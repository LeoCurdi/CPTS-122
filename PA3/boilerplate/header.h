/*
	Author: Leo Curdi
	Date: 1-25
	Class: cpts 122, lab section 2
	TA: Harry
	Assignment: PA 3
	Description:
		This program is a digital music manager (DMM). It loads song information from a file, stores it in a linked list,
		and gives the user options for viewing and modifying the data
*/
#ifndef HEADER_H
#define HEADER_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

typedef struct duration {
	int minutes;
	int seconds;
} Duration;

typedef struct record {
	char artist[30];
	char albumTitle[30];
	char songTitle[30];
	char genre[15];
	Duration length;
	int totalPlays;
	int rating;
} Record;

typedef struct node {
	Record record;
	struct node* pNext;
	struct node* pPrev;
} Node;


/*
	loads the list by reading each line from the file, creating a node, and inserting them at the front of the list
	if an artist name consists of two words encased in quotes, function removes quotes
*/
void loadList(Node** pHead);

/*
	The “store” command writes the current records, in the dynamic doubly linked list, to the musicPlayList.csv file.
	The store will completely overwrite the previous contents in the file.
	Also adds quotes back to artist names containing first and last
*/
void store(Node* pHead);

/*
	gives the user a choice to print to the sscreen either all records or records by a specific artist
*/
void display(Node* pHead);

/*
	The “insert” command prompts the user for the details of a new record.
	The prompt requests the artist name, album title, song title, genre, song length, number of times played, and rating.
	The new record is inserted at the front of the list.
*/
void insert(Node** pHead);

/*
	The “delete” command prompts the user for a song title, and removes the matching record from the list.
	If the song title does not exist, then the list remains unchanged.
*/
void delete(Node** pHead);

/*
	The “edit” command must allow the user to find a record in the list by artist.
	If there are multiple records with the same artist, then the program prompts the user which one to edit.
	The user may modify all of the attributes in the record.
*/
void edit(Node* pHead);

/*
	The “sort” command prompts the user for 4 different methods to sort the records in the list. These include:
	1.	Sort based on artist (A-Z)
	2.	Sort based on album title (A-Z)
	3.	Sort based on rating (1-5)
	4.	Sort based on times played (largest-smallest)
	Once a sort method is selected by the user, the sort is performed on the records in the list.

	precondition: all names must start with uppercase bc we're comparing ascii values. lowercase names will be placed after uppercase names
*/
void sort(Node** pHead);

/*
	allows the user to assign a value of 1 – 5 to a song. The rating will replace the previous rating.
	if the user enters an invalid name, they will be prompted to try again
	
	precondition: list has to have at least one song, otherwise the user wont be able to enter a valid name and the function will turn into an infinite loop
*/
void rate(Node* pHead);

/*
	The “play” command allows the user to select a song, and starts “playing” each song in order from the current song.
	“Playing” the song in this program means displaying the contents of the record that represents the song for a short period of time,
	clearing the screen and showing the next record in the list, etc. This continues until all songs have been played.
*/
void play(Node* pHead);

/*
	The “shuffle” command must provide a random order in which the songs are played. This command must not modify
	the links in the list. It must just specify the order in which songs are played, based on the position of the song in the list.
	For example, let’s say we have a list with 5 songs at positions 1 – 5 in the list, shuffle must generate
	an order 1 – 5 in which the songs are played. An order 2, 5, 3, 1, 4 would require that the second song in the
	list is played first, the fifth song in the list is played second, the third song in the list is played third,
	the first song in the list is played fourth, and the fourth song in the list is played fifth.
	The songs are accessed by traversing the list both forwards and backwards to satisfy the order. Hence, the need for a doubly linked list!
*/
void shuffle(Node* pHead);

/*
	once the shuffle order has been determined, this lil function does the actual shuffle playing
*/
void shufflePlay(Node* pHead, int listLength, int* shuffleOrderArray);



/*
	Allocates space on the heap for, and creates a new node. used whenever adding a new item to a linked list
*/
Node* makeNode(Record record);

/*
	calls makeNode(), inserts the new node at the front of the list, and returns 1 for successfully allocating space for a node; 0 otherwise
*/
int insertAtFront(Node** pHead, Record record);

/*
	calls makeNode() and inserts the new node at the end of the list
	pHead will be modified if the list is empty
	this function is not currently in use in this program
*/
void insertAtEnd(Node** pHead, Record record);

/*
	Finds the target node and deletes it from a doubly linked circular list
*/
int deleteNode(Node** pHead, Node* pTarget);


// recursively prints the list with no formatting, returns nothing. modified to accommodate circular list
// input: must input pHead for pCur parameter when calling  if u want to print the whole list and 0 for lap.
void printList(Node* pHead, Node* pCur, int lap);
// prints the list with formatting, returns the number of songs printed
int printList2(Node* pHead);
// prints the songs by a specific artist with formatting, returns the number of songs printed
int printByArtist(Node* pHead, char* targetArtist);

#endif