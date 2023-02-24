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
    if (!qHead)
        return NULL;
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
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    element->value = strdup(s);
    if (!element->value) {
        free(element);
        return false;
    }
    list_add(&element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    element->value = strdup(s);
    if (!element->value) {
        free(element);
        return false;
    }
    list_add_tail(&element->list, head);
    return true;
}

/**
 * q_remove_head() - Remove the element from head of queue
 * @head: header of queue
 * @sp: string would be inserted
 * @bufsize: size of the string
 *
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * Reference:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 *
 * Return: the pointer to element, %NULL if queue is NULL or empty.
 */
/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *element = list_first_entry(head, element_t, list);
    list_del(&element->list);
    if (sp) {
        for (char *i = element->value; bufsize > 1 && *i; sp++, i++, bufsize--)
            *sp = *i;
        *sp = '\0';
    }

    return element;
}

/**
 * q_remove_tail() - Remove the element from tail of queue
 * @head: header of queue
 * @sp: string would be inserted
 * @bufsize: size of the string
 *
 * Return: the pointer to element, %NULL if queue is NULL or empty.
 */
/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *element = list_last_entry(head, element_t, list);
    list_del(&element->list);
    if (sp) {
        for (char *i = element->value; bufsize > 1 && *i; sp++, i++, bufsize--)
            *sp = *i;
        *sp = '\0';
    }

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
    struct list_head *turtle, *rabbit;
    turtle = rabbit = head->next;
    while (rabbit != head) {
        if (rabbit->next != head)
            rabbit = rabbit->next->next;
        else
            break;
        turtle = turtle->next;
    }
    // remove turtle node
    turtle->prev->next = turtle->next;
    turtle->next->prev = turtle->prev;

    // TODO: delete the node
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *cur, *sp, *tail = NULL;
    bool isUniqueNum = true;
    list_for_each_safe (cur, sp, head) {
        if (cur->next != head) {
            if (isUniqueNum) {
                if (strcmp(list_entry(cur, element_t, list)->value,
                           list_entry(cur->next, element_t, list)->value) ==
                    0) {
                    list_del(cur);
                    q_release_element(list_entry(cur, element_t, list));
                    isUniqueNum = false;
                } else {
                    if (!tail) {
                        head->next = cur;
                        cur->prev = head;
                        tail = cur;
                    } else {
                        tail->next = cur;
                        cur->prev = tail;
                        tail = tail->next;
                    }
                }
            } else {
                if (strcmp(list_entry(cur, element_t, list)->value,
                           list_entry(cur->next, element_t, list)->value) !=
                    0) {
                    list_del(cur);
                    q_release_element(list_entry(cur, element_t, list));
                    isUniqueNum = true;
                } else {
                    list_del(cur);
                    q_release_element(list_entry(cur, element_t, list));
                }
            }
        } else {
            if (isUniqueNum) {
                if (!tail) {
                    head->next = cur;
                    cur->prev = head;
                } else {
                    tail->next = cur;
                    cur->prev = tail;
                    tail = tail->next;
                }
            } else {
                list_del(cur);
                q_release_element(list_entry(cur, element_t, list));
            }
        }
    }
    printf(" tail is null : %d \n", tail == NULL);
    if (tail) {
        tail->next = head;
        head->prev = tail;
    }

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
void q_reverse(struct list_head *head)
{
    if (q_size(head) > 1) {
        struct list_head *cur = head->next;
        struct list_head *nextNode;
        while (cur != head) {
            nextNode = cur->next;
            cur->next = cur->prev;
            cur->prev = nextNode;
            cur = nextNode;
        }
        cur = head->prev;
        head->prev = head->next;
        head->next = cur;
    }
}

void printList(struct list_head *head)
{
    struct list_head *cur = head->next;
    printf("print list : ");
    while (cur != head) {
        printf("%c  ", *list_entry(cur, element_t, list)->value);
        cur = cur->next;
    }
    printf("\n");
    printf("ttt");
}

void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    struct list_head *cur = head->next;
    struct list_head *tHead, *tTail, *nextNode;
    tHead = tTail = NULL;
    int size = q_size(head);
    while (size - k >= 0) {
        int temp = k;
        while (temp) {
            if (temp == k) {
                tHead = cur;
            }
            nextNode = cur->next;
            cur->next = cur->prev;
            cur->prev = nextNode;
            cur = nextNode;

            temp--;
            if (temp == 0) {
                tTail = cur->prev;
                break;
            }
        }
        tTail->prev = tHead->next;
        tTail->prev->next = tTail;
        tHead->next = cur;
        cur->prev = tHead;
        size = size - k;
    }
}

int cmpChar(const void *p1, const void *p2)
{
    const char *c1 = (const char *) p1;
    const char *c2 = (const char *) p2;
    return *c1 - *c2;
}


/**
 * q_sort() - Sort elements of queue in ascending order
 * @head: header of queue
 *
 * No effect if queue is NULL or empty. If there has only one element, do
 * nothing.
 */
void q_sort(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head))
        return;

    struct list_head list_less, list_greater;
    INIT_LIST_HEAD(&list_less);
    INIT_LIST_HEAD(&list_greater);

    element_t *pivot = list_first_entry(head, element_t, list);
    list_del(&pivot->list);

    element_t *itm = NULL, *is = NULL;
    list_for_each_entry_safe (itm, is, head, list) {  // CCC
        if (strcmp(itm->value, pivot->value) < 0)
            list_move(&itm->list, &list_less);  // DDD
        else
            list_move(&itm->list, &list_greater);  // EEE
    }

    q_sort(&list_less);
    q_sort(&list_greater);

    list_add(&pivot->list, head);
    list_splice(&list_less, head);
    list_splice_tail(&list_greater, head);  // FFF
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (q_size(head) > 1) {
        struct list_head *node, *temp;
        struct list_head *minNode = head->prev;
        // for (node = head->prev->prev; node != head; node = node->prev) {
        //     if (*list_entry(node, element_t, list)->value >=
        //         *list_entry(minNode, element_t, list)->value) {
        //         if (node->next != minNode) {
        //             minNode->prev = node;
        //             node->next = minNode;
        //         }
        //         minNode = node;
        //     }
        // }
        node = minNode->prev;
        while (node != head) {
            if (strcmp(list_entry(node, element_t, list)->value,
                       list_entry(minNode, element_t, list)->value) >= 0) {
                if (node->next != minNode) {
                    minNode->prev = node;
                    node->next = minNode;
                }
                minNode = node;
                node = node->prev;
            } else {
                temp = node;
                node = node->prev;
                list_del(temp);
                q_release_element(list_entry(temp, element_t, list));
            }
        }



        // node = minNode;
        while (node != head) {
            temp = node;
            node = node->prev;
            list_del(temp);
            q_release_element(list_entry(temp, element_t, list));
        }
        minNode->prev = head;
        head->next = minNode;
    }
    return q_size(head);
}


//////
void mergeTwoList(struct list_head *l1head, struct list_head *l2head)
{
    struct list_head *l1 = l1head->next, *l2 = l2head->next;

    while (l1 != l1head && l2 != l2head) {
        if (strcmp(list_entry(l1, element_t, list)->value,
                   list_entry(l2, element_t, list)->value) <= 0) {
            l1 = l1->next;
        } else {
            l2 = l2->next;
            struct list_head *insertNode = l2->prev;
            list_del(insertNode);
            list_add_tail(insertNode, l1);
        }
    }
    if (l2 != l2head)
        list_splice_tail(l2head, l1);
    INIT_LIST_HEAD(l2head);
}



/**
 * q_merge() - Merge all the queues into one sorted queue, which is in ascending
 * order.
 * @head: header of chain
 *
 * This function merge the second to the last queues in the chain into the first
 * queue. The queues are guaranteed to be sorted before this function is called.
 * No effect if there is only one queue in the chain. Allocation is disallowed
 * in this function. There is no need to free the 'qcontext_t' and its member
 * 'q' since they will be released externally. However, q_merge() is responsible
 * for making the queues to be NULL-queue, except the first one.
 *
 * Reference:
 * https://leetcode.com/problems/merge-k-sorted-lists/
 *
 * Return: the number of elements in queue after merging
 */

void IndPrintList(struct list_head **head, int num)
{
    struct list_head *cur = *head;
    cur = cur->next;
    printf("%p\n", *head);
    printf("print list : \n");
    while (num--) {
        printf("%p\n", cur);
        printf("-------------\n");
        printf("%c  ", *list_entry(cur, element_t, list)->value);
        cur = cur->next;
    }
    printf("head : %p\n", cur);
    printf("\n");
    printf("ttt");
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || head->next == head) {
        return 0;
    }


    queue_contex_t *firstQ = list_first_entry(head, queue_contex_t, chain);
    struct list_head *cur = head->next->next;

    int length = firstQ->size;
    while (cur != head) {
        queue_contex_t *curQ = list_entry(cur, queue_contex_t, chain);
        mergeTwoList(firstQ->q, curQ->q);
        length += curQ->size;
        cur = cur->next;
    }

    return length;
}
