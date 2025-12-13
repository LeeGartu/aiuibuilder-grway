/**
 * @file mem_alloc.h
 * @brief 内存分配和变量声明配置文件
 *
 * 定义了系统中各种寄存器变量的内存分配策略，
 * 包括只读变量（机器状态、故障信息、芯片外设）和读写变量（参数）的存储空间划分。
 *
 * @defgroup mem_size 内存大小
 * @defgroup mem_declare 内存声明
 */

#ifndef __GW_MEM_ALLOC_H__
#define __GW_MEM_ALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MEM_ALLOC_EXT
#define MEM_ALLOC_EXT
#else
#define MEM_ALLOC_EXT extern
#endif

#include "../utils/gw_utils.h"

/**
 * @addtogroup mem_size 内存大小
 * @{
 */
#define RO_MEM_STATUS_BASE  (0)
#define RO_MEM_STATUS_SIZE  (0x0800) /**< 只读变量(机器状态) */

#define RW_MEM_PARAM_BASE   (0)
#define RW_MEM_PARAM_SIZE   (0x0800) /**< 读写变量(参数) */
/** @} */

/**
 * @addtogroup mem_declare 内存声明
 * @{
 */
MEM_ALLOC_EXT ALIGN(4) uint8_t ro_mem_status[RO_MEM_STATUS_SIZE]; /**< 只读变量(机器状态) */
MEM_ALLOC_EXT ALIGN(4) uint8_t rw_mem_param[RW_MEM_PARAM_SIZE];   /**< 读写变量(参数) */
/** @} */

bool_t mem_is_ro_regs(uint16_t sAddr, uint16_t eAddr, int16_t **buf);
bool_t mem_is_rw_regs(uint16_t sAddr, uint16_t eAddr, int16_t **buf);

#ifdef __cplusplus
}
#endif

#endif /* __GW_MEM_ALLOC_H__ */
