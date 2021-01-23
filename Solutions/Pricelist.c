#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>

#define UPPER_LIMIT 150
#define FILE_NAME_SIZE 50
#define MAX_ARRAY_SIZE 20

struct priceInfo {
	char name[UPPER_LIMIT];
	float price;
};

void printHeader(void);
void printList(const struct priceInfo* itemList, int size);

int main(void)
{

	FILE *inputFile;
	char filename[FILE_NAME_SIZE];
	char line[UPPER_LIMIT];
	char nameRead[UPPER_LIMIT];
	float priceRead;
	struct priceInfo itemList[MAX_ARRAY_SIZE] = { 0 };
	int counter = 0;
	
	printf("Please, enter the filename: ");     
	scanf("%s", filename);
	inputFile = fopen(filename, "r");

	if (!inputFile) {
		printf("Incorrect file name.");
	}

	else {
		while (!feof(inputFile) && counter < MAX_ARRAY_SIZE) {
			if (fgets(line, UPPER_LIMIT, inputFile)) {
				if (sscanf(line, "%f;%150[^\n]", &priceRead, &nameRead) == 2) {
					strcpy(itemList[counter].name, nameRead);
					itemList[counter].price = priceRead;
				}
				else {
					printf("Incorrect format input detected at line %d\n", counter + 1);
					counter--;
				}
				counter++;
			}
		}
		printList(itemList, counter);
	}

	return 0;
}

void printList(const struct priceInfo *itemList, int size)
{
	printHeader();
	for (int i = 0; i < size; i++) {
		printf("%-50s%15.2f\n", itemList[i].name, itemList[i].price);
	}
}

void printHeader(void)
{
	printf("\n%-60s%-60s\n\n", "Name", "Price");
}
