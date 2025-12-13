/**
 * @file mem_param.h
 * @brief 参数内存分配文件
 *
 * @defgroup param_freak 参数异常处理
 * @defgroup mem_param_def 参数定义
 * @defgroup mem_param_len 参数长度
 * @defgroup mem_param_buf 不同类型数组定义
 * @defgroup mem_param_declare 参数声明
 */
#ifndef __GW_MEM_PARAM_H__
#define __GW_MEM_PARAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../mem/mem.h"
#include "mem_alloc.h"

/**
 * @addtogroup param_freak 参数异常处理
 * @{
 */
gw_inline void param_freak_min_max(void *pvar, uint8_t var_type, int32_t *var, bool_t *is_float)
{
#define pvar_type_float_var(_pvar, _type, _var, _is_float) \
    if ((pvar == &_pvar) && (var_type == _type)) {         \
        *var      = _var;                                  \
        *is_float = _is_float;                             \
    }
}

gw_inline void param_freak_var(void *pvar, int32_t *var, uint8_t und)
{
    if (und == UP) { // 显示
    } else { // 保存
    }
}

gw_inline void param_freak_offset(mem_range_t *param_u, uint32_t offset)
{
    param_u->mem.pvar = PTR_INCREMENT(param_u->mem.pvar, uint8_t, offset);
}
/** @} */

/**
 * @addtogroup mem_param_def 参数定义
 * @{
 */
// clang-format off
#define param_flagsave_ram				iparam_buf(0)//参数标志定义(如:开/关机,制冷/制热,手动电热,膨胀阀手动/自动...)
#define param_xkpower_f					bparam_buf(0,0)//线控开关机标志
#define param_stepmd_f					bparam_buf(0,1)
#define param_bptest_f					bparam_buf(0,2)
#define param_fanmode_f					bparam_buf(0,3)
#define param_resetcmpadd_f				bparam_buf(0,4)
#define param_stepbmd_f					bparam_buf(0,5)
#define param_stepinimd_f				bparam_buf(0,6)
#define param_val4type_f				bparam_buf(0,7)

#define param_stepmin_auto0				iparam_buf(33)//*2 P
#define param_stepmin_auto1				iparam_buf(34)
#define param_stepmin_auto2				iparam_buf(35)
#define param_stepmin_auto3				iparam_buf(36)
#define param_stepmin_auto4				iparam_buf(37)
#define param_stepmin_auto5				iparam_buf(38)
#define param_stepmin_auto6				iparam_buf(39)
#define param_stepmin_auto7				iparam_buf(40)
// clang-format on
/** @} */

/**
 * @addtogroup mem_param_len 参数长度
 * @{
 */
#define PARAM_LEN (490)
/** @} */

/**
 * @addtogroup mem_param_buf 不同类型数组定义
 * @{
 */
#define param_buf(i)     (((uint16_t *)rw_mem_param)[(i)])
#define iparam_buf(i)    (((int16_t *)rw_mem_param)[(i)])
#define bparam_buf(i, b) (((bit16_t *)rw_mem_param)[(i)].bits.bit##b)

#define param_8buf(i)     (((uint8_t *)rw_mem_param)[(i)])
#define iparam_8buf(i)    (((int8_t *)rw_mem_param)[(i)])
#define bparam_8buf(i, b) (((bit8_t *)rw_mem_param)[(i)].bits.bit##b)

#define param_8_16buf(i)     (*(uint16_t *)&(rw_mem_param[(i)]))
#define iparam_8_16buf(i)    (*(int16_t *)&(rw_mem_param[(i)]))
#define bparam_8_16buf(i, b) ((*(bit16_t *)&(rw_mem_param[(i)])).bits.bit##b)
/** @} */

/**
 * @addtogroup mem_param_def 参数声明
 * @{
 */
extern const mem_range_t  param_tab_user[];
extern const mem_range_t *param_tab_list[];
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __GW_MEM_PARAM_H__ */
