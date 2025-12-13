/**
 * @file cpo_icon_menu.h
 * @brief 图表菜单控件模块
 */
#ifndef __CPO_ICON_MENU_H__
#define __CPO_ICON_MENU_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../screen/scr.h"

typedef struct cpo_icon_menu_desc_ {
    lv_event_cb_t event_cb;
    char *image_path;
    uint16_t text;
} cpo_icon_menu_desc_t;

lv_obj_t *cpo_icon_menu_create(lv_obj_t *parent, cpo_icon_menu_desc_t *menu_tab);

#ifdef __cplusplus
}
#endif
#endif /* __CPO_ICON_MENU_H__ */
