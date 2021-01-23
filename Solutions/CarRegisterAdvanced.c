#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME "cars.dat"
#define STRING_LEN 30

typedef struct car_ {
	char make[STRING_LEN];
	char model[STRING_LEN];
	int price;
	float emission;
	struct car_ *next;
} car;

bool readList(char *filename, car **ppn);
bool writeList(char *filename, car **ppn);
bool addCar(car** ppn, int size);
void printCars(car **ppn);
void printMenu(void);
int readOption(void);
bool validateInput(char* inputString, int size);

//Problem in string memory allocation?
int main(void)
{
	int option;
	car *carListHead = NULL;

	if (!readList(FILENAME, &carListHead)) {
		printf("No previuos data to read..\n");
	}

	printMenu();
	option = readOption();
	while (option != 0) {
		switch (option) {
			case 1:
				printCars(&carListHead);
				break;

			case 2:
				if (!addCar(&carListHead, STRING_LEN)) {
					printf("Process abandoned.\n");
				};
				break;
		}

		printMenu();
		option = readOption();
	}

	if (!writeList(FILENAME, &carListHead)) {
		printf("List was not saved.");
	}

	return 0;
}

void printMenu(void)
{
	printf("\n1) print all cars");
	printf("\n2) add new car");
	printf("\n0) exit");
	printf("\nChoose an option: ");
}

// Here I pass the "size" not as a pointer, because I want to save initial value
// as long as I think that 15 characters will be enough for most cases
bool addCar(car **ppn, int size)
{
	// Allocate memory for node and strings inside
	car *newCar = (car*)malloc(sizeof(car));
	if (newCar == NULL) {
		printf("Error occured when allocating memory.\n");
		return false;
	}

	// Specify the parameters
	printf("\nSpecify the informating of the car.\n");
	printf("Specify the make: ");
	while (!validateInput(newCar->make, size)) {
		printf("Invalid input (max: 28 characters) Re-enter: ");
	}

	printf("Specify the model: ");
	while (!validateInput(newCar->model, size)) {
		printf("Invalid input (max: 28 characters) Re-enter: ");
	}

	printf("Specify the price in euroes: ");
	while (scanf("%d", &newCar->price) != 1) {
		while (getchar() != '\n');
		printf("Error, re-enter the value: ");
	}

	printf("Specify CO2 emission in g/km: ");
	while (scanf("%f", &newCar->emission) != 1) {
		while (getchar() != '\n');
		printf("Error, re-enter the value: ");
	}

	// Add the car to the end of the list
	while (*ppn != NULL) {
		ppn = &(*ppn)->next;
	}
	*ppn = newCar;
	newCar->next = NULL;

	// Clear buffer
	while (getchar() != '\n');

	return true;
}

// Upload the list when starting the program, if exists
bool readList(char *filename, car **ppn)
{
	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		printf("\nError while reading the file\n");
		return false;
	}

	// Alocate memory for the first node
	car *readCar = (car*)malloc(sizeof(car));
	if (readCar == NULL) {
		printf("\nError in memory allocation.");
		return false;
	}
	
	// Read the file
	while (fread(readCar, sizeof(car), 1, file) == 1) {
		while (*ppn != NULL) {
			ppn = &(*ppn)->next;
		}
		*ppn = readCar;
		readCar->next = NULL;

		// Allocate memory for the next node
		readCar = (car*)malloc(sizeof(car));
		if (readCar == NULL) {
			printf("\nError while trying to re-allocate memory.\n");
			return false;
		}
	}
	free(readCar);

	fclose(file);
	return true;
}

// Write and free the memort
bool writeList(char *filename, car **ppn)
{
	FILE *file = fopen(filename, "wb");
	if (file == NULL) {
		printf("No such file in the directory.\n");
		return false;
	}
	
	car *prev = *ppn;

	while (*ppn != NULL) {
		fwrite((*ppn), sizeof(car), 1, file);
		*ppn = (*ppn)->next;
		free(prev);
		prev = *ppn;
	}

	return true;
}

// Print car list
void printCars(car **ppn) {
	if (*ppn == NULL) {
		printf("\nNo cars are on the list. Add some first.\n");
	}
	else {
		printf("\n%-20s%-20s%-20s%-20s\n", "Make", "Model", "Price", "Emission");
		while (*ppn != NULL) {
			printf("%-20s%-20s%5d%23.2f\n", (*ppn)->make, (*ppn)->model, (*ppn)->price, (*ppn)->emission);
			ppn = &(*ppn)->next;
		}
	}
}

int readOption(void)
{
	int number = 0;
	char input[10];

	fgets(input, 10, stdin);

	// If number is invalid 
	if (strlen(input) > 10) {
		while ((getchar()) != '\n'); // clearing stdin buffer
		return 0;
	}

	if (sscanf(input, "%d", &number) != 1) {
		number = -1;
	}

	return number;
}

/* Standard validation for String inputs from ex7-9
I tried to dynamically allocate characters for strings
But then I could not solve the problem with reading the structure
From binary file afterwards */
bool validateInput(char* inputString, int size)
{
	// check if the string is not empty
	if (!fgets(inputString, size, stdin) || strlen(inputString) == 1) {
		strcpy(inputString, "\n");
		return false; // return if fgets error occured
	}

	else if (inputString[strlen(inputString) - 1] == '\n') {
		inputString[strlen(inputString) - 1] = '\0';
		return true;
	}

	while ((getchar()) != '\n'); // clearing stdin buffer

	return false; // return if the string is too long
}