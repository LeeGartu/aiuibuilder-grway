#include <rtthread.h>

#define LOG_TAG              "port"
#define LOG_LVL              LOG_LVL_DBG
#include "ulog.h"

void port_timer(void *param)
{
    LOG_D("hello from %s\n", __FILE__);
}

int port_entry(void)
{
    LOG_D("create timer");
    rt_timer_t _port_timer = rt_timer_create("port_timer", port_timer, NULL, 1000, RT_TIMER_FLAG_PERIODIC);
    LOG_D("start timer");
    rt_timer_start(_port_timer);

    return 0;
}

INIT_COMPONENT_EXPORT(port_entry);
