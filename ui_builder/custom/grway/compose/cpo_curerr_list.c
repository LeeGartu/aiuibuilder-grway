/**
 * @file cpo_curerr_list.c
 * @brief 当前故障控件模块
 */
#include "cpo_curerr_list.h"
#include "../mem/err.h"

struct cpo_curerr_list_ctl {
    lv_obj_t   *c_curerr_list; /**< 当前故障容器 */
    lv_timer_t *timer;         /**< 当前故障定时器 */
};

static struct cpo_curerr_list_ctl ctl = {
    .c_curerr_list = NULL,
    .timer         = NULL,
};

/**
 * @brief 绘制当前故障列表
 *
 * @param c_curerr_list 当前故障列表容器
 */
void cpo_curerr_list_paint(lv_obj_t *c_curerr_list)
{
#define C_ITEM_POS_X    (9)
#define C_ITEM_POS_Y(i) (7 + i * (C_ITEM_HEIGHT + 10))
#define C_ITEM_WIDTH    (637)
#define C_ITEM_HEIGHT   (44)

#define L_NO_POS_X  (27)
#define L_NO_POS_Y  (9)
#define L_NO_WIDTH  (229)
#define L_NO_HEIGHT (LV_SIZE_CONTENT)

#define L_ERR_POS_X  (279)
#define L_ERR_POS_Y  (7)
#define L_ERR_WIDTH  (341)
#define L_ERR_HEIGHT (LV_SIZE_CONTENT)

    int i = 0;

    for (i = 0; err_current[i].no != ERN; i++) {
        // Init c_curerr_item
        lv_obj_t *c_curerr_item = lv_obj_create(c_curerr_list);
        // lv_obj_set_pos(c_curerr_item, 9, 7);
        // lv_obj_set_size(c_curerr_item, 637, 44);
        lv_obj_set_scrollbar_mode(c_curerr_item, LV_SCROLLBAR_MODE_OFF);

        // Set style of c_curerr_item
        lv_obj_set_style_bg_color(c_curerr_item, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(c_curerr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Init l_curno
        lv_obj_t *l_curno = lv_label_create(c_curerr_item);
        // lv_label_set_text(l_curno, "Label");
        lv_label_set_long_mode(l_curno, LV_LABEL_LONG_WRAP);
        // lv_obj_set_pos(l_curno, 27, 9);
        // lv_obj_set_size(l_curno, 229, 26);

        // Set style of l_curno
        lv_obj_set_style_text_font(l_curno, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(l_curno, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

        // Init l_curerr
        lv_obj_t *l_curerr = lv_label_create(c_curerr_item);
        // lv_label_set_text(l_curerr, "Label");
        lv_label_set_long_mode(l_curerr, LV_LABEL_LONG_WRAP);
        // lv_obj_set_pos(l_curerr, 279, 7);
        // lv_obj_set_size(l_curerr, 341, 30);

        // Set style of l_curerr
        lv_obj_set_style_text_font(l_curerr, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(l_curerr, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

        /*******************************************二次开发****************************************/
        lv_obj_set_pos(c_curerr_item, C_ITEM_POS_X, C_ITEM_POS_Y(i));
        lv_obj_set_size(c_curerr_item, C_ITEM_WIDTH, C_ITEM_HEIGHT);

        lv_label_set_text_fmt(l_curno, "%d", i + 1);
        lv_obj_set_pos(l_curno, L_NO_POS_X, L_NO_POS_Y);
        lv_obj_set_size(l_curno, L_NO_WIDTH, L_NO_HEIGHT);

        lv_label_set_text_fmt(l_curerr, "error %d", err_current[i].no);
        lv_obj_set_pos(l_curerr, L_ERR_POS_X, L_ERR_POS_Y);
        lv_obj_set_size(l_curerr, L_ERR_WIDTH, L_ERR_HEIGHT);
    }
}

/**
 * @brief 定时检测是否需要更新故障显示
 *
 * @param timer 当前故障定时器
 */
static void cpo_curerr_list_timer_cb(lv_timer_t *timer)
{
    if (err_is_update()) {
        lv_obj_clean(ctl.c_curerr_list);
        cpo_curerr_list_paint(ctl.c_curerr_list);
    }
}

/**
 * @brief 创建当前故障列表
 *
 * @param perant 父控件
 * @return lv_obj_t*
 */
lv_obj_t *cpo_curerr_list_create(lv_obj_t *perant)
{
    // Init c_curerr_list
    ctl.c_curerr_list = lv_obj_create(perant);
    lv_obj_set_pos(ctl.c_curerr_list, 179, 203);
    lv_obj_set_size(ctl.c_curerr_list, 663, 270);
    lv_obj_set_scrollbar_mode(ctl.c_curerr_list, LV_SCROLLBAR_MODE_AUTO);

    // Set style of c_curerr_list
    lv_obj_set_style_bg_color(ctl.c_curerr_list, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ctl.c_curerr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ctl.c_curerr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ctl.c_curerr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ctl.c_curerr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ctl.c_curerr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctl.c_curerr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ctl.c_curerr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*******************************************二次开发****************************************/
    cpo_curerr_list_paint(ctl.c_curerr_list);
    if (ctl.timer == NULL) {
        ctl.timer = lv_timer_create(cpo_curerr_list_timer_cb, 200, NULL);
    }

    return ctl.c_curerr_list;
}
