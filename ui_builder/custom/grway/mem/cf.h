#ifndef __GW_CF_H__
#define __GW_CF_H__

#ifdef CF_EXT
#define CF_EXT
#else
#define CF_EXT extern
#endif

#include "../config/cf_config.h"

/*****************************************************************************
 * 常量定义
 *****************************************************************************/
typedef enum {
    CF_1   = 1,
    CF_05  = 5,
    CF_01  = 10,
    CF_001 = 100,
} cf_m_e;

/*****************************************************************************
 * 宏函数声明
 *****************************************************************************/
#define cf_disp_temp(m, var)      cf_disp_convert(m, var, &var, UP)   // m 精度
#define cf_set_temp(m, var, pvar) cf_disp_convert(m, var, pvar, DOWN) // m 精度
#define cf_view_stemp(m, var, pvar, und)             \
    if (und == UP) {                                 \
        var = cf_disp_convert(m, pvar, &pvar, UP);   \
    } else {                                         \
        pvar = cf_disp_convert(m, var, &pvar, DOWN); \
    }

/*****************************************************************************
 * 函数声明
 *****************************************************************************/
int32_t cf_to_f(uint8_t m, int32_t c, void *pvar);
int32_t cf_to_c(uint8_t m, int32_t f, void *pvar);
int32_t cf_convert(uint8_t m, int32_t var, void *pvar, bool_t und);
int32_t cf_disp_convert(uint8_t m, int32_t var, void *pvar, bool_t und);

#endif /* __GW_CF_H__ */
