/******************************************************************************
*  文件名称：error.h
*  版 本 号：V1.0
*  作    者：ljt
*  生成日期：2023年8月11日
*  
*  功能描述：故障
*  
*  修改日期        作者
*  2023年8月11日    ljt
*          修改内容：创建文件
*  
******************************************************************************/
#ifndef __ERR_H__
#define __ERR_H__

#ifdef ERR_EXT
#define ERR_EXT
#else
#define ERR_EXT extern
#endif

#include "../config/error_config.h"

#define ERR_HIS_SIZE		sizeof(err_history)

/*****************************************************************************
* 数据类型
*****************************************************************************/
typedef struct err_ {
	uint8_t hisIdx;		// 历史故障索引
	uint8_t err_update;
} err_t;

typedef struct err_node_ {
	uint16_t no;		// 故障号
	uint8_t unit_no;	// 机组号
} err_node_t;

/*****************************************************************************
* 变量声明
*****************************************************************************/
ERR_EXT ALIGN(4) err_node_t err_current[ERR_CUR_LEN];

ERR_EXT uint16_t _err_ram[MACH_UNITS_NUM*ERR_BUF_NUM];
ERR_EXT uint16_t _err_ram_old[MACH_UNITS_NUM*ERR_BUF_NUM];
#define err_ram(u,n) _err_ram[(u)*ERR_BUF_NUM+(n)]
#define err_ram_old(u,n) _err_ram_old[(u)*ERR_BUF_NUM+(n)]

ERR_EXT uint16_t _err_cp_ram[ERR_CP_BUF_NUM];
ERR_EXT uint16_t _err_cp_ram_old[ERR_CP_BUF_NUM];
#define err_cp_ram(n) _err_cp_ram[n]
#define err_cp_ram_old(n) _err_cp_ram_old[n]

/*****************************************************************************
* 宏函数声明
*****************************************************************************/
#define err_write(u,no,v)		((v)?err_set((u),(no)):err_clr((u),(no)))

/*****************************************************************************
* 函数声明
*****************************************************************************/
bool_t err_is_update(void);
void err_set(uint8_t u, uint16_t no);
void err_clr(uint8_t u, uint16_t no);
bool_t err_get(uint8_t u, uint16_t no);
uint16_t err_get_no(void);
uint8_t err_no_to_bits(uint16_t errno);
void err_clean_his(void);
void err_read_his(void (*cb)(err_his_node_t node));
void err_reset_his(void);
void err_deal(void);
void err_init(void);

#endif /* __ERR_H__ */

