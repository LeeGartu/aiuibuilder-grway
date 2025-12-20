#include "uart.h"
#include <rtthread.h>

extern int port_entry(void);
// static void* __force_ref = (void*)port_entry;  // 强制引用，防止被优化掉
int board_init(void)
{
    rt_components_init();
    // uart_init();
    return 0;
}

