#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 13
#define MAX_PASS_LENGTH 20

char *passGenerate(char *password, int size, const char *dictionary[]);

int main(void)
{
	srand(time(0));
	const char *dictionary[] = { "Pavel", "Arefyev", "Metropolia", "Petrozavodsk", "Finland",
								"Somehow", "Password", "Security", "Random", "Discord", NULL };
	char password[MAX_PASS_LENGTH];

	for (int i = 0; i < 5; i++) {
		passGenerate(password, MAX_PASS_LENGTH, dictionary);
		printf("%s\n", password);
	}

	return 0;
}

char *passGenerate(char *password, int size, const char *dictionary[])
{
	// Clear the string
	strcpy(password, "null");

	int count; // To generate random password length
	int dict_index; // To generate random word
	int dict_length;

	// Calculate dictionaty length
	for (dict_length = 0; dictionary[dict_length] != NULL; dict_length++);

	// Calculate index from dictionary
	dict_index = rand() % dict_length;

	// Store characters to password 
	if (dict_index != NULL) {
		int wLength = strlen(dictionary[dict_index]);
		count = size - wLength - 1;

		int position = rand() % count;

		for (int i = 0; i < size-1; i++) {
			if (i == position) {
				password[i] = '\0';
				strcat(password, dictionary[dict_index]);
				i += strlen(dictionary[dict_index]) - 1;
			}
			else if (rand() % 2 == 1) {
				password[i] = rand() % 26 + 'A';
			}
			else {
				password[i] = rand() % 26 + 'a';
			}
		}

		password[size - 1] = '\0';
	}

	return password;
}