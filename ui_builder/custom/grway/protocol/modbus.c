#include "modbus.h"
#include "../config/mem_alloc.h"
#include "../utils/gw_utils.h"

int8_t protocol_no[] = {0, 1};

int modbus_cmd_push(uint8_t mb_no, uint8_t dev_addr, uint8_t func_code, uint16_t addr, uint16_t num, int16_t *buf, int16_t var)
{
    struct protocol_node node;
    node.cmd.modbus.dev_addr = dev_addr;
    node.cmd.modbus.func_code = func_code;
    node.cmd.modbus.addr = addr;
    node.cmd.modbus.num = num;
    if (buf) {
        node.cmd.modbus.buf = buf;
    } else {
        node.cmd.modbus.var = var;
    }
    node.type = P_TYPE_MODBUS;
    node.need_recv = TRUE;
    node.try_cnt = 2;
    node.tx_dly1ms_tm = 5;
    node.rx_ovf1ms_tm = 500;

    return protocol_list_push_no(protocol_no[mb_no], &node);
}

static void mb_send_addr_num(struct modbus_cmd *pmb, uint8_t *buf, uint16_t *len) {
	buf[(*len)++] = G_HIGH_BYTE(pmb->addr);
	buf[(*len)++] = G_LOW_BYTE(pmb->addr);
	buf[(*len)++] = G_HIGH_BYTE(pmb->num);
	buf[(*len)++] = G_LOW_BYTE(pmb->num);
}

static void mb_send_addr(struct modbus_cmd *pmb, uint8_t *buf, uint16_t *len) {
	buf[(*len)++] = G_HIGH_BYTE(pmb->addr);
	buf[(*len)++] = G_LOW_BYTE(pmb->addr);
}

static void mb_load_sg_coil(struct modbus_cmd *pmb, uint8_t *buf, uint16_t *len) {
	buf[(*len)++] = (pmb->var)?0xFF:0x00;
	buf[(*len)++] = 0x00;
}

static void mb_load_coils(struct modbus_cmd *pmb, uint8_t *buf, uint16_t *len) {
	uint16_t i;
	
	for (i = 0; i < pmb->num; i++) {
		G_WRITE_BUF_BIT(&buf[*len], i,
			G_GET_16BUF_BIT(pmb->buf, pmb->var+i));
	}
	*len+=G_NOC(pmb->num,8);
}

static void mb_unload_sg_coil(struct modbus_cmd *pmb, uint8_t *buf) {
	G_WRITE_16BUF_BIT(pmb->buf, pmb->var, ((buf[0]==0xFF)?1:0));
	pmb->var = ((buf[0]==0xFF)?1:0);
}

static void mb_unload_coils(struct modbus_cmd *pmb, uint8_t *buf) {
	uint16_t i;
	for (i = 0; i < pmb->num; i++) {
		G_WRITE_16BUF_BIT(pmb->buf, pmb->var+i, G_GET_BUF_BIT(buf, i));
	}
}

static void mb_load_sg_reg(struct modbus_cmd *pmb, uint8_t *buf, uint16_t *len) {
	buf[(*len)++] = G_HIGH_BYTE(pmb->var);
	buf[(*len)++] = G_LOW_BYTE(pmb->var);
}

static void mb_load_regs(struct modbus_cmd *pmb, uint8_t *buf, uint16_t *len) {
	uint16_t i;
	for (i = 0; i < pmb->num; i++) {
		buf[(*len)++] = G_HIGH_BYTE(pmb->buf[i]);
		buf[(*len)++] = G_LOW_BYTE(pmb->buf[i]);
	}
}

static void mb_unload_sg_reg(struct modbus_cmd *pmb, uint8_t *buf) {
	pmb->var = G_COMBINE_BYTE(buf[0], buf[1]);
	memcpy(pmb->buf,&pmb->var,sizeof(pmb->var));
}

static void mb_unload_regs(struct modbus_cmd *pmb, uint8_t *buf) {
	uint16_t i;
	for (i = 0; i < pmb->num; i++) {
		pmb->var = G_COMBINE_BYTE(buf[2*i], buf[2*i+1]);
        pmb->buf[i] = pmb->var;
	}
}

static int mb_recv_addr(struct modbus_cmd *pmb, uint8_t *buf)
{
	if (pmb->addr!=G_COMBINE_BYTE(buf[0], buf[1])) {
		return -1;
	}
	return 0;
}

static int mb_recv_addr_num(struct modbus_cmd *pmb, uint8_t *buf)
{
	if (pmb->addr!=G_COMBINE_BYTE(buf[0], buf[1])) {
		return -1;
	}
	if (pmb->num!=G_COMBINE_BYTE(buf[2], buf[3])) {
		return -1;
	}
	return 0;
}

static int mb_recv_num(struct modbus_cmd *pmb, uint8_t *buf)
{
	/* 魔改指令，计数长度失效 */
    if (pmb->func_code == MB_CMD_RD_REGS_CC) {
        return 0;
    }
	if ((pmb->func_code == MB_CMD_RD_COILS)||(pmb->func_code == MB_CMD_RD_INP_COILS)) {
		if (buf[0] != G_NOC(pmb->num,8)) { // 判断 计数长度是否符合
			return -1;
		}
	} else {
		if (buf[0] != (pmb->num*2)) { // 判断 计数长度是否符合
			return -1;
		}
	}
    return 0;
}

static int mb_recv_rd_datas(struct modbus_cmd *pmb, uint8_t *buf, uint16_t len) {
    int err;
    err = mb_recv_num(pmb,buf);
    if (err) {
        return err;
    }
	buf++; // 偏移1位 跳过 字节数
	
	switch (pmb->func_code) {
		case MB_CMD_RD_INP_COILS:
		case MB_CMD_RD_COILS:
			mb_unload_coils(pmb,buf);
			break;
		case MB_CMD_RD_INP_REGS:
		case MB_CMD_RD_REGS:
			mb_unload_regs(pmb,buf);
			break;
		default:
			break;
	}

	return 0;
}

static bool_t mb_respond_rd_datas(struct modbus_cmd *pmb,
                                uint8_t *in_buf, uint8_t *out_buf, uint16_t *out_len)
{
    pmb->addr = G_COMBINE_BYTE(in_buf[0],in_buf[1]);
    pmb->num = G_COMBINE_BYTE(in_buf[2],in_buf[3]);
    
    switch (pmb->func_code) {
        case MB_CMD_RD_INP_COILS:
            if (!mem_is_ro_regs(pmb->addr/16, (pmb->addr+pmb->num)/16, &pmb->buf)) {
                return FALSE;
            }
            pmb->var=pmb->addr%16;
            out_buf[(*out_len)++] = G_NOC(pmb->num,8);
            mb_load_coils(pmb, out_buf, out_len);
            break;
        case MB_CMD_RD_INP_REGS:
            if (!mem_is_ro_regs(pmb->addr, pmb->addr+pmb->num, &pmb->buf)) {
                return FALSE;
            }
            out_buf[(*out_len)++] = 2*pmb->num;
            mb_load_regs(pmb, out_buf, out_len);
            break;
        case MB_CMD_RD_COILS:
            if (!mem_is_rw_regs(pmb->addr/16, (pmb->addr+pmb->num)/16, &pmb->buf)) {
                return FALSE;
            }
            pmb->var=pmb->addr%16;
            out_buf[(*out_len)++] = G_NOC(pmb->num,8);
            mb_load_coils(pmb, out_buf, out_len);
            break;
        case MB_CMD_RD_REGS:
            if (!mem_is_rw_regs(pmb->addr, pmb->addr+pmb->num, &pmb->buf)) {
                return FALSE;
            }
            out_buf[(*out_len)++] = 2*pmb->num;
            mb_load_regs(pmb, out_buf, out_len);
            break;
        default:
            break;
    }
    return TRUE;
}

static bool_t mb_respond_wr_sg_coil(struct modbus_cmd *pmb,
                                    uint8_t *in_buf, uint8_t *out_buf, uint16_t *out_len) {
    pmb->addr = G_COMBINE_BYTE(in_buf[0],in_buf[1]);

    if (!mem_is_rw_regs(pmb->addr/16, (pmb->addr)/16, &pmb->buf)) {
        return FALSE;
    }
    in_buf+=2; // 跳过 写入地址

    /* 解包接收数据 */
    pmb->var=pmb->addr%16;
    mb_unload_sg_coil(pmb,in_buf);

    /* 打包发送数据 */
    mb_send_addr(pmb, out_buf, out_len);
    mb_load_sg_coil(pmb, out_buf, out_len);

    return TRUE;
}

static bool_t mb_respond_wr_coils(struct modbus_cmd *pmb,
                                    uint8_t *in_buf, uint8_t *out_buf, uint16_t *out_len) {
    pmb->addr = G_COMBINE_BYTE(in_buf[0],in_buf[1]);
    pmb->num = G_COMBINE_BYTE(in_buf[2],in_buf[3]);

    if (!mem_is_rw_regs(pmb->addr/16, (pmb->addr+pmb->num)/16, &pmb->buf)) {
        return FALSE;
    }
    in_buf+=4; // 跳过 写入地址 和 写入数量
    in_buf+=1; // 跳过 写入字节数

    /* 解包接收数据 */
    pmb->var=pmb->addr%16;
    mb_unload_coils(pmb,in_buf);

    /* 打包发送数据 */
    mb_send_addr_num(pmb, out_buf, out_len);

    return TRUE;
}

static bool_t mb_respond_wr_sg_reg(struct modbus_cmd *pmb,
                                    uint8_t *in_buf, uint8_t *out_buf, uint16_t *out_len) {
    pmb->addr = G_COMBINE_BYTE(in_buf[0],in_buf[1]);
    pmb->addr=pmb->addr;

    if (!mem_is_rw_regs(pmb->addr, pmb->addr, &pmb->buf)) {
        return FALSE;
    }
    in_buf+=2; // 跳过 写入地址

    /* 解包接收数据 */
    mb_unload_sg_reg(pmb,in_buf);

    /* 打包发送数据 */
    mb_send_addr(pmb, out_buf, out_len);
    mb_load_sg_reg(pmb, out_buf, out_len);

    return TRUE;
}

static bool_t mb_respond_wr_regs(struct modbus_cmd *pmb,
                                uint8_t *in_buf, uint8_t *out_buf, uint16_t *out_len) {
    pmb->addr = G_COMBINE_BYTE(in_buf[0],in_buf[1]);
    pmb->num = G_COMBINE_BYTE(in_buf[2],in_buf[3]);

    if (!mem_is_rw_regs(pmb->addr, pmb->addr+pmb->num, &pmb->buf)) {
        return FALSE;
    }
    in_buf+=4; // 跳过 写入地址 和 写入数量
    in_buf+=1; // 跳过 写入字节数

    /* 解包接收数据 */
    mb_unload_regs(pmb,in_buf);

    /* 打包发送数据 */
    mb_send_addr_num(pmb, out_buf, out_len);

    return TRUE;
}

void modbus_pack(struct protocol_node *node, uint8_t *out_buf, uint16_t *out_len)
{
    struct modbus_cmd *pmb;
    pmb = &node->cmd.modbus;

    /* 打包帧头-设备地址-功能码 */
    *out_len = 0;
    out_buf[(*out_len)++] = pmb->dev_addr;
    out_buf[(*out_len)++] = ((pmb->func_code)&(0x3F));

    switch (node->cmd.modbus.func_code) {
        case MB_CMD_RD_COILS: // 读 可读写的多个位
        case MB_CMD_RD_INP_COILS: // 读 只读的多个位
        case MB_CMD_RD_REGS: // 读 可读写的多个16位寄存器
        case MB_CMD_RD_INP_REGS: // 读 只读的多个16位寄存器
        case MB_CMD_RD_REGS_CC: // 智科魔改 读 可读写的多个16位寄存器
            mb_send_addr_num(pmb, out_buf, out_len);
            break;
        case MB_CMD_WR_SG_COIL: // 写 单一个位
            mb_send_addr(pmb, out_buf, out_len);
            mb_load_sg_coil(pmb, out_buf, out_len);
            break;
        case MB_CMD_WR_COILS: // 写 多个位
            mb_send_addr_num(pmb, out_buf, out_len);
            out_buf[(*out_len)++] = G_NOC(pmb->num, 8);
            mb_load_coils(pmb, out_buf, out_len);
            break;
        case MB_CMD_WR_SG_REG: // 写 单一个16位寄存器
            mb_send_addr(pmb, out_buf, out_len);
            mb_load_sg_reg(pmb, out_buf, out_len);
            break;
        case MB_CMD_WR_REGS: // 写 多个16位寄存器
            mb_send_addr_num(pmb, out_buf, out_len);
            out_buf[(*out_len)++] = 2*pmb->num;
            mb_load_regs(pmb, out_buf, out_len);
            break;
        default:
            break;
    }

    /* 打包帧尾-CRC16 */
    uint16_t crc16 = crc16_modbus(out_buf, *out_len);
    out_buf[(*out_len)++] = G_LOW_BYTE(crc16);
    out_buf[(*out_len)++] = G_HIGH_BYTE(crc16);

    /* 广播无需回复 */
    if (pmb->dev_addr==0) {
        node->need_recv = FALSE;
    }
}

int modbus_unpack(struct protocol_node *node, uint8_t *in_buf, uint16_t *in_len, uint8_t *out_buf, uint16_t *out_len)
{
    if (*in_len<5) {
        return RECV_PARSEING;
    } else {
        uint16_t crc16 = crc16_modbus(in_buf, *in_len - 2);
        if ((G_LOW_BYTE(crc16)!=in_buf[*in_len - 2]) ||
            (G_HIGH_BYTE(crc16)!=in_buf[*in_len - 1])) {
            return RECV_PARSEING;
        }
    }

    /* 请求后等待响应 */
    struct modbus_cmd *pmb;
    pmb = &node->cmd.modbus;
    /* 检查地址 */
    if (pmb->dev_addr!=in_buf[0]) {
        return RECV_FORMAT_ERR;
    }

    /* 事件响应，还是处理回复 */
    if (node->need_recv==TRUE) {
        /* 检查功能码 */
        if ((pmb->func_code!=in_buf[1])||(in_buf[1]&0x80)) {
            return RECV_DATA_ERR;
        }

        in_buf += 2; // 偏移2位 跳过 设备地址号 和 功能码
        *in_len -= 4; // 减去 设备地址号 功能码 和 CRC 校验码的长度
        int ret = 0;
        switch (pmb->func_code) {
            case MB_CMD_RD_COILS:
            case MB_CMD_RD_INP_COILS:
            case MB_CMD_RD_REGS:
            case MB_CMD_RD_INP_REGS:
            case MB_CMD_RD_REGS_CC:
                ret = mb_recv_rd_datas(pmb,in_buf,*in_len);
                break;
            case MB_CMD_WR_SG_COIL:
            case MB_CMD_WR_SG_REG:
                ret = mb_recv_addr(pmb,in_buf);
                break;
            case MB_CMD_WR_COILS:
            case MB_CMD_WR_REGS:
                ret = mb_recv_addr_num(pmb,in_buf);
                break;
        }

        if (ret!=0) {
            return RECV_DATA_ERR;
        }

        node->need_recv = FALSE;
        return RECV_SUCCESS;
    } else {
        uint8_t errCode=0;
        pmb->func_code = in_buf[1];

        *out_len = 0;
        /* 打包帧头-设备地址,功能码 */
        out_buf[(*out_len)++] = pmb->dev_addr;
        out_buf[(*out_len)++] = pmb->func_code;

        in_buf += 2; // 偏移2位 跳过 设备地址号 和 功能码
        *in_len -= 4; // 减去 设备地址号 功能码 和 CRC 校验码的长度

        switch (pmb->func_code) {
            case MB_CMD_RD_COILS:
            case MB_CMD_RD_INP_COILS:
            case MB_CMD_RD_REGS:
            case MB_CMD_RD_INP_REGS:
                if (!mb_respond_rd_datas(pmb, in_buf, out_buf, out_len)) {
                    errCode = 0x02;
                }
                break;
            case MB_CMD_WR_SG_COIL:
                if (!mb_respond_wr_sg_coil(pmb, in_buf, out_buf, out_len)) {
                    errCode = 0x02;
                }
                break;
            case MB_CMD_WR_COILS:
                if (!mb_respond_wr_coils(pmb, in_buf, out_buf, out_len)) {
                    errCode = 0x02;
                }
                break;
            case MB_CMD_WR_SG_REG:
                if (!mb_respond_wr_sg_reg(pmb, in_buf, out_buf, out_len)) {
                    errCode = 0x02;
                }
                break;
            case MB_CMD_WR_REGS:
                if (!mb_respond_wr_regs(pmb, in_buf, out_buf, out_len)) {
                    errCode = 0x02;
                }
                break;
            default: // 非法功能
                errCode = 0x01;
                break;
        }

        /* 故障码 */
        if (errCode!=0) {
            --(*out_len);
            out_buf[(*out_len)++] |= 0x80;
            out_buf[(*out_len)++] = errCode;
        }

        /* 打包帧尾-CRC16 */
        uint16_t crc16 = crc16_modbus(out_buf, *out_len);
        out_buf[(*out_len)++] = G_LOW_BYTE(crc16);
        out_buf[(*out_len)++] = G_HIGH_BYTE(crc16);

        return RECV_EVENT;
    }
    return RECV_FORMAT_ERR;
}

REG_PROTO(P_TYPE_MODBUS, modbus_pack, modbus_unpack);
