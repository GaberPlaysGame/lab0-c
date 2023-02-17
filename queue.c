#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "report.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head)
        INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;  // if head is NULL
    element_t *iter, *next;
    list_for_each_entry_safe (iter, next, l, list) {
        list_del(&iter->list);    // delete node
        q_release_element(iter);  // free element
    }
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;  // if head == NULL, return false
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;  // if malloc() failed
    size_t len_s = strlen(s) + 1;
    new->value = (char *) malloc(len_s * sizeof(char));
    if (!new->value) {
        free(new);
        return false;  // if malloc() failed, free new and return false
    }

    // copy string value
    memcpy(new->value, s, len_s);

    // utilize function of list.h
    list_add(&new->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;  // if head == NULL, return false
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;  // if malloc() failed
    size_t len_s = strlen(s) + 1;
    new->value = (char *) malloc(len_s * sizeof(char));
    if (!new->value) {
        free(new);
        return false;  // if malloc() failed, free new and return false
    }

    // copy string value
    memcpy(new->value, s, len_s);

    // utilize function of list.h
    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))  // there is no element in list
        return NULL;
    element_t *remove = list_entry(head->next, element_t, list);

    list_del(&remove->list);  // an element in list is removed

    if (sp && bufsize) {  // sp != NULL and bufsize != 0
        memcpy(sp, remove->value, bufsize);
        sp[bufsize - 1] = '\0';
    }

    return remove;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))  // there is no element in list
        return NULL;
    element_t *remove = list_entry(head->prev, element_t, list);
    list_del(&remove->list);  // an element in list is removed

    if (sp && bufsize) {  // sp != NULL and bufsize != 0
        memcpy(sp, remove->value, bufsize);
        sp[bufsize - 1] = '\0';
    }

    return remove;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    // if head is NULL or head->next = head, the queue has no element
    if (!head || list_empty(head))
        return 0;

    int count = 0;
    struct list_head *iter;
    list_for_each (iter, head)
        count++;

    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head
        *tortoise = head->next,
        *hare = head->next;  // given two node points to 0th node, using floyd's
                             // tortoise and hare algorithm.
    while (hare != head &&
           hare != head->prev) {  // when hare reaches end of list or head,
                                  // tortoise will be in the middle of the list
        tortoise = tortoise->next;
        hare = hare->next->next;
    }

    element_t *element = list_entry(tortoise, element_t, list);
    list_del(&element->list);
    q_release_element(element);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;  // list is NULL

    element_t *cur, *next;
    bool flag = false;
    list_for_each_entry_safe (cur, next, head, list) {
        int cmp = &next->list != head
                      ? strcmp(cur->value, next->value)
                      : true;  // no comparison if next is head,
                               // comparison gives 0 or nonzero value
        if (cmp && !flag)
            continue;

        list_del(&cur->list);
        q_release_element(cur);
        flag = !cmp;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
