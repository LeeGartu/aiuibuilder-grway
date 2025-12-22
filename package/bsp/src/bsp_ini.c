#include "ini.h"
#include <stdlib.h>
#include <string.h>
#include <rtdevice.h>

// 添加头文件用于 access 函数
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

typedef struct {
    int version;
    int uart_index;
    int baud_rate;
    int data_bits;
    int parity;
    int stop_bits;
} configuration;

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("protocol", "version")) {
        pconfig->version = atoi(value);
    } else if (MATCH("user", "uart_index")) {
        pconfig->uart_index = atoi(value);
    } else if (MATCH("user", "baud_rate")) {
        pconfig->baud_rate = atoi(value);
    } else if (MATCH("user", "data_bits")) {
        pconfig->data_bits = atoi(value);
    } else if (MATCH("user", "parity")) {
        pconfig->parity = atoi(value);
    } else if (MATCH("user", "stop_bits")) {
        pconfig->stop_bits = atoi(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return -1;
}

int _set_ini(configuration* cfg)
{
    FILE* file = fopen("uart.ini", "w");
    if (!file) {
        printf("Failed to open 'uart.ini' for writing\n");
        return -1;
    }

    // 写入协议部分
    fprintf(file, "[protocol]\n");
    fprintf(file, "version = 1\n\n");
    
    // 写入用户配置部分
    fprintf(file, "[user]\n");
    fprintf(file, "uart_index = %d\n", cfg->uart_index);
    fprintf(file, "baud_rate = %d\n", cfg->baud_rate);
    fprintf(file, "data_bits = %d\n", cfg->data_bits);
    fprintf(file, "parity = %d\n", cfg->parity);
    fprintf(file, "stop_bits = %d\n", cfg->stop_bits);

    if (ferror(file)) {
        printf("Error occurred while writing to 'uart.ini'\n");
        fclose(file);
        return -1;
    }

    fclose(file);
    printf("Configuration saved to 'uart.ini'\n");
    return 0;
}

int set_ini(struct serial_configure* cfg)
{
    configuration config;
    
    config.uart_index = cfg->uart_index;
    config.baud_rate = cfg->baud_rate;
    config.data_bits = cfg->data_bits;
    config.parity = cfg->parity;
    config.stop_bits = cfg->stop_bits;

    return _set_ini(&config);
}

int get_ini(struct serial_configure* cfg)
{
    configuration config;
    config.version = 0;  /* set defaults */
    config.uart_index = 1;
    config.baud_rate = 9600;
    config.data_bits = 8;
    config.parity = 0;
    config.stop_bits = 1;
    
    if (access("uart.ini", F_OK) == -1) {
        printf("uart.ini not found, creating default configuration...\n");
        if (_set_ini(&config) < 0) {
            printf("Failed to create default configuration file\n");
            return -1;;
        }
    }

    if (ini_parse("uart.ini", handler, &config) < 0) {
        printf("Can't load 'uart.ini'\n");
        return -1;
    }
    cfg->uart_index = config.uart_index;
    cfg->baud_rate = config.baud_rate;
    cfg->data_bits = config.data_bits;
    cfg->parity = config.parity;
    cfg->stop_bits = config.stop_bits;

    return 0;
}
