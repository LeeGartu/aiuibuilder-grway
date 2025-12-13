#include "cf.h"

static int32_t cf_m1_buf[CF_M1_PARAM_ALL_NUM];

/**
 * @brief 修正华氏度温度值
 *
 * @param m 修正系数
 * @param f 原始华氏度值
 *
 * @return 修正后的华氏度值
 */
gw_inline int16_t cf_correct_f(uint8_t m, int16_t f)
{
    static int8_t f_ofs[] = {0, -1, -2, -3, -4, 4, 3, 2, 1}; // 华氏度 修正补偿
    int8_t        i;
    i = ((f - 32 * m) / m) % 9;
    if (i < 0) { i += 9; }
    f *= 10;
    f += (int16_t)f_ofs[i] * m;

    return f;
}

/**
 * @brief 将摄氏度温度值转换为华氏度温度值，支持不同的精度模式和缓存机制。
 *
 * @param m 精度模式，1 表示低精度（分离存储法），其他值表示高精度（公式计算法）
 * @param c 摄氏度温度值（可能已放大倍数）
 * @param pvar 指向变量上下文的指针，用于判断是否为回差温度、获取索引等
 *
 * @return 返回转换后的华氏度值（可能已放大倍数）
 */
int32_t cf_to_f(uint8_t m, int32_t c, void *pvar)
{
    uint32_t i;
    int32_t  ret;
    int32_t  cal_ret;

    /* 1精度-分离存储法 */
    if (m == 1) {
        /* 回差温度 */
        if (cf_is_diff(pvar)) {
            ret = ((c) * 2);

            /* 普通温度 */
        } else {
            i = cf_get_temp_idx(pvar);

            /* 非法值 */
            if (i == CF_M1_IDX_NUL) {
                ret = CF_ERROR_CODE;

                /* 参数值 */
            } else if (i < CF_M1_PARAM_ALL_NUM) {
                /* 计算当前摄氏度对应的华氏度 */
                if (c < (-18)) {                              // 摄氏度小于 -18 度， 华氏度对应负数
                    cal_ret = (((c * 18) - (5)) / 10) + (32); // 此处 除数 和 被除数 都放大了两倍
                } else {
                    cal_ret = (((c * 18) + (5)) / 10) + (32); // 此处 除数 和 被除数 都放大了两倍
                }

                /* 读取缓存区的华氏度的值 */
                ret = cf_m1_buf[i];

                /* 若未初始化，或差值过大，需要刷新缓存值 */
                if ((ret == CF_M1_INIT)
                    || ((cal_ret - ret) > 2)
                    || ((ret - cal_ret) > 2)) {
                    ret          = cal_ret;
                    cf_m1_buf[i] = ret;
                }

                /* 状态值 */
            } else {
                if (c < (-18)) {                          // 摄氏度小于 -18 度， 华氏度对应负数
                    ret = (((c * 18) - (5)) / 10) + (32); // 此处 除数 和 被除数 都放大了两倍
                } else {
                    ret = (((c * 18) + (5)) / 10) + (32); // 此处 除数 和 被除数 都放大了两倍
                }
            }
        }

        /* 其他高精度-公式计算法 */
    } else {
        /* 回差温度 */
        if (cf_is_diff(pvar)) {
            ret = ((c) * 2);

            /* 普通温度 */
        } else {
            if (m <= 5) { // 倍数小于等于 5 时，需要 四舍五入
                if (m == 5) { m = 10; }
                if (c < (-18 * m)) {                              // 摄氏度小于 -18 度， 华氏度对应负数
                    ret = (((c * 18) - (5 * m)) / 10) + (32 * m); // 此处 除数 和 被除数 都放大了两倍
                } else {
                    ret = (((c * 18) + (5 * m)) / 10) + (32 * m); // 此处 除数 和 被除数 都放大了两倍
                }
                if (m == 10) { // 消除小数位(注：此处 10 倍 非原生的 10 倍)
                    ret /= m;
                    ret *= m;
                }
            } else {
                ret = ((c * 9) / 5) + (32 * m); // 此处 除数 和 被除数 是 一倍
            }
        }
    }
    return ret;
}

/**
 * @brief 将华氏温度值转换为摄氏温度值
 *
 * @param m 精度模式，1表示低精度分离存储法，其他值表示高精度公式计算法
 * @param f 华氏温度值
 * @param pvar 指向变量的指针，用于判断是否为回差温度及获取温度索引
 *
 * @return 转换后的摄氏温度值
 */
int32_t cf_to_c(uint8_t m, int32_t f, void *pvar)
{
    uint32_t i;
    int32_t  ret;

    /* 1精度-分离存储法 */
    if (m == 1) {
        /* 回差温度 */
        if (cf_is_diff(pvar)) {
            ret = ((f) / 2);

            /* 普通温度 */
        } else {
            i = cf_get_temp_idx(pvar);

            if (i == CF_M1_IDX_NUL) {
                ret = CF_ERROR_CODE;
            } else {
                /* 保存华氏度到缓存区 */
                cf_m1_buf[i] = f;

                if (f < 32) {                         // 华氏度小于 32 度， 摄氏度对应负数
                    ret = (((f - 32) * 10) - 9) / 18; // 此处 被除数 放大了 两倍 除数放大了 两倍
                } else {
                    ret = (((f - 32) * 10) + 9) / 18; // 此处 被除数 放大了 两倍 除数放大了 两倍
                }
            }
        }

        /* 其他高精度-公式计算法 */
    } else {
        /* 回差温度 */
        if (cf_is_diff(pvar)) {
            ret = ((f) / 2);

            /* 普通温度 */
        } else {
            if (m <= 5) { // 倍数小于等于 5 时，需要修正
                if (m == 5) { m = 10; }
                f = cf_correct_f(m, f);
            } else {
                f *= 10;
            }
            ret = (f - (32 * m * 10)) / 18;
        }
    }
    return ret; // 结果 是 1 倍
}

/**
 * @brief 温度单位转换函数
 *
 * @param m 精度模式，1表示低精度分离存储法，其他值表示高精度公式计算法
 * @param var 待转换的温度值
 * @param pvar 指向温度值存储区的指针
 * @param und 转换方向标识，DOWN表示向下转换，其他值表示向上转换
 *
 * @return 转换后的温度值
 */
int32_t cf_convert(uint8_t m, int32_t var, void *pvar, bool_t und)
{
    if (IS_TEMP_F()) {
        if (und == DOWN) {
            return cf_to_c(m, var, pvar);
        } else {
            return cf_to_f(m, var, pvar);
        }
    }
    return var;
}

/**
 * @brief 温度单位转换函数，根据当前温度显示模式在摄氏度和华氏度之间进行转换
 *
 * @param m 精度模式，1表示低精度分离存储法，其他值表示高精度公式计算法
 * @param var 待转换的温度值
 * @param pvar 指向额外参数的指针，可能包含转换相关的配置信息
 * @param und 转换方向标识，DOWN表示向下转换，其他值表示向上转换
 *
 * @return 转换后的温度值
 */
int32_t cf_disp_convert(uint8_t m, int32_t var, void *pvar, bool_t und)
{
    if (IS_TEMP_F()) {
        if (und == DOWN) {
            var = cf_to_c(m, var, pvar);
            if (m == CF_05) { var /= 5; }
        } else {
            if (m == CF_05) { var *= 5; }
            var = cf_to_f(m, var, pvar);
        }
    } else {
        if (und == DOWN) {
            if (m == CF_05) { var /= 5; }
        } else {
            if (m == CF_05) { var *= 5; }
        }
    }

    return var;
}
