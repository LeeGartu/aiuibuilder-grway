#include "uart.h"

int board_init(void)
{
    uart_init();
    return 0;
}
