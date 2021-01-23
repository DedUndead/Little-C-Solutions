#pragma warning (disable:4996)

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define UPPER_LIMIT 40

typedef struct chunk {
	uint8_t *data;
	uint16_t size;
	uint16_t crc;
	uint16_t capacity;
} CHUNK;

CHUNK *readChunkData(char *name, CHUNK *readChunks, int *result, int capacity);
uint16_t crc16(const uint8_t *data_p, unsigned int length);
CHUNK *allocChunk(int size);

int main(void)
{
	int count = 0;
	int result = 0;
	char filename[UPPER_LIMIT];
	char answer[] = "yes";

	while (strcmp(answer, "yes") == 0) {
		printf("Enter the filename: ");
		if (scanf("%s", &filename) != 1) {
			printf("Incorrect input!");
		}

		// Allocate memory for the first element
		int chunk_size;
		printf("Please, specify the chunk size: ");
		scanf("%d", &chunk_size);
		CHUNK* readChunks = allocChunk(chunk_size);

		readChunks = readChunkData(filename, readChunks, &result, chunk_size);


		// Calculating CRC for each read chunk
		printf("\n");
		for (count = 0; count < result; count++) {
			readChunks[count].crc = crc16(readChunks[count].data, readChunks[count].size);
			printf("CHUNK %d: CRC = 0x%x SIZE = %d CAPACITY = %d\n", count, readChunks[count].crc, readChunks[count].size, chunk_size);
		}

		free(readChunks);

		printf("\nWould you like to enter another file (yes/no)?: ");
		scanf("%s", &answer);
	}

	return 0;
}

CHUNK *readChunkData(char *name, CHUNK *readChunks, int *result, int capacity)
{
	int count = 0;

	FILE* inputFile = fopen(name, "rb");
	if (inputFile == NULL) {
		printf("\nError occured while reading the file.\n");
	}

	// Reading the data from the file
	else {
		int i = 0;
		char temp;

		// Read data character by character
		while (fread(&temp, sizeof(char), 1, inputFile) == 1) {
			readChunks[count].data[i] = temp;
			i++;

			if (i == capacity) {
				readChunks[count].size = i;
				readChunks[count].crc = 0;
				readChunks[count].capacity = capacity;
				count++;

				// Reallocate memory
				CHUNK* p = realloc(readChunks, sizeof(CHUNK) * (count + 1));;
				if (p != NULL) {
					readChunks = p;
					readChunks[count].data = (uint8_t*)malloc(sizeof(uint8_t) * capacity); 
				}

				i = 0;
			}
		}

		// Filling last chunk
		if (i != 0) {
			readChunks[count].size = i;
			readChunks[count].crc = 0;
			readChunks[count].capacity = capacity;
			count++;
		}

		fclose(inputFile);
	}

	*result = count;
	return readChunks;
}

/* Actually, after I wrote the function I understood that I cannot
use her for realloc purposes. Still, I decided to leave it here as
a refetence for my future projects */
CHUNK *allocChunk(int size)
{
	CHUNK *p = (CHUNK*)malloc(sizeof(CHUNK));
	if (p != NULL) {
		p->data = (uint8_t*)malloc(sizeof(uint8_t) * size);
	}
	return p;
}

uint16_t crc16(const uint8_t* data_p, unsigned int length) {
	uint8_t x;
	uint16_t crc = 0xFFFF;
	while (length--) {
		x = crc >> 8 ^ *data_p++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
	}
	return crc;
}
