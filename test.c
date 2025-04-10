#include "dict.h"


#define assert(x)\
	if (!(x)) {\
		fprintf(stderr, #x " is false\n");\
		exit(1);\
	}\

#define assert_eq(x, y) assert(x && y && !strcmp(x, y))

void test_basic() {
	dict_t *dict = new_dict(100);	
	assert(!dict_lookup(dict,"foo"));		
	assert(dict_add(dict, "foo", "bar"));
	assert_eq(dict_lookup(dict, "foo"), "bar");
	assert(dict_remove(dict, "foo"));
	assert(!dict_lookup(dict, "foo"));
	free_dict(dict);
}

void test_capacity() {
	dict_t *dict = new_dict(1);
	assert(dict_add(dict, "apple", "pear"));
	assert(!dict_add(dict, "bananna", "pineapple"));
	assert(dict_remove(dict, "apple"));
	assert(dict_add(dict, "plum", "pineapple"));
	free_dict(dict);
}


void test_empty_remove() {
	dict_t * dict = new_dict(10);
	assert(!dict_remove(dict, "pan"));	
	free_dict(dict);
}


void test_no_capacity() {
	dict_t *dict = new_dict(0);
	assert(!dict);
}



int main(int argc, char ** argv) {
	test_basic();
	test_capacity();
	test_empty_remove();
	test_no_capacity();
	printf("All tests passed\n");
	return 0;
}
