#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *qHead = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(qHead);
    return qHead;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (l) {
        element_t *entry;
        element_t *safe;
        list_for_each_entry_safe (entry, safe, l, list) {
            free(entry->value);
            free(entry);
        }
        free(l);
    }
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *newElement = (element_t *) malloc(sizeof(element_t));
    newElement->value = malloc(strlen(s) + 1);
    strncpy(newElement->value, s, strlen(s));
    s[strlen(s)] = '\0';
    list_add(&newElement->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *newElement = malloc(sizeof(element_t));
    newElement->value = malloc(strlen(s) + 1);
    strncpy(newElement->value, s, strlen(s));
    s[strlen(s)] = '\0';

    list_add_tail(&newElement->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *element = list_entry(head, element_t, list);

    if (element) {
        strncpy(sp, element->value, bufsize);
    }
    list_del(head);


    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *element = list_last_entry(head, element_t, list);

    if (element) {
        strncpy(sp, element->value, bufsize);
    }
    list_del(head);

    return element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}


struct list_head *swapPair(const struct list_head *head,
                           struct list_head *first,
                           struct list_head **second)
{
    if (first->next != head) {
        struct list_head *nextNode = first->next;
        first->next = nextNode->next;
        first->next->prev = first;
        nextNode->prev = first->prev;
        nextNode->prev->next = nextNode;
        first->prev = nextNode;
        nextNode->next = first;
        *second = first;
        return nextNode;
    }
    *second = first;
    return first;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/

    if (head->next == head) {
        return;
    }
    struct list_head *cur;

    head->next = swapPair(head, head->next, &cur);

    while (cur->next != head) {
        struct list_head *temp = cur;
        temp->next = swapPair(head, cur->next, &cur);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

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
