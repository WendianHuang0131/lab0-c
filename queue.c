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
    element_t *newElement = malloc(sizeof(element_t));
    if (!newElement)
        return false;
    newElement->value = strdup(s);
    if (!newElement->value) {
        free(newElement);
        return false;
    }
    list_add(&newElement->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *newElement = malloc(sizeof(element_t));
    if (!newElement)
        return false;
    newElement->value = strdup(s);
    if (!newElement->value) {
        free(newElement);
        return false;
    }
    list_add_tail(&newElement->list, head);
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
    if (q_size(head) == 0)
        return NULL;

    element_t *element = list_entry(head->next, element_t, list);

    if (sp) {
        strncpy(sp, element->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->next);


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
    if (q_size(head) == 0)
        return NULL;

    element_t *element = list_entry(head->prev, element_t, list);

    if (sp) {
        strncpy(sp, element->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->prev);

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
                if (*list_entry(cur, element_t, list)->value ==
                    *list_entry(cur->next, element_t, list)->value) {
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
                if (*list_entry(cur, element_t, list)->value !=
                    *list_entry(cur->next, element_t, list)->value) {
                    q_release_element(list_entry(cur, element_t, list));
                    isUniqueNum = true;
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
                q_release_element(list_entry(cur, element_t, list));
            }
        }
    }
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
        if (cmpChar(itm->value, pivot->value) < 0)
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
        struct list_head *node;
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
        node = head->prev->prev;
        while (node != head) {
            if (*list_entry(node, element_t, list)->value >=
                *list_entry(minNode, element_t, list)->value) {
                if (node->next != minNode) {
                    minNode->prev = node;
                    node->next = minNode;
                }
                minNode = node;
                node = node->prev;
            } else {
                node = node->prev;
                q_release_element(list_entry(node->next, element_t, list));
            }
        }



        node = minNode;
        while (node != head) {
            node = node->prev;
            q_release_element(list_entry(node->next, element_t, list));
        }
        if (minNode->prev != head) {
            printf("Wrong!!!");
        }
        minNode->prev = head;
        head->next = minNode;
    }
    return q_size(head);
}


//////
struct list_head *mergeTwoList(struct list_head *l1head,
                               struct list_head *l2head)
{
    struct list_head *rt = l1head;
    // struct list_head *cur = &rt;
    struct list_head *l1 = l1head->next, *l2 = l2head->next;
    // printList(l1head);
    // printList(l2head);
    // printf("%p \n", l1);
    // printf("%p \n", l1head);
    // printf("%p \n", l2);
    // printf("%p \n", l2head);



    printf("test=================");
    printf("%d \n", l1 == l1head);
    printf("%d \n ", l2 == l2head);
    // printf("l1 = l1head : %d, l2 = l2head : %d", l1 == l1head, l2 == l2head);
    // INIT_LIST_HEAD(rt);
    printf("test=================");
    while (l1 != l1head && l2 != l2head) {
        if (*list_entry(l1, element_t, list)->value <=
            *list_entry(l2, element_t, list)->value) {
            l1 = l1->next;
        } else {
            l2 = l2->next;
            struct list_head *insertNode = l2->prev;
            list_del(insertNode);
            list_add_tail(insertNode, l1);
        }
    }
    if (l2 != l2head)
        list_splice_tail(l2head, rt);
    INIT_LIST_HEAD(l2head);
    return rt;
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

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    // struct list_head *cur = head -> next;
    // int length = 0;
    // while(cur != head){
    //     queue_contex_t *q_contex = list_entry(cur, queue_contex_t, chain);
    //     // printList(q_contex -> q);
    //     if(q_contex -> size == 0)
    //         continue;

    //     length += q_contex -> size;
    //     //head = mergeTwoList()
    //     cur = cur -> next;
    // }

    // queue_contex_t *q1 = list_entry(head->next, queue_contex_t, chain);
    // queue_contex_t *q2 = list_entry(head->next->next, queue_contex_t, chain);

    // // printList(q1->q);
    // // printList(q2->q);
    // head = mergeTwoList(q1->q, q2->q);
    // head = mergeTwoList(&q1 -> chain, &q2 -> chain);

    return 8;
}
