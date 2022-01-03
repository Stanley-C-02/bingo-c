/**A2 Bingo Terminal Application
 * source: A2.c
 *
 * A2 expects two CLAs; an unsigned integer, and a string representing the path to a readable file
 *
 * s46chow, Chow, Stanley, 501022142, 10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define CARD_HEIGHT 5
#define CARD_WIDTH 5

int validInteger(char str[]);
int hasCardFormat(FILE *fp, int card[CARD_HEIGHT][CARD_WIDTH]);
void printPrompt(IntList *callList, int card[CARD_HEIGHT][CARD_WIDTH], char marks[CARD_HEIGHT][CARD_WIDTH], char hasWon);
int isWinner(char marks[CARD_HEIGHT][CARD_WIDTH]);
int intArrContains(int num, int m, int arr[m]);
int intArr2Contains(int num, int m, int n, int arr[m][n]);

int main(int argc, char* argv[]) {
	FILE *fp = NULL;
	int card[CARD_HEIGHT][CARD_WIDTH];
	for(int y = 0; y < CARD_HEIGHT; y++) {
		for(int x = 0; x < CARD_WIDTH; x++) {
			card[y][x] = -1;
		}
	}
	
	if(argc != 3) {
		fprintf(stderr, "Usage: %s seed cardFile\n", argv[0]);
		return 1;
	} else if(!validInteger(argv[1])) {
		fprintf(stderr, "Expected integer seed, but got %s\n", argv[1]);
		return 2;
	} else if((fp = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "%s is nonexistent or unreadable\n", argv[2]);
		return 3;
	} else if(!hasCardFormat(fp, card)) {
		fprintf(stderr, "%s has bad format\n", argv[2]);
		return 4;
	}
	
	fclose(fp);
	
	unsigned int seed = atoi(argv[1]); srand(seed);
	char c;
	IntList *callList = intList(20);
	char marks[CARD_HEIGHT][CARD_WIDTH];
	char hasWon = 0;
	
	for(int i = 0; i < CARD_HEIGHT; i++)
		for(int j = 0; j < CARD_WIDTH; j++)
			marks[i][j] = ' ';
	
	system("clear");
	printPrompt(callList, card, marks, hasWon);
	while((c = getchar()) != 'q') {
		if(c != '\n') {
			// call unique number in [1, 75]
			int call = rand() % 75 + 1;
			while(contains(callList, call)) call = rand() % 75 + 1;

			add(callList, call);
			
			// mark cell if possible
			for(int y = 0; y < CARD_HEIGHT; y++)
				for(int x = 0; x < CARD_WIDTH; x++)
					if(card[y][x] == call) {
						marks[y][x] = 'm';
						if(isWinner(marks)) {
							hasWon = 1;
							system("clear");
							printPrompt(callList, card, marks, hasWon);
							putchar('\n');
							return 0;
						}
					}

			// call one more value when input is longer than one char
			if(ungetc(getchar(), stdin) != 'q') continue;
		}

		system("clear");
		printPrompt(callList, card, marks, hasWon);
		if(size(callList) == 75) {
			break;
		}
	}
	
	return 0;
}

/* returns 1 if the given argument is a string containing only a positive integer
 * the string cannot have a +/- sign; it only contains digits, including leading zeros
 * returns 0 otherwise if there are non-digits
 */
int validInteger(char str[]) {
	char input[strlen(str)];
	sprintf(input, "%0*d", (int)strlen(str), atoi(str)); // * takes variable format specifier
	return strcmp(str, input) == 0;
}

/**returns 1 if the file given is successfully read into the card
 * and 0 otherwise, when one of the following occurs:
 * - invalid input format
 * - invalid input data
 */
int hasCardFormat(FILE *fp, int card[CARD_HEIGHT][CARD_WIDTH]) {
	int ranges[] = {0, 1, 16, 31, 46, 61, 76};
	int format[] = {1, 2, 3, 4, 5,
                        1, 2, 3, 4, 5,
                        1, 2, 0, 4, 5,
                        1, 2, 3, 4, 5,
                        1, 2, 3, 4, 5};

	char input[3] = "  \0";
	int inputNum, fIndex;

	for(int y = 0; y < CARD_HEIGHT; y++) {
		for(int x = 0; x < CARD_WIDTH; x++) {
			fgets(input, strlen(input) + 1, fp);

			if(!validInteger(input)) return 0;

			inputNum = atoi(input);
			fIndex = x + y * CARD_WIDTH;

			if(ranges[format[fIndex]] <= inputNum &&
			   inputNum < ranges[format[fIndex] + 1] &&
			   !intArr2Contains(inputNum, CARD_HEIGHT, CARD_WIDTH, card))
				card[y][x] = inputNum;
			else {
				return 0;
			}

			switch(fgetc(fp)) {
				case ' ':
					if(x == CARD_WIDTH - 1) return 0;
					break;
				case '\n':
					if(x != CARD_WIDTH - 1) return 0;
					break;
				default:
					return 0;
			}
		}
	}
	return fgetc(fp) == EOF;
}

/**prints the user prompt while playing the game with given parameters
 */
void printPrompt(IntList *callList, int card[CARD_HEIGHT][CARD_WIDTH], char marks[CARD_HEIGHT][CARD_WIDTH], char hasWon) {
	printf("CallList: ");
	printInts(callList);
	printf("\n\nL   I   N   U   X\n");
	for(int y = 0; y < CARD_HEIGHT; y++) {
		printf("%02d%c", card[y][0], marks[y][0]);
		for(int x = 1; x < CARD_WIDTH; x++) {
			printf(" %02d%c", card[y][x], marks[y][x]);
		}
		putchar('\n');
	}
	if(hasWon) {
		printf("WINNER");
	} else {
		printf("enter any non-enter key for Call (q to quit): ");
	}
}

/**returns 1 if the given array containing the markings of the card
 * match a winning solution (one row or column marked, or all four corners marked)
 * and 0 otherwise
 */
int isWinner(char marks[CARD_HEIGHT][CARD_WIDTH]) {
	// check corners
	if(              marks[0][0] == 'm' &&               marks[0][CARD_WIDTH - 1] == 'm' &&
	   marks[CARD_HEIGHT - 1][0] == 'm' && marks[CARD_HEIGHT - 1][CARD_WIDTH - 1] == 'm')
		return 1;

	// check columns
	for(int x = 0; x < CARD_WIDTH; x++) {
		int isColMarked = 1;
		for(int y = 0; y < CARD_HEIGHT; y++) {
			if(marks[y][x] != 'm') {
				isColMarked = 0;
				break;
			}
		}
		if(isColMarked) return 1;
	}

	// check rows
	for(int y = 0; y < CARD_HEIGHT; y++) {
		int isRowMarked = 1;
		for(int x = 0; x < CARD_WIDTH; x++) {
			if(marks[y][x] != 'm') {
				isRowMarked = 0;
				break;
			}
		}
		if(isRowMarked) return 1;
	}

	return 0;
}

/**returns 1 if a given int array contains a given num
 * and 0 if the num is not found
 */
int intArrContains(int num, int m, int arr[m]) {
	for(int i = 0; i < m; i++) {
		if(arr[i] == num) {
			return 1;
		}
	}
	return 0;
}

/**returns 1 if a given 2D int array contains a given num
 * and 0 if the num is not found
 */
int intArr2Contains(int num, int m, int n, int arr[m][n]) {
	for(int i = 0; i < m; i++) {
		if(intArrContains(num, n, arr[i])) {
			return 1;
		}
	}
	return 0;
}

