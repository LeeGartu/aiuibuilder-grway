#ifndef __GW_CF_CONFIG_H__
#define __GW_CF_CONFIG_H__

#include "../utils/gw_utils.h"
#include "mem_param.h"
#include "mem_status.h"

#define CF_M1_INIT    (0x7FFF)
#define CF_M1_IDX_NUL (0xFFFF)
#define CF_ERROR_CODE (756) // 错误码

#define CF_M1_PARAM_NUM     (1)
#define CF_M1_PARAM_U_NUM   (1)
#define CF_M1_PARAM_ALL_NUM (CF_M1_PARAM_NUM + CF_M1_PARAM_U_NUM * 1)
typedef enum {
    TEMP_C,
    TEMP_F,
} temp_cf_e;
#define TEMP_CF     (param_xkpower_f)
#define IS_TEMP_F() (TEMP_CF == TEMP_F)

/**
 * @brief 统计回差类参数
 */
#define cf_is_diff(pvar) \
    (pvar == NULL)

/**
 * @brief 获取1精度温度的索引
 *
 * @param pvar
 * @return uint16_t
 */
gw_inline uint16_t cf_get_temp_idx(void *pvar)
{
    uint16_t base = 0;
    uint16_t i;

    /* 参数类 */
    if (NULL == pvar) { // 1
        return base;
    }
    base += 1;

    /* 从机参数类 */
    for (i = 0; i < MACH_UNITS_NUM; i++) {
        if (NULL == pvar) { // 1
            return base;
        }
        base += 1;
        if (pvar < NULL) {
            return CF_M1_IDX_NUL;
        }
        /* 注意！需要根据不同的变量类型设置指针偏移 int8_t, int16_t, int32_t */
        pvar = PTR_DECREMENT(pvar, int16_t, PARAM_LEN);
    }

    /* 状态类 */
    for (i = 0; i < MACH_UNITS_NUM; i++) {
        if (NULL == pvar) { // 1
            return CF_M1_PARAM_ALL_NUM;
        }
        if (pvar < NULL) {
            return CF_M1_IDX_NUL;
        }
        /* 注意！需要根据不同的变量类型设置指针偏移 int8_t, int16_t, int32_t */
        pvar = PTR_DECREMENT(pvar, int16_t, STATUS_LEN);
    }

    return CF_M1_IDX_NUL;
}

#ifdef __cplusplus
}
#endif

#endif /* __GW_CF_CONFIG_H__ */
