#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "../utils/gw_utils.h"

/* 最多支持 3 路，可按需扩大 */
#define PROTO_PORT_MAX        1

typedef enum p_type_ {
    P_TYPE_MODBUS,
    P_TYPE_CCDX,
    P_TYPE_TUYA,
    P_TYPE_MAX,
} p_type_e;

enum recv_type_e {
    RECV_PARSEING,
    RECV_SUCCESS,
    RECV_DATA_ERR,
    RECV_FORMAT_ERR,
    RECV_EVENT,
};

struct modbus_cmd {
    uint8_t dev_addr;
    uint8_t func_code;
    uint16_t addr;
    uint16_t num;
    int16_t var;
    int16_t *buf;
};

struct ccdx_cmd {
    uint8_t cmd;
};

struct tuya_cmd {
    uint8_t cmd;
    uint16_t idx;
    uint8_t num;
};

struct protocol_node {
    union protocol_cmd {
        struct modbus_cmd modbus;
        struct ccdx_cmd ccdx;
        struct tuya_cmd tuya;
    } cmd;
    p_type_e type;
    int need_recv;
    int try_cnt;
    int         tx_dly1ms_tm;   // 发送延时计数
    int         rx_ovf1ms_tm;     // 接收超时计数
};

#define PROTO_NODE_NUM 16
struct protocol_list {
    struct protocol_node nodes[PROTO_NODE_NUM];
    uint8_t head;
    uint8_t tail;
};

typedef void (*pack_fn_t)(struct protocol_node *node, uint8_t *out_buf, uint16_t *out_len);
typedef int (*unpack_fn_t)(struct protocol_node *node, uint8_t *in_buf, uint16_t *in_len, uint8_t *out_buf, uint16_t *out_len);
typedef struct p_func{
    p_type_e       type;
    pack_fn_t      pack_fn;
    unpack_fn_t    unpack_fn;
} p_func_t;
#define REG_PROTO(_type, _pack_fn, _unpack_fn)  \
    static const p_func_t __attribute__((used, section("proto_tab"))) \
    _proto_##_type = { _type, _pack_fn, _unpack_fn }

struct protocol_recv {
    bool_t (*recv)(uint8_t *data, size_t len);
    struct gw_list_node list;
};

void protocol_list_init(struct protocol_list *plist);
void protocol_list_clear(struct protocol_list *plist);
int protocol_list_is_empty(struct protocol_list *plist);
int protocol_list_push(struct protocol_list *plist, struct protocol_node *node);
int protocol_list_push_no(int no, struct protocol_node *node);
int protocol_list_pop(struct protocol_list *plist, struct protocol_node *node);
void protocol_list_peek(struct protocol_list *plist, struct protocol_node *node);


#endif
