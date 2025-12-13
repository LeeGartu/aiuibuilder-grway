/**
 * @file cpo_keyboard_n.h
 * @brief 数字键盘控件模块
 */
#ifndef __CPO_KEYBOARD_N_H__
#define __CPO_KEYBOARD_N_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../screen/scr.h"
#include "../mem/mem_item.h"

lv_obj_t *cpo_keyboard_n_create(lv_obj_t *parent, uint8_t fx, void (*cb)(int32_t val, bool_t x), void *user_data);

#ifdef __cplusplus
}
#endif
#endif /* __CPO_KEYBOARD_N_H__ */
