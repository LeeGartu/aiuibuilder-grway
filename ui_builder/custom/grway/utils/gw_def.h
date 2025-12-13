#ifndef __GW_DEF_H__
#define __GW_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

// clang-format off
/* Compiler Related Definitions */
#if defined(__ARMCC_VERSION)           /* ARM Compiler */
#define GW_SECTION(x)               __attribute__((section(x)))
#define GW_USED                     __attribute__((used))
#define ALIGN(n)                    __attribute__((aligned(n)))
#define GW_WEAK                     __attribute__((weak))
#define gw_inline                   static __inline
#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
#define GW_SECTION(x)               @ x
#define GW_USED                     __root
#define PRAGMA(x)                   _Pragma(#x)
#define ALIGN(n)                    PRAGMA(data_alignment=n)
#define GW_WEAK                     __weak
#define gw_inline                   static inline
#elif defined (__GNUC__)                /* GNU GCC Compiler */
/* the version of GNU GCC must be greater than 4.x */
typedef __builtin_va_list           __gnuc_va_list;
typedef __gnuc_va_list              va_list;
#define va_start(v,l)               __builtin_va_start(v,l)
#define va_end(v)                   __builtin_va_end(v)
#define va_arg(v,l)                 __builtin_va_arg(v,l)
#define GW_SECTION(x)               __attribute__((section(x)))
#define GW_USED                     __attribute__((used))
#define ALIGN(n)                    __attribute__((aligned(n)))
#define GW_WEAK                     __attribute__((weak))
#define gw_inline                   static __inline
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
#define GW_SECTION(x)               __attribute__((section(x)))
#define GW_USED                     __attribute__((used))
#define ALIGN(n)                    __attribute__((aligned(n)))
#define GW_WEAK                     __attribute__((weak))
#define gw_inline                   static inline
#elif defined (_MSC_VER)
#define GW_SECTION(x)
#define GW_USED
#define ALIGN(n)                    __declspec(align(n))
#define GW_WEAK
#define gw_inline                   static __inline
#elif defined (__TI_COMPILER_VERSION__)
/* The way that TI compiler set section is different from other(at least
    * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
    * details. */
#define GW_SECTION(x)
#define GW_USED
#define PRAGMA(x)                   _Pragma(#x)
#define ALIGN(n)
#define GW_WEAK
#define gw_inline                   static inline
#elif defined (__TASKING__)
#define GW_SECTION(x)               __attribute__((section(x)))
#define GW_USED                     __attribute__((used, protect))
#define PRAGMA(x)                   _Pragma(#x)
#define ALIGN(n)                    __attribute__((__align(n)))
#define GW_WEAK                     __attribute__((weak))
#define gw_inline                   static inline
#else
    #error not supported tool chain
#endif /* __ARMCC_VERSION */
// clang-format on

/**
 * Double List structure
 */
struct gw_list_node {
    struct gw_list_node *next; /**< point to next node. */
    struct gw_list_node *prev; /**< point to prev node. */
};
typedef struct gw_list_node gw_list_t; /**< Type for lists. */

/**
 * Single List structure
 */
struct gw_slist_node {
    struct gw_slist_node *next; /**< point to next node. */
};
typedef struct gw_slist_node gw_slist_t; /**< Type for single list. */

// 布尔值
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
// 其他布尔值
#define OFF  FALSE
#define ON   TRUE
#define STOP FALSE
#define RUN  TRUE
#define DOWN FALSE
#define UP   TRUE
// 布尔类型
typedef int8_t bool_t;
// 空指针
#ifndef NULL
#define NULL ((void *)0)
#endif
// void 函数指针
typedef void (*void_fn_t)();
// 位变量
typedef union {
    uint8_t byte; // 8位无符号整数类型
    struct {      // 定义位域结构体
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
    } bits;
} bit8_t;

typedef union {
    uint16_t word;
    struct {
        uint8_t lo : 8;
        uint8_t hi : 8;
    } byte;
    struct { // 定义位域结构体
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
        uint8_t bit8 : 1;
        uint8_t bit9 : 1;
        uint8_t bit10 : 1;
        uint8_t bit11 : 1;
        uint8_t bit12 : 1;
        uint8_t bit13 : 1;
        uint8_t bit14 : 1;
        uint8_t bit15 : 1;
    } bits;
} bit16_t;

typedef union {
    uint32_t dword;
    struct {
        uint16_t lo : 16;
        uint16_t hi : 16;
    } word;
    struct {
        uint8_t byte0 : 8;
        uint8_t byte1 : 8;
        uint8_t byte2 : 8;
        uint8_t byte3 : 8;
    } byte;
    struct {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
        uint8_t bit8 : 1;
        uint8_t bit9 : 1;
        uint8_t bit10 : 1;
        uint8_t bit11 : 1;
        uint8_t bit12 : 1;
        uint8_t bit13 : 1;
        uint8_t bit14 : 1;
        uint8_t bit15 : 1;
        uint8_t bit16 : 1;
        uint8_t bit17 : 1;
        uint8_t bit18 : 1;
        uint8_t bit19 : 1;
        uint8_t bit20 : 1;
        uint8_t bit21 : 1;
        uint8_t bit22 : 1;
        uint8_t bit23 : 1;
        uint8_t bit24 : 1;
        uint8_t bit25 : 1;
        uint8_t bit26 : 1;
        uint8_t bit27 : 1;
        uint8_t bit28 : 1;
        uint8_t bit29 : 1;
        uint8_t bit30 : 1;
        uint8_t bit31 : 1;
    } bits;
} bit32_t;

typedef union {
    uint8_t *arr; // 数组
    char    *str; // 字符串

    uint32_t u32; // 无符号双字
    int32_t  s32; // 有符号双字
} type_un_t;      // 内存类型

typedef union {
    void *pvoid; // 空指针

    uint8_t *arr; // 数组
    char    *str; // 字符串

    uint8_t **parr; // 数组指针
    char    **pstr; // 字符串指针

    bool_t *pbol; // 布尔指针

    uint8_t *pu8; // 无符号字节 指针
    int8_t  *ps8; // 有符号字节 指针
    bit8_t  *pb8; // 位变量指针

    uint16_t *pu16; // 无符号字 指针
    int16_t  *ps16; // 有符号字 指针
    bit16_t  *pb16; // 位变量指针

    uint32_t *pu32; // 无符号双字 指针
    int32_t  *ps32; // 有符号双字 指针
    bit32_t  *pb32; // 位变量指针

    float *pf32; // 浮点数指针
} p_type_un_t;   // 内存指针类型

#ifdef __cplusplus
}
#endif

#endif /* __GW_DEF_H__ */
