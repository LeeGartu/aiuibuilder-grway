/**
 * @file mem_item.h
 * @brief 参数模块
 *
 * @defgroup param_const 常量
 * @defgroup param_api 功能接口
 */
#ifndef __GW_MEM_ITEM_H__
#define __GW_MEM_ITEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../config/mem_param.h"
#include "../config/mem_status.h"
#include "cf.h"

/**
 * @addtogroup param_const 常量
 * @{
 */
typedef enum {
    P_MIN,
    P_INIT,
    P_MAX,
} param_min_of_max_e;
/** @} */

/**
 * @addtogroup param_api 功能接口
 * @{
 */
#define param_get_min(param)  param_get_range_var(param, P_MIN)
#define param_get_init(param) param_get_range_var(param, P_INIT)
#define param_get_max(param)  param_get_range_var(param, P_MAX)

int32_t        param_get_range_var(const mem_range_t *param, uint8_t var_type);
void           param_set_value_offset(const mem_range_t *param, int32_t var, uint32_t offset);
gw_inline void param_set_value(const mem_range_t *param, int32_t var)
{
    param_set_value_offset(param, var, 0);
}
int32_t           param_get_value_offset(const mem_range_t *param, uint32_t offset);
gw_inline int32_t param_get_value(const mem_range_t *param)
{
    return param_get_value_offset(param, 0);
}
void param_data_init(void);

int32_t           status_get_value_offset(const mem_t *status, uint32_t offset);
gw_inline int32_t status_get_value(const mem_t *status)
{
    return status_get_value_offset(status, 0);
}
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __GW_MEM_ITEM_H__ */
