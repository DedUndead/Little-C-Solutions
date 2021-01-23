#pragma warning (disable:4996)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define INITIAL_SIZE 20
#define FILENAME "stringlog.txt"

char *validateInput(char *inputString, int *size);

int main(void)
{
	FILE* destFile;


	// Allocating initial memory for string
	char *inputString = malloc(INITIAL_SIZE * sizeof(char));
	if (inputString == NULL) {
		printf("Error while allocating memory.\n");
		return 1;
	}

	// Making variable to be changed later
	int size = INITIAL_SIZE;

	destFile = fopen(FILENAME, "a+");
	if (destFile == NULL) {
		printf("Error trying to read the file.\n");
	}

	else {
		printf("Enter the string: ");
		inputString = validateInput(inputString, &size);
		while (inputString[0] != '.') {
			fprintf(destFile, "%d:%s\n", strlen(inputString), inputString);

			printf("Enter another string: ");
			inputString = validateInput(inputString, &size);
		}

		fprintf(destFile, "Final memory size of the inputString = %d", size);
		fclose(destFile);
	}

	free(inputString);

	return 0;
}

char *validateInput(char *inputString, int *size) {

	if (!fgets(inputString, *size, stdin)) {
		strcpy(inputString, "Error");
		return inputString;
	}

	// Re-allocate needed amount of  memory
	while (inputString[strlen(inputString) - 1] != '\n') {
		*size += 1;
		char *p = realloc(inputString, *size * sizeof(char));

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