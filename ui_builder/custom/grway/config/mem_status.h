/**
 * @file mem_status.h
 * @brief 状态内存分配文件
 *
 * @defgroup mem_status_def 参数声明
 * @defgroup mem_status_len 参数长度
 * @defgroup mem_status_buf 不同类型数组定义
 */
#ifndef __GW_MEM_STATUS_H__
#define __GW_MEM_STATUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../mem/mem.h"
#include "mem_alloc.h"

gw_inline void status_freak_var(void *pvar, int32_t *var)
{
    pvar = pvar;
    var  = var;
}

gw_inline void status_freak_offset(mem_t *status_u, uint32_t offset)
{
    status_u->pvar = PTR_INCREMENT(status_u->pvar, uint8_t, offset);
}

/**
 * @addtogroup mem_status_def 参数声明
 * @{
 */
// clang-format off
#define status_flagsave_ram				istatus_buf(0)//参数标志定义(如:开/关机,制冷/制热,手动电热,膨胀阀手动/自动...)
#define status_xkpower_f				bstatus_buf(0,0)//线控开关机标志
#define status_stepmd_f					bstatus_buf(0,1)
#define status_bptest_f					bstatus_buf(0,2)
#define status_fanmode_f				bstatus_buf(0,3)
#define status_resetcmpadd_f			bstatus_buf(0,4)
#define status_stepbmd_f				bstatus_buf(0,5)
#define status_stepinimd_f				bstatus_buf(0,6)
#define status_val4type_f				bstatus_buf(0,7)

#define status_stepmin_auto0			istatus_buf(33)//*2 P
#define status_stepmin_auto1			istatus_buf(34)
#define status_stepmin_auto2			istatus_buf(35)
#define status_stepmin_auto3			istatus_buf(36)
#define status_stepmin_auto4			istatus_buf(37)
#define status_stepmin_auto5			istatus_buf(38)
#define status_stepmin_auto6			istatus_buf(39)
#define status_stepmin_auto7			istatus_buf(40)

// clang-format on
/** @} */

/**
 * @addtogroup mem_status_len 参数长度
 * @{
 */
#define STATUS_LEN (490)
#define MACH_UNITS_NUM  1
/** @} */

/**
 * @addtogroup mem_status_buf 不同类型数组定义
 * @{
 */
#define status_buf(i)     (((uint16_t *)ro_mem_status)[(i)])
#define istatus_buf(i)    (((int16_t *)ro_mem_status)[(i)])
#define bstatus_buf(i, b) (((bit16_t *)ro_mem_status)[(i)].bits.bit##b)

#define status_8buf(i)     (((uint8_t *)ro_mem_status)[(i)])
#define istatus_8buf(i)    (((int8_t *)ro_mem_status)[(i)])
#define bstatus_8buf(i, b) (((bit8_t *)ro_mem_status)[(i)].bits.bit##b)

#define status_8_16buf(i)     (*(uint16_t *)&(ro_mem_status[(i)]))
#define istatus_8_16buf(i)    (*(int16_t *)&(ro_mem_status[(i)]))
#define bstatus_8_16buf(i, b) ((*(bit16_t *)&(ro_mem_status[(i)])).bits.bit##b)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __GW_MEM_STATUS_H__ */
