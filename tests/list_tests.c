#include "minunit.h"
#include <list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    return NULL;
}

char *test_destroy()
{
    List_destroy(list);

    return NULL;

}

char *test_push_pop()
{
    List_push(list, test1);
    mu_assert(List_tail(list) == test1, "Wrong tail value.");

    List_push(list, test2);
    mu_assert(List_tail(list) == test2, "Wrong tail value");

    List_push(list, test3);
    mu_assert(List_tail(list) == test3, "Wrong tail value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    return NULL;
}

char *test_head_push()
{
    List_head_push(list, test1);
    mu_assert(List_head(list) == test1, "Wrong head value.");

    List_head_push(list, test2);
    mu_assert(List_head(list) == test2, "Wrong head value");

    List_head_push(list, test3);
    mu_assert(List_head(list) == test3, "Wrong tail value.");
    mu_assert(List_count(list) == 3, "Wrong count on head_push.");

    return NULL;
}

char *test_remove()
{
    // we only need to test the middle remove case since push/head_pop 
    // already tests the other cases

    char *val = List_remove(list, list->head->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_head(list) == test3, "Wrong head after remove.");
    mu_assert(List_tail(list) == test1, "Wrong tail after remove.");

    return NULL;
}

char *test_head_pop()
{
    mu_assert(List_count(list) != 0, "Wrong count before head_pop.");

    char *val = List_head_pop(list);
    mu_assert(val == test3, "Wrong value on head_pop.");

    val = List_head_pop(list);
    mu_assert(val == test1, "Wrong value on head_pop.");
    mu_assert(List_count(list) == 0, "Wrong count after head_pop.");

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_head_push);
    mu_run_test(test_remove);
    // mu_run_test(test_head_pop);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
