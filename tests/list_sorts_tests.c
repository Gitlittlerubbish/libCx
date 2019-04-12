#include "minunit.h"
#include "list.h"
#include "list_sorts.h"
#include <assert.h>
#include <string.h>

char *test[] = { "kuangzifan", "jinzhiyu", "daiminshen", "chenxiao", "chenshuyu", "hello", "moto"};
#define TEST_NUM  7

//make sure the sort algo is right
int is_sorted(List *list) {
    if (List_count(list) <= 1)
        return 1;
    LIST_FOREACH(list, head, next, cur) {
        if (cur->next != NULL && strcmp(cur->value, cur->next->value) > 0){
            printf("WRONG UNIT:%s %s\n", cur->value, cur->next->value);
            return 0;
        }
    }
    return 1;
}

//test the bubble sort function
char *test_bubble_sort()
{
    List *words = NULL;
    words = List_create();
    int i = 0;
    
    for (i = 0; i < TEST_NUM; i++) {
        List_push(words, test[i]); 
    }

    //an unsorted list
    int rc = list_bubble_sort(words, (list_compare)strcmp);
    mu_assert(rc == 0, "sort failed.");
    mu_assert(is_sorted(words), "bubble algo I wrong.");

    // printf("Bubble sort result:\n");
    // LIST_FOREACH(words, head, next, cur) {
    //     printf("%s ", cur->value);
    // }
    // printf("\n");
    // printf("length of bubble_sort result: %d\n", List_count(words));

    //should work on an already sorted list
    rc = list_bubble_sort(words, (list_compare)strcmp);
    mu_assert(rc == 0, "seconde sort test on sorted list failed.");
    mu_assert(is_sorted(words), "bubble algo II wrong.");

    List_destroy(words);
    //should work on a NULL list
    words = List_create();
    rc = list_bubble_sort(words, (list_compare)strcmp);
    mu_assert(rc == 0, "NULL list sort failed.");

    // printf("----------------------------------\n");

    List_destroy(words);
    return NULL;
}

char *test_merge_sort()
{
    List *words = NULL;
    words = List_create();
    int i = 0;
    for (i = 0; i < TEST_NUM; i++) {
        List_push(words, test[i]); 
    }

    //should work on an unsorted list
    List *res = list_merge_sort(words, (list_compare)strcmp);
    // printf("Merge sort result:\n");
    // LIST_FOREACH(res, head, next, cur) {
    //     printf("%s ", cur->value);
    // }
    // printf("\n");
    // printf("length of merge_sort result: %d\n", List_count(res));
    // printf("return value of is_sorted(merge list) is: %d\n", is_sorted(res));

    // printf("----------------------------------\n");
    mu_assert(is_sorted(res), "Merge algo error.");

    //should work on a sorted list

    List *res2 = list_merge_sort(res, (list_compare)strcmp);
    mu_assert(is_sorted(res2), "Merge algo failed on a sorted list, go check it.");

    List_destroy(res2);
    List_destroy(res);
    List_destroy(words);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);

    return NULL;
}

RUN_TESTS(all_tests);
