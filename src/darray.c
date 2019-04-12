#include "darray.h"
#include "dbg.h"
#include "darray_sorts.h"

//create DArray and check memory for array->contents and array;
DArray *DArray_create(size_t element_size, size_t initial_max) {
	DArray *array = calloc(1, sizeof(DArray));
	check_mem(array);
	check(initial_max > 0, "DO NOT set wrong max.");
	
	array->end = 0;
	array->max = initial_max;
	array->element_size = element_size;
	array->expand_rate = DEFAULT_EXPAND_RATE;
	//malloc fo contents
	array->contents = calloc(initial_max, sizeof(void *));
	check_mem(array->contents);

	return array;
error:
	//just use DArray_destroy
	DArray_destroy(array);
	return NULL;
}

int DArray_expand(DArray *array) {
	check(array, "Can not expand a NULL DArray.");

	int old_max = array->max;
	check(DArray_resize(array, array->max + array->expand_rate) == 0, "FAILED to expand.");
	//memset expanded memory
	memset(array->contents + old_max, 0, array->expand_rate + 1);

	return 0;
error:
	return -1;
}

int DArray_shrink(DArray *array) {
	check(array, "Can not shrink a NULL DArray.");

	int new_max = array->end > (int)array->expand_rate ? array->end + 1: (int)array->expand_rate + 1;
	check(DArray_resize(array, new_max) == 0, "FAILED to shrink.");

	return 0;
error:
	return -1;
}

//push from the end
void DArray_push(DArray *array, void *ele) {
	check(array, "Can not push into a NULL DArray.");

	array->contents[array->end] = ele;
	array->end++;

	if (array->end >= array->max) {
		check(DArray_expand(array) == 0, "FULL, failed to push.");
	}

error:
	return;
}


void DArray_sort_add(DArray *array, void *ele, DArray_compare cmp) {
	DArray_push(array, ele);
	DArray_quicksort(array, cmp);

	return;
}

void *DArray_pop(DArray *array) {
	check(array->end > 0, "Failed to pop from a empty DArray.");

	void *ele = DArray_remove(array, array->end - 1);
	array->end--;

	if (DArray_end(array) > (int)array->expand_rate 
			&& DArray_end(array) % array->expand_rate) {
		DArray_shrink(array);
	}

	return ele;
error:
	return NULL;
}

//only set the darray[index] NULL, and no operations on array->end.
void *DArray_remove(DArray *array, int index) {
	void *ele = array->contents[index];

	array->contents[index] = NULL;

	return ele;
}

//destroy pointers except contents[i]
void  DArray_destroy(DArray *array) {
	if (array) {
		if (array->contents) 
			free(array->contents);
		free (array);
	}	
}

//free contents[i], except not-alloc char *
void DArray_clear(DArray *array) {
	if (array) {
		if (array->element_size > 0) {
			int i = 0;
			for (i = 0; i < array->max; i++) 
				if (array->contents[i] != NULL)
					free(array->contents[i]);
		}
	}

}

//clear and then destroy
void DArray_clear_destroy(DArray *array) {
	DArray_clear(array);
	DArray_destroy(array);
}


