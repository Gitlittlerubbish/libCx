#include "darray_sorts.h"
#include "darray.h"
#include "dbg.h"
#include <assert.h>
#include "minunit.h"
#include <string.h>

static DArray *array = NULL;
char *tests[] = { "kuangzifan", "jinzhiyu", "daiminshen", "chenxiao", "chenshuyu", "hello", "moto"};
#define TESTS_NUM 7

int myStrcmp(const void **a, const void **b) {
	return strcmp(*(const char **)a, *(const char **)b);
}

void myCreate() {
	array = DArray_create(sizeof(void *), 100);

	int i = 0;
	for (i = 0; i < TESTS_NUM; i++) {
		DArray_push(array, tests[i]);
	}
}

int is_sorted(DArray * array)
{
    int i = 0;

    for (i = 0; i < DArray_count(array) - 1; i++) {
        if (strcmp(DArray_get(array, i), DArray_get(array, i + 1)) > 0) {
            return 0;
        }
    }

    return 1;
}

char *test_mergesort()
{
	mu_assert(DArray_mergesort(array, (DArray_compare)myStrcmp) == 0, "sort error.");
	mu_assert(is_sorted(array) == 1, "merge algo error.");

	//should work on a sorted array.
	mu_assert(DArray_mergesort(array, (DArray_compare)myStrcmp) == 0, "sort error.");
	mu_assert(is_sorted(array) == 1, "quick algo error.");
    return NULL;
}

char *test_heapsort() 
{
	DArray_swap(array, 0, 1);
	DArray_swap(array, 2, 3);

	mu_assert(DArray_heapsort(array, (DArray_compare)myStrcmp) == 0, "sort error.");
	mu_assert(is_sorted(array) == 1, "heap algo error.");

	//should work on a sorted array.
	mu_assert(DArray_heapsort(array, (DArray_compare)myStrcmp) == 0, "sort error.");
	mu_assert(is_sorted(array) == 1, "quick algo error.");

	return NULL;
}

char *test_quicksort() 
{
	DArray_swap(array, 0, 1);
	DArray_swap(array, 2, 3);
	//should work on an unsorted array.
	mu_assert(DArray_quicksort(array, (DArray_compare)myStrcmp) == 0, "sort error.");
	mu_assert(is_sorted(array) == 1, "quick algo error.");

	//should work on a sorted array.
	mu_assert(DArray_quicksort(array, (DArray_compare)myStrcmp) == 0, "sort error.");
	mu_assert(is_sorted(array) == 1, "quick algo error.");

	return NULL;
}

char *all_tests()
{
    mu_suite_start();

    myCreate();

    mu_run_test(test_mergesort);
    mu_run_test(test_heapsort);
    mu_run_test(test_quicksort);

    DArray_destroy(array);

    return NULL;
}

RUN_TESTS(all_tests);