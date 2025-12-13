#ifndef __GW_LIB_H__
#define __GW_LIB_H__

#include "gw_def.h"

/*****************************************************************************
 * 宏函数
 *****************************************************************************/
#define G_STR(x)               (#x)
#define G_MIN(a, b)            (((a) > (b)) ? (b) : (a))
#define G_MAX(a, b)            (((a) > (b)) ? (a) : (b))
#define G_NOC(x, b)            (((x) / (b)) + (((x) % (b)) ? 1 : 0)) // 组数（向上取整）
#define G_SIZE_OF_ARR(x)       (sizeof(x) / sizeof(x[0]))
#define G_SET_BIT(x, n)        ((x) |= ((1UL) << (n)))                             // 设位
#define G_CLR_BIT(x, n)        ((x) &= ~((1UL) << (n)))                            // 清位
#define G_GET_BIT(x, n)        (((x) >> (n)) & (1UL))                              // 查位
#define G_TOGGLE_BIT(x, n)     ((x) ^= ((1UL) << (n)))                             // 反转
#define G_WRITE_BIT(x, n, v)   ((v) ? G_SET_BIT((x), (n)) : G_CLR_BIT((x), (n)))   // 写位
#define G_HIGH_BYTE(x)         (uint8_t)((x) >> 8)                                 // 获取变量高8位
#define G_LOW_BYTE(x)          (uint8_t)((x) & 0xff)                               // 获取变量低8位
#define G_HIGH_WORD(x)         (uint16_t)((x) >> 16)                               // 获取变量高16位
#define G_LOW_WORD(x)          (uint16_t)((x) & 0xffff)                            // 获取变量低16位
#define G_COMBINE_BYTE(hi, lo) (((uint16_t)((uint8_t)hi) << 8) | ((uint8_t)lo))    // 组合成16位
#define G_COMBINE_WORD(hi, lo) (((uint32_t)((uint16_t)hi) << 16) | ((uint16_t)lo)) // 组合成32位
#define G_SET_BUF_BIT(buf, no)			    g_set_buf_bit(8,buf,no) // 8 位数组操作
#define G_CLR_BUF_BIT(buf, no)				g_clr_buf_bit(8,buf,no)
#define G_GET_BUF_BIT(buf, no)				g_get_buf_bit(8,buf,no)
#define G_TOGGLE_BUF_BIT(buf, no)			g_toggle_buf_bit(8,buf,no)
#define G_WRITE_BUF_BIT(buf, no, var)		g_write_buf_bit(8,buf,no,var)
#define G_GET_BUF_BIT_NO(p)					g_get_buf_bit_no(8,p)
#define G_SET_16BUF_BIT(buf, no)		    g_set_buf_bit(16,buf,no) // 16 位数组操作
#define G_CLR_16BUF_BIT(buf, no)			g_clr_buf_bit(16,buf,no)
#define G_GET_16BUF_BIT(buf, no)			g_get_buf_bit(16,buf,no)
#define G_TOGGLE_16BUF_BIT(buf, no)			g_toggle_buf_bit(16,buf,no)
#define G_WRITE_16BUF_BIT(buf, no, var)		g_write_buf_bit(16,buf,no,var)
#define G_GET_16BUF_BIT_NO(p)				g_get_buf_bit_no(16,p)
#define G_SET_32BUF_BIT(buf, no)		    g_set_buf_bit(32,buf,no) // 32 位数组操作
#define G_CLR_32BUF_BIT(buf, no)			g_clr_buf_bit(32,buf,no)
#define G_GET_32BUF_BIT(buf, no)			g_get_buf_bit(32,buf,no)
#define G_TOGGLE_32BUF_BIT(buf, no)			g_toggle_buf_bit(32,buf,no)
#define G_WRITE_32BUF_BIT(buf, no, var)		g_write_buf_bit(32,buf,no,var)
#define G_GET_32BUF_BIT_NO(p)				g_get_buf_bit_no(32,p)

/**
 * @brief 将指针向前移动指定的元素个数
 *
 * @param ptr 要移动的指针地址
 * @param type 指针指向的数据类型
 * @param count 要向前移动的元素个数
 *
 * @return 向前移动后的void*类型指针
 */
#define PTR_DECREMENT(ptr, type, count) \
    ((void *)((char *)(ptr) - (count) * sizeof(type)))

/**
 * @brief 将指针向前移动指定的元素个数
 *
 * @param ptr 要移动的指针地址
 * @param type 指针指向的数据类型
 * @param count 要向前移动的元素个数
 *
 * @return 向前移动后的void*类型指针
 */
#define PTR_INCREMENT(ptr, type, count) \
    ((void *)((char *)(ptr) + (count) * sizeof(type)))

gw_inline void g_reverse(void *buf, uint8_t len)
{
    uint8_t *u8_buf = buf;
    uint8_t  i      = 0;
    uint8_t  t;

    len--;
    for (; i < len; i++, len--) {
        t           = u8_buf[len];
        u8_buf[len] = u8_buf[i];
        u8_buf[i]   = t;
    }
}

gw_inline void g_set_buf_bit(uint8_t bn, void *buf, uint16_t no) {
	p_type_un_t pbuf;

	pbuf.pvoid = buf;
	
	switch (bn) {
		default:
		case 8 :
			G_SET_BIT(pbuf.pu8[no/bn], no%bn);
			break;
		case 16 :
			G_SET_BIT(pbuf.pu16[no/bn], no%bn);
			break;
		case 32 :
			G_SET_BIT(pbuf.pu32[no/bn], no%bn);
			break;
	}
}

gw_inline void g_clr_buf_bit(uint8_t bn, void *buf, uint16_t no) {
	p_type_un_t pbuf;

	pbuf.pvoid = buf;
	
	switch (bn) {
		default:
		case 8 :
			G_CLR_BIT(pbuf.pu8[no/bn], no%bn);
			break;
		case 16 :
			G_CLR_BIT(pbuf.pu16[no/bn], no%bn);
			break;
		case 32 :
			G_CLR_BIT(pbuf.pu32[no/bn], no%bn);
			break;
	}
}

gw_inline bool_t g_get_buf_bit(uint8_t bn, void *buf, uint16_t no) {
	p_type_un_t pbuf;

	pbuf.pvoid = buf;
	
	switch (bn) {
		default:
		case 8 :
			return G_GET_BIT(pbuf.pu8[no/bn], no%bn);
		case 16 :
			return G_GET_BIT(pbuf.pu16[no/bn], no%bn);
		case 32 :
			return G_GET_BIT(pbuf.pu32[no/bn], no%bn);
	}
}

gw_inline void g_toggle_buf_bit(uint8_t bn, void *buf, uint16_t no) {
	p_type_un_t pbuf;

	pbuf.pvoid = buf;
	
	switch (bn) {
		default:
		case 8 :
			G_TOGGLE_BIT(pbuf.pu8[no/bn], no%bn);
			break;
		case 16 :
			G_TOGGLE_BIT(pbuf.pu16[no/bn], no%bn);
			break;
		case 32 :
			G_TOGGLE_BIT(pbuf.pu32[no/bn], no%bn);
			break;
	}
}

gw_inline void g_write_buf_bit(uint8_t bn, void *buf, uint16_t no, bool_t var) {
	p_type_un_t pbuf;

	pbuf.pvoid = buf;
	
	switch (bn) {
		default:
		case 8 :
			G_WRITE_BIT(pbuf.pu8[no/bn], no%bn, var);
			break;
		case 16 :
			G_WRITE_BIT(pbuf.pu16[no/bn], no%bn, var);
			break;
		case 32 :
			G_WRITE_BIT(pbuf.pu32[no/bn], no%bn, var);
			break;
	}
}

gw_inline char *g_itoa(int32_t num, char str[], uint8_t base)
{
    uint8_t len     = 0;
    bool_t  isMinus = FALSE;
    int32_t digit;

    /* 处理负数 */
    if (num < 0) {
        isMinus = TRUE;
        if (base == 10) {
            num = -num;
        } else {
            G_CLR_BIT(num, 31);
        }
    }

    /* 将数字从低位到高位迭代计算转换后的字符并存储在结果字符串中 */
    do {
        digit      = num % base;
        str[len++] = (digit > 9) ? (digit - 10) + 'A' : digit + '0';
        num /= base;
    } while (num);

    /* 如果数字为负数 */
    if (base == 10) {
        /* 添加 '-' 符号 */
        if (isMinus) {
            str[len++] = '-';
        }
    } else if (base == 16) {
        /* 补充高位 */
        if (isMinus) {
            len--;
            digit += 8; // 此处 digit 必定是最高位的
            str[len++] = (digit > 9) ? (digit - 10) + 'A' : digit + '0';
        }
        /* 16 位 0x */
        str[len++] = 'x';
        str[len++] = '0';
    }

    /* 反转字符串，以得到正确的顺序 */
    g_reverse(str, len);

    /* 添加空结束符 */
    str[len] = '\0';

    return str;
}

gw_inline char *g_ftoa(int32_t num, char str[], uint8_t fx)
{
    uint8_t len     = 0;
    bool_t  isMinus = FALSE;
    int32_t digit;

    /* 无小数点 */
    if (fx == 0) {
        return g_itoa(num, str, 10);
    }

    /* 处理负数 */
    if (num < 0) {
        isMinus = TRUE;
        num     = -num;
    }

    /* 将数字从低位到高位迭代计算转换后的字符并存储在结果字符串中 */
    str[fx] = '.';
    while ((num != 0) || (len <= (fx + 1))) {
        digit = num % 10;

        if ((len == fx) && (fx != 0)) {
            len++;
        }

        str[len++] = digit + '0';

        if (num != 0) {
            num /= 10;
        }
    }

    /* 负数添加 '-' 符号 */
    if (isMinus) {
        str[len++] = '-';
    }

    /* 反转字符串，以得到正确的顺序 */
    g_reverse(str, len);

    /* 添加空结束符 */
    str[len] = '\0';

    return str;
}

/*****************************************************************************
*  函 数 名： g_get_setbit_no
*  功能描述： 获取第一个置 1 的位数
*  输入参数： uint16_t var
*  返 回 值： uint8_t
*****************************************************************************/
gw_inline uint8_t g_get_setbit_no(uint16_t var) {
	uint8_t i;
	for (i = 0; i < 16; i++) {
		if ((var>>i)&(0x01)) {
			break;
		}
	}
	return i;
}


#endif /* __GW_LIB_H__ */
