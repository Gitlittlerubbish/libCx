#ifndef __libcx_darry_sorts_h__
#define __libcx_darry_sorts_h__

#include <stdio.h>
#include <stdlib.h>
#include "darray.h"
#include "dbg.h"

typedef int (*DArray_compare) (const void *a, const void *b);

int DArray_mergesort(DArray *array, DArray_compare cmp);
int DArray_heapsort(DArray *array, DArray_compare cmp);
int DArray_quicksort(DArray *array, DArray_compare cmp);

#endif