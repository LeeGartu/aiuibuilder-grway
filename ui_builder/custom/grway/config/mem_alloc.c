#define MEM_ALLOC_EXT
#include "mem_alloc.h"

bool_t mem_is_ro_regs(uint16_t sAddr, uint16_t eAddr, int16_t **buf)
{
    if ((RO_MEM_STATUS_BASE<=sAddr)&&(eAddr<(RO_MEM_STATUS_BASE+RO_MEM_STATUS_SIZE))) {
        *buf=(int16_t*)&ro_mem_status[sAddr-RO_MEM_STATUS_BASE];
        return TRUE;
    }
    return FALSE;
}

bool_t mem_is_rw_regs(uint16_t sAddr, uint16_t eAddr, int16_t **buf)
{
    if ((RW_MEM_PARAM_BASE<=sAddr)&&(eAddr<(RW_MEM_PARAM_BASE+RW_MEM_PARAM_SIZE))) {
        *buf=(int16_t*)&rw_mem_param[sAddr-RW_MEM_PARAM_BASE];
        return TRUE;
    }
    return FALSE;
}

void mem_alloc_init(void)
{
    memset(ro_mem_status, 0, sizeof(ro_mem_status));
    memset(rw_mem_param, 0, sizeof(rw_mem_param));
}
