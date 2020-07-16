#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// To execute C, please define "int main()"

// remove any debug print statements
// #define DEBUG_
#ifdef DEBUG_
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif


#define TABLE_SIZE 10
#define MAX_NAME_SIZE 20

typedef struct person {
	char name[MAX_NAME_SIZE];
	uint8_t age;
	struct person *next;
} person;

person *hash_table[TABLE_SIZE];

uint8_t hash_function(const char *name)
{
	uint8_t key = 0;

	while (*name != '\0')
	{
		key += *name;
		key %= TABLE_SIZE;
		name++; // increment to next char
	}

	return key;
}

// initialize table to all NULL
void init_hash_table()
{
	for (int i = 0; i < TABLE_SIZE; i++) {
		hash_table[i] = NULL;
	}
}

void print_hash_table()
{
	DEBUG("\nBeginning %s\n", __func__);

	person *current;
	for (int i = 0; i < TABLE_SIZE; i++) {
		// assign next location in hash table
		current = hash_table[i];

		// weird formatting just for print formatting
		if (current != NULL) {
			printf("\t%d\t",i);
			while (current != NULL) {
				printf("--%s",current->name);
				current = current->next;
			}
			printf("\n");
		}
		else {
			printf("\t%d\t--\n",i);
		}
	}
}

void free_hash_table()
{
	DEBUG("\nBeginning %s\n", __func__);
	person *current;
	person *prev;

	for (int i = 0; i < TABLE_SIZE; i++) {
		// assign next location in hash table
		current = hash_table[i];
		prev = NULL;

		while (current != NULL) {
			prev = current;
			current = current->next;

			DEBUG("Freeing: %s\n",prev->name);
			free(prev);
		}
	}
}

// put person struct, should never fail
void put(const char *name, uint8_t age)
{
	DEBUG("\nBeginning %s\n", __func__);
	uint8_t key = hash_function(name);

	// create new person
	person * new = malloc(sizeof(person));
	memset(new, 0, sizeof(person));
	strcpy(new->name,name);
	new->age = age;

	if (hash_table[key] == NULL) {
		hash_table[key] = new;
		DEBUG("%s | %d\n",hash_table[key]->name, hash_table[key]->age);
	} else {
		// iterate to end of list
		person *current = hash_table[key];
		while(current->next != NULL) {
			current = current->next;
		}

		DEBUG("%s | %d\n",current->name, current->age);
		current->next = new;
	}
}

// get person struct
person *get(const char *name)
{
	DEBUG("\nBeginning %s\n", __func__);
	uint8_t key = hash_function(name);
	DEBUG("Hash Key: %d\n",key);

	person *current = hash_table[key];
	DEBUG("Current Entry: %s\n",current->name);
	while (current != NULL)
	{
		DEBUG("Evaluating %s vs %s\n",current->name,name);
		if (strcmp(current->name,name) == 0)
		{
			DEBUG("Found %s\n",name);
			return current;
		}

		current = current->next;
	}

	return NULL;
}

// removes person from table (usr must free)
person *remove_p(const char *name)
{
	DEBUG("\nBeginning %s\n", __func__);
	uint8_t key = hash_function(name);

	person **current = &hash_table[key];
	person *prev = NULL;

	while ((*current) != NULL)
	{
		if (strcmp((*current)->name,name) == 0)
		{
			// if first iteration
			if (prev == NULL)
			{
				prev = *current;
				*current = (*current)->next;
				DEBUG("Hash (%d): %s\n",key,hash_table[key]->name);
				return prev;
			}

			DEBUG("Hash (%d): %s\n",key,(*current)->name);
			DEBUG("Current Next: %d\n",(*current)->next);
			person *tmp = *current;
			prev->next = (*current)->next;
			return tmp;
		}

		DEBUG("Current: %s\n",(*current)->name);
		prev = *current;
		current = &(*current)->next;
	}

	return NULL;
}

int main()
{
	// clear hash table
	init_hash_table();

	char names[][MAX_NAME_SIZE] = {"Jimmy", "Yolanda", "Akash", "Manal",
								  "Devin", "David", "Azita", "Nicolas",
							      "Testing", "Testy", "WaduHek", "Testin",
							  	  "Matan", "Yotam", "Amit", "Anne", "Baba",
							  	  "Melis", "Seth"};

	for (uint8_t i = 0; i < (sizeof(names)/sizeof(names[0])); i++)
	{
		put(names[i],i);
	}
	DEBUG("\n");

	print_hash_table();

	printf("\tJimmy\t%d\n",get("Jimmy")->age);
	printf("\tYolanda\t%d\n",get("Yolanda")->age);
	if (get("BROSEF") != NULL)
		printf("Get null failed!\n");
	else
		printf("Get null passed!\n");

	person *p = remove_p("Devin");
	printf("%s Removed: %d\n",p->name,get(p->name) == NULL);
	free(p);
	p = remove_p("Baba");
	// printf("%s\n",p->name);
	printf("%s Removed: %d\n",p->name,get(p->name) == NULL);
	free(p);


	print_hash_table();

	free_hash_table();

	return 0;
}
