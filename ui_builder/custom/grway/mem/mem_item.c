#include "mem_item.h"

int32_t status_get_value_offset(const mem_t *status, uint32_t offset)
{
    mem_t status_u = *status;
    int32_t  ret;

    if (offset != 0) {
        status_freak_offset(&status_u, offset);
    }
    /* 获取参数值 */
    mem_ops_value(&status_u, &ret, UP);

    /* 特殊显示处理 */
    status_freak_var(status->pvar, &ret);

    return ret;
}

/**
 * @brief 获取参数范围值
 *
 * @param param 参数内存结构体
 * @param var_type 范围值类型
 * @return int32_t 返回范围值
 */
int32_t param_get_range_var(const mem_range_t *param, uint8_t var_type)
{
    static const attr_config_t float_attr_table[] = {
        {10, 5},     // A_F05
        {10, 10},    // A_F01
        {100, 100},  // A_F001
        {1000, 1000} // A_F0001
    };
    int32_t  var;
    uint16_t m;
    bool_t   is_float=0;

    if (var_type == P_MIN) {
        var = param->min;
    } else if (var_type == P_MAX) {
        var = param->max;
    } else {
        var = param->init;
    }

    /* 特殊上下限处理 */
    param_freak_min_max(param->mem.pvar, var_type, &var, &is_float);

    /* 数值属性处理 */
    switch (param->mem.atype) {
    case ATYP_INT:
        /* 温度单位转换 */
        if ((param->mem.unit == U_C) && (IS_TEMP_F())) {
            var *= 10;
            m   = 10;
            var = cf_to_f(m, var, param->mem.pvar) / 10;
        }
        break;
    case ATYP_FLOAT:
        if (!is_float) { var *= float_attr_table[param->mem.attr].aVal; }
        m = float_attr_table[param->mem.attr].c2f2_m;
        /* 温度单位转换 */
        if ((param->mem.unit == U_C) && (IS_TEMP_F())) {
            var = cf_to_f(m, var, param->mem.pvar);
        }
        break;
    default:
        break;
    }

    return var;
}

/**
 * @brief 设置参数值
 *
 * @param param 参数内存结构体
 * @param var 参数值（含物理属性）
 */
void param_set_value_offset(const mem_range_t *param, int32_t var, uint32_t offset)
{
    int32_t vmax = param_get_max(param);
    int32_t vmin = param_get_min(param);
    mem_range_t param_u = *param;

    if (offset != 0) {
        param_freak_offset(&param_u, offset);
    }

    /* 比较最大最小值 */
    var = G_MAX(var, vmin);
    var = G_MIN(var, vmax);

    /* 特殊处理 */
    param_freak_var(param->mem.pvar, &var, DOWN);

    /* 写入参数值 */
    mem_ops_value(&param_u.mem, &var, DOWN);
}

/**
 * @brief 获取参数值
 *
 * @param param 参数内存结构体
 * @return int32_t 参数值（含物理属性）
 */
int32_t param_get_value_offset(const mem_range_t *param, uint32_t offset)
{
    int32_t ret = 0;
    mem_range_t param_u = *param;

    if (offset != 0) {
        param_freak_offset(&param_u, offset);
    }

    /* 获取参数值 */
    mem_ops_value(&param_u.mem, &ret, UP);

    /* 特殊处理 */
    param_freak_var(param->mem.pvar, &ret, UP);

    return ret;
}

/**
 * @brief 参数初始化
 */
void param_data_init(void)
{
    const mem_range_t *param_tab;
    uint8_t               i;
#ifdef TEMP_CF
    bool_t temp_cf_bak;
    temp_cf_bak = TEMP_CF;
    TEMP_CF     = TEMP_C;
#endif

    /* 遍历参数表链表 */
    for (i = 0; param_tab_list[i] != NULL; i++) {
        param_tab = param_tab_list[i];

        /* 逐个参数表初始化 */
        while (param_tab->mem.pvar != NULL) {
            param_set_value(param_tab, param_get_init(param_tab));
            param_tab++;
        }
    }

#ifdef TEMP_CF
    TEMP_CF = temp_cf_bak;
#endif
}
