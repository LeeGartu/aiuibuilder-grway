/**
 * @file error_config.h
 * @brief 故障号配置文件
 *
 * @defgroup error_buf 故障号缓存定义
 * @defgroup error_cp 线控故障号定义
 */

#ifndef __ERROR_CONFIG_H__
#define __ERROR_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mem_status.h"

/**
 * @addtogroup mem_size 内存大小
 * @{
 */
#define ERR_BIT_NUM			8 // 一个故障组的位数
#define ERR_BUF_NUM			9 // 故障组数量
#define ERR_CP_BUF_NUM		1 // 线控故障组数量

#define ERR_CUR_LEN			30 // 当前故障缓存长度
#define ERR_HIS_LEN			30 // 历史故障缓存长度
/** @} */

/**
 * @addtogroup mem_size 内存大小
 * @{
 */
typedef enum {
	ERR_COMM	= 0xF000,	// 通信故障
	ERR_NULL	= 0xFFFE,
	ERN			= ERR_NULL,
} err_no_e;
/** @} */

typedef struct err_his_node_ {
	uint16_t no;		// 故障号
	uint8_t unit_no;	// 机组号
	int16_t wttemp;		// 热水温度
} err_his_node_t;

#ifdef __cplusplus
}
#endif

#endif /* __ERROR_CONFIG_H__ */
