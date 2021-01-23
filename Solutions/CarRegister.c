#pragma warning (disable:4996)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define CAR_NUMBER 15
#define UPPER_LIMIT 20
#define PLATE_MAX 9
#define CATEGORY_MAX 3

typedef struct carInfromation
{
	char make[UPPER_LIMIT];
	char model[UPPER_LIMIT];
	char category[UPPER_LIMIT];
	char plate[UPPER_LIMIT];
	int mileage;
	char avaliability[UPPER_LIMIT];
} car;

void printCar(const car *carList, int size, char *category);
void printHeader(void);
void printMenu(void);
int readOption(const char *prompt);
bool changeState(car *carList, int size, char *plate);
bool validateInput(char *inputString, int size);

int main(void)
{
	int option = 0;
	char optionPrompt[] = "Choose an option from the menu: ";
	char inputPlate[PLATE_MAX];
	char inputCategory[CATEGORY_MAX];

	car carList[CAR_NUMBER] =
	{
		{"Opel", "Agila", "M", "MMG-123", 500, "Avaliable"},
		{"Toyota", "Aygo", "M", "ABC-420", 473, "Avaliable"},
		{"VW", "Up", "M", "SAY-313", 740, "Avaliable"},
		{"VW", "Polo", "E", "COF-221", 1250, "Avaliable"},
		{"Opel", "Corsa", "E", "BBC-001", 1321, "Avaliable"},
		{"VW", "Polo", "E", "KEK-331", 902, "Avaliable"},
		{"Toyota", "Yaris", "E", "HBK-112", 1512, "Avaliable"},
		{"Nissan", "Micra", "E", "FSA-102", 721, "Avaliable"},
		{"Nissan", "Quashqai", "F", "ACC-120", 1231, "Avaliable"},
		{"Opel", "Astra", "C", "VWV-120", 2312, "Avaliable"},
		{"BMW", "5-Series", "L", "BAV-400", 560, "Avaliable"},
		{"Skoda", "Fabia", "I", "FAL-212", 1241, "Avaliable"},
		{"Mazda", "6", "S", "DLK-111", 2312, "Avaliable"},
		{"Audi", "A4", "S", "VAS-560", 2313, "Avaliable"},
		{"Ford", "Focus", "C", "REN-133", 900, "Avaliable"}
	};

	printf("======================\nWelcome to Car Manager v1.0 by paulbekk!\n======================\n");
	// Main loop process
	printMenu();
	option = readOption(optionPrompt);
	while (option != 4) {

		switch (option)
		{
		case 1:
			printCar(carList, CAR_NUMBER, "*");
			break;
		case 2:
			printf("Please, enter the category, 1 character: ");
			if (!validateInput(inputPlate, CATEGORY_MAX)) {
				printf("Error occured. Incorrect input.\n");
			}
			else {
				printCar(carList, CAR_NUMBER, inputPlate);
			}

			break;
		case 3:
			printf("Please, enter the plate number, max 7 characters: ");
			if (!validateInput(inputPlate, PLATE_MAX)) {
				printf("Error occured. Incorrect input.\n");
			}

			else if (!changeState(carList, CAR_NUMBER, inputPlate)) {
				printf("Error occured. Wrong plate number.\n");
			}

			else {
				printf("Avaliability state changed successfully.\n");
			}

			break;
		default:
			printf("Unknown option!\n");
		}

		printMenu();
		option = readOption(optionPrompt);
	}

	printf("\n======================\nTerminating program.\n======================\n");

	return 0;
}


// Print the list of cars by the category
void printCar(const car *carList, int size, char *category)
{
	int count = 0;

	printHeader();
	for(int i = 0; i < size; i++) {
		if (strcmp(category, "*") == 0 || (strcmp(carList[i].category, category) == 0 && strcmp(carList[i].avaliability, "Avaliable") == 0)) {
			printf("%-20s%-20s%-20s%-20s%7d%22s\n", carList[i].make, carList[i].model, carList[i].category, carList[i].plate, carList[i].mileage, carList[i].avaliability);
			count++;
		}
	}

	if (count == 0) {
		printf("No free cars found in the current category.");
	}
}

// Print header of the table
void printHeader(void)
{
	printf("\n%-20s%-20s%-20s%-20s%-20s%-20s\n\n", "Make", "Model", "Category", "Reg. Plate", "Mileage", "Avaliability");
}

// Standard validation for String inputs
bool validateInput(char *inputString, int size)
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

// Print menu
void printMenu(void)
{
	printf("\n\t1. print all cars\n");
	printf("\t2. print free cars of given category\n");
	printf("\t3. change state of a car\n");
	printf("\t4. quit the program\n\n");
}

// Read int to choose a menu option
int readOption(const char *prompt)
{
	int number = 0;
	char input[10];

	printf("%s", prompt);
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

// Change avaliability
bool changeState(car *carList, int size, char *plate)
{
	for (int i = 0; i < size; i++) {
		if (strcmp(carList[i].plate, plate) == 0) {
			if (strcmp(carList[i].avaliability, "Avaliable") == 0) {
				strcpy(carList[i].avaliability, "Rented");
			}
			else {
				strcpy(carList[i].avaliability, "Avaliable");
			}
			return true;
		}
	}

	return false;
}