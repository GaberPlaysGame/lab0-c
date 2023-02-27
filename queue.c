#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_sort.h"
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
    free(l);
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
        memcpy(sp, remove->value,
               bufsize > strlen(remove->value) + 1 ? strlen(remove->value) + 1
                                                   : bufsize);
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
        memcpy(sp, remove->value,
               bufsize > strlen(remove->value) + 1 ? strlen(remove->value) + 1
                                                   : bufsize);
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
void swap(struct list_head *n1, struct list_head *n2)
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

void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *n1 = head->next, *n2 = n1->next;
    while (n1 != head && n2 != head) {
        swap(n1, n2);
        n1 = n1->next;
        n2 = n1->next;
    }
}

/* Reverse elements in queue */
void reverse(struct list_head *start, struct list_head *end)
{
    while (start != end && start != end->next) {
        swap(start, end);
        struct list_head *tmp = end->next;
        end = start->prev;
        start = tmp;
    }
}

void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    reverse(head->next, head->prev);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *start = head->next, *end = start;
    while (end != head) {
        int count = 1;
        while (count != k && end->next != head) {
            end = end->next;
            count++;
        }
        struct list_head *tmp = end->next;
        reverse(start, end);
        start = tmp, end = start;
    }
}

/* Sort elements of queue in ascending order */
struct list_head *merge(struct list_head *l1, struct list_head *l2)
{
    struct list_head *new_head = NULL, **indirect = &new_head;
    while (l1 && l2) {
        if (strcmp(list_entry(l1, element_t, list)->value,
                   list_entry(l2, element_t, list)->value) < 0) {
            *indirect = l1;
            l1 = l1->next;
        } else {
            *indirect = l2;
            l2 = l2->next;
        }
        indirect = &(*indirect)->next;
    }
    *indirect = (struct list_head *) ((uintptr_t) l1 | (uintptr_t) l2);
    return new_head;
}

struct list_head *mergeSort(struct list_head *head, struct list_head *tail)
{
    if (head == tail)
        return head;
    struct list_head *start = head, *end = tail;
    while (start != end && start != end->prev) {
        start = start->next;
        end = end->prev;
    }
    if (start == end)
        end = end->next;
    start->next = NULL;
    end->prev = NULL;

    struct list_head *l1 = mergeSort(head, start), *l2 = mergeSort(end, tail);
    return merge(l1, l2);
}

void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) ||
        list_is_singular(head))  // if head == NULL or empty or one element
        return;

    struct list_head *start = head->next, *end = head->prev;

    // Break the circular linked list
    start->prev = NULL;
    end->next = NULL;

    // Mergesort
    struct list_head *new_head = mergeSort(start, end);
    start = new_head;
    end = start->next;

    // Rebuild circular linked list
    for (; end != NULL; start = start->next, end = end->next) {
        end->prev = start;
    }
    start->next = head;
    new_head->prev = head;
    head->next = new_head;
    head->prev = start;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))  // if head == NULL or empty or one element
        return 0;
    if (list_is_singular(head))
        return 1;

    int count = 1;
    struct list_head *left = head->next, *right = left->next;
    while (right != head) {
        element_t *left_entry = list_entry(left, element_t, list);
        if (strcmp(left_entry->value,
                   list_entry(right, element_t, list)->value) < 0) {
            left = left->next;
            list_del(&left_entry->list);
            q_release_element(left_entry);
            count--;
        } else {
            right = right->next;
            count++;
        }
    }
    return count;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    int interval = 1, list_amount = 0;
    queue_contex_t *contex = NULL;
    list_for_each_entry (contex, head, chain) {
        list_amount++;
        contex->q->prev->next = NULL;
    }

    struct list_head *l1 = NULL, *l2 = NULL;
    while (interval < list_amount) {
        for (int i = 0; i < list_amount - interval; i += interval * 2) {
            list_for_each_entry (contex, head, chain) {
                if (contex->id == i)
                    l1 = contex->q;
                else if (contex->id == i + interval)
                    l2 = contex->q;
            }
            l1->next = merge(l1->next, l2->next);
            l2->prev = l2;
            l2->next = l2;
        }
        interval *= 2;
    }

    struct list_head *start, *end;
    head = l1;
    start = head->next;
    end = start->next;

    // Rebuild circular linked list
    for (; end != NULL; start = start->next, end = end->next) {
        end->prev = start;
    }
    start->next = head;
    head->next->prev = head;
    head->prev = start;
    return q_size(head);
}
