#include "minunit.h"
#include <darray.h>
#include <assert.h>

static int *val1 = NULL;
static int *val2 = NULL;
static DArray *array = NULL;

int my_cmp(const void **a, const void **b) {
    return **(int **)a - **(int **)b;
}

char *test_create()
{
    array = DArray_create(sizeof(int), 100);
    mu_assert(array != NULL, "Failed to create list.");
    mu_assert(array->end == 0, "end initialization error.");
    mu_assert(array->max == 100, "max error.");
    mu_assert(array->element_size == sizeof(int), "element_size error.");
   
    return NULL;
}

char *test_destroy()
{
    DArray_free(val1);
    DArray_free(val2);
    DArray_destroy(array);

    return NULL;

}

char *test_new()
{
    val1 = DArray_new(sizeof(int));
    mu_assert(val1 != NULL, "Failed to new an int.")

    val2 = DArray_new(sizeof(int));
    mu_assert(val2 != NULL, "Failed to new an int.")

    return NULL;
}

char *test_resize()
{
    // int rc = DArray_resize(array, 0);
    // mu_assert(rc == -1, "Resize should have failed.");

    int rc = DArray_resize(array, 300);
    mu_assert(rc == 0, "Resize should have succeeded.");
    mu_assert(array->max == 300, "resize max error.");

    return NULL;
}

char *test_get_set() 
{
    *val1 = 3;
    *val2 = 4;

    DArray_set(array, 0, val1);
    mu_assert(DArray_get(array, 0) == val1, "set val1 error.");

    DArray_set(array, 1, val2);
    mu_assert(DArray_get(array, 1) == val2, "set val2 error.");

    return NULL;
}

char *test_swap()
{
    DArray_swap(array, 0, 1);
    mu_assert(DArray_get(array, 0) == val2, "swap 0 error.");
    mu_assert(DArray_get(array, 1) == val1, "swap 1 error.");

    return NULL;
}

char *test_expand_shrink()
{   
    DArray_expand(array);
    mu_assert(DArray_max(array) == 600, "expand error.");

    DArray_shrink(array);
    mu_assert(DArray_max(array) == 301, "shrink error.");

    DArray_shrink(array);
    mu_assert((unsigned int)array->max == array->expand_rate + 1,
            "Should stay at the expand_rate at least.");

    return NULL;
}

char *test_remove()
{
    int *valCheck = NULL;
    valCheck = DArray_remove(array, 1);
    mu_assert(valCheck == val1, "remove index 1 error.");

    valCheck = DArray_remove(array, 0);
    mu_assert(valCheck == val2, "remove index 0 error.");
    mu_assert(DArray_get(array, 0) == NULL, "Should be gone.");


    return NULL;
}

char *test_push_pop()
{
    int i = 0;
    for (i = 0; i < 1000; i++) {
        int *val = DArray_new(array->element_size);
        *val = i * 2;
        DArray_push(array, val);
    }

    mu_assert(array->end == 1002, "wrong end.");
    mu_assert(DArray_max(array) == 1201, "after push, wrong max.");

    for (i = 999; i >= 0; i--) {
        int *val = DArray_pop(array);
        mu_assert(val != NULL, "should not be NULL");
        mu_assert(*val == i * 2, "value error.");
        DArray_free(val);
    }

    mu_assert(array->end == 2, "after push_pop, wrong end.");

    return NULL;
}

char *test_sort_add()
{
    *val1 = 6;
    *val2 = 7;

    DArray_set(array, 0, val1);
    DArray_set(array, 1, val2);

    int i = 0;
    for (i = 2; i < 7; i++) {
        int *val_temp = DArray_new(array->element_size);
        *val_temp = 7 - i;
        DArray_sort_add(array, val_temp, (DArray_compare) my_cmp);
    }

    for (i = 0; i < DArray_count(array); i++) {
        debug("%d\n", *(int *)array->contents[i]);
    }

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_resize);
    mu_run_test(test_new);
    mu_run_test(test_get_set);
    mu_run_test(test_swap);
    mu_run_test(test_expand_shrink);

    mu_run_test(test_remove);
    mu_run_test(test_push_pop);


    mu_run_test(test_sort_add);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
