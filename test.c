#include "dict.h"


int tests_ran = 0;

#define assert(x)\
	if (!(x)) {\
		fprintf(stderr, #x " is false\n");\
		exit(1);\
	}\
	else tests_ran += 1; 
#define assert_eq(x, y) assert(x && y && !strcmp(x, y))


void test_basic() {
	dict_t *dict = new_dict(100);	
	assert(dict->length == 0);
	assert(!dict_lookup(dict,"foo"));		
	assert(dict_add(dict, "foo", "bar"));
	assert(dict->length == 1);
	assert_eq(dict_lookup(dict, "foo"), "bar");
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
	}
}

void test_empty_remove() {
	dict_t * dict = new_dict(10);
	assert(!dict_remove(dict, "pan"));	
	assert(dict->length == 0);
	dict_free(dict);
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


int main(int argc, char ** argv) {
	test_basic();
	test_capacity();
	test_lots();
	test_empty_remove();
	test_no_capacity();
	test_variadic();
	printf("All %d tests passed\n", tests_ran);
	return 0;
}
