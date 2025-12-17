#include "libserialport.h"
#include <stdio.h>

int selectport(void)
{
    struct sp_port **port_list;

    enum sp_return ret = sp_list_ports(&port_list);
    
    if (ret != SP_OK) {
        return -1;
    } else {
        for (int i = 0; port_list[i] != NULL; i++) {
            printf("Port %d: %s\n", i, sp_get_port_name(port_list[i]));
        }
    }
    return 0;
}