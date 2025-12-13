
#ifndef __GW_LIST_H__
#define __GW_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gw_def.h"

/**
 * @addtogroup KernelService
 */

/**@{*/

/**
 * gw_container_of - return the start address of struct type, while ptr is the
 * member of struct type.
 */
#define gw_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (size_t)(&((type *)0)->member)))

/**
 * @brief initialize a list object
 */
#define GW_LIST_OBJECT_INIT(object) {&(object), &(object)}

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
gw_inline void gw_list_init(gw_list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
gw_inline void gw_list_inse_after(gw_list_t *l, gw_list_t *n)
{
    l->next->prev = n;
    n->next       = l->next;

    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
gw_inline void gw_list_inse_before(gw_list_t *l, gw_list_t *n)
{
    l->prev->next = n;
    n->prev       = l->prev;

    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
gw_inline void gw_list_remove(gw_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
gw_inline int gw_list_isempty(const gw_list_t *l)
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
gw_inline unsigned int gw_list_len(const gw_list_t *l)
{
    unsigned int     len = 0;
    const gw_list_t *p   = l;
    while (p->next != l) {
        p = p->next;
        len++;
    }

    return len;
}

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define gw_list_entry(node, type, member) \
    gw_container_of(node, type, member)

/**
 * gw_list_for_each - iterate over a list
 * @pos:    the gw_list_t * to use as a loop cursor.
 * @head:   the head for your list.
 */
#define gw_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * gw_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:    the gw_list_t * to use as a loop cursor.
 * @n:      another gw_list_t * to use as temporary storage
 * @head:   the head for your list.
 */
#define gw_list_for_each_safe(pos, n, head)                \
    for (pos = (head)->next, n = pos->next; pos != (head); \
         pos = n, n = pos->next)

/**
 * gw_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define gw_list_for_each_entry(pos, head, member)                 \
    for (pos = gw_list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head);                                  \
         pos = gw_list_entry(pos->member.next, typeof(*pos), member))

/**
 * gw_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define gw_list_for_each_entry_safe(pos, n, head, member)             \
    for (pos = gw_list_entry((head)->next, typeof(*pos), member),     \
        n    = gw_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head);                                      \
         pos = n, n = gw_list_entry(n->member.next, typeof(*n), member))

/**
 * gw_list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define gw_list_first_entry(ptr, type, member) \
    gw_list_entry((ptr)->next, type, member)

#define GW_SLIST_OBJECT_INIT(object) {NULL}

/**
 * @brief initialize a single list
 *
 * @param l the single list to be initialized
 */
gw_inline void gw_slist_init(gw_slist_t *l)
{
    l->next = NULL;
}

gw_inline void gw_slist_append(gw_slist_t *l, gw_slist_t *n)
{
    struct gw_slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next    = NULL;
}

gw_inline void gw_slist_insert(gw_slist_t *l, gw_slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

gw_inline unsigned int gw_slist_len(const gw_slist_t *l)
{
    unsigned int      len  = 0;
    const gw_slist_t *list = l->next;
    while (list != NULL) {
        list = list->next;
        len++;
    }

    return len;
}

gw_inline gw_slist_t *gw_slist_remove(gw_slist_t *l, gw_slist_t *n)
{
    /* remove slist head */
    struct gw_slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (gw_slist_t *)0) node->next = node->next->next;

    return l;
}

gw_inline gw_slist_t *gw_slist_first(gw_slist_t *l)
{
    return l->next;
}

gw_inline gw_slist_t *gw_slist_tail(gw_slist_t *l)
{
    while (l->next) l = l->next;

    return l;
}

gw_inline gw_slist_t *gw_slist_next(gw_slist_t *n)
{
    return n->next;
}

gw_inline int gw_slist_isempty(gw_slist_t *l)
{
    return l->next == NULL;
}

/**
 * @brief get the struct for this single list node
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define gw_slist_entry(node, type, member) \
    gw_container_of(node, type, member)

/**
 * gw_slist_for_each - iterate over a single list
 * @pos:    the gw_slist_t * to use as a loop cursor.
 * @head:   the head for your single list.
 */
#define gw_slist_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

/**
 * gw_slist_for_each_entry  -   iterate over single list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your single list.
 * @member: the name of the list_struct within the struct.
 */
#define gw_slist_for_each_entry(pos, head, member)                 \
    for (pos = gw_slist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (NULL);                                   \
         pos = gw_slist_entry(pos->member.next, typeof(*pos), member))

/**
 * gw_slist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define gw_slist_first_entry(ptr, type, member) \
    gw_slist_entry((ptr)->next, type, member)

/**
 * gw_slist_tail_entry - get the tail element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define gw_slist_tail_entry(ptr, type, member) \
    gw_slist_entry(gw_slist_tail(ptr), type, member)

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
