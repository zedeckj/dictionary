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
	dict->length = 0;
	dict->retrieved = 0;
	return dict;
}


dict_t *new_dict_with_func(size_t capacity, ...) {
	dict_t *dict = new_dict(capacity);
	if (!dict) return 0;
	va_list args;
	va_start(args, capacity);
	for (size_t i = 0; i < capacity; i += 2) {
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


void free_entry(dict_t *dict, size_t i) {
	free(dict->entries[i].key);
}

void dict_free(dict_t *dict) {
	size_t freed = 0;
	for (size_t i = 0; i < dict->capacity && freed < dict->length; i++) {
		if (dict->is_occupied[i]) {
			free_entry(dict, i);
			freed += 1;
		}
	}
	free(dict->retrieved);
	free(dict->entries);
	free(dict);
}




// Checks if the provided index has an entry that matches the given key and hash
bool is_matching_entry(dict_t *dict, char *key, size_t index, size_t hash) {
	table_entry_t entry = dict->entries[index];
	return dict->is_occupied[index] && entry.hash_val == hash && !strcmp(entry.key, key);
}

bool dict_index_of(dict_t *dict, char *key, size_t hash, size_t *index_out) {
	if (dict) {
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


void update_last_retrieved(dict_t *dict, char *val) {
	if (dict->retrieved) {
		free(dict->retrieved);
	}
	if (val) {
		dict->retrieved = malloc(strlen(val) + 1);
		strcpy(dict->retrieved, val);
	}
	else dict->retrieved = 0;
}



void *dict_lookup(dict_t *dict, char *key) {
	size_t index;
	size_t hash = dict_hash(key);
	if (dict_index_of(dict, key, hash, &index)) {
		update_last_retrieved(dict,dict->entries[index].value); 
		return dict->retrieved;
	}
	return 0;	
}


char *dict_remove(dict_t *dict, char *key) {
	size_t index;
	size_t hash = dict_hash(key);
	if (dict_index_of(dict, key, hash, &index)) {
		dict->is_occupied[index] = false;
		dict->length -= 1;

		void *val = dict->entries[index].value;
		update_last_retrieved(dict,val); 
		free_entry(dict, index);	
		
		return dict->retrieved;
	}
	return false;	
}

void add_entry_helper(dict_t *dict, char *key, void *value, size_t index, size_t hash) {
	table_entry_t new_entry;
	new_entry.key = malloc(strlen(key) + 1);
	strcpy(new_entry.key, key);
	new_entry.value = value;
	new_entry.hash_val = hash;
	dict->is_occupied[index] = true;
	dict->entries[index] = new_entry;
	dict->length += 1;
}

bool dict_add(dict_t *dict, char *key, void *value) {
	if (dict) {
		size_t old_index;
		size_t hash = dict_hash(key);
		if (dict_index_of(dict, key, hash, &old_index)) {
			free_entry(dict, old_index);
			add_entry_helper(dict, key, value, old_index, hash);
			return true;
		}
		size_t trunc_hash = hash % dict->capacity;
		size_t check_index = trunc_hash;
		do {
			if (!dict->is_occupied[check_index]) {
				add_entry_helper(dict, key, value, check_index, hash);
				return true;					
			}
		} while ((check_index = (check_index + 1) % dict->capacity), check_index != trunc_hash);
	} 
	return false;	
}


