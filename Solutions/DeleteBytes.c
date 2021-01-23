#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define STRING_LEN 15

char* readFlexible(char* inputString, int* size);
bool deleteBytes(FILE* inputFile, int offset, int number, int filesize, char* filename);

// NOTE: FIRST BYTE (INDEXED 0) IS 1 FOR THE USER
int main(void)
{
	int strln = STRING_LEN;
	int filesize = 0; // Normal int, as long as I do not expect huge files in this case;
	int offset, number;
	char *filename = (char*)malloc(strln * sizeof(char));

	// Asking for file name
	printf("Enter the file name: ");
	filename = readFlexible(filename, &strln);
	FILE* inputFile = fopen(filename, "rb");
	if (inputFile == NULL) {
		printf("Incorrect file name!\n");
	}

	else {
		// SEEK_END = last byte index + 1
		fseek(inputFile, 0, SEEK_END);
		filesize = ftell(inputFile);

		// Asking for deleting parameters
		printf("Enter the offeset (start of the file = 1): ");
		while (scanf("%d", &offset) != 1 || offset < 1 || offset > filesize) {
			while (getchar() != '\n');
			printf("Error: incorrect offset input. Re-enter the value.\n");
		}

		// Offset = index + 1, therefore:
		// Offest = 3, number = 3, delete bytes indexed {2, 3, 4}
		printf("Enter the number of bytes to be deleted: ");
		while (scanf("%d", &number) != 1 || number <= 0 || offset + number - 2 > filesize - 1) {
			while (getchar() != '\n');
			printf("Error: incorrect number input. Re-enter the value.\n");
		}

		if (!deleteBytes(inputFile, offset, number, filesize, filename)) {
			printf("Error occured in memory allocation.\n");
		}
		
		free(filename);
		fclose(inputFile);
	}

	return 0;
}

// Re-write file with the beggining and en; with middle part cut
bool deleteBytes(FILE *inputFile, int offset, int number, int filesize, char *filename)
{
	int beginningSize;
	int endSize;
	uint8_t *beginning; // Beggining in bytes
	uint8_t *end; // End in bytes
	
	beginningSize = offset - 1;
	endSize = filesize - (beginningSize + number);

	beginning = (uint8_t*)malloc(beginningSize * sizeof(uint8_t));
	if (beginning == NULL) {
		printf("Error while allocationg the memory.");
		return false;
	}

	end = (uint8_t*)malloc(endSize * sizeof(uint8_t));
	if (end == NULL) {
		printf("Error while allocating the memory.");
		return false;
	}

	// Set the cursor to start to read the beginning 
	fseek(inputFile, 0, SEEK_SET);
	fread(beginning, sizeof(uint8_t), beginningSize, inputFile);
	
	// Set the cursor to the next byte after deleted to read the end
	fseek(inputFile, -1 * endSize, SEEK_END);
	fread(end, sizeof(uint8_t), endSize, inputFile);

	fclose(inputFile);

	// Rewrite the file
	inputFile = fopen(filename, "wb");
	fwrite(beginning, sizeof(uint8_t), beginningSize, inputFile);
	fwrite(end, sizeof(uint8_t), endSize, inputFile);

	free(beginning);
	free(end);

	return true;
}

// Dynamic string allocation taken from ex. 13
char *readFlexible(char* inputString, int* size) {

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