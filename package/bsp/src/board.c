#include "uart.h"
#include <rtthread.h>

int board_init(void)
{
    if (uart_init()<0) {
        return -1;
    }
    rt_components_init();
    return 0;
}

