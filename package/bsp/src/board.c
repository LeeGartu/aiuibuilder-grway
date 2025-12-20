#include "uart.h"
#include <rtthread.h>

int board_init(void)
{
    rt_components_init();
    // uart_init();
    // port_entry();
    return 0;
}

