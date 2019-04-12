#ifndef __libcx_darray_h__
#define __libcx_darray_h__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "dbg.h"

typedef struct DArray {
	int end;
	int max;
	size_t element_size;
	size_t expand_rate;
	void **contents;
} DArray;

typedef int (*DArray_compare) (const void *a, const void *b);

#define DEFAULT_EXPAND_RATE 300;

DArray *DArray_create(size_t element_size, size_t initial_max);
void  DArray_destroy(DArray *array);
void DArray_clear(DArray *array);
void DArray_clear_destroy(DArray *array);

void DArray_push(DArray *array, void *ele); //from end
void *DArray_pop(DArray *array); //from end
void *DArray_remove(DArray *array, int index); 	//remove value due to index

int DArray_expand(DArray *array);
int DArray_shrink(DArray *array);



void DArray_sort_add(DArray *array, void *ele, DArray_compare cmp);

#define DArray_end(A) ((A)->end) 
#define DArray_count(A) (DArray_end(A))
#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_first(A) ((A)->contents[0])
#define DArray_max(A) ((A)->max)

//just use the arg new_max to re_size the DArray, so check before call this func
static inline int DArray_resize(DArray *array, size_t new_max) {
	check(new_max > 0, "The new size must > 0.");
	array->max = new_max;
 
	//re allocate
	void *new_contents = realloc(
				array->contents, array->max * (sizeof(void *)));//because void * is the actual stored value type
	check_mem(new_contents);
	array->contents = new_contents;

	return 0;
error:
	return -1;
}

static inline void DArray_set(DArray *array, int index, void *ele) {
	check(index < array->max, "DONOT try to set beyond max. MAX: %d", DArray_max(array));

	array->contents[index] = ele;
	if (index > array->end) {
		array->end = index + 1;
	}

error:
	return;
}

static inline void *DArray_get(DArray *array, int index) {
	check(index < array->max, "Beyond reach.");

	void *ele = array->contents[index];

	return ele;
error:
	return NULL;
}

static inline void DArray_swap(DArray *array, int i, int j) {
	void *temp = NULL;
	temp = array->contents[i];
	array->contents[i] = array->contents[j];
	array->contents[j] = temp;
}

static inline void *DArray_new(size_t element_size) {
	check(element_size > 0, "Sorry, element_size < 0.");

	void *ele = calloc(1, sizeof(element_size));
	check_mem(ele);

	return ele;
error:
	return NULL;
}

#define DArray_free(A) free(A)

#endif







