#include "mem.h"
#include "cf.h"

uint32_t mem_buf[256]; // 数组缓存

/**
 * @brief 根据ID在内存表中查找对应的内存项
 *
 * @param tab 内存表指针，指向存储内存项的数组
 * @param id 要查找的内存项ID
 *
 * @return 如果找到对应ID的内存项，返回指向该内存项的指针；否则返回NULL
 */
struct mem_id_ *mem_get_by_id(struct mem_id_ *tab, uint16_t id)
{
    static uint16_t i = 0;
    uint16_t        i_old;

    i_old = i;

    do {
        /* 先判断当前项是否就是需要的 */
        if (tab[i].id == id) {
            return &tab[i];
        }
        /* 再后移一项 */
        i++;
        if (tab[i].mem.pvar == NULL) {
            i = 0;
        }
    } while (i_old != i);

    return NULL;
}

/**
 * @def MEM_OPS_INT(aVal, lpVal, cpVal, und, c2f2_m)
 * @brief 整形数读写
 *
 * @param aVal    缩放系数，正数表示普通倍数，负数表示特殊精度
 * @param lpVal   指向本地数据的指针
 * @param cpVal   指向外部数据的指针
 * @param und     方向标志 DOWN cpVal→lpVal，否则 lpVal→cpVal
 * @param c2f2_m  温标切换的倍数
 */
#define MEM_OPS_INT(aVal, lpVal, cpVal, und, c2f2_m)                              \
    do {                                                                          \
        if (und == DOWN) {                                                        \
            if (c2f2_m != 0) {                                                    \
                if ((aVal == -2) || (aVal == -10)) { /* 0.5/0.1 精度温度补丁*/    \
                    *cpVal *= 10;                                                 \
                }                                                                 \
                *cpVal = cf_convert(c2f2_m, *cpVal, lpVal, und);                  \
            }                                                                     \
            if (aVal > 0) {                                                       \
                (*lpVal) = (*cpVal) / aVal;                                       \
            } else {                                                              \
                if ((c2f2_m != 0) && (aVal == -2)) { /*0.5 精度温度补丁*/         \
                    (*lpVal) = (*cpVal) / 5;                                      \
                } else if ((c2f2_m != 0) && (aVal == -10)) {                      \
                    (*lpVal) = (*cpVal);                                          \
                } else {                                                          \
                    (*lpVal) = (*cpVal) * (-aVal);                                \
                }                                                                 \
            }                                                                     \
        } else {                                                                  \
            *cpVal = *lpVal;                                                      \
            if (c2f2_m != 0) {                                                    \
                if ((aVal == -2)) { /*0.5 精度温度补丁*/                          \
                    *cpVal = *lpVal * 5;                                          \
                }                                                                 \
                *cpVal = cf_convert(c2f2_m, *cpVal, lpVal, und);                  \
            }                                                                     \
            if (aVal > 0) {                                                       \
                (*cpVal) = (*cpVal) * aVal;                                       \
            } else {                                                              \
                if ((c2f2_m != 0) && (aVal == -2)) { /*0.5 精度温度补丁*/         \
                    (*cpVal) = (*cpVal) / 10;                                     \
                } else if ((c2f2_m != 0) && (aVal == -10)) { /*0.1 精度温度补丁*/ \
                    if ((*cpVal) < 0) {                                           \
                        (*cpVal) = ((*cpVal) - 5) / (-aVal);                      \
                    } else {                                                      \
                        (*cpVal) = ((*cpVal) + 5) / (-aVal);                      \
                    }                                                             \
                } else {                                                          \
                    (*cpVal) = (*cpVal) / (-aVal);                                \
                }                                                                 \
            }                                                                     \
        }                                                                         \
    } while (0)

/**
 * @def MEM_OPS_FLOAT(aVal, lpVal, cpVal, und, c2f2_m)
 * @brief 浮点数值缩放读写
 *
 * @param aVal    缩放系数
 * @param lpVal   指向本地数据的指针
 * @param cpVal   指向外部数据的指针
 * @param und     方向标志 DOWN cpVal→lpVal，否则 lpVal→cpVal
 * @param c2f2_m  温标切换的倍数
 */
#define MEM_OPS_FLOAT(aVal, lpVal, cpVal, und, c2f2_m)           \
    do {                                                         \
        if (und == DOWN) {                                       \
            if (c2f2_m != 0) {                                   \
                *cpVal = cf_convert(c2f2_m, *cpVal, lpVal, und); \
            }                                                    \
            (*lpVal) = (*cpVal) / aVal;                          \
        } else {                                                 \
            (*cpVal) = (*lpVal) * aVal;                          \
            if (c2f2_m != 0) {                                   \
                *cpVal = cf_convert(c2f2_m, *cpVal, lpVal, und); \
            }                                                    \
        }                                                        \
    } while (0)

/**
 * @def RAM_OPS_BOOL(aVal, lpVal, cpVal, und)
 * @brief 布尔位读写
 *
 * @param aVal    位序号
 * @param lpVal   指向本地数据的指针
 * @param cpVal   指向外部数据的指针
 * @param und     方向标志 DOWN cpVal→lpVal，否则 lpVal→cpVal
 */
#define MEM_OPS_BOOL(aVal, lpVal, cpVal, und)      \
    do {                                           \
        if (und == DOWN) {                         \
            G_WRITE_BIT((*lpVal), aVal, (*cpVal)); \
        } else {                                   \
            (*cpVal) = G_GET_BIT((*lpVal), aVal);  \
        }                                          \
    } while (0)

/**
 * @def RAM_OPS_ENUM(aVal, lpVal, cpVal, und)
 * @brief 枚举值读写
 *
 * @param aVal    未使用（占位参数，保持接口一致）
 * @param lpVal   指向本地数据的指针
 * @param cpVal   指向外部数据的指针
 * @param und     方向标志 DOWN cpVal→lpVal，否则 lpVal→cpVal
 */
#define MEM_OPS_ENUM(aVal, lpVal, cpVal, und) \
    do {                                      \
        if (und == DOWN) {                    \
            (*lpVal) = (*cpVal);              \
        } else {                              \
            (*cpVal) = (*lpVal);              \
        }                                     \
    } while (0)

/**
 * @def MEM_OPS_ARRAY(aVal, lpVal, cpVal, und)
 * @brief 数组读写
 *
 * @param aVal    未使用（占位参数，保持接口一致）
 * @param lpVal   指向本地数据的指针
 * @param cpVal   指向外部数据的指针
 * @param und     方向标志 DOWN cpVal→lpVal，否则 lpVal→cpVal
 */
#define MEM_OPS_ARRAY(i, len, lpVal, cpVal, und) \
    do {                                         \
        if (und == DOWN) {                       \
            for (i = 0; i < len; i++) {          \
                (lpVal)[i] = (*cpVal)[i];        \
            }                                    \
        } else {                                 \
            for (i = 0; i < len; i++) {          \
                (*cpVal)[i] = (lpVal)[i];        \
            }                                    \
        }                                        \
    } while (0)

/**
 * @def MEM_OPS_STRING(aVal, lpVal, cpVal, und)
 * @brief 字符串读写
 *
 * @param aVal    未使用（占位参数，保持接口一致）
 * @param lpVal   指向本地数据的指针
 * @param cpVal   指向外部数据的指针
 * @param und     方向标志 DOWN cpVal→lpVal，否则 lpVal→cpVal
 */
#define MEM_OPS_STRING(i, len, pVal, lpVal, cpVal, und) \
    do {                                                \
        if (und == DOWN) {                              \
            for (i = 0; i < len; i++) {                 \
                if ((*cpVal)[i] == '\0') { break; }     \
                (lpVal)[i] = (*cpVal)[i];               \
            }                                           \
            (lpVal)[i] = '\0';                          \
        } else {                                        \
            for (i = 0; i < len; i++) {                 \
                if ((lpVal)[i] == '\0') { break; }      \
                (*cpVal)[i] = (lpVal)[i];               \
            }                                           \
            (*cpVal)[i] = '\0';                         \
        }                                               \
    } while (0)

/**
 * @brief 根据数据类型和属性对内存中的值进行操作，支持多种数据类型和操作类型（如整数、浮点、布尔等）。
 *
 * @param mem   指向 mem_ 结构体的指针，包含内存操作的相关信息，如数据类型、单位、属性等。
 * @param pVal  指向待处理数据的指针，数据类型由 mem->dtype 决定。
 * @param und   方向标志 DOWN cpVal→lpVal，否则 lpVal→cpVal
 */
void mem_ops_value(const mem_t *mem, void *pVal, uint8_t und)
{
    static const attr_config_t int_attr_table[] = {
        {1, 1},    // A_I1
        {2, 0},    // A_I2
        {5, 0},    // A_I5
        {10, 0},   // A_I10
        {25, 0},   // A_I25
        {100, 0},  // A_I100
        {-2, 5},   // A_I05
        {-10, 10}, // A_I01
        {-100, 0}  // A_I001
    };
    static const attr_config_t float_attr_table[] = {
        {5, 5},   // A_F05
        {1, 10},  // A_F01
        {1, 100}, // A_F001
        {1, 1000} // A_F0001
    };
    uint8_t  unit  = mem->unit;
    uint8_t  dtype = mem->dtype;
    uint8_t  attr  = mem->attr;
    uint8_t  atype = mem->atype;
    int16_t  aVal;
    uint8_t  i;
    int32_t  lVal;   // 临时变量
    uint16_t c2f2_m; // 摄氏度 华氏度 转换模

    p_type_un_t cpVal;
    p_type_un_t lpVal;

    cpVal.pvoid = pVal;
    lpVal.pvoid = mem->pvar;

    switch (atype) {
    case ATYP_INT:
        if (attr <= A_I001) {
            aVal   = int_attr_table[attr].aVal;
            c2f2_m = int_attr_table[attr].c2f2_m;
        } else {
            aVal   = 1;
            c2f2_m = 0;
        }
        if (unit != U_C || !IS_TEMP_F()) { // 温度单位转换补丁
            c2f2_m = 0;
        }
        switch (dtype) {
        case TYP_U8: MEM_OPS_INT(aVal, lpVal.pu8, cpVal.pu32, und, c2f2_m); break;
        case TYP_S8: MEM_OPS_INT(aVal, lpVal.ps8, cpVal.ps32, und, c2f2_m); break;
        case TYP_U16: MEM_OPS_INT(aVal, lpVal.pu16, cpVal.pu32, und, c2f2_m); break;
        case TYP_S16: MEM_OPS_INT(aVal, lpVal.ps16, cpVal.ps32, und, c2f2_m); break;
        case TYP_U32: MEM_OPS_INT(aVal, lpVal.pu32, cpVal.pu32, und, c2f2_m); break;
        case TYP_S32: MEM_OPS_INT(aVal, lpVal.ps32, cpVal.ps32, und, c2f2_m); break;
        case TYP_F32:
            /* float to int */
            lVal        = *lpVal.pf32;
            lpVal.pvoid = &lVal;
            MEM_OPS_INT(aVal, lpVal.ps32, cpVal.ps32, und, c2f2_m);
            /* int to float */
            lpVal.pvoid = mem->pvar;
            *lpVal.pf32 = lVal;
            break;
        default: break;
        }
        break;
    case ATYP_FLOAT:
        if (attr <= A_F0001) {
            aVal   = float_attr_table[attr].aVal;
            c2f2_m = float_attr_table[attr].c2f2_m;
        } else {
            aVal   = 1;
            c2f2_m = 10;
        }
        if (unit != U_C || !IS_TEMP_F()) { // 温度单位转换补丁
            c2f2_m = 0;
        }
        switch (dtype) {
        case TYP_U8: MEM_OPS_FLOAT(aVal, lpVal.pu8, cpVal.pu32, und, c2f2_m); break;
        case TYP_S8: MEM_OPS_FLOAT(aVal, lpVal.ps8, cpVal.ps32, und, c2f2_m); break;
        case TYP_U16: MEM_OPS_FLOAT(aVal, lpVal.pu16, cpVal.pu32, und, c2f2_m); break;
        case TYP_S16: MEM_OPS_FLOAT(aVal, lpVal.ps16, cpVal.ps32, und, c2f2_m); break;
        case TYP_U32: MEM_OPS_FLOAT(aVal, lpVal.pu32, cpVal.pu32, und, c2f2_m); break;
        case TYP_S32: MEM_OPS_FLOAT(aVal, lpVal.ps32, cpVal.ps32, und, c2f2_m); break;
        case TYP_F32:
            /* float to int */
            lVal        = *lpVal.pf32 * float_attr_table[attr].aVal;
            lpVal.pvoid = &lVal;
            MEM_OPS_FLOAT(aVal, lpVal.ps32, cpVal.ps32, und, c2f2_m);
            /* int to float */
            lpVal.pvoid = mem->pvar;
            *lpVal.pf32 = lVal;
            *lpVal.pf32 /= float_attr_table[attr].aVal;
            break;
        default: break;
        }
        break;
    case ATYP_BOOL:
        aVal = attr;
        switch (dtype) {
        case TYP_U8:
        case TYP_S8: MEM_OPS_BOOL(aVal, lpVal.pu8, cpVal.pu32, und); break;
        case TYP_U16:
        case TYP_S16: MEM_OPS_BOOL(aVal, lpVal.pu16, cpVal.pu32, und); break;
        case TYP_U32:
        case TYP_S32: MEM_OPS_BOOL(aVal, lpVal.pu32, cpVal.pu32, und); break;
        default: break;
        }
        break;
    case ATYP_ENUM:
        aVal = 1;
        switch (dtype) {
        case TYP_U8:
        case TYP_S8: MEM_OPS_ENUM(aVal, lpVal.pu8, cpVal.pu32, und); break;
        case TYP_U16:
        case TYP_S16: MEM_OPS_ENUM(aVal, lpVal.pu16, cpVal.pu32, und); break;
        case TYP_U32:
        case TYP_S32: MEM_OPS_ENUM(aVal, lpVal.pu32, cpVal.pu32, und); break;
        default: break;
        }
        break;
    case ATYP_ARRAY:
        aVal = attr;
        switch (dtype) {
        default:
        case TYP_U8:
        case TYP_S8: break;
        case TYP_U16:
        case TYP_S16: aVal *= 2; break;
        case TYP_U32:
        case TYP_S32: aVal *= 4; break;
        }
        (*cpVal.parr) = (uint8_t *)mem_buf;
        MEM_OPS_ARRAY(i, aVal, lpVal.arr, cpVal.parr, und);
        break;
    case ATYP_STR:
        aVal          = sizeof(mem_buf);
        (*cpVal.pstr) = (char *)mem_buf;
        MEM_OPS_STRING(i, aVal, lpVal.pvoid, lpVal.str, cpVal.pstr, und);
        break;
    default:
        break;
    }
}
