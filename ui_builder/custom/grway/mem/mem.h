#ifndef __GW_MEM_H__
#define __GW_MEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/gw_utils.h"

/*****************************************************************************
 * 常量定义
 *****************************************************************************/
typedef enum {
    U_NUL  = 0,  // 无单位
    U_C    = 1,  // 温度
    U_A    = 2,  // 电流-安
    U_V    = 3,  // 电流-伏
    U_STP  = 4,  // 步数-步
    U_HZ   = 5,  // 频率-赫兹
    U_BAR  = 6,  // 压力-巴
    U_RPM  = 7,  // 转速-转每分
    U_SEC  = 8,  // 时间-秒
    U_MIN  = 9,  // 时间-分
    U_HOUR = 10, // 时间-时
    U_DAY  = 11, // 时间-日
    U_PCT  = 12, // 百分之
    U_KW   = 13, // 能量-千瓦
    U_W    = 14, // 能量-瓦
    U_M3H  = 15, // 流速-立方每小时
    U_KWH  = 16, // 能量-千瓦时

    U_MAX = 255, // 最大单位数量
} mem_unit_e;    // 单位类型

typedef enum {
    ATYP_INT,
    ATYP_FLOAT,
    ATYP_BOOL,
    ATYP_ENUM,
    ATYP_ARRAY,
    ATYP_STR,

    ATYP_MAX = 255, // 最大属性数量
} mem_atype_e;      // 属性类型

// clang-format off
typedef enum {
    /* 整型 */
    A_I1 = 0,   // 1 倍整数
    A_I2,       // 2 倍整数
    A_I5,       // 5 倍整数
    A_I10,      // 10 倍整数
    A_I25,      // 25 倍整数
    A_I100,     // 100 倍整数
    A_I05,      // 0.5 倍整数 // 与浮点类型的差别，这部分只保留整数部分
    A_I01,      // 0.1 倍整数
    A_I001,     // 0.01 倍整数
    /* 浮点型 */
    A_F05 = 0,  // 0.5 精度 // 与整形类型的差别，保留整数和小数点部分
    A_F01,      // 0.1 精度 // 小数数量
    A_F001,     // 0.01 精度
    A_F0001,    // 0.001 精度
    /* 布尔型 */
    A_B0=0,A_B1,  A_B2,  A_B3,  A_B4, // 0~4 位
    A_B5,  A_B6,  A_B7,  A_B8,  A_B9, // 5~9 位
    A_B10, A_B11, A_B12, A_B13, A_B14,// 10~14 位
    A_B15, A_B16, A_B17, A_B18, A_B19,// 15~19 位
    A_B20, A_B21, A_B22, A_B23, A_B24,// 20~24 位
    A_B25, A_B26, A_B27, A_B28, A_B29,// 25~29 位
    A_B30, A_B31,                     // 30~31 位
    /* 枚举型 */
    A_ENUM = 0,	// 枚举
    /* 数组型 */
    A_A0=0,A_A1,  A_A2,  A_A3,  A_A4, // 0~4 个数
    A_A5,  A_A6,  A_A7,  A_A8,  A_A9, // 5~9 个数
    A_A10, A_A11, A_A12, A_A13, A_A14,// 10~14 个数
    A_A15, A_A16, A_A17, A_A18, A_A19,// 15~19 个数
    A_A20, A_A21, A_A22, A_A23, A_A24,// 20~24 个数
    A_A25, A_A26, A_A27, A_A28, A_A29,// 25~29 个数
    A_A30, A_A31, A_A32, A_A33, A_A34,// 30~34 个数
    A_A35, A_A36, A_A37, A_A38, A_A39,// 35~39 个数
    A_A40, A_A41, A_A42, A_A43, A_A44,// 40~44 个数
    A_A45, A_A46, A_A47, A_A48, A_A49,// 45~49 个数
    /* 字符串型 */
    A_STR = 0,	// 字符串
    /* 请求型 */
    A_REQ = 0,	// 请求值
} mem_attr_e; // 属性值
// clang-format on

typedef enum {
    TYP_U8,
    TYP_S8,
    TYP_U16,
    TYP_S16,
    TYP_U32,
    TYP_S32,
    TYP_F32,
} mem_dtype_e; // 数值类型

typedef struct mem_ {
    uint8_t unit;  // 单位
    uint8_t atype; // 属性类型
    uint8_t attr;  // 属性值
    uint8_t dtype; // 大小端|变量类型
    void   *pvar;  // 首地址
} mem_t;

typedef struct mem_range_ {
    int32_t  min;  // 最小值
    int32_t  init; // 默认值
    int32_t  max;  // 最大值
    mem_t mem;
} mem_range_t;

typedef struct mem_id_ {
    int32_t  id;
    mem_t mem;
} mem_id_t;

typedef struct attr_config_ {
    int16_t  aVal;
    uint16_t c2f2_m;
} attr_config_t;

void mem_ops_value(const mem_t *mem, void *pVal, uint8_t und);

#ifdef __cplusplus
}
#endif

#endif /* __GW_MEM_H__ */
