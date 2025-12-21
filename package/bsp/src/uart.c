#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "libserialport.h"

#define LOG_TAG              "uart"
#define LOG_LVL              LOG_LVL_DBG
#define ULOG_OUTPUT_LVL                LOG_LVL_DBG
#include "ulog.h"

struct rt_serial_device serial;

int get_port_cfg(struct serial_configure *cfg)
{
    int get_ini(struct serial_configure* cfg);
    int set_ini(struct serial_configure* cfg);

    struct sp_port **ports;
    enum sp_return ret = sp_list_ports(&ports);
    
    if (ret != SP_OK) {
        fprintf(stderr, "Failed to list ports\n");
        return -1;
    }

    if (ports[0] == NULL) {
        printf("No serial ports found.\n");
        LOG_E("No serial ports found.\n");
        return -1;
    }

    printf("Available serial ports:\n");
    int count;
    for (count = 0; ports[count]; count++) {
        const char *name = sp_get_port_name(ports[count]);
        const char *desc = sp_get_port_description(ports[count]);
        printf("  [%d] %s (%s)\n", count, name, desc ? desc : "no description");
    }
    sp_free_port_list(ports);
    count--;
    
    // 获取串口名
    while (1) {
        printf("\nEnter port index 0~%d ('q' to quit): ", count);
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) return -1;

        if (input[0] >= '0' && input[0] <= ('0'+count)) {
            cfg->uart_index = atoi(input);
            break;
        } else if (input[0] == 'q') {
            return -1;
        } else if (input[0] == '\r' || input[0] == '\n') {
            return get_ini(cfg);
        }
    }

    // 获取串口配置
    while (1) {
        printf("\nEnter baudrate databits parity stopbits ('q' to quit)");
        printf("\nExample: 9600 8 0 1 (N=0, O=1, E=2):");
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) return -1;
        if (input[0] == 'q') {
            ret = -1;
            break;
        }
        int32_t data_bits,parity,stop_bits;
        if (sscanf(input, "%d %d %c %d", &cfg->baud_rate, &data_bits, &parity, &stop_bits) == 4) {
            cfg->data_bits = data_bits;
            cfg->parity = parity;
            cfg->stop_bits = stop_bits;
            ret = set_ini(cfg);
            break;
        }
    }

    return ret;
}

int open_port(struct sp_port **port, struct serial_configure *cfg)
{
    struct sp_port **ports;

    enum sp_return ret = sp_list_ports(&ports);
    
    if (ret != SP_OK) {
        fprintf(stderr, "Failed to list ports\n");
        return -1;
    }
    *port = ports[cfg->uart_index];

    // 打开串口
    if (sp_open(*port, SP_MODE_READ_WRITE) != SP_OK) {
        fprintf(stderr, "Cannot open port %d\n", cfg->uart_index);
        sp_free_port(*port);
        return -1;
    }

    // 设置常用参数
    sp_set_baudrate(*port, cfg->baud_rate);
    sp_set_bits(*port, cfg->data_bits);
    sp_set_parity(*port, cfg->parity);
    sp_set_stopbits(*port, cfg->stop_bits);

    printf("%s opened at %u %d %d %d\n", sp_get_port_name(*port), cfg->baud_rate, cfg->data_bits, cfg->parity, cfg->stop_bits);

    return 0;
}

static rt_err_t drv_uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    int ret;
    uint32_t index = 0;
    struct sp_port *port;

    RT_ASSERT(serial != RT_NULL);
    port = (struct sp_port *)serial->parent.user_data;
    RT_ASSERT(port != RT_NULL);

    sp_set_baudrate(port, cfg->baud_rate);
    sp_set_bits(port, cfg->data_bits);
    sp_set_parity(port, cfg->parity);
    sp_set_stopbits(port, cfg->stop_bits);

    return RT_EOK;
}

static rt_err_t drv_uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct sp_port *port;

    RT_ASSERT(serial != RT_NULL);
    port = (struct sp_port *)serial->parent.user_data;
    RT_ASSERT(port != RT_NULL);

    return (RT_EOK);
}

static int drv_uart_putc(struct rt_serial_device *serial, char c)
{
    struct sp_port *port;

    RT_ASSERT(serial != RT_NULL);
    port = (struct sp_port *)serial->parent.user_data;
    RT_ASSERT(port != RT_NULL);

    return (sp_blocking_write(port, &c, 1, 0)==0)?(1):(-1);
}

static int drv_uart_getc(struct rt_serial_device *serial)
{
    int ch;
    char c;
    struct sp_port *port;

    RT_ASSERT(serial != RT_NULL);
    port = (struct sp_port *)serial->parent.user_data;
    RT_ASSERT(port != RT_NULL);
    
    // 如果成功读取到数据，返回字符；否则返回负值表示无数据
    if (sp_blocking_read(port, &c, 1, 0) == 0) {
        ch = (int)c;
    } else {
        ch = -1; // 表示没有读取到数据
    }

    return ch;
}

const struct rt_uart_ops drv_uart_ops =
{
    drv_uart_configure,
    drv_uart_control,
    drv_uart_putc,
    drv_uart_getc,
};

rt_err_t port_rx_ind(rt_device_t dev, rt_size_t size)
{
    uint8_t buf[255];
    rt_device_read(dev, 0, buf, size);

    printf("uart_rx: %s\n", buf);
}

int uart_init(void)
{
    struct sp_port *port;
    if (get_port_cfg(&serial.config) < 0) return -1;
    if (open_port(&port, &serial.config) < 0) return -1;

    rt_hw_serial_register(&serial, "uart1", RT_DEVICE_FLAG_RDWR, port);
    struct rt_device *dev = rt_device_find("uart1");
    rt_device_set_rx_indicate(dev, port_rx_ind);
    rt_device_open(dev, RT_DEVICE_OFLAG_RDWR);

    return 0;
}
