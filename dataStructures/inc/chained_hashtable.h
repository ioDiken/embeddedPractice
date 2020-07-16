/**
 * @file chained_hashtable.h
 * @author Nicolas Diken (nicolasdiken@ieee.org)
 * @brief 
 * @date 2020-07-15
 */

#ifndef C2DB7E3C_8530_438A_9788_5E59831718DA
#define C2DB7E3C_8530_438A_9788_5E59831718DA

#include <stdint.h>

#define MAX_NAME_SIZE 20

typedef struct person {
	char name[MAX_NAME_SIZE];
	uint8_t age;
	struct person *next;
} person;

class chainedHash 
{
public:
	chainedHash(uint16_t table_size);
	~chainedHash();

	bool put(const char *name, uint8_t age);
	person *get(const char *name);
	bool remove(const char *name); // remove person

private:
	person **hash_table;
	uint16_t table_sz;

	uint16_t hash_function(const char *);
	void init_table();
	void free_table();
	void print_table(); // used only for debug purposes
};


#endif /* C2DB7E3C_8530_438A_9788_5E59831718DA */
