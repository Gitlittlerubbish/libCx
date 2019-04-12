#include "list_sorts.h"
#include "dbg.h"

//only swap ListNode value
void swap_node(ListNode *a, ListNode *b){
    check(a != NULL && b != NULL, "Shit, ListNode cannot be NULL.");

    void *temp = NULL;
    temp = a->value;
    a->value = b->value;
    b->value = temp;

error:
    return;
}   

int list_bubble_sort(List *list, list_compare cmp) {
    if (List_count(list) <= 1) 
        return 0;
    //good idea to reduce iteration
    int sorted = 1;
    do {
        sorted = 1; //great idea
        LIST_FOREACH(list, head, next, cur) {
            if (cur->next) {
                if (cmp(cur->value, cur->next->value) >= 0) {
                    swap_node(cur, cur->next);
                    sorted = 0; //good idea, if swap needed, sort algo is not completed.
                }
            }
        }
    } while (!sorted);

    return 0;
}

//merge sort: top down(recursive)
List *list_merge_sort(List *list, list_compare cmp) {

    List *result = NULL;

    if (List_count(list) <= 1)  //already sorted
        return list;

    int middle = List_count(list)/2;
    List *left_list = List_create();
    List *right_list = List_create();

    //get the origin list split into left and right
    LIST_FOREACH(list, head, next, cur) {
        if (middle > 0) {
            List_push(left_list, cur->value);
        } else {
            List_push(right_list, cur->value);
        }
        middle--;
    }

    //recursive merge_sort, left side and right side
    debug("spitting: %d %d\n", List_count(left_list), List_count(right_list));
    List *left = list_merge_sort(left_list, cmp);
    List *right = list_merge_sort(right_list, cmp);

    result = merge(left, right, cmp);
    /********
    WARNING: bug not fixed
    left_list and right_list not freed
    *********/
    if (left != left_list)
        List_destroy(left_list);
    if (right != right_list)
        List_destroy(right_list);

        //only return result, so free left and right
    List_destroy(left);
    List_destroy(right);

    return result;
}










