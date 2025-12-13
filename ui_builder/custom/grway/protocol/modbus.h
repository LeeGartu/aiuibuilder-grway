#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "protocol.h"

// #ifndef USE_BOARD_DEF // 非板层定义
#define EN_MB_MASTER		// 使能 MB 主机
#define EN_MB_SLAVE			// 使能 MB 从机
#define EN_MB_RO_COIL		// 使能 只读 COIL
#define EN_MB_RW_COIL		// 使能 读写 COIL
#define EN_MB_RO_REG		// 使能 只读 REG
#define EN_MB_RW_REG		// 使能 读写 REG
// #endif
#define MB_WR_PAGE_SIZE		40
#define MB_RD_PAGE_SIZE		80

typedef enum {
	MB_CMD_RD_COILS			= 0x01, // 读 可读写的多个位
	MB_CMD_RD_INP_COILS		= 0x02, // 读 只读的多个位
	MB_CMD_RD_REGS			= 0x03, // 读 可读写的多个16位寄存器
	MB_CMD_RD_INP_REGS		= 0x04, // 读 只读的多个16位寄存器
	MB_CMD_WR_SG_COIL		= 0x05, // 写 单一个位
	MB_CMD_WR_SG_REG		= 0x06, // 写 单一个16位寄存器
	MB_CMD_WR_COILS			= 0x0F, // 写 多个位
	MB_CMD_WR_REGS			= 0x10, // 写 多个16位寄存器

	MB_CMD_RD_REGS_CC		= 0x83, // 智科魔改 读 可读写的多个16位寄存器
	MB_CMD_NUM,
} mb_cmd_e; // Modbus指令

int modbus_cmd_push(uint8_t mb_no, uint8_t dev_addr, uint8_t func_code, uint16_t addr, uint16_t num, int16_t *buf, int16_t var);
#ifdef EN_MB_RO_COIL
static inline int mb_rd_inp_coils_push(uint8_t no, uint8_t devAddr, int16_t *rdBuf, uint16_t rdAddr, uint16_t rdNum)
{
    return modbus_cmd_push(no, devAddr, MB_CMD_RD_INP_COILS, rdAddr, rdNum, rdBuf, 0);
}
#endif
#ifdef EN_MB_RO_REG
static inline int mb_rd_inp_regs_push(uint8_t no, uint8_t devAddr, int16_t *rdBuf, uint16_t rdAddr, uint16_t rdNum)
{
    while (rdNum>MB_RD_PAGE_SIZE) {
        modbus_cmd_push(no , devAddr, MB_CMD_RD_INP_REGS, rdAddr, MB_RD_PAGE_SIZE, rdBuf, 0);
        rdBuf+=MB_RD_PAGE_SIZE;
        rdAddr+=MB_RD_PAGE_SIZE;
        rdNum-=MB_RD_PAGE_SIZE;
    }
    return modbus_cmd_push(no, devAddr, MB_CMD_RD_INP_REGS, rdAddr, rdNum, rdBuf, 0);
}
#endif
#ifdef EN_MB_RW_COIL
static inline int mb_rd_coils_push(uint8_t no, uint8_t devAddr, int16_t *rdBuf, uint16_t rdAddr, uint16_t rdNum)
{
    return modbus_cmd_push(no, devAddr, MB_CMD_RD_COILS, rdAddr, rdNum, rdBuf, 0);
}
static inline int mb_wr_sg_coil_push(uint8_t no, uint8_t devAddr, uint16_t var, uint16_t wrAddr)
{
    return modbus_cmd_push(no, devAddr, MB_CMD_WR_SG_COIL, wrAddr, 1, NULL, var);
}
static inline int mb_wr_coils_push(uint8_t no, uint8_t devAddr, int16_t *wrBuf, uint16_t wrAddr, uint16_t wrNum)
{
    return modbus_cmd_push(no, devAddr, MB_CMD_WR_COILS, wrAddr, wrNum, wrBuf, 0);
}
#endif
#ifdef EN_MB_RW_REG
static inline int mb_rd_regs_push(uint8_t no, uint8_t devAddr, int16_t *rdBuf, uint16_t rdAddr, uint16_t rdNum)
{
    while (rdNum>MB_RD_PAGE_SIZE) {
        modbus_cmd_push(no, devAddr, MB_CMD_RD_REGS, rdAddr, MB_RD_PAGE_SIZE, rdBuf, 0);
        rdBuf+=MB_RD_PAGE_SIZE;
        rdAddr+=MB_RD_PAGE_SIZE;
        rdNum-=MB_RD_PAGE_SIZE;
    }
    return modbus_cmd_push(no, devAddr, MB_CMD_RD_REGS, rdAddr, rdNum, rdBuf, 0);
}
static inline int mb_rd_regs_cc_push (uint8_t no, uint8_t devAddr, int16_t *rdBuf, uint16_t rdAddr, uint16_t rdNum)
{
    return modbus_cmd_push(no, devAddr, MB_CMD_RD_REGS_CC, rdAddr, rdNum, rdBuf, 0);
}
static inline int mb_wr_sg_reg_push(uint8_t no, uint8_t devAddr, uint16_t var, uint16_t wrAddr)
{
    return modbus_cmd_push(no, devAddr, MB_CMD_WR_SG_REG, wrAddr, 1, NULL, var);
}
static inline int mb_wr_regs_push(uint8_t no, uint8_t devAddr, int16_t *wrBuf, uint16_t wrAddr, uint16_t wrNum)
{
    while (wrNum>MB_WR_PAGE_SIZE) {
        modbus_cmd_push(no , devAddr, MB_CMD_WR_REGS, wrAddr, MB_WR_PAGE_SIZE, wrBuf, 0);
        wrBuf+=MB_WR_PAGE_SIZE;
        wrAddr+=MB_WR_PAGE_SIZE;
        wrNum-=MB_WR_PAGE_SIZE;
    }
    return modbus_cmd_push(no, devAddr, MB_CMD_WR_REGS, wrAddr, wrNum, wrBuf, 0);
}
#endif

#endif // !__MODBUS_H__
