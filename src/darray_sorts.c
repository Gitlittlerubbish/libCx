#include "darray.h"
#include "darray_sorts.h"

//all use BSD function calls.
int DArray_mergesort(DArray *array, DArray_compare cmp) {

	int rc = mergesort(array->contents, 
						DArray_count(array), sizeof(void *), cmp);
	check(rc == 0, "mergesort error.");

	return 0;
error:
	return -1;
}

int DArray_heapsort(DArray *array, DArray_compare cmp) {
	int rc = heapsort(array->contents, 
						DArray_count(array), sizeof(void *), cmp);
	check(rc == 0, "heapsort error.");

	return 0;
error:
	return -1;
}

int DArray_quicksort(DArray *array, DArray_compare cmp) {
	qsort(array->contents, 
				DArray_count(array), sizeof(void *), cmp);

	return 0;
}