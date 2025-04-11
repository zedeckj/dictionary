#include "dict.h"


int tests_ran = 0;

#define assert(x)\
	if (!(x)) {\
		fprintf(stderr, "On line %d, " #x " is false", __LINE__);\
		exit(1);\
	}\
	else tests_ran += 1; 
#define assert_eq(x, y) assert((!x && !y) || (x && y && !strcmp(x, y)))


void test_basic() {
	dict_t *dict = new_dict(100);	
	assert(dict->length == 0);
	assert(!dict->retrieved);
	assert(!dict_lookup(dict,"foo"));		
	assert(dict_add(dict, "foo", "bar"));
	assert(dict->length == 1);
	assert_eq(dict_lookup(dict, "foo"), "bar");
	assert_eq(dict->retrieved, "bar");
	assert(dict_remove(dict, "foo"));
	assert(!dict_lookup(dict, "foo"));
	assert(dict->length == 0);
	dict_free(dict);
}

void test_capacity() {
	dict_t *dict = new_dict(1);
	assert(dict_add(dict, "apple", "pear"));
	assert(!dict_add(dict, "bananna", "pineapple"));
	assert(dict_remove(dict, "apple"));
	assert(dict_add(dict, "plum", "pineapple"));
	dict_free(dict);
}

void test_lots() {
	#define COUNT 1000
	dict_t *dict = new_dict(COUNT);
	char key_bufs[COUNT][10];
	char val_bufs[COUNT][10];
	for (int i = 0; i < COUNT; i++) {
		sprintf(key_bufs[i], "key %d", i); 
		sprintf(val_bufs[i], "val %d", i); 
		dict_add(dict, key_bufs[i], val_bufs[i]);
	}
	assert(dict->length == COUNT);
	for (int i = 0; i < COUNT; i++) {
		char temp_key[10];
		char temp_val[10];
		sprintf(temp_key, "key %d", i); 
		sprintf(temp_val, "val %d", i); 
		assert_eq(dict_lookup(dict, temp_key), temp_val);
		assert_eq(dict->retrieved, temp_val);
	}
}

void test_empty_remove() {
	dict_t * dict = new_dict(10);
	assert(!dict_remove(dict, "pan"));	
	assert(dict->length == 0);
	dict_free(dict);
}


void test_duplicate(){ 
	dict_t *dict = new_dict(10);
	assert(dict_add(dict, "florida", "orlando"));
	assert(dict_add(dict, "florida", "miami"));		
	assert_eq(dict_lookup(dict, "florida"), "miami");
}

void test_no_capacity() {
	dict_t *dict = new_dict(0);
	assert(!dict);
}

void test_variadic() {
	dict_t *dict = dict_with(10, "foo", "bar", "baz", "qux");
	assert_eq(dict_lookup(dict, "foo"), "bar");
	assert_eq(dict_lookup(dict, "baz"), "qux");
	assert(dict->length == 2);
	dict_free(dict);
}


void test_mem() {
	dict_t *dict = new_dict(10);
	char *str = malloc(10);
	strcpy(str,"mallocd");
	char *val = "val";
	assert(dict_add(dict, str, val));
	free(str);
	assert_eq(dict_lookup(dict, "mallocd"), val);
}

int main(int argc, char ** argv) {
	test_basic();
	test_capacity();
	test_lots();
	test_empty_remove();
	test_no_capacity();
	test_variadic();
	test_duplicate();
	test_mem();
	printf("All %d tests passed\n", tests_ran);
	return 0;
}
