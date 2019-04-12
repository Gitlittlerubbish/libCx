#include "hashmap.h"
#include "bstrlib.h"
#include "minunit.h"
#include <assert.h>

Hashmap *map;
static int traverse_time = 0;
struct tagbstring test1 = bsStatic("hello key1");
struct tagbstring test2 = bsStatic("hello key2");
struct tagbstring test3 = bsStatic("hello moto key3");

struct tagbstring expected1 = bsStatic("hello data1");
struct tagbstring expected2 = bsStatic("hello data2");
struct tagbstring expected3 = bsStatic("hello moto data3");

static inline int good_traverse(HashmapNode *node)
{
    traverse_time++;
    debug("data:%s\n", bdata((bstring) node->key));
    return 0;
}

static inline int bad_traverse(HashmapNode *node)
{
    traverse_time++;
    int result = 0;

    debug("DATA:%s\n", bdata((bstring) node->data));
    if (traverse_time == 2) 
        result = 1;
    else 
        result = 0;

    return result;
}


char *test_create()
{
    map = Hashmap_create(NULL, NULL);
    mu_assert(map != NULL, "Hashmap created error.");

    return NULL;
}

char *test_destroy()
{
    Hashmap_destroy(map);
    return NULL;
}

char *test_set_get()
{
    int rc = 0;
    bstring check_bstring;
    rc = Hashmap_set(map, &test1, &expected1);
    mu_assert(rc == 0, "Set data set 1 error.");
    check_bstring = (bstring) Hashmap_get(map, &test1);
    mu_assert(check_bstring == &expected1, "Get data set 1 error.");

    rc = Hashmap_set(map, &test2, &expected2);
    mu_assert(rc == 0, "Set data set 2 error.");
    check_bstring = (bstring) Hashmap_get(map, &test2);
    mu_assert(check_bstring == &expected2, "Get data set 2 error.");

    rc = Hashmap_set(map, &test3, &expected3);
    mu_assert(rc == 0, "Set data set 3 error.");
    check_bstring = (bstring) Hashmap_get(map, &test3);
    mu_assert(check_bstring == &expected3, "Get data set 3 error.");

    return NULL;
}

char *test_traverse()
{
    int rc = 0;
    rc = Hashmap_traverse(map, good_traverse);
    mu_assert(traverse_time == 3, "Good traverse failed.");

    traverse_time = 0;
    rc = Hashmap_traverse(map, bad_traverse);
    mu_assert(rc == 1, "Bad traverse failed.");
    mu_assert(traverse_time == 2, "Bad traverse quit error.");

    return NULL;
}

char *test_delete()
{
    bstring check_bstring;
    check_bstring = Hashmap_delete(map, &test1);
    mu_assert(check_bstring == &expected1, "Delete data set 1 error.");

    check_bstring = Hashmap_delete(map, &test2);
    mu_assert(check_bstring == &expected2, "Delete data set 2 error.");

    check_bstring = Hashmap_delete(map, &test3);
    mu_assert(check_bstring == &expected3, "Delete data set 3 error.");

    return NULL;
}
char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_set_get);
    mu_run_test(test_traverse);
    mu_run_test(test_delete);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);




