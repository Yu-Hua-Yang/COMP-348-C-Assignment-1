// don't forget our header guards (we can use whatever label we like)
#ifndef MY_LIB_H_
#define MY_LIB_H_


// this is a struct used by the program, so we include it
// in the header file so that both source files can see
// the definition
struct foo{
	int    count;
	double weight;
};


// public functions avaiabled in lib.c
int* create_simple_array(int size);
void sort_simple_array(int *array, int count);

struct foo* *create_complex_array(int size);
void sort_complex_array(struct foo* *array, int count);

#endif /* MY_LIB_H_ */

