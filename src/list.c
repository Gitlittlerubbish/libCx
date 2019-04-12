#include <list.h>
#include <dbg.h>

List *List_create()
{
    return calloc(1, sizeof(List));
}

//if char *, use this to clear
void List_destroy(List *list)
{
    LIST_FOREACH(list, head, next, cur) {
        if (cur->prev) {
            free(cur->prev);
        }
    }

    free(list->tail);
    free(list);
}

//clean allocated values, only free values
void List_clear(List *list)
{
    LIST_FOREACH(list, head, next, cur) {
        free(cur->value);
    }
}

//free values and list quickly
void List_clear_destroy(List *list)
{
    List_clear(list);
    List_destroy(list);
}

//push value at the tail
void List_push(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }

    list->count++;

error:
    return;
}

//pop value from the tail
void *List_pop(List *list)
{
    ListNode *node = list->tail;
    return node != NULL ? List_remove(list, node) : NULL;
}

//head push
void List_head_push(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->count++;

error:
    return;
}

//pop from the head
void *List_head_pop(List *list)
{
    ListNode *node = list->head;
    return node != NULL ? List_remove(list, node) : NULL;
}

//use for pop&head_pop, inner function
void *List_remove(List *list, ListNode *node)
{
    void *result = NULL;

    check(list->head && list->tail, "List is empty.");
    check(node, "node can't be NULL");

    if (node == list->head && node == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else if (node == list->head) {
        list->head = node->next;
        check(list->head != NULL,
                "Invalid list, somehow got a head that is NULL.");
        list->head->prev = NULL;
    } else if (node == list->tail) {
        list->tail = node->prev;
        check(list->tail != NULL,
                "Invalid list, somehow got a next that is NULL.");
        list->tail->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

error:
    return result;
}
