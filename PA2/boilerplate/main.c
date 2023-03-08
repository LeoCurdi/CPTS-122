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
/*
	to do
		- work on ui, input validation

		- why is there a load option? data should be automtically loaded on startup. otherwise if you select store or exit without first selecting load you wipe the entire file.
		- am i supposed to do anything with the function success value?
*/
#include "header.h"

int main(void) {

	Node* pHead = NULL, *pTail = NULL; // initialize the head pointer for the list and the tail pointer

	int menuSelection = 0, isLoaded = 0, numSongsPrinted;
	while (menuSelection != 11 && menuSelection != 12) {
		// get user input with an input validation loop
		do {
			printf("Select from the following choices by entering a number (1-11):\n\t1.  load\n\t2.  store\n\t3.  display\n\t4.  insert\n\t5.  delete\n\t6.  edit\n\t7.  sort\n\t8.  rate\n\t9.  play\n\t10. shuffle\n\t11. save & quit\n\t12. quit w/o saving\n\n");
			scanf("%d", &menuSelection);
			system("cls");
			if (menuSelection < 1 || menuSelection > 12) printf("\t!! Enter a valid option (1-11)\n\n");
			if (isLoaded == 0 && !(menuSelection < 1 || menuSelection > 12)) {
				if (menuSelection > 1 && menuSelection < 12) {
					printf("\t!! WARNING !! list has not yet been loaded\n\n");
				}
			}
		} while (menuSelection < 1 || menuSelection > 12);

		// switch chain based on user input
		switch (menuSelection) {
		case 1: // load
			loadList(&pHead, &pTail); // reads in all the data from the file and puts together the linked list
			isLoaded = 1;
			printf("\t !! List has been successfully loaded!\n\n");
			break;
		case 2: // store
			store(pHead, pTail);
			break;
		case 3: // display
			display(pHead);
			break;
		case 4: // insert
			system("cls");
			printf("\t!! This feature is not currently available\n\n");
			break;
		case 5: // delete
			system("cls");
			printf("\t!! This feature is not currently available\n\n");
			break;
		case 6: // edit
			edit(pHead);
			break;
		case 7: // sort
			system("cls");
			printf("\t!! This feature is not currently available\n\n");
			break;
		case 8: // rate
			if (pHead != NULL) { // only call the function if theres actually songs in the list, rate will break if you call it with an empty list
				rate(pHead);
			}
			else {
				system("cls");
				printf("\t!! ERROR !! list is empty. Try loading it if you have not done so already\n\n");
			}
			break;
		case 9: // play
			if (pHead != NULL) { // only call the function if theres actually songs in the list, rate will break if you call it with an empty list
				play(pHead);
			}
			else {
				system("cls");
				printf("\t!! ERROR !! list is empty. Try loading it if you have not done so already\n\n");
			}
			break;
		case 10: // shuffle
			system("cls");
			printf("\t!! This feature is not currently available\n\n");
			break;
		case 11: // save and exit
			system("cls");
			printf("\n\n\n\n\t\t\t\t\t\t   Exiting program.\n\n\n\n\n");
			store(pHead, pTail);
			break;
		case 12: // exit w/o saving
			system("cls");
			printf("\n\n\n\n\t\t\t\t\t\t   Exiting program.\n\n\n\n\n");
			break;
		}
	}

	printf("\n\n\n");
	return 0;
}