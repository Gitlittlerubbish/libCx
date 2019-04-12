#ifndef __list_algo_h__
#define __list_algo_h__

#include "list.h"
#include "dbg.h"

//compare strings
typedef int (*list_compare) (const void *a, const void *b);

//use doubly linked list to bubble sort 
int list_bubble_sort(List *list, list_compare cmp);

//use doubly linked list to merge sort(top down), using merge();
List *list_merge_sort(List *list, list_compare cmp);
//merge two lists into one
static inline List *merge(List *left_list, List *right_list, list_compare cmp) {
    debug("In merge: %d %d\n", List_count(left_list), List_count(right_list));
    // LIST_FOREACH(right_list, head, next, cur) {
    //     printf("%s ", cur->value);
    // }
    debug("\n");
    List *result = List_create();

    //merge two lists into one, compare each head->value and push into result.
    do{
        if (cmp(left_list->head->value, right_list->head->value) >= 0) {
            List_push(result, List_head_pop(right_list));
        } else {
            List_push(result, List_head_pop(left_list));
        }
    } while (List_count(left_list) > 0 && List_count(right_list) > 0);

    //push the left list node into result.
    List *temp = NULL;
    temp = List_count(left_list) > 0 ? left_list : right_list;
    LIST_FOREACH(temp, head, next, cur) {
        List_push(result, cur->value);
    }

    return result;
}

#endif