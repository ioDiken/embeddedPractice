/**
 * @file chained_hashtable.hpp
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief class implementation in header because templated
 * @date 2020-07-15
 */

#ifndef C2DB7E3C_8530_438A_9788_5E59831718DA
#define C2DB7E3C_8530_438A_9788_5E59831718DA

#include <stdint.h>
#include <stdlib.h>
#include "global.h"

// Partially abstract class for chained hash tables
template<typename hash_entry, typename key_t>
class chainedHash_Base
{
public:
	chainedHash_Base(uint16_t table_size)
	{
		table_sz = table_size;
		init_table(); // throws std::bad_alloc if malloc fails
	}

	~chainedHash_Base()
	{
		free_table();
	}

	virtual bool put(const hash_entry entry) = 0;

	hash_entry *get(const key_t key)
	{
		DEBUG("\nBeginning %s\n", __func__);

		if (!check_key_valid(key))
			return NULL;

		uint16_t code = hash_function(key);

		hash_entry *current = hash_table[code];
		DEBUG("Current Entry: %s\n",current->name);
		while (current != NULL)
		{
			DEBUG("Evaluating %s vs %s\n",current->name,key);
			if (cmp_key(current,key))
			{
				DEBUG("Found %s\n",key);
				return current;
			}

			current = current->next;
		}

		return NULL;
	}

	bool remove(const key_t key)
	{
		DEBUG("\nBeginning %s\n", __func__);

		if (!check_key_valid(key))
			return false;

		uint16_t code = hash_function(key);

		hash_entry **current = &hash_table[code];
		hash_entry *prev = NULL;

		while ((*current) != NULL)
		{
			if (cmp_key((*current),key))
			{
				DEBUG("Hash (%d): %s\n",code,(*current)->name);

				// if first iteration
				if (prev == NULL)
				{
					prev = *current;
					*current = (*current)->next;

					free(prev);

					return true;
				}

				DEBUG("Current Next: %d\n",(*current)->next);

				hash_entry *tmp = *current;
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

protected:
	hash_entry **hash_table;
	uint16_t table_sz;

	virtual uint16_t hash_function(const key_t key) = 0;
	virtual bool check_key_valid(const key_t key) = 0;
	virtual inline bool cmp_key(hash_entry *entry, const key_t key) = 0;
	virtual void print_table() = 0;

private:
	void init_table()
	{
		hash_table = (hash_entry **) malloc(sizeof(hash_entry)*table_sz);
		check_malloc((void *) hash_table, "Failed to allocate mem for hashtable\n");

		// set table pointers to NULL to show uninit
		for (uint16_t i = 0; i < table_sz; i++)
		{
			hash_table[i] = NULL;
		}
	}

	void free_table()
	{
		DEBUG("\nBeginning %s\n", __func__);
		hash_entry *current;
		hash_entry *prev;

		for (uint16_t i = 0; i < table_sz; i++) {
			// assign next location in hash table
			current = hash_table[i];
			prev = NULL;

			while (current != NULL) {
				prev = current;
				current = current->next;

				// DEBUG("Freeing: %s\n",prev->name); 
				free(prev);
			}
		}
	}

};

#define MAX_NAME_SIZE 20

typedef struct person {
	char name[MAX_NAME_SIZE];
	uint8_t age;
	struct person *next;
} person;

class chainedHash_person : public chainedHash_Base<person,const char*>
{
public:
	chainedHash_person(uint16_t table_size) : chainedHash_Base<person,const char*>(table_size) {}
	~chainedHash_person() = default;

	virtual bool put(person entry) {} // required to "implement virtual function"
	virtual bool put(const char *name, uint8_t age) // overload function
	{
		DEBUG("\nBeginning %s\n", __func__);

		if (!check_key_valid(name))
			return false;

		uint16_t code = hash_function(name);

		// create new person
		person *new_e = (person *) malloc(sizeof(person));
		check_malloc((void *) new_e, "Cannot create new person, malloc failed\n");
		memset(new_e, 0, sizeof(person));
		strcpy(new_e->name,name);
		new_e->age = age;

		// if code entry is emtpy
		if (hash_table[code] == NULL) {
			hash_table[code] = new_e;
			DEBUG("%s | %d\n",hash_table[code]->name, hash_table[code]->age);
		} else {
			// iterate to end of list
			person *current = hash_table[code];
			while(current->next != NULL) {
				current = current->next;
			}

			DEBUG("%s | %d\n",current->name, current->age);
			current->next = new_e;
		}

		return true;
	}

protected:

	virtual uint16_t hash_function(const char *key)
	{
		uint16_t code = 0;

		while (*key != '\0')
		{
			code += *key;
			code %= table_sz;
			key++; // increment to next char
		}

		return code;
	}

	virtual bool check_key_valid(const char *key)
	{
		if (MAX_NAME_SIZE == strnlen(key, MAX_NAME_SIZE))
		{
			DEBUG("%.*s... longer than max name len (%d)\n",
				MAX_NAME_SIZE,key,MAX_NAME_SIZE);
			return false;
		}
		return true;
	}

	virtual inline bool cmp_key(person *entry, const char *key)
	{
		return strcmp(entry->name,key) == 0;
	}

	virtual void print_table() // used only for debug purposes
	{
		DEBUG("\nBeginning %s\n", __func__);

		person *current;
		for (int i = 0; i < table_sz; i++)
		{
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
};

#endif /* C2DB7E3C_8530_438A_9788_5E59831718DA */
