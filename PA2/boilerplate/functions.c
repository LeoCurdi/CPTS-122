/*
	Author: Leo Curdi
	Date: 1-25
	Class: cpts 122, lab section 2
	TA: Harry
	Assignment: PA 2
	Description:
		This program is a digital music manager (DMM). It loads song information from a file, stores it in a linked list,
		and gives the user options for viewing and modifying the data
*/
#include "header.h"

/*
	loads the list by reading each line from the file, creating a node, and inserting them at the front of the list
	if an artist name consists of two words encased in quotes, function removes quotes
*/
void loadList(Node** pHead, Node** pTail) {
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
			// split the string only if theres an even number of quotes behind it
			// but thats hard so for now use this hackaround since its easier
			if (*(fileline) == '"') {
				
				strcpy(record.artist, strtok((fileline + 1), "\"")); // we're passing in an address for fileline[1] so we dont need the star here

				//int i = 0, k = 0, l = 0;

				//// manually remove quotes for storing name
				//for (k = 0; k < strlen(fileline); k++) { // shift everthing left to remove the first quote
				//	*(fileline + k) = *(fileline + k + 1);
				//}
				//while (*(fileline + l) != '"') l++; // find second quote
				//while (*(fileline + l) != '\0') { // shift everything after the second quote left
				//	*(fileline + l) = *(fileline + l + 1);
				//	l++;
				//}

				//// manually string toke
				//while (*(fileline + i) != ',') i++; // find first comma
				//while (*(fileline + i + 1) != ',') i++; // find second comma
				//*(fileline + i + 1) = '\0'; // manually replace the second comma with null

				//strcpy(record.artist, fileline);
				//// hacky stuff
				//*(fileline + i + 1) = ','; // then put the comma back so i can start the string toke chain
				//strtok(fileline, ","); // burner
				//strtok(NULL, ","); // burner 2
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
			insertAtFront(pHead, pTail, record);
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
void store(Node* pHead, Node* pTail) {
	FILE* infile = fopen("musicPlayList.csv", "w");
	Node* pCur = pTail; // pCur now points to the end of the list

	// were at the end of the list. print backwards
	while (pCur != NULL) {
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
		if (pCur->pPrev != '\0') { // make sure to put in a new line char at all lines except the last. use pPrev since the list is double linked and were printing in reverse order from the back
			fprintf(infile, "\n");
		}
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
			printf("\t!! No songs were found!\n\n");
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
	if (numSongsByArtist == 0) {
		system("cls");
		printf("\t!! No songs were found!\n\n");
	}
	else if (numSongsByArtist > 1) {
		// ask the user which song to edit
		printf("Enter the name of the song you'd like to edit:   ");
		scanf(" %[^\n]s", &targetSongName);
		// find the song
		Node* pCur = pHead;
		while (strcmp(pCur->record.songTitle, targetSongName) != 0 && pCur->pNext != NULL) { // find the song. while the song hasnt been found && ist not the last in the list
			pCur = pCur->pNext;
		}
		// if the song doesnt exist
		if (strcmp(pCur->record.songTitle, targetSongName) != 0 && pCur->pNext == NULL) {
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
			printf("\nEnter new value:   ");
			scanf("%d", &newValueInt);
			pCur->record.totalPlays = newValueInt;
		}
		else if (strcmp(attributeToEdit, "rating") == 0) {
			printf("\nEnter new value:   ");
			scanf("%d", &newValueInt);
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
		while (strcmp(pCur->record.artist, targetArtist) != 0 && pCur->pNext != NULL) { // find the song. while the song hasnt been found && ist not the last in the list
			pCur = pCur->pNext;
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
			printf("\nEnter new value:   ");
			scanf("%d", &newValueInt);
			pCur->record.totalPlays = newValueInt;
		}
		else if (strcmp(attributeToEdit, "rating") == 0) {
			printf("\nEnter new value:   ");
			scanf("%d", &newValueInt);
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
	allows the user to assign a value of 1 – 5 to a song. The rating will replace the previous rating.
	if the user enters an invalid name, they will be prompted to try again
	precondition: list has to have at least one song, otherwise the user wont be able to enter a valid name and the function will turn into an infinite loop
*/
void rate(Node* pHead) {
	char targetSongName[50];
	top:
	// get the song from the user
	printf("Enter the name of the song you'd like to rate:   ");
	scanf(" %[^\n]s", &targetSongName);

	Node* pCur = pHead;
	// find it
	while (strcmp(pCur->record.songTitle, targetSongName) != 0 && pCur->pNext != NULL) { // find the song. while the song hasnt been found && ist not the last in the list
		pCur = pCur->pNext;
	}
	// if the song doesnt exist
	if (pCur->pNext == NULL && strcmp(pCur->record.songTitle, targetSongName) != 0) { // if we reached the end of the list and the song name still doesnt match
		system("cls");
		printf("\t!! song doesn't exist\n\n");
		goto top;
	}
	// if it exists, change it
	else {
		int rate;
		printf("Enter your rating:   ");
		scanf("%d", &rate);
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
	while (strcmp(pCur->record.songTitle, songName) != 0 && pCur != NULL) {
		pCur = pCur->pNext;
	}
	if (pCur == NULL) { // we didnt find the song (user typed wrong name)
		printf("\t!! song doesn't exist\n\n");
	}

	// play the remainder of the list
	else {
		for (int i = 3; i > 0; i--) {
			system("cls");
			printf("Playing in %d", i);
			Sleep(1000);
		}
		while (pCur != NULL) {
			system("cls");
			printf("\n\t%s, %s, %s, %s, %d:%d, %d, %d\n", pCur->record.artist, pCur->record.albumTitle, pCur->record.songTitle, pCur->record.genre, pCur->record.length.minutes, pCur->record.length.seconds, pCur->record.totalPlays, pCur->record.rating);
			Sleep(1500);
			pCur = pCur->pNext;
		}
		system("cls");
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
int insertAtFront(Node** pHead, Node** pTail, Record record) {
	// make a node
	Node* pMem = makeNode(record), *pNext = *pHead;

	// if we failed to make space for a node. dont complete the function
	if (pMem == NULL) {
		return 0;
	}

	// if the list is empty. pPrev and pNext will both stay null, pHead and pTail will point to current node
	if (*pHead == NULL) { 
		*pHead = pMem; // the indirect value of *pHead = the direct value of pHead = the direct value of pMem
		*pTail = pMem;
	}
	// if the list is not empty. the next nodes pPrev will point to the new node, new node pPrev will stay null, pNext will point to the next node (currently pHead), pHead will point to current node, pTail will remain unchanged
	else {
		pNext->pPrev = pMem; // this line is key for successfully double linking the list
		pMem->pNext = pNext;
		*pHead = pMem;
	}

	return 1;
}

/*
	calls makeNode() and inserts the new node at the end of the list
	pHead will be modified if the list is empty
	this function is not currently in use in this program
*/
void insertAtEnd(Node** pHead, Node** pTail, Record record) {
	// make a node
	Node* pMem = makeNode(record), *pCur = *pTail; // start pCur right at the end node

	// if we successfully made a node
	if (pMem != NULL) {
		// if list is empty, point pHead and pTail to the new node
		if (*pHead == NULL) {
			// connect the new node to the list
			*pHead = pMem; // the indirect value of *pHead = the direct value of pHead = the direct value of pMem
			*pTail = pMem;
		}
		// if list is not empty. pHead remains unchanged, pTail points to the new node
		else {
			// connect the new node to the list
			pCur->pNext = pMem;
			pMem->pPrev = pCur;
			// make sure to update pTail
			*pTail = pMem;
		}
	}
}


// recursively prints the list with no formatting, returns nothing
void printList(Node* pHead) {
	if (pHead != NULL) {
		printf("%s, %s, %s, %s, %d:%d, %d, %d\n", pHead->record.artist, pHead->record.albumTitle, pHead->record.songTitle, pHead->record.genre, pHead->record.length.minutes, pHead->record.length.seconds, pHead->record.totalPlays, pHead->record.rating);
		printList(pHead->pNext); // recursive call
	}
}
// prints the list with formatting, returns the number of songs printed
int printList2(Node* pHead) {
	printf("\n    artist                   album                     song                 genre        length     plays    rating\n");
	int i = 0; // use i to make the list numbered
	while (pHead != NULL) {
		i++;
		printf("(%d) |%s ", i, pHead->record.artist);
		if (strlen(pHead->record.artist) < 23) {
			for (int i = 0; i < 23 - strlen(pHead->record.artist); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		printf("|%s ", pHead->record.albumTitle);
		if (strlen(pHead->record.albumTitle) < 24) {
			for (int i = 0; i < 24 - strlen(pHead->record.albumTitle); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		printf("|%s ", pHead->record.songTitle);
		if (strlen(pHead->record.songTitle) < 19) {
			for (int i = 0; i < 19 - strlen(pHead->record.songTitle); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		printf("|%s ", pHead->record.genre);
		if (strlen(pHead->record.genre) < 11) {
			for (int i = 0; i < 11 - strlen(pHead->record.genre); i++) {
				printf(" "); // use this to make the equal spacing in the columns
			}
		}

		printf("|%d:%d     ", pHead->record.length.minutes, pHead->record.length.seconds);
		if (pHead->record.length.minutes < 10) {
			printf(" "); // use this to make the equal spacing in the columns
		}

		printf("|%d     ", pHead->record.totalPlays);
		if (pHead->record.totalPlays < 10) {
			printf("  "); // use this to make the equal spacing in the columns
		}
		else if (pHead->record.totalPlays < 100) {
			printf(" "); // use this to make the equal spacing in the columns
		}

		printf("|%d", pHead->record.rating);

		putchar('\n');
		pHead = pHead->pNext;
	}
	putchar('\n');
	return i; // return the number of songs printed
}
// prints the songs by a specific artist with formatting, returns the number of songs printed
int printByArtist(Node* pHead, char *targetArtist) {
	printf("\n    artist                   album                     song                 genre        length     plays    rating\n");
	int i = 0; // use i to make the list numbered
	while (pHead != NULL) {
		if (strcmp(pHead->record.artist, targetArtist) == 0) { // only print songs by desired artist
			i++;
			printf("(%d) |%s ", i, pHead->record.artist);
			if (strlen(pHead->record.artist) < 23) {
				for (int i = 0; i < 23 - strlen(pHead->record.artist); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			printf("|%s ", pHead->record.albumTitle);
			if (strlen(pHead->record.albumTitle) < 24) {
				for (int i = 0; i < 24 - strlen(pHead->record.albumTitle); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			printf("|%s ", pHead->record.songTitle);
			if (strlen(pHead->record.songTitle) < 19) {
				for (int i = 0; i < 19 - strlen(pHead->record.songTitle); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			printf("|%s ", pHead->record.genre);
			if (strlen(pHead->record.genre) < 11) {
				for (int i = 0; i < 11 - strlen(pHead->record.genre); i++) {
					printf(" "); // use this to make the equal spacing in the columns
				}
			}

			printf("|%d:%d     ", pHead->record.length.minutes, pHead->record.length.seconds);
			if (pHead->record.length.minutes < 10) {
				printf(" "); // use this to make the equal spacing in the columns
			}

			printf("|%d     ", pHead->record.totalPlays);
			if (pHead->record.totalPlays < 10) {
				printf("  "); // use this to make the equal spacing in the columns
			}
			else if (pHead->record.totalPlays < 100) {
				printf(" "); // use this to make the equal spacing in the columns
			}

			printf("|%d", pHead->record.rating);

			putchar('\n');
		}
		pHead = pHead->pNext;
	}
	putchar('\n');
	return i;
}