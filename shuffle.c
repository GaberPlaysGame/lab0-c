#include <stdlib.h>

#include "list.h"
#include "shuffle.h"

void shuffle_swap(struct list_head *n1, struct list_head *n2)
{
    struct list_head *tmp = n1->next;
    n1->next = n2->next;
    n2->next->prev = n1;
    n2->next = tmp;
    n2->next->prev = n2;

    tmp = n2->prev;
    n2->prev = n1->prev;
    n1->prev->next = n2;
    n1->prev = tmp;
    n1->prev->next = n1;
}

void q_shuffle(struct list_head *head, size_t len)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *tail = head->prev;

    for (int i = len - 1; i > 0; i--) {
        struct list_head *cur = head;
        int j = rand() % (i + 1);
        for (int tmp = 0; tmp <= j; tmp++)
            cur = cur->next;
        shuffle_swap(cur, tail);
        tail = cur->prev;
    }
}