/**
 * @file cpo_hiserr_detail.h
 * @brief 故障详情控件模块
 */
#ifndef __CPO_HISERR_DETAIL_H__
#define __CPO_HISERR_DETAIL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../screen/scr.h"
#include "../mem/err.h"

lv_obj_t *cpo_hiserr_detail_create(lv_obj_t *perant, err_his_node_t *hiserr_node);

#ifdef __cplusplus
}
#endif
#endif /* __CPO_HISERR_DETAIL_H__ */
