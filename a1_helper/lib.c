/*
 * lib.c
 * 
 * This is a collection of functions that will be called from app.c
 * 
 * Note the following:
 *  1] The prototypes of any functions available to app.c should be added to lib.h
 *  2] Any function private to this file should be declared as static.
 */


// standard library header files
#include <stdio.h>
#include <stdlib.h>

#include "lib.h" // we need to include this because our struct definition is located there


// protoypes for qsort comparison functions defined in this file
static int compare_by_int(const void* a, const void* b);
static int compare_by_struct(const void* a, const void* b);



/*
 * We will dynamically create of array of integers, with the size specified
 * as an argument at runtime. We check for a malloc error, just in case. We then
 * initialize the array with some values and return the new array to the calling 
 * function.
 * 
 * The function will return a pointer to the new int array
 */ 
int* create_simple_array(int size){
	
	// malloc space for the new array and check for errors. Note that malloc will
	// return a pointer to an array of integers
	int *new_array = malloc(size * sizeof(int));
	if(new_array == NULL){
		fprintf(stderr, "Oops, malloc failed...we have to quit\n");
		exit(EXIT_FAILURE);  // EXIT_FAILURE is defined in stdlib.h
	}
	
	// now let's put some values in the array
	for(int i = 0; i < size; i++){
		int value = (size - 1) - i;
		new_array[i] = value * value;  // initialize with 'i squared', from 9 down to 0
	}
	
	return new_array;
}



/*
 * We will dynamically create an array of structs, with the size specified
 * as an argument at runtime. In this case, we will have 2 dynamic allocations:
 *  1] each struct is dynamically allocated
 *  2] The array that holds the struct is dynamically allocated
 * 
 * Note: to make the code simpler to understand, I don't provide the error checking on maslloc this time
 */ 
struct foo* *create_complex_array(int size){
	
	// use malloc to create space for the main array and check for errors
	
	// IMPORTANT: In this case, the array is NOT actually going to hold structs; instead, it will
	// be an array of "pointers to structs". So we have to be very careful to tell malloc
	// (i) to create enough bytes to hold pointers to structs (i.e., "sizeof(struct foo *)"
	// AND (ii) malloc should return a pointer to an array of struct pointers (i.e., "struct foo* *new_array)"
	
	// Note that the return type of this functions uses this same logic - the function returns a 
	// pointer to an array of struct pointers.
	
	struct foo* *new_array = malloc(size * sizeof(struct foo*));
	
	// now let's put some values in the array. In this case, we (i) create new structs dynamically, 
	// (ii) give the structs some values, and (iii) assign the struct pointers to the array created above
	for(int i = 0; i < size; i++){
		
		// create a new struct and return its address
		struct foo* new_struct = malloc(size * sizeof(struct foo));
		
		// initialize the struct with values. Note that we use the '->' to access
		// the struct members since we are following a pointer to get to the struct
		int count_value = (size - 1) - i;
		new_struct->count = count_value; // count runs from 9 to 0
		new_struct->weight = count_value * 0.99;
		
		// finally, assign the pointer to the initialized struct to its position in the main array
		new_array[i] = new_struct;
	}
	
	return new_array;
}



/*
 * Sort the array of ints. qsort needs the following parameters:
 *  1] a pointer to the array
 *  2] the number of elements to sort
 *  3] the size (in bytes) of each element in the array
 *  4] the sort function used to compare any two integers with each other (defined below)
 * 
 * That's it!
 * 
 */
void sort_simple_array(int *array, int count){
	
	qsort(array, count, sizeof(int), compare_by_int);
		
}



/*
 * Comparison functions are used by qsort to compare values housed
 * in any two positions in the array
 * 
 * The two values to be compared are actually passed to qsort as pointers to the array 
 * positions that hold the two values (Using pointers to the array positions is necessary since 
 * qsort has no idea what kind of objects you are storing in the array). As a result, 
 * you have to "dereference" the pointers to get the actual values at each position. 
 * 
 * In the example below, we see how to refer to the two values. Note the following:
 *  1] We have to tell qsort the 'type' of the values to compare. So the expression
 *     "(int *)a" tells the compiler that the address 'a' should be interpreted as
 *     a pointer to an integer. 
 *  2] To actually get the integer value referenced by this pointer, we use the expression
 *     "*(int *)a" This essentially says "since 'a' is a pointer to an int, then give me
 *     the integer value that 'a' points to. It may look a little confusing since we 
 *     definitely don't have to do this with Java, but it makes sense to qsort.
 */
static int compare_by_int(const void* a, const void* b){
    
    if ( *(int *)a < *(int *)b ) return -1; // if what 'a' points to is less than what 'b' points to
    if ( *(int *)a > *(int *)b ) return 1;  // if what 'a' points to is greater than than what 'b' points to
    return 0;                               // if the first two conditions are not true
}





/*
 * Sort the array of foo structs. 
 * 
 * IMPORTANT: The logic is the same as the comparison
 * function for ints but we must be careful to tell qsort that the size of
 * each element in the array is the size of a "pointer to a foo struct", NOT
 * the size of a foo struct itself. 
 * 
 * Remember that the foo structs are allocated on the heap (with malloc) and 
 * the array just holds the pointers to these structs
 * 
 */
void sort_complex_array(struct foo* *array, int count){
	
	qsort(array, count, sizeof(struct foo *), compare_by_struct);
		
}



/*
 * The comparison function for the foo struct elements. Here we are soring based upon
 * the 'count' value in the struct.
 * 
 * The logic is the same as the function used for ints (see compare_by_int above). However, 
 * in this case, our 'a' and 'b' addresses are refering to pointers to structs. So we have to 
 * deference the 'a' and 'b' pointers twice in order to get to the struct objects 
 * (the first pointer is the address of 'a' or 'b' and the second pointer is the 
 * reference to the struct object.
 * 
 * Because the syntax can be kind of tricky in this case, we derefence the 'a' and 'b'
 * pointers first to make the actual comparisons easier. You don't have to do this but 
 * the syntax can be quite ugly if you try to do a "double dereference" while accessing and
 * comparing struct values at the same time.
 * 
 * Finally, note that the 'const' qualifier is just an instruction to the compiler to tell it
 * that qsort is not allowed to modify the values in the caller's array. This is required.
 */
static int compare_by_struct(const void* a, const void* b){
    
    // get the struct pointers at address 'a' and address 'b'
    const struct foo *struct_a = *(const struct foo **)a;
    const struct foo *struct_b = *(const struct foo **)b;
    
    if (struct_a->count < struct_b->count) return -1;
    if (struct_a->count > struct_b->count) return 1;
    return 0; // if the first two conditions are not true
}





