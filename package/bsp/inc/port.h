#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>
#include "windows.h"

struct port_cfg {
	char port_id[10];
    int port_num;
    int baudrate;
    int databits;
    int parity;
    int stopbits;
};

/* 运行时表项 */
struct uart_port {
    struct port_cfg cfg;
    HANDLE      hCom;
    uint8_t     tx_sta;
    uint8_t     *tx_buf;
    size_t      tx_len;        /* 软件指针 */
    uint8_t     *buf;
    int         size;
    DWORD       rd;        /* 软件指针 */
    uint32_t    event_bit;     /* 1<<idx */
    
    int         ovf1ms_cnt;  // 接收溢出计数
    int         tx_dly1ms_cnt;   // 发送延时计数
    int         tx_dly1ms_tm;   // 发送延时计数
    int         rx_ovf1ms_cnt;     // 接受超时计数
    int         rx_ovf1ms_tm;     // 接受超时计数
};

int port_scan(char *port_name, int i);
int port_init(struct port_cfg *tab, int num);

__attribute__((weak)) void protocol_decode(uint8_t no, uint8_t *data, size_t len);
__attribute__((weak)) int protocol_encode(uint8_t no, uint8_t *data, size_t *len, int *tx_dly1ms_tm, int *rx_ovf1ms_tm);
__attribute__((weak)) void protocol_ovtime(uint8_t no);

#endif // !__PORT_H__