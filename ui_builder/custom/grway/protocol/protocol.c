#include "protocol.h"
#include <stdio.h>

struct protocol_list g_proto[PROTO_PORT_MAX];
bool_t g_slock[PROTO_PORT_MAX] = {0};

int user_uart_frame(uint8_t no, uint8_t *data, size_t len)
{
    printf("uart%d rx %d bytes\n", no + 1, len);

    return 0;
}

void protocol_list_init(struct protocol_list *plist)
{
    plist->head = 0;
    plist->tail = 0;
}

void protocol_list_clear(struct protocol_list *plist)
{
    plist->head = plist->tail;
}

int protocol_list_is_empty(struct protocol_list *plist)
{
    return plist->head == plist->tail;
}

int protocol_list_push(struct protocol_list *plist, struct protocol_node *node)
{
    if (((plist->tail + 1) % PROTO_NODE_NUM) == plist->head) {
        return -1;
    }
    plist->nodes[plist->tail] = *node;
    plist->tail = (plist->tail + 1) % PROTO_NODE_NUM;

    return 0;
}

int protocol_list_push_no(int no, struct protocol_node *node)
{
    return protocol_list_push(&g_proto[no], node);
}

int protocol_list_pop(struct protocol_list *plist, struct protocol_node *node)
{
    if (plist->head == plist->tail) {
        return -1;
    }
    *node = plist->nodes[plist->head];
    plist->head = (plist->head + 1) % PROTO_NODE_NUM;

    return 0;
}

void protocol_list_peek(struct protocol_list *plist, struct protocol_node *node)
{
    *node = plist->nodes[plist->head];
}
