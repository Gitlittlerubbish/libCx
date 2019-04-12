#ifndef __libcx_List_h__
#define __libcx_List_h__

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *head;
    ListNode *tail;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_head(A) ((A)->head != NULL ? (A)->head->value : NULL)
#define List_tail(A) ((A)->tail != NULL ? (A)->tail->value : NULL)

void List_push(List *list, void *value);
void *List_pop(List *list);

void List_head_push(List *list, void *value);
void *List_head_pop(List *list);

void *List_remove(List *list, ListNode * node);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
                                                   ListNode *V = NULL;\
for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
