#include "error_config.h"

// clang-format off
const static uint16_t err_no[] = {
//	0		1		2		3		4		5		6		7
    14,		21,		16,		ERN,	27,		5,		ERN,	1,//1
    3,		ERN,	32,		30,		13,		17,		7,		8,//2
    ERN,	ERN,	ERN,	ERN,	ERN,	ERN,	18,		19,//3
    15,		12,		62,		63,		ERN,	23,		29,		35,//4
    69,		70,		33,		28,		36,		ERN,	68,		67,//5
    ERN,	ERN,	26,		34,		ERN,	ERN,	43,		ERN,//6
    45,		44,		11,		ERN,	10,		66,		64,		65,//7
    201,	202,	203,	204,	205,	206,	207,	208, // bp_err1
    209,	210,	211,	212,	213,	214,	215,	216, // bp_err2
};
const static uint16_t err_cp_no[] = {
    ERR_COMM,ERN,	ERN,	ERN,	ERN,	ERN,	ERN,	ERN,
};
// clang-format on

/*****************************************************************************
 *  函 数 名： err_align
 *  功能描述： 故障组对齐
 *  输入参数： void
 *  返 回 值： static void
 *****************************************************************************/
gw_inline void err_align(void)
{
    // uint8_t u;

    // for (u = 0; u < hmi_param.units_num; u++) {
    //     err_ram(u, 0) = mach_ramerr1_u(u);
    //     err_ram(u, 1) = mach_ramerr2_u(u);
    //     err_ram(u, 2) = mach_ramerr3_u(u);
    //     err_ram(u, 3) = mach_ramerr4_u(u);
    //     err_ram(u, 4) = mach_ramerr5_u(u);
    //     err_ram(u, 5) = mach_ramerr6_u(u);
    //     err_ram(u, 6) = mach_ramerr7_u(u);
    //     err_ram(u, 7) = mach_bp_err1_u(u);
    //     err_ram(u, 8) = mach_bp_err2_u(u);
    // }
}
