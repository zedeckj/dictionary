#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#ifndef DICTLIBHEAD
#define DICTLIBHEAD

typedef struct {
	char *key;
	char *value;
	size_t hash_val;
} table_entry_t;

typedef struct {
		bool *is_occupied;
		size_t length;
		table_entry_t *entries;
		size_t capacity;
} dict_t;


#define dict_with(capacity, ...) new_dict_with_func(capacity, __VA_ARGS__, 0)


// Construct a dict with preset key value pairs. Call with `dict_with` macro
dict_t *new_dict_with_func(size_t capacity, ...);

// Allocates a new dictionary
dict_t *new_dict(size_t capacity);
							
// Frees a dictionary and its entries pointer.
void free_dict(dict_t *dict);

// Performs a lookup of the given key on the dictionary. Returns 0 if not found. 
char *dict_lookup(dict_t *dict, char *key);

// Returns true if adding the given key value pair was succseful
bool dict_add(dict_t *dict, char *key, char *value);

// Returns true if remoing the given key was succesful. False if the key was not present.
bool dict_remove(dict_t *dict, char *key); 



#endif
