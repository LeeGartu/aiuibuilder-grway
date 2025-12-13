/**
 * @file mem_param.c
 * @brief 参数表配置文件
 *
 * @defgroup param_tab 参数表
 */
#include "mem_param.h"

/**
 * @addtogroup param_tab 参数表
 * @{
 */
// clang-format off
const mem_range_t param_tab_user[] = {
//最小	默认	最大	单位	属性类型	属性	变量类型	变量指针
{1,		11,		11,		{U_C,	ATYP_INT,	A_I1,	TYP_S16,	&param_stepmin_auto0}},
{-22,	22,		22,		{U_C,	ATYP_FLOAT,	A_F01,	TYP_S16,	&param_stepmin_auto1}},
{0,		1,		1,		{U_NUL,	ATYP_BOOL,	A_B0,	TYP_S16,	&param_stepmin_auto2}},
{3,		44,		44,		{U_NUL,	ATYP_ENUM,	A_ENUM,	TYP_S16,	&param_stepmin_auto3}},
{0,		1,		1,		{U_NUL,	ATYP_BOOL,	A_B0,	TYP_S16,	&param_stepmin_auto2}},
{3,		44,		44,		{U_NUL,	ATYP_ENUM,	A_ENUM,	TYP_S16,	&param_stepmin_auto3}},
// END
{0,		0,		0,		{U_NUL,	ATYP_INT,	A_I1,	TYP_U8,		NULL}},
};

const mem_range_t *param_tab_list[] = {
param_tab_user,
NULL
};
// clang-format on
/** @} */
