#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_CHAR 256

typedef struct symbol {
	char character;
	int frequency;
} symbol;

void readFreq(FILE *inputFile, symbol *characters, int charNum);
char *readFlexible(char *inputString, int *size);
int freq_cmp(const void *a, const void *b);
void printResult(symbol *characters);

int main(void) 
{
	int initialSize = 10;
	FILE *inputFile;
	symbol characters[MAX_CHAR];
	char *filename = (char*)malloc(initialSize * sizeof(char));

	// Asking for file name
	printf("Enter the file name: ");
	filename = readFlexible(filename, &initialSize);

	inputFile = fopen(filename, "rb");
	if (inputFile == NULL) {
		printf("Unable to open the file.\n");
	}

	else {
		// Initialize symbols array
		for (int i = 0; i < MAX_CHAR; i++) {
			characters[i].character = i;
			characters[i].frequency = 0;
		}

		readFreq(inputFile, characters, MAX_CHAR);
		qsort(characters, MAX_CHAR, sizeof(symbol), freq_cmp);
		printResult(characters);
		
		fclose(inputFile);
	}

	return 0;
}

void readFreq(FILE *inputFile, symbol *characters, int charNum)
{
	int i = 0;
	int j = 0;
	char current;

	while (fread(&current, sizeof(char), 1, inputFile) == 1) {
		for (j = 0; characters[j].character != current; j++);
		characters[j].frequency++;
	}
}

void printResult(symbol *characters)
{
	printf("\n");
	for (int i = 0; i < 10; i++) {
		// Unprintable characters are replaced with decimal represantation in ASCII table
		if (isprint(characters[i].character) == 0) {
			printf(" %2d) Character: [%02x] %32s: %d\n", i + 1, characters[i].character, "Frequency", characters[i].frequency);
		}

		//Printable characters are printed on the screen
		else {
			printf(" %2d) Character: %c %35s: %d\n", i + 1, characters[i].character, "Frequency", characters[i].frequency);
		}
	}
}

// Qsort comparing property, compare in reverse order
int freq_cmp(const void *a, const void *b)
{
	symbol ca = *(const symbol *)a;
	symbol cb = *(const symbol *)b;

	if (ca.frequency < cb.frequency) {
		return 1;
	}
	else if (ca.frequency > cb.frequency) {
		return -1;
	}
	else {
		return 0;
	}
}

// Dynamic string allocation taken from ex. 13
char *readFlexible(char *inputString, int *size) 
{

	if (!fgets(inputString, *size, stdin)) {
		strcpy(inputString, "Error");
		return inputString;
	}

	// Re-allocate needed amount of  memory
	while (inputString[strlen(inputString) - 1] != '\n') {
		*size += 1;
		char* p = realloc(inputString, *size * sizeof(char));

		if (p == NULL) {
			printf("\nError while trying to re-allocate memory.\n");
			return inputString;
		}

		inputString = p;

		char tmp[2];
		fgets(tmp, 2, stdin);

		strncat(inputString, tmp, 1);
	}

	inputString[strlen(inputString) - 1] = '\0';
	return inputString;
}
