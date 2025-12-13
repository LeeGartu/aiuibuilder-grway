#include "port.h"
#include <stdio.h>
#include <pthread.h>

#define PORT_MAX 8
static struct uart_port g_ports[PORT_MAX];
int g_port_num;

int port_scan(char *port_name, int i)
{
    HANDLE hCom;

    sprintf(port_name, "\\\\.\\COM%d", i);
    hCom = CreateFile(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hCom != INVALID_HANDLE_VALUE) {
        sprintf(port_name, "COM%d", i);
        CloseHandle(hCom);
        return 0;
    }

    return -1;
}

HANDLE port_open(int i)
{
    HANDLE hCom;
    char port_name[20];

    sprintf(port_name, "\\\\.\\COM%d", i);
    hCom = CreateFile(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hCom != INVALID_HANDLE_VALUE) {
        printf("COM%d opened\n", i);
        return hCom;
    }

    printf("COM%d not opened\n", i);
    return NULL;
}

int port_set_cfg(HANDLE hCom, int baudrate, int databits, int parity, int stopbits)
{
    // 检查传入的句柄是否有效
    if (hCom == NULL) {
        return -1; // 返回 -1 表示句柄无效
    }

    // 打印串口参数
    printf("Baud Rate: %d\n", baudrate);
    printf("Data Bits: %d\n", databits);
    printf("Parity: %d\n", parity);
    printf("Stop Bits: %d\n", stopbits);

    DCB dcb;
    // 获取当前串口的 DCB 设置
    GetCommState(hCom, &dcb);

    // 设置波特率，常见的波特率有 9600, 19200, 115200 等
    dcb.BaudRate = baudrate;

    // 设置数据位，常见的数据位数有 5, 6, 7, 8
    dcb.ByteSize = databits;

    // 设置校验位，可选参数如下：
    // NOPARITY (无校验)，ODDPARITY (奇校验)，EVENPARITY (偶校验)，MARKPARITY (标记校验)，SPACEPARITY (空格校验)
    dcb.Parity = parity;

    // 设置停止位，常见的停止位有 ONESTOPBIT (1), ONE5STOPBITS (1.5), TWOSTOPBITS (2)
    dcb.StopBits = stopbits;

    // 应用新的 DCB 设置到串口
    SetCommState(hCom, &dcb);

    COMMTIMEOUTS timeouts;
    // 计算传输一个字节所需的时间（毫秒）
    int bits_per_byte = 1 + databits + (stopbits == ONESTOPBIT ? 1 : 2) + (parity != NOPARITY ? 1 : 0);
    int time_per_byte_ms = (1000 * bits_per_byte) / baudrate;
    if (time_per_byte_ms < 1) {
        time_per_byte_ms = 1;
    }

    // 设置超时时间
    timeouts.ReadIntervalTimeout         = time_per_byte_ms*15;
    timeouts.ReadTotalTimeoutMultiplier  = time_per_byte_ms*2;
    timeouts.ReadTotalTimeoutConstant    = time_per_byte_ms*15;
    timeouts.WriteTotalTimeoutMultiplier = time_per_byte_ms*2;
    timeouts.WriteTotalTimeoutConstant   = time_per_byte_ms*5;
    SetCommTimeouts(hCom, &timeouts);

    return 0; // 返回 0 表示配置成功
}

void port_send(uint8_t no, uint8_t *data, size_t len)
{
    if (no>=g_port_num) return;
    DWORD dwWritten;
    struct uart_port *p = &g_ports[no];
    WriteFile(p->hCom, data, len, &dwWritten, NULL);
}

void* rx_thread_entry(void* arg)
{
    while(1) {
        for (int i = 0; i<g_port_num; i++) {
            struct uart_port *p = &g_ports[i];
            uint8_t ch = '\n';
            DWORD len;
            while (ReadFile(p->hCom, &ch, 1, &len, NULL)) {
                if (len==0) {
                    if (ch != '\n') ch = '\t';
                    break;
                }
                p->ovf1ms_cnt = 0;
                if (p->rd < p->size) {
                    p->buf[p->rd] = ch;
                    p->rd++;
                }
            }
            if ((p->rd>2 && protocol_decode)&&(ch=='\t')) {
                protocol_decode(i, p->buf, p->rd);
            }

        }
    }
    return NULL;
}

enum {
    TX_IDLE,
    TX_SEND_DLY,
    TX_WAIT_RECV,
};

void *tx_thread_entry(void *arg)
{
    while (1) {
        for (int i = 0; i < g_port_num; i++) {
            struct uart_port *p = &g_ports[i];
            switch (p->tx_sta) {
                case TX_IDLE:
                    if (protocol_encode(i, p->tx_buf, &p->tx_len, &p->tx_dly1ms_tm, &p->rx_ovf1ms_tm)) {
                        p->tx_dly1ms_cnt = 0;
                        p->tx_sta = TX_SEND_DLY;
                    }
                    break;
                case TX_SEND_DLY:
                    if (p->tx_dly1ms_cnt >= p->tx_dly1ms_tm) {
                        port_send(i, p->tx_buf, p->tx_len);
                        p->tx_len = 0;
                        p->tx_dly1ms_cnt = 0;
                        p->tx_sta = TX_WAIT_RECV;
                        p->rx_ovf1ms_cnt = 0;
                    }
                    break;
                case TX_WAIT_RECV:
                    if (p->rx_ovf1ms_cnt >= p->rx_ovf1ms_tm) {
                        protocol_ovtime(i);
                        p->tx_sta = TX_IDLE;
                    }
                    break;
                default:
                    break;
            }
        }
        Sleep(1);
    }
    return NULL;
}

#define RX_OVF_CNT 20
// 定时器回调函数
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    for (int i = 0; i < g_port_num; i++) {
        struct uart_port *p = &g_ports[i];
        if (p->ovf1ms_cnt<RX_OVF_CNT) {
            p->ovf1ms_cnt++;
        } else {
            p->rd = 0;
        }
        if (p->tx_dly1ms_cnt<p->tx_dly1ms_tm) {
            p->tx_dly1ms_cnt++;
        }
        if (p->rx_ovf1ms_cnt<p->rx_ovf1ms_tm) {
            p->rx_ovf1ms_cnt++;
        }
    }
}

int port_init(struct port_cfg *tab, int num)
{
    for (int i = 0; i < num; i++) {
        struct uart_port *p = &g_ports[i];
        p->cfg = tab[i];
        p->size = 512;
        p->buf = malloc(p->size);
        p->rd = 0;
        p->hCom = port_open(p->cfg.port_num);
        if (p->hCom!=NULL) {
            port_set_cfg(p->hCom, p->cfg.baudrate, p->cfg.databits, p->cfg.parity, p->cfg.stopbits);
        }
    }
    g_port_num = num;

    // 创建线程
    pthread_t rx_thread;
    if (pthread_create(&rx_thread, NULL, rx_thread_entry, NULL) != 0) {
        perror("pthread_create");
    }
    pthread_t tx_thread;
    if (pthread_create(&tx_thread, NULL, tx_thread_entry, NULL) != 0) {
        perror("pthread_create");
    }

    // 创建一个定时器，设置为 1ms 触发
    UINT_PTR timerId = SetTimer(NULL, 0, 1, TimerProc);

    return 0;
}
