#ifndef __AT2410_PRIV_H__
#define __AT2410_PRIV_H__

#include <stdint.h>

/*----------------------------------*
 *  基本约定
 *----------------------------------*/
#define AT2410_HEAD_TX      0x58        // 上位机→模组 帧头
#define AT2410_HEAD_RX      0x59        // 模组→上位机 帧头

/* 校验算法：2 字节累加和，低字节在前 */
#define AT2410_CALC_CHECK(ptr,len)  at2410_sum_check(ptr,len)
static inline unsigned short at2410_sum_check(uint8_t *p, uint16_t len)
{
    uint16_t sum = 0;
    for(uint16_t i = 0; i < len; i++) sum += p[i];
    return sum;
}

/*----------------------------------*
 *  3.1 基础配置命令
 *----------------------------------*/
#define AT2410_CMD_SET_LIGHT_MS         0x00    // 设置亮灯时间(ms)
#define AT2410_CMD_GET_LIGHT_MS         0x01    // 查询亮灯时间(ms)
#define AT2410_CMD_SET_LIGHT_S          0x04    // 设置亮灯时间(s,不推荐)
#define AT2410_CMD_GET_LIGHT_S          0x05    // 查询亮灯时间(s)
#define AT2410_CMD_SET_PHOTOSW          0x0C    // 光敏开关
#define AT2410_CMD_GET_PHOTOSW          0x0D    // 查询光敏开关
#define AT2410_CMD_SET_PHOTOTH          0x06    // 光敏阈值(mV)
#define AT2410_CMD_GET_PHOTOTH          0x07    // 查询光敏阈值
#define AT2410_CMD_SAVE_PARAM           0x08    // 保存/恢复参数
#define AT2410_CMD_GET_SAVE_STA         0x09    // 查询保存状态
#define AT2410_CMD_SET_OUT_LVL          0x0A    // 控制 OUT 引脚电平
#define AT2410_CMD_SOFT_RESET           0x13    // 软复位模组
#define AT2410_CMD_GET_VERSION          0xFE    // 查询软硬件版本

/*----------------------------------*
 *  3.2 雷达配置命令
 *----------------------------------*/
#define AT2410_CMD_RADAR_EN             0xD1    // 雷达功能开关
#define AT2410_CMD_RADAR_EN_Q           0xD0    // 查询雷达开关
#define AT2410_CMD_TARGET_INFO          0x30    // 查询感应信息
#define AT2410_CMD_ALGO_TYPE            0x31    // 查询算法类型
#define AT2410_CMD_DET_BOUND            0x32    // 算法边界值
#define AT2410_CMD_DET_CFG              0x33    // 算法感应配置
#define AT2410_CMD_SET_MV_DIST_MAX      0xD2    // 运动检测最大距离
#define AT2410_CMD_SET_MV_DIST_MIN      0x34    // 运动检测最小距离
#define AT2410_CMD_SET_MV_SENS          0x35    // 运动检测灵敏度
#define AT2410_CMD_SET_ST_DIST_MAX      0x36    // 静态检测最大距离
#define AT2410_CMD_SET_ST_DIST_MIN      0x37    // 静态检测最小距离
#define AT2410_CMD_SET_ST_SENS          0x38    // 静态检测灵敏度
#define AT2410_CMD_SET_BR_DIST_MAX      0x39    // 存在检测最大距离
#define AT2410_CMD_SET_BR_DIST_MIN      0x3A    // 存在检测最小距离
#define AT2410_CMD_SET_BR_SENS          0x3B    // 存在检测灵敏度
/* 查询类 */
#define AT2410_CMD_GET_MV_DIST_MAX      0x40
#define AT2410_CMD_GET_MV_DIST_MIN      0x41
#define AT2410_CMD_GET_MV_SENS          0x42
#define AT2410_CMD_GET_BR_DIST_MAX      0x43
#define AT2410_CMD_GET_BR_DIST_MIN      0x44
#define AT2410_CMD_GET_BR_SENS          0x45
/* FMCW */
#define AT2410_CMD_SET_FRAME_PRD        0x50    // 设置帧周期(ms)
#define AT2410_CMD_GET_FRAME_PRD        0x51    // 查询帧周期

/*----------------------------------*
 *  3.3 环境自学习
 *----------------------------------*/
#define AT2410_CMD_LEARN_CTRL           0x80    // 启动/取消自学习
#define AT2410_CMD_GET_MV_LEARN_STA     0x81    // 运动学习状态
#define AT2410_CMD_GET_BR_LEARN_STA     0x82    // 存在学习状态
#define AT2410_CMD_GET_ST_LEARN_STA     0x83    // 静态学习状态
#define AT2410_CMD_GET_MV_NOISE         0x84    // 运动底噪值
#define AT2410_CMD_GET_BR_NOISE         0x86
#define AT2410_CMD_GET_ST_NOISE         0x88
#define AT2410_CMD_SET_MV_NOISE         0x8A    // 设置运动底噪
#define AT2410_CMD_SET_BR_NOISE         0x8C    // 设置存在底噪
#define AT2410_CMD_SET_ST_NOISE         0x8E    // 设置静态底噪
#define AT2410_CMD_GET_MV_NOISE_Q       0x8B
#define AT2410_CMD_GET_BR_NOISE_Q       0x8D
#define AT2410_CMD_GET_ST_NOISE_Q       0x8F

/*----------------------------------*
 *  3.4 调试/自动上报
 *----------------------------------*/
#define AT2410_CMD_AUTO_RESULT          0x53    // 算法结果自动上报开关
#define AT2410_CMD_FFT_CTRL             0x55    // 上报 Range-FFT
#define AT2410_CMD_MV_SIG_CTRL          0x57    // 运动信号
#define AT2410_CMD_BR_SIG_CTRL          0x59    // 存在信号
#define AT2410_CMD_ST_SIG_CTRL          0x5B    // 静态信号

/* 模组主动上报的命令码 */
#define AT2410_CMD_REPORT_RESULT        0x52    // 自动上报算法结果
#define AT2410_CMD_REPORT_FFT           0x54
#define AT2410_CMD_REPORT_MV_SIG        0x56
#define AT2410_CMD_REPORT_BR_SIG        0x58
#define AT2410_CMD_REPORT_ST_SIG        0x5A

/*----------------------------------*
 *  常用结构体（与文档 3.2.4/3.4 对应）
 *----------------------------------*/
typedef struct {
    uint16_t range_cm;
    int16_t  angle;
    uint16_t mag;
    uint16_t snr;
} at2410_target_info_t;

typedef struct {
    uint32_t frame_id;
    uint16_t is_detected;      // bit0:运动 bit1:存在 bit2:静态
    uint8_t  mv_max_tgt;
    uint8_t  mv_tgt_cnt;
    at2410_target_info_t mv_info[1];  // 动态数组，实际按 mv_tgt_cnt 决定
    uint8_t  br_max_tgt;
    uint8_t  br_tgt_cnt;
    at2410_target_info_t br_info[1];
    uint8_t  st_max_tgt;
    uint8_t  st_tgt_cnt;
    at2410_target_info_t st_info[1];
} at2410_algo_result_t;

#endif /* __AT2410_PRIV_H__ */