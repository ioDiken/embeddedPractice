/**
 * @file chained_hashtable.cpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief 
 * @date 2020-07-16
 */
 
#include "chained_hashtable.h"
#include "global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>
#include <new>

/**
 * @brief check malloc pass, throw std::bad_alloc
 * 
 * @param var pointer to be checked
 * @param str string to print prior to throwing error
 */
inline void check_malloc(void *var, const char *str)
{
	if (var == NULL)
	{
		printf("%s", str);
		throw std::bad_alloc();
	}
}

inline bool check_name_len(const char *name)
{
	if (MAX_NAME_SIZE == strnlen(name, MAX_NAME_SIZE))
	{
		DEBUG("%s longer than max name len(%d)\n",name,MAX_NAME_SIZE);
		return false;
	}
	return true;
}

chainedHash::chainedHash(uint16_t table_size)
{
	table_sz = table_size;
	init_table(); // throws std::bad_alloc if malloc fails
}

chainedHash::~chainedHash()
{
	free_table();
}

// initialize table to all NULL
void chainedHash::init_table()
{
	hash_table = (person **) malloc(sizeof(person)*table_sz);
	check_malloc((void *) hash_table, "Failed to allocate mem for hashtable\n");

	// set table pointers to NULL to show uninit
	for (uint16_t i = 0; i < table_sz; i++)
	{
		hash_table[i] = NULL;
	}
}

void chainedHash::free_table()
{
	DEBUG("\nBeginning %s\n", __func__);
	person *current;
	person *prev;

	for (uint16_t i = 0; i < table_sz; i++) {
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

uint16_t chainedHash::hash_function(const char *name)
{
	uint16_t key = 0;

	while (*name != '\0')
	{
		key += *name;
		key %= table_sz;
		name++; // increment to next char
	}

	return key;
}

void chainedHash::print_table()
{
	DEBUG("\nBeginning %s\n", __func__);

	person *current;
	for (int i = 0; i < table_sz; i++) {
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

// put person struct, should never fail
bool chainedHash::put(const char *name, uint8_t age)
{
	DEBUG("\nBeginning %s\n", __func__);

	if (!check_name_len(name))
		return false;

	uint16_t key = hash_function(name);

	// create new person
	person * new_p = (person *) malloc(sizeof(person));
	check_malloc((void *) new_p, "Cannot create new person, malloc failed\n");
	memset(new_p, 0, sizeof(person));
	strcpy(new_p->name,name);
	new_p->age = age;

	// if key entry is emtpy
	if (hash_table[key] == NULL) {
		hash_table[key] = new_p;
		DEBUG("%s | %d\n",hash_table[key]->name, hash_table[key]->age);
	} else {
		// iterate to end of list
		person *current = hash_table[key];
		while(current->next != NULL) {
			current = current->next;
		}

		DEBUG("%s | %d\n",current->name, current->age);
		current->next = new_p;
	}

	return true;
}

// get person struct
person * chainedHash::get(const char *name)
{
	DEBUG("\nBeginning %s\n", __func__);

	if (!check_name_len(name))
		return NULL;

	uint16_t key = hash_function(name);

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

// removes person from table
bool chainedHash::remove(const char *name)
{
	DEBUG("\nBeginning %s\n", __func__);

	if (!check_name_len(name))
		return false;

	uint16_t key = hash_function(name);

	person **current = &hash_table[key];
	person *prev = NULL;

	while ((*current) != NULL)
	{
		if (strcmp((*current)->name,name) == 0)
		{
			DEBUG("Hash (%d): %s\n",key,(*current)->name);

			// if first iteration
			if (prev == NULL)
			{
				prev = *current;
				*current = (*current)->next;

				free(prev);

				return true;
			}

			DEBUG("Current Next: %d\n",(*current)->next);

			person *tmp = *current;
			prev->next = (*current)->next;

			free(tmp);

			return true;
		}

		DEBUG("Current: %s\n",(*current)->name);
		prev = *current;
		current = &(*current)->next;
	}

	return false;
}
