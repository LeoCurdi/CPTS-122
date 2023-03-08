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
#include "header.h"

/*
	loads the list by reading each line from the file, creating a node, and inserting them at the front of the list
	if an artist name consists of two words encased in quotes, function removes quotes
*/
void loadList(Node** pHead) {
	// open the data file with mode read
	FILE* infile = fopen("musicPlayList.csv", "r");
	char fileline[100];
	Record record;
	
	// checks if the file opened successfully
	if (infile != NULL) {
		while (!feof(infile)) {
			// read a line
			fgets(fileline, 100, infile);

			// parce the data into the record struct variable. check for names in quotes with commas init
			if (*(fileline) == '"') {
				strcpy(record.artist, strtok((fileline + 1), "\"")); // we're passing in an address for fileline[1] so we dont need the star here
			}
			else {
				strcpy(record.artist, strtok(fileline, ","));
			}

			// parce the rest of the data
			strcpy(record.albumTitle, strtok(NULL, ","));
			strcpy(record.songTitle, strtok(NULL, ","));
			strcpy(record.genre, strtok(NULL, ","));
			record.length.minutes = atoi(strtok(NULL, ":"));
			record.length.seconds = atoi(strtok(NULL, ","));
			record.totalPlays = atoi(strtok(NULL, ","));
			record.rating = atoi(strtok(NULL, "\n"));
			
			// store the data by inserting it at the front of the list
			insertAtFront(pHead, record);
		}
	}
	// close the data file when done
	fclose(infile);
}

/*
	The “store” command writes the current records, in the dynamic doubly linked list, to the musicPlayList.csv file. 
	The store will completely overwrite the previous contents in the file.
	Also adds quotes back to artist names containing first and last
*/
void store(Node* pHead) {
	FILE* infile = fopen("musicPlayList.csv", "w");
	Node* pCur = pHead->pPrev; // pCur now points to the end of the list
	int lap = 0;
	// were at the end of the list. print backwards
	while (lap == 0) {
		// check if quotes need to be added
		int needsQuotes = 0;
		for (int i = 0; i < strlen(pCur->record.artist); i++) {
			if (*(pCur->record.artist + i) == ',') {
				needsQuotes = 1;
			}
		}
		// add quotes
		if (needsQuotes == 1) {
			// add a quote to the end
			*(pCur->record.artist + strlen(pCur->record.artist) + 1) = '\0';
			*(pCur->record.artist + strlen(pCur->record.artist)) = '"';

			// shift everything and add a quote to the beginning
			for (int i = strlen(pCur->record.artist); i >= 0; i--) {
				*(pCur->record.artist + i + 1) = *(pCur->record.artist + i);
			}
			*(pCur->record.artist) = '"';
		}

		fprintf(infile, "%s,%s,%s,%s,%d:%d,%d,%d", pCur->record.artist, pCur->record.albumTitle, pCur->record.songTitle, pCur->record.genre, pCur->record.length.minutes, pCur->record.length.seconds, pCur->record.totalPlays, pCur->record.rating);
		if (pCur != pHead) { // make sure to put in a new line char at all lines except the last. use pPrev since the list is double linked and were printing in reverse order from the back
			fprintf(infile, "\n");
		}
		// if we've printed an entire iteration of the list
		if (pCur == pHead) lap++;

		pCur = pCur->pPrev; // traverse backwards
	}

	fclose(infile);
}

/*
	gives the user a choice to print to the sscreen either all records or records by a specific artist
*/
void display(Node* pHead) {
	int displayChoice = 0, numSongsPrinted;
	char targetArtist[50];
	do {
		printf("Which songs would you like to display?\n");
		printf("\t1. all songs\n\t2. songs by a certain artist\n");
		scanf("%d", &displayChoice);
		system("cls");
	} while (displayChoice < 1 || displayChoice > 2);
	if (displayChoice == 1) {
		numSongsPrinted = printList2(pHead);
		if (numSongsPrinted == 0) {
			system("cls");
			printf("\t!! List is empty!\n\n");
		}
	}
	else {
		printf("Which artist would you like to display songs by?\nIf artist name is two words use format: Last, First   ");
		scanf(" %[^\n]s", &targetArtist);
		numSongsPrinted = printByArtist(pHead, targetArtist);
		if (numSongsPrinted == 0) {
			system("cls");
			printf("\t!! No songs were found!\n\n");
		}
	}
}

/*
	The “insert” command prompts the user for the details of a new record. 
	The prompt requests the artist name, album title, song title, genre, song length, number of times played, and rating. 
	The new record is inserted at the front of the list.
*/
void insert(Node** pHead) {
// make a struct for the new data
	Record record;
	char newString[50];
	int newData;

// get the new data from the user
	// get the strings
	printf("Enter the artist name:   ");
	scanf(" %[^\n]s", &newString);
	strcpy(record.artist, newString);
	printf("Enter the album title:   ");
	scanf(" %[^\n]s", &newString);
	strcpy(record.albumTitle, newString);
	printf("Enter the song title:   ");
	scanf(" %[^\n]s", &newString);
	strcpy(record.songTitle, newString);
	printf("Enter the genre:   ");
	scanf(" %[^\n]s", &newString);
	strcpy(record.genre, newString);

	// get the time
	printf("Enter the song length. Use format: mm:ss   ");
	scanf(" %[^\n]s", &newString);
	// parce the song length
	record.length.minutes = atoi(strtok(newString, ":"));
	record.length.seconds = atoi(strtok(NULL, "\0"));

	// get the ints
	do {
		printf("Enter the number of times played:   ");
		scanf("%d", &newData);
		if (newData < 0) {
			system("cls");
			printf("\t!! ERROR: Value cannot be negative\n\n");
		}
	} while (newData < 0);
	record.totalPlays = newData;

	do {
		printf("Enter the rating:   ");
		scanf("%d", &newData);
		if (newData < 1 || newData > 5) {
			system("cls");
			printf("\t!! ERROR: Rating must be 1-5\n\n");
		}
	} while (newData < 1 || newData > 5);
	record.rating = newData;

// insert it at the front
	insertAtFront(pHead, record);

	system("cls");
	printf("\t!! Value updated\n\n");
}

/*
	The “delete” command prompts the user for a song title, and removes the matching record from the list. 
	If the song title does not exist, then the list remains unchanged.
*/
void delete(Node** pHead) {
	char targetSongName[50];
	// get the song from the user
	printf("Enter the name of the song you'd like to delete:   ");
	scanf(" %[^\n]s", &targetSongName);

	Node* pCur = *pHead;
	int lap = 0; // cuz circular list
	// find the song or if it doesnt exist
	while (strcmp(pCur->record.songTitle, targetSongName) != 0 && lap == 0) { // find the song. while the song hasnt been found && we havent lapped the list
		pCur = pCur->pNext;
		if (pCur == *pHead) lap = 1;
	}
	// if the song doesnt exist
	if (lap == 1 && strcmp(pCur->record.songTitle, targetSongName) != 0) { // if we reached the end of the list and the song name still doesnt match
		system("cls");
		printf("\t!! song doesn't exist\n\n");
	}
	// if it exists, delete it (deleting pCur)
	else {
		int success = deleteNode(pHead, pCur);
		system("cls");
		printf("\t!! Song deleted\n\n");
	}
}

/*
	The “edit” command must allow the user to find a record in the list by artist. 
	If there are multiple records with the same artist, then the program prompts the user which one to edit. 
	The user may modify all of the attributes in the record.
*/
void edit(Node* pHead) {
	char targetArtist[50], attributeToEdit[20], targetSongName[50], newValueChar[50];
	int numSongsByArtist = 0, newValueInt, newMin, newSec;

	printf("Which song would you like to edit? (enter an artist)\nIf artist name is two words use format: Last, First   ");
	scanf(" %[^\n]s", &targetArtist);
	top:
	numSongsByArtist = printByArtist(pHead, targetArtist);
	int lap = 0; // bc its a circular list
	if (numSongsByArtist == 0) {
		system("cls");
		printf("\t!! Artist not found!\n\n");
	}
	else if (numSongsByArtist > 1) {
		// ask the user which song to edit
		printf("Enter the name of the song you'd like to edit:   ");
		scanf(" %[^\n]s", &targetSongName);
		// find the song
		Node* pCur = pHead;
		while (strcmp(pCur->record.songTitle, targetSongName) != 0 && lap == 0) { // find the song. while the song hasnt been found && ist not the last in the list
			pCur = pCur->pNext;
			if (pCur == pHead) lap++;
		}
		// if the song doesnt exist
		if (strcmp(pCur->record.songTitle, targetSongName) != 0 && lap != 0) {
			system("cls");
			printf("\t!! No songs were found!\n\n");
			goto top;
		}
		// ask the user which attribute to edit
		printf("\nWhich attribute would you like to edit?   ");
		scanf(" %[^\n]s", &attributeToEdit);
		// edit the specified attribute
		if (strcmp(attributeToEdit, "artist") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.artist, newValueChar);
		}
		else if (strcmp(attributeToEdit, "album") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.albumTitle, newValueChar);
		}
		else if (strcmp(attributeToEdit, "song") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.songTitle, newValueChar);
		}
		else if (strcmp(attributeToEdit, "genre") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.genre, newValueChar);
		}
		else if (strcmp(attributeToEdit, "length") == 0) {
			printf("\nEnter new value (m:ss):   ");
			scanf("%d:%d", &newMin, &newSec);
			pCur->record.length.minutes = newMin, pCur->record.length.seconds = newSec;
		}
		else if (strcmp(attributeToEdit, "plays") == 0) {
			do {
				printf("\nEnter new value:   ");
				scanf("%d", &newValueInt);
				system("cls");
				if (newValueInt < 0) printf("\t!! ERROR: value cannot be negative\n\n");
			} while (newValueInt < 0);
			pCur->record.totalPlays = newValueInt;
		}
		else if (strcmp(attributeToEdit, "rating") == 0) {
			do {
				printf("\nEnter new value:   ");
				scanf("%d", &newValueInt);
				system("cls");
				if (newValueInt < 1 || newValueInt > 5) printf("\t!! ERROR: Rating must be 1-5\n\n");
			} while (newValueInt < 1 || newValueInt > 5);
			pCur->record.rating = newValueInt;
		}
		else {
			system("cls");
			printf("\t!! invalid entry\n\n");
			goto top;
		}
		system("cls");
		printf("\t!! Value updated\n\n");
	}
	else {
		// find the song
		Node* pCur = pHead;
		while (strcmp(pCur->record.artist, targetArtist) != 0 && lap == 0) { // find the song. while the song hasnt been found && ist not the last in the list
			pCur = pCur->pNext;
			if (pCur == pHead) lap++;
		}
		// ask the user which attribute to edit
		printf("\nWhich attribute would you like to edit?   ");
		scanf(" %[^\n]s", &attributeToEdit);
		// edit the attribute
		if (strcmp(attributeToEdit, "artist") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.artist, newValueChar);
		}
		else if (strcmp(attributeToEdit, "album") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.albumTitle, newValueChar);
		}
		else if (strcmp(attributeToEdit, "song") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.songTitle, newValueChar);
		}
		else if (strcmp(attributeToEdit, "genre") == 0) {
			printf("\nEnter new value:   ");
			scanf(" %[^\n]s", &newValueChar);
			strcpy(pCur->record.genre, newValueChar);
		}
		else if (strcmp(attributeToEdit, "length") == 0) {
			printf("\nEnter new value (m:ss):   ");
			scanf("%d:%d", &newMin, &newSec);
			pCur->record.length.minutes = newMin, pCur->record.length.seconds = newSec;
		}
		else if (strcmp(attributeToEdit, "plays") == 0) {
			do {
				printf("\nEnter new value:   ");
				scanf("%d", &newValueInt);
				system("cls");
				if (newValueInt < 0) printf("\t!! ERROR: value cannot be negative\n\n");
			} while (newValueInt < 0);
			pCur->record.totalPlays = newValueInt;
		}
		else if (strcmp(attributeToEdit, "rating") == 0) {
			do {
				printf("\nEnter new value:   ");
				scanf("%d", &newValueInt);
				system("cls");
				if (newValueInt < 1 || newValueInt > 5) printf("\t!! ERROR: Rating must be 1-5\n\n");
			} while (newValueInt < 1 || newValueInt > 5);
			pCur->record.rating = newValueInt;
		}
		else {
			system("cls");
			printf("\t!! invalid entry\n\n");
			goto top;
		}
		system("cls");
		printf("\t!! Value updated\n\n");
	}
}

/*
	The “sort” command prompts the user for 4 different methods to sort the records in the list. These include:
	1.	Sort based on artist (A-Z)
	2.	Sort based on album title (A-Z)
	3.	Sort based on rating (1-5)
	4.	Sort based on times played (largest-smallest)
	Once a sort method is selected by the user, the sort is performed on the records in the list.

	precondition: all names must start with uppercase bc we're comparing ascii values. lowercase names will be placed after uppercase names
*/
void sort(Node** pHead) {
	// check if list is empty
	if (*pHead == NULL) {
		system("cls");
		printf("\t!! List is empty!\n\n");
		return;
	}

	// if not empty, get user selection
	int menuSelection = 0;
	do {
		system("cls");
		printf("Select from the following sort methods by entering a number (1-4):\n\t1.  Sort based on artist (A-Z)\n\t2.  Sort based on album title (A-Z)\n\t3.  Sort based on rating (1-5)\n\t4.  Sort based on times played (largest-smallest)\n\n");
		scanf("%d", &menuSelection);
	} while (menuSelection < 1 || menuSelection > 4);

	// get length of list
	int lap = 0, listLength = 0;
	Node* pCur = *pHead;
	while (lap == 0) {
		pCur = pCur->pNext;
		if (pCur == *pHead) lap = 1;
		listLength++;
	}

// bubble sort
	// sort by artist
	if (menuSelection == 1) {
		Node* pPrev, * p1, * p2, * pNext; // having a circular list makes this so much easier
		for (int i = 0; i < listLength - 1; i++) {
			pPrev = (*pHead)->pPrev, p1 = *pHead, p2 = (*pHead)->pNext, pNext = p2->pNext; // have to reset these bad boys every round
			for (int j = 0; j < listLength - i - 1; j++) {
				if (strcmp(p1->record.artist, p2->record.artist) > 0) { // if p1 > p2, they need to be swapped
					if (p1 == *pHead) {// if we need to update pHead
						*pHead = p2;
					}
					// had to draw diagrams to figure out all this shit
					pPrev->pNext = p2;
					p2->pPrev = pPrev;

					p1->pNext = pNext;
					pNext->pPrev = p1;

					p2->pNext = p1;
					p1->pPrev = p2;

					// move to the next nodes. p1 does not need to be updated
					pPrev = p2;
					p2 = pNext;
					pNext = pNext->pNext;
				}
				else {
					pPrev = p1;
					p1 = p2;
					p2 = pNext;
					pNext = pNext->pNext; // jesus with these fucking pointers
				}
			}
		}
	}
	// sort by album
	if (menuSelection == 2) {
		Node* pPrev, * p1, * p2, * pNext; // having a circular list makes this so much easier
		for (int i = 0; i < listLength - 1; i++) {
			pPrev = (*pHead)->pPrev, p1 = *pHead, p2 = (*pHead)->pNext, pNext = p2->pNext; // have to reset these bad boys every round
			for (int j = 0; j < listLength - i - 1; j++) {
				if (strcmp(p1->record.albumTitle, p2->record.albumTitle) > 0) { // if p1 > p2, they need to be swapped
					if (p1 == *pHead) {// if we need to update pHead
						*pHead = p2;
					}
					// had to draw diagrams to figure out all this shit
					pPrev->pNext = p2;
					p2->pPrev = pPrev;

					p1->pNext = pNext;
					pNext->pPrev = p1;

					p2->pNext = p1;
					p1->pPrev = p2;

					// move to the next nodes. p1 does not need to be updated
					pPrev = p2;
					p2 = pNext;
					pNext = pNext->pNext;
				}
				else {
					pPrev = p1;
					p1 = p2;
					p2 = pNext;
					pNext = pNext->pNext; // jesus with these fucking pointers
				}
			}
		}
	}
	// sort by rating (low to high)
	if (menuSelection == 3) {
		Node* pPrev, * p1, * p2, * pNext; // having a circular list makes this so much easier
		for (int i = 0; i < listLength - 1; i++) {
			pPrev = (*pHead)->pPrev, p1 = *pHead, p2 = (*pHead)->pNext, pNext = p2->pNext; // have to reset these bad boys every round
			for (int j = 0; j < listLength - i - 1; j++) {
				if (p1->record.rating > p2->record.rating) { // if p1 > p2, they need to be swapped
					if (p1 == *pHead) {// if we need to update pHead
						*pHead = p2;
					}
					// had to draw diagrams to figure out all this shit
					pPrev->pNext = p2;
					p2->pPrev = pPrev;

					p1->pNext = pNext;
					pNext->pPrev = p1;

					p2->pNext = p1;
					p1->pPrev = p2;

					// move to the next nodes. p1 does not need to be updated
					pPrev = p2;
					p2 = pNext;
					pNext = pNext->pNext;
				}
				else {
					pPrev = p1;
					p1 = p2;
					p2 = pNext;
					pNext = pNext->pNext; // jesus with these fucking pointers
				}
			}
		}
	}
	// sort by times played (high to low)
	if (menuSelection == 4) {
		Node* pPrev, * p1, * p2, * pNext; // having a circular list makes this so much easier
		for (int i = 0; i < listLength - 1; i++) {
			pPrev = (*pHead)->pPrev, p1 = *pHead, p2 = (*pHead)->pNext, pNext = p2->pNext; // have to reset these bad boys every round
			for (int j = 0; j < listLength - i - 1; j++) {
				if (p1->record.totalPlays < p2->record.totalPlays) { // if p1 < p2, they need to be swapped
					if (p1 == *pHead) {// if we need to update pHead
						*pHead = p2;
					}
					// had to draw diagrams to figure out all this shit
					pPrev->pNext = p2;
					p2->pPrev = pPrev;

					p1->pNext = pNext;
					pNext->pPrev = p1;

					p2->pNext = p1;
					p1->pPrev = p2;

					// move to the next nodes. p1 does not need to be updated
					pPrev = p2;
					p2 = pNext;
					pNext = pNext->pNext;
				}
				else {
					pPrev = p1;
					p1 = p2;
					p2 = pNext;
					pNext = pNext->pNext; // jesus with these fucking pointers
				}
			}
		}
	}

	system("cls");
	printf("\t!! List sorted!\n\n");
}

/*
	allows the user to assign a value of 1 – 5 to a song. The rating will replace the previous rating.
	if the user enters an invalid name, they will be prompted to try again

	precondition: list has to have at least one song, otherwise the user wont be able to enter a valid name and the function will turn into an infinite loop
*/
void rate(Node* pHead) {
	char targetSongName[50];
	// get the song from the user
	printf("Enter the name of the song you'd like to rate:   ");
	scanf(" %[^\n]s", &targetSongName);

	Node* pCur = pHead;
	int lap = 0; // cuz circular list
	// find it
	while (strcmp(pCur->record.songTitle, targetSongName) != 0 && lap == 0) { // find the song. while the song hasnt been found && ist not the last in the list
		pCur = pCur->pNext;
		if (pCur == pHead) lap = 1;
	}
	// if the song doesnt exist
	if (lap == 1 && strcmp(pCur->record.songTitle, targetSongName) != 0) { // if we reached the end of the list and the song name still doesnt match
		system("cls");
		printf("\t!! song doesn't exist\n\n");
	}
	// if it exists, change it
	else {
		int rate;
		do {
			printf("Enter your rating:   ");
			scanf("%d", &rate);
			system("cls");
			if (rate < 1 || rate > 5) printf("\t!! ERROR: Rating must be 1-5\n\n");
		} while (rate < 1 || rate > 5);
		pCur->record.rating = rate;
		system("cls");
		printf("\t!! Value updated\n\n");
	}
}

/*
	The “play” command allows the user to select a song, and starts “playing” each song in order from the current song. 
	“Playing” the song in this program means displaying the contents of the record that represents the song for a short period of time, 
	clearing the screen and showing the next record in the list, etc. This continues until all songs have been played.
*/
void play(Node* pHead) {
	char songName[50];
	// get the song from the user
	printf("Enter the name of the song you'd like to start playing from:   ");
	scanf(" %[^\n]s", &songName);

	// get to the selected song
	Node* pCur = pHead;
	int lap = 0;
	while (strcmp(pCur->record.songTitle, songName) != 0 && lap == 0) {
		pCur = pCur->pNext;
		if (pCur == pHead) lap = 1;
	}
	if (lap == 1) { // we didnt find the song (user typed wrong name)
		system("cls");
		printf("\t!! song doesn't exist\n\n");
	}

	// play the remainder of the list
	else {
		Node* pStart = pCur; // mark where we started in the list
		lap = 0;
		for (int i = 3; i > 0; i--) {
			system("cls");
			printf("Playing in %d", i);
			Sleep(1000);
		}
		while (lap == 0) {
			system("cls");
			printf("\n\t%s, %s, %s, %s, %d:%d, %d, %d\n", pCur->record.artist, pCur->record.albumTitle, pCur->record.songTitle, pCur->record.genre, pCur->record.length.minutes, pCur->record.length.seconds, pCur->record.totalPlays, pCur->record.rating);
			Sleep(1000);
			pCur = pCur->pNext;
			if (pCur == pStart) lap = 1;
		}
		system("cls");
	}
}

/*
	The “shuffle” command must provide a random order in which the songs are played. This command must not modify 
	the links in the list. It must just specify the order in which songs are played, based on the position of the song in the list. 
	For example, let’s say we have a list with 5 songs at positions 1 – 5 in the list, shuffle must generate 
	an order 1 – 5 in which the songs are played. An order 2, 5, 3, 1, 4 would require that the second song in the 
	list is played first, the fifth song in the list is played second, the third song in the list is played third, 
	the first song in the list is played fourth, and the fourth song in the list is played fifth. 
	The songs are accessed by traversing the list both forwards and backwards to satisfy the order. Hence, the need for a doubly linked list!
*/
void shuffle(Node* pHead) {
	// check if list is empty
	if (pHead == NULL) {
		system("cls");
		printf("\t!! List is empty!\n\n");
		return;
	}

	// get length of list
	int lap = 0, listLength = 0;
	Node* pCur = pHead;
	while (lap == 0) {
		pCur = pCur->pNext;
		if (pCur == pHead) lap = 1;
		listLength++;
	}

	// dynamically allocate an array the size of the list
	int* shuffleOrderArray = (int*)malloc(listLength * sizeof(int));

	// fill the array with random non repeating numbers between 1 and length of list
	srand((unsigned int)time(NULL));
	for (int i = 0; i < listLength; i++) { // fill the array
		int nonRepeatingNum, randomInt;
		// dont accept the random number unless its a new number
		do {
			nonRepeatingNum = 0;
			// get a random num
			randomInt = rand() % listLength + 1;
			// check if its non repeating
			for (int k = 0; k < listLength; k++) {
				if (*(shuffleOrderArray + k) == randomInt) nonRepeatingNum = 1;
			}
		} while (nonRepeatingNum == 1);
		// once u have a non repeating number, run it
		*(shuffleOrderArray + i) = randomInt;
	}
	// now that we have our shuffle order, play dat
	for (int i = 3; i > 0; i--) {
		system("cls");
		printf("Shuffle playing in %d", i);
		Sleep(1000);
	}

	shufflePlay(pHead, listLength, shuffleOrderArray);

	system("cls");
}

/*
	once the shuffle order has been determined, this lil function does the actual shuffle playing
*/
void shufflePlay(Node* pHead, int listLength, int* shuffleOrderArray) {
	Node* pCur = pHead;
	for (int i = 0; i < listLength; i++) {
		pCur = pHead;
		// get to the song and play it
		for (int j = 0; j < *(shuffleOrderArray + i) - 1; j++) {
			pCur = pCur->pNext;
		}
		system("cls");
		printf("\n\t%s, %s, %s, %s, %d:%d, %d, %d\n", pCur->record.artist, pCur->record.albumTitle, pCur->record.songTitle, pCur->record.genre, pCur->record.length.minutes, pCur->record.length.seconds, pCur->record.totalPlays, pCur->record.rating);
		Sleep(1000);
	}
}


/*
	Allocates space on the heap for, and creates a new node. used whenever adding a new item to a linked list
*/
Node* makeNode(Record record) {
	// create space for a node
	Node* pMem = (Node*)malloc(sizeof(Node));

	// if we successfully allocated memory
	if (pMem != NULL) {
		// shove all the new data into the new node
		strcpy(pMem->record.artist, record.artist);
		strcpy(pMem->record.albumTitle, record.albumTitle);
		strcpy(pMem->record.songTitle, record.songTitle);
		strcpy(pMem->record.genre, record.genre);
		pMem->record.length.minutes = record.length.minutes;
		pMem->record.length.seconds = record.length.seconds;
		pMem->record.totalPlays = record.totalPlays;
		pMem->record.rating = record.rating;

		// initialize the links
		pMem->pNext = NULL;
		pMem->pPrev = NULL;
	}

	// return the node
	return pMem;
}

/*
	calls makeNode(), inserts the new node at the front of the list, and returns 1 for successfully allocating space for a node; 0 otherwise
*/
int insertAtFront(Node** pHead, Record record) {
	// make sure data is actually eligible to be inserted
	if (record.totalPlays < 0 || record.rating < 1 || record.rating > 5) return 0;

	// make a node
	Node* pNew = makeNode(record), *pNext = *pHead;

	// if we failed to make space for a node. dont complete the function
	if (pNew == NULL) {
		return 0;
	}

	// if the list is empty. pPrev and pNext will both point to pNew (circular implementation), pHead will point to current node
	if (*pHead == NULL) { 
		pNew->pPrev = pNew, pNew->pNext = pNew; // circular
		*pHead = pNew; // the indirect value of *pHead = the direct value of pHead = the direct value of pMem
	}
	// if the list is not empty. the next nodes pPrev will point to the new node, new node pPrev will point to the end node, end node's pNext will point to the new node. pNext will point to the next node (currently pHead), pHead will point to current node
	else {
		pNew->pPrev = (*pHead)->pPrev, pNew->pPrev->pNext = pNew; // circular
		pNext->pPrev = pNew; // this line is key for successfully double linking the list
		pNew->pNext = pNext;
		*pHead = pNew;
	}

	return 1;
}

/*
	calls makeNode() and inserts the new node at the end of the list
	pHead will be modified if the list is empty
	this function is not currently in use in this program
*/
void insertAtEnd(Node** pHead, Record record) {
	// make a node
	Node* pNew = makeNode(record), *pCur = (*pHead)->pPrev; // start pCur right at the end node

	// if we successfully made a node
	if (pNew != NULL) {
		// if list is empty, point pHead and pTail to the new node
		if (*pHead == NULL) {
			// connect the new node to the list
			pNew->pPrev = pNew, pNew->pNext = pNew; // circular
			*pHead = pNew; // the indirect value of *pHead = the direct value of pHead = the direct value of pMem
		}
		// if list is not empty. pHead remains unchanged, pTail points to the new node
		else {
			// connect the new node to the list
			pNew->pNext = *pHead, (*pHead)->pPrev = pNew; // circular
			pCur->pNext = pNew;
			pNew->pPrev = pCur;
		}
	}
}

/*
	Finds the target node and deletes it from a doubly linked circular list
	Accepts as a parameter a pointer to a node in the list that needs to be deleted
*/
int deleteNode(Node** pHead, Node* pTarget) {
	//Node* pCur = *pHead;
	//int lap = 0;
	//// find it
	//while (pCur != target && lap == 0) { // find it or stop after lapping the list
	//	pCur = pCur->pNext;
	//	if (pCur == *pHead) lap = 1;
	//}
	//// if it doesnt exist
	//if (pCur != target && lap == 1) return 0;

	//// if we found it, delete it (deleting pCur)
	//pCur->pPrev->pNext = pCur->pNext;
	//pCur->pNext->pPrev = pCur->pPrev;
	//// if we're deleting pHead, prolly wanna update that
	//if (pCur == *pHead) {
	//	*pHead = pCur->pNext;
	//}
	//free(pCur);
// turns out its a lot easier than that since weve already found the target (we're literally passing the target in as a pointer to a node lmfao)
	// we need a case for if were deleting the only item in the list
	if (pTarget->pNext == pTarget) {
		*pHead = NULL;
	}
	else {
		pTarget->pPrev->pNext = pTarget->pNext;
		pTarget->pNext->pPrev = pTarget->pPrev;

		if (pTarget == *pHead) {
			*pHead = pTarget->pNext;
		}
	}

	free(pTarget);
	
	return 1;
}


// recursively prints the list with no formatting, returns nothing. modified to accommodate circular list
// input: must input pHead for pCur parameter when calling  if u want to print the whole list and 0 for lap.
void printList(Node* pHead, Node* pCur, int lap) {
	if (lap != 1) {
		printf("%s, %s, %s, %s, %d:%d, %d, %d\n", pCur->record.artist, pCur->record.albumTitle, pCur->record.songTitle, pCur->record.genre, pCur->record.length.minutes, pCur->record.length.seconds, pCur->record.totalPlays, pCur->record.rating);
		pCur = pCur->pNext;
		if (pCur == pHead) lap = 1;
		printList(pHead, pCur, lap); // recursive call
	}
}
// prints the list with formatting, returns the number of songs printed
int printList2(Node* pHead) {
	printf("\n     artist                   album                     song                 genre        length     plays    rating\n");
	int i = 0; // use i to make the list numbered
	int lap = 0; // use lap to only go through the list once, since its circular
	Node* pCur = pHead;

	// check if the list is empty
	if (pHead == NULL) return 0;

	while (lap == 0) {
		i++;
		if (i > 9) printf("(%d) ", i);
		else printf("(%d)  ", i);
		printf("|%s ", pCur->record.artist);
		if (strlen(pCur->record.artist) < 23) {
			for (int i = 0; i < 23 - strlen(pCur->record.artist); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		printf("|%s ", pCur->record.albumTitle);
		if (strlen(pCur->record.albumTitle) < 24) {
			for (int i = 0; i < 24 - strlen(pCur->record.albumTitle); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		printf("|%s ", pCur->record.songTitle);
		if (strlen(pCur->record.songTitle) < 19) {
			for (int i = 0; i < 19 - strlen(pCur->record.songTitle); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		printf("|%s ", pCur->record.genre);
		if (strlen(pCur->record.genre) < 11) {
			for (int i = 0; i < 11 - strlen(pCur->record.genre); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		if (pCur->record.length.seconds < 10) printf("|%d:0%d     ", pCur->record.length.minutes, pCur->record.length.seconds);
		else printf("|%d:%d     ", pCur->record.length.minutes, pCur->record.length.seconds);
		if (pCur->record.length.minutes < 10) {
			printf(" "); // use this to make the equal spacing in the columns
		}

		printf("|%d     ", pCur->record.totalPlays);
		if (pCur->record.totalPlays < 10) {
			printf("  "); // use this to make the equal spacing in the columns
		}
		else if (pCur->record.totalPlays < 100) {
			printf(" "); // use this to make the equal spacing in the columns
		}

		printf("|%d", pCur->record.rating);

		putchar('\n');
		pCur = pCur->pNext;
		if (pCur == pHead) lap++; // dont start printing from pHead again
	}
	putchar('\n');
	return i; // return the number of songs printed
}
// prints the songs by a specific artist with formatting, returns the number of songs printed
int printByArtist(Node* pHead, char *targetArtist) {
	printf("\n     artist                   album                     song                 genre        length     plays    rating\n");
	int i = 0; // use i to make the list numbered
	int lap = 0; // use lap to only go through the list once, since its circular
	Node* pCur = pHead;

	// check if the list is empty
	if (pHead == NULL) return 0;

	while (lap == 0) {
		if (strcmp(pCur->record.artist, targetArtist) == 0) { // only print songs by desired artist
			i++;
			if (i > 9) printf("(%d) ", i);
			else printf("(%d)  ", i);
			printf("|%s ", pCur->record.artist);
			if (strlen(pCur->record.artist) < 23) {
				for (int i = 0; i < 23 - strlen(pCur->record.artist); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			printf("|%s ", pCur->record.albumTitle);
			if (strlen(pCur->record.albumTitle) < 24) {
				for (int i = 0; i < 24 - strlen(pCur->record.albumTitle); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			printf("|%s ", pCur->record.songTitle);
			if (strlen(pCur->record.songTitle) < 19) {
				for (int i = 0; i < 19 - strlen(pCur->record.songTitle); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			printf("|%s ", pCur->record.genre);
			if (strlen(pCur->record.genre) < 11) {
				for (int i = 0; i < 11 - strlen(pCur->record.genre); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			if (pCur->record.length.seconds < 10) printf("|%d:0%d     ", pCur->record.length.minutes, pCur->record.length.seconds);
			else printf("|%d:%d     ", pCur->record.length.minutes, pCur->record.length.seconds);
			if (pCur->record.length.minutes < 10) {
				printf(" "); // use this to make the equal spacing in the columns
			}

			printf("|%d     ", pCur->record.totalPlays);
			if (pCur->record.totalPlays < 10) {
				printf("  "); // use this to make the equal spacing in the columns
			}
			else if (pCur->record.totalPlays < 100) {
				printf(" "); // use this to make the equal spacing in the columns
			}

			printf("|%d", pCur->record.rating);

			putchar('\n');
		}
		pCur = pCur->pNext;
		if (pCur == pHead) lap++; // dont start printing from pHead again
	}
	putchar('\n');
	return i;
}