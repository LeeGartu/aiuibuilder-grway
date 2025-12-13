/******************************************************************************
 *  文件名称：error.c
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
#define ERR_EXT

#include "../config/error_config_priv.h"
#include "nv_fdb.h"
#include "err.h"

/*****************************************************************************
 * 私有变量
 *****************************************************************************/
err_t            err;
struct fdb_tsdb *err_tsdb;
void (*err_read_cb)(err_his_node_t node) = NULL;

bool_t err_is_update(void)
{
    if (err.err_update) {
        err.err_update = FALSE;
        return TRUE;
    } else {
        return FALSE;
    }
}

/*****************************************************************************
 *  函 数 名： err_set
 *  功能描述： 设置故障
 *  输入参数： uint8_t u
 *             uint16_t no
 *  返 回 值： void
 *****************************************************************************/
void err_set(uint8_t u, uint16_t no)
{
    uint8_t i;
    for (i = 0; i < G_SIZE_OF_ARR(err_cp_no); i++) {
        if (err_cp_no[i] == no) {
            G_SET_BIT(err_cp_ram(i / ERR_BIT_NUM), i % ERR_BIT_NUM);
            return;
        }
    }
    for (i = 0; i < G_SIZE_OF_ARR(err_no); i++) {
        if (err_no[i] == no) {
            G_SET_BIT(err_ram(u, i / ERR_BIT_NUM), i % ERR_BIT_NUM);
            return;
        }
    }
}

/*****************************************************************************
 *  函 数 名： err_clr
 *  功能描述： 清除故障
 *  输入参数： uint8_t u
 *             uint16_t no
 *  返 回 值： void
 *****************************************************************************/
void err_clr(uint8_t u, uint16_t no)
{
    uint8_t i;
    for (i = 0; i < G_SIZE_OF_ARR(err_cp_no); i++) {
        if (err_cp_no[i] == no) {
            G_CLR_BIT(err_cp_ram(i / ERR_BIT_NUM), i % ERR_BIT_NUM);
            return;
        }
    }
    for (i = 0; i < G_SIZE_OF_ARR(err_no); i++) {
        if (err_no[i] == no) {
            G_CLR_BIT(err_ram(u, i / ERR_BIT_NUM), i % ERR_BIT_NUM);
            return;
        }
    }
}

/*****************************************************************************
 *  函 数 名： err_get
 *  功能描述： 获取故障
 *  输入参数： uint8_t u
 *             uint16_t no
 *  返 回 值： bool_t
 *****************************************************************************/
bool_t err_get(uint8_t u, uint16_t no)
{
    uint8_t i;
    for (i = 0; i < G_SIZE_OF_ARR(err_cp_no); i++) {
        if (err_cp_no[i] == no) {
            return G_GET_BIT(err_cp_ram(i / ERR_BIT_NUM), i % ERR_BIT_NUM);
        }
    }
    for (i = 0; i < G_SIZE_OF_ARR(err_no); i++) {
        if (err_no[i] == no) {
            return G_GET_BIT(err_ram(u, i / ERR_BIT_NUM), i % ERR_BIT_NUM);
        }
    }
    return FALSE;
}

/*****************************************************************************
 *  函 数 名： err_get_no
 *  功能描述： 获取故障号
 *  输入参数： void
 *  返 回 值： uint16_t
 *****************************************************************************/
uint16_t err_get_no(void)
{
    return err_current[0].no;
}

/*****************************************************************************
 *  函 数 名： err_no_to_bits
 *  功能描述： 故障号转位数
 *  输入参数： uint16_t errno
 *  返 回 值： uint8_t
 *****************************************************************************/
uint8_t err_no_to_bits(uint16_t errno)
{
    uint8_t bits;
    for (bits = 0; bits < (ERR_BUF_NUM * ERR_BIT_NUM); bits++) {
        if (errno == err_no[bits]) {
            return bits;
        }
    }
    for (bits = 0; bits < (ERR_CP_BUF_NUM * ERR_BIT_NUM); bits++) {
        if (errno == err_cp_no[bits]) {
            return bits + (ERR_BUF_NUM * ERR_BIT_NUM);
        }
    }
    return 0;
}

/*****************************************************************************
 *  函 数 名： err_is_stop
 *  功能描述： 判断故障是否要停机
 *  输入参数： uint16_t no
 *  返 回 值： static bool_t
 *****************************************************************************/
static bool_t err_is_stop(uint16_t no)
{
    no = no;
    return FALSE;
}

/*****************************************************************************
 *  函 数 名： err_clean_cur
 *  功能描述： 清空当前故障
 *  输入参数： void
 *  返 回 值： void
 *****************************************************************************/
void err_clean_cur(void)
{
    uint8_t i;
    uint8_t u;
    /* 只需清空故障号 */
    for (i = 0; i < ERR_CUR_LEN; i++) {
        err_current[i].no      = ERR_NULL;
        err_current[i].unit_no = 0;
    }

    /* 清空故障寄存器 */
    for (u = 0; u < MACH_UNITS_NUM; u++) {
        for (i = 0; i < ERR_BUF_NUM; i++) {
            err_ram_old(u, i) = 0;
        }
    }
}

/*****************************************************************************
 *  函 数 名： err_insert_cur
 *  功能描述： 插入当前故障
 *  输入参数： uint8_t u
 *             uint16_t no
 *  返 回 值： static void
 *****************************************************************************/
static void err_insert_cur(uint8_t u, uint16_t no)
{
    uint8_t i;

    /* 插入数组 */
    for (i = ERR_CUR_LEN - 1; i > 0; i--) {
        err_current[i].no      = err_current[i - 1].no;
        err_current[i].unit_no = err_current[i - 1].unit_no;
    }
    err_current[0].no      = no;
    err_current[0].unit_no = u;
}

/*****************************************************************************
 *  函 数 名： err_delete_cur
 *  功能描述： 移出当前故障
 *  输入参数： uint8_t u
 *             uint16_t no
 *  返 回 值： static void
 *****************************************************************************/
static void err_delete_cur(uint8_t u, uint16_t no)
{
    uint8_t i;

    /* 移出数组 */
    for (i = 0; i < ERR_CUR_LEN; i++) {
        if ((err_current[i].no == no) && (err_current[i].unit_no == u)) {
            for (; i < ERR_CUR_LEN - 1; i++) {
                err_current[i].no      = err_current[i + 1].no;
                err_current[i].unit_no = err_current[i + 1].unit_no;
            }
            break;
        }
    }
}

/*****************************************************************************
 *  函 数 名： err_clean_his
 *  功能描述： 清空历史故障
 *  输入参数： void
 *  返 回 值： void
 *****************************************************************************/
void err_clean_his(void)
{
    uint8_t i;
    uint8_t u;

    /* 只需清空故障号 */
    fdb_tsl_clean(err_tsdb);
    err.hisIdx = 0;

    /* 清空当前故障 */
    for (i = 0; i < ERR_CUR_LEN; i++) {
        err_current[i].no      = ERR_NULL;
        err_current[i].unit_no = 0;
    }

    /* 清空故障寄存器 */
    for (u = 0; u < MACH_UNITS_NUM; u++) {
        for (i = 0; i < ERR_BUF_NUM; i++) {
            err_ram_old(u, i) = 0;
        }
    }
}

/*****************************************************************************
 *  函 数 名： err_write_his
 *  功能描述： 保存历史故障
 *  输入参数： uint8_t u
 *             uint16_t no
 *  返 回 值： void
 *****************************************************************************/
static void err_write_his(uint8_t u, uint16_t no)
{
    struct fdb_blob blob;
    err_his_node_t  err_node;
    err_node.no      = no;
    err_node.unit_no = u;
    fdb_tsl_append(err_tsdb, fdb_blob_make(&blob, &err_node, sizeof(err_node)));
}

static bool err_read_his_cb(fdb_tsl_t tsl, void *arg)
{
    struct fdb_blob blob;
    err_his_node_t  node;
    fdb_tsdb_t      db = arg;

    fdb_blob_read((fdb_db_t)db, fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, &node, sizeof(node))));
    if (err_read_cb) {
        err_read_cb(node);
    }
    return false;
}

void err_read_his(void (*cb)(err_his_node_t node))
{
    err_read_cb = cb;
    fdb_tsl_iter(err_tsdb, err_read_his_cb, err_tsdb);
}

/*****************************************************************************
 *  函 数 名： err_reset_his
 *  功能描述： 复位历史故障
 *  输入参数： void
 *  返 回 值： static void
 *****************************************************************************/
void err_reset_his(void)
{
    /* 清空历史故障 */
    err_clean_his();
}

/*****************************************************************************
 *  函 数 名： err_deal
 *  功能描述： 故障更新处理
 *  输入参数： void
 *  返 回 值： void
 *****************************************************************************/
void err_deal(void)
{
    uint8_t  i;
    uint8_t  u;
    uint8_t  bits;
    uint16_t no;
    uint16_t t_err_ram;

    /* 对齐故障 */
    err_align();

    /* 遍历机组故障 */
    for (u = 0; u < 1; u++) {
        // for (u = 0; u < hmi_param.units_num; u++) {

        /* 更新故障状态的变化情况 */
        for (i = 0; i < ERR_BUF_NUM; i++) {
            if (err_ram(u, i) != err_ram_old(u, i)) {
                err.err_update = true;
                /* 获取新增的故障 */
                t_err_ram = err_ram(u, i) ^ err_ram_old(u, i);
                t_err_ram &= err_ram(u, i);

                /* 将故障保存到历史故障和当前故障中 */
                while (t_err_ram) {
                    bits = g_get_setbit_no(t_err_ram);
                    no = err_no[i * ERR_BIT_NUM + bits];
                    G_CLR_BIT(t_err_ram, bits);

                    if (no != ERR_NULL) {
                        err_write_his(u, no);
                        err_insert_cur(u, no);
                    }
                }

                /* 获取移除的故障 */
                t_err_ram = err_ram(u, i) ^ err_ram_old(u, i);
                t_err_ram &= err_ram_old(u, i);

                /* 当前故障移除 */
                while (t_err_ram) {
                    bits = g_get_setbit_no(t_err_ram);
                    no = err_no[i * ERR_BIT_NUM + bits];
                    G_CLR_BIT(t_err_ram, bits);

                    if (no != ERR_NULL) {
                        err_delete_cur(u, no);
                    }
                }

                /* 更新旧值 */
                err_ram_old(u, i) = err_ram(u, i);
            }
        }
    }

    /* 线控故障 */
    /* 更新故障状态的变化情况 */
    for (i = 0; i < ERR_CP_BUF_NUM; i++) {
        if (err_cp_ram(i) != err_cp_ram_old(i)) {
            err.err_update = true;
            /* 获取新增的故障 */
            t_err_ram = err_cp_ram(i) ^ err_cp_ram_old(i);
            t_err_ram &= err_cp_ram(i);

            /* 将故障保存到历史故障和当前故障中 */
            while (t_err_ram) {
                bits = g_get_setbit_no(t_err_ram);
                no = err_cp_no[i * ERR_BIT_NUM + bits];
                G_CLR_BIT(t_err_ram, bits);

                if (no != ERR_NULL) {
                    err_write_his(0, no);
                    err_insert_cur(0, no);
                }
            }

            /* 获取移除的故障 */
            t_err_ram = err_cp_ram(i) ^ err_cp_ram_old(i);
            t_err_ram &= err_cp_ram_old(i);

            /* 当前故障移除 */
            while (t_err_ram) {
                bits = g_get_setbit_no(t_err_ram);
                no = err_cp_no[i * ERR_BIT_NUM + bits];
                G_CLR_BIT(t_err_ram, bits);

                if (no != ERR_NULL) {
                    err_delete_cur(0, no);
                }
            }

            /* 更新旧值 */
            err_cp_ram_old(i) = err_cp_ram(i);
        }
    }
}

/*****************************************************************************
 *  函 数 名： err_init
 *  功能描述： 初始化故障模块
 *  输入参数： void
 *  返 回 值： void
 *****************************************************************************/
void err_init(void)
{
    memset(&err, 0, sizeof(err));
    memset(&err_ram(0, 0), 0, ERR_BUF_NUM * MACH_UNITS_NUM * 2);
    memset(&err_cp_ram(0), 0, ERR_CP_BUF_NUM * 2);
    err_clean_cur();
    nv_fdb_init();
    err_tsdb = nv_fdb_get_tsdb();
}
