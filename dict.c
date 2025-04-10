#include "dict.h"

// Uses djb2 algorithm
size_t dict_hash(char * str) {
	if (!str) return 0;
	size_t val = 5919; 
	char c;
	while ((c = *(str++))) {
		val += (val << 5) + c;
	}
	return val;
}

dict_t *new_dict(size_t capacity) {
	if (!capacity) return 0;
	dict_t *dict = (dict_t *)malloc(sizeof(dict_t));
	if (!dict) return 0;
	dict->entries = (table_entry_t *)malloc(sizeof(table_entry_t) * capacity);
	if (!dict->entries) return 0;
	dict->is_occupied = (bool *)malloc(sizeof(bool) * capacity);
	if (!dict->is_occupied) return 0;
	memset(dict->is_occupied, false, sizeof(bool) * capacity);
	dict->capacity = capacity;
	return dict;
}


dict_t *new_dict_with_func(size_t capacity, ...) {
	dict_t *dict = new_dict(capacity);
	if (!dict) return 0;
	va_list args;
	va_start(args, capacity);
	for (int i = 0; i < capacity; i += 2) {
		char *key = va_arg(args, char *);
		if (!key) return dict;
		else {
			char *value = va_arg(args, char *);
			if (!value) return 0;
			dict_add(dict, key, value);
		}
	}	
	return 0;
}

#define new_dict_with(capacity, ...) new_dictionary_function(capacity, ..., 0); 


void free_dict(dict_t *dict) {
	free(dict->entries);
	free(dict);
}


// Checks if the provided index has an entry that matches the given key and hash
bool is_matching_entry(dict_t *dict, char *key, size_t index, size_t hash) {
	table_entry_t entry = dict->entries[index];
	return dict->is_occupied[index] && entry.hash_val == hash && !strcmp(entry.key, key);
}

bool dict_index_of(dict_t *dict, char *key, size_t *index_out) {
	if (dict) {
		size_t hash = dict_hash(key);
		size_t trunc_hash = hash % dict->capacity;
		size_t check_index = trunc_hash;
		do {
			if (is_matching_entry(dict, key, check_index, hash)) {
				*index_out = check_index;
				return true;
			}	
		} while((check_index = (check_index + 1) % dict->capacity), check_index != trunc_hash);	
	}
	return false;	
}

char *dict_lookup(dict_t *dict, char *key) {
	size_t index;
	if (dict_index_of(dict, key, &index)) {
		return dict->entries[index].value;
	}
	return 0;	
}


bool dict_remove(dict_t *dict, char *key) {
	size_t index;
	if (dict_index_of(dict, key, &index)) {
		dict->is_occupied[index] = false;
		dict->entries[index].value = 0;
		return true;
	}
	return false;	
}

bool dict_add(dict_t *dict, char *key, char *value) {
	if (dict) {
		size_t hash = dict_hash(key);
		size_t trunc_hash = hash % dict->capacity;
		size_t check_index = trunc_hash;
		do {
			if (!dict->is_occupied[check_index]) {
				table_entry_t new_entry;
				new_entry.key = key;
				new_entry.value = value;
				new_entry.hash_val = hash;
				dict->is_occupied[check_index] = true;
				dict->entries[check_index] = new_entry;
				return true;					
			}
		} while ((check_index = (check_index + 1) % dict->capacity), check_index != trunc_hash);
	} 
	return false;	
}


