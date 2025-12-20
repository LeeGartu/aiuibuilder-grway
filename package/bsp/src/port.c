#include <rtthread.h>

int port_entry(void)
{
    printf("\nhello from %s", __FILE__);
    return 0;
}

INIT_COMPONENT_EXPORT(port_entry);
