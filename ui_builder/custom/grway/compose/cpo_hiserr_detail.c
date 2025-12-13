/**
 * @file cpo_hiserr_detail.c
 * @brief 故障详情控件模块
 */
#include "cpo_hiserr_detail.h"

struct cpo_hiserr_detail_ctl {
    lv_obj_t *c_hiserr_detail; /**< 历史故障详情容器 */
};

static struct cpo_hiserr_detail_ctl ctl = {
    .c_hiserr_detail = NULL,
};

/**
 * @brief 历史故障详情控件点击返回按钮事件处理
 * 
 * @param e 事件句柄
 */
static void wdg_hiserr_detail_b_return_clicked(lv_event_t *e)
{
    lv_obj_del(ctl.c_hiserr_detail);
}

/**
 * @brief 创建历史故障详情控件
 * 
 * @param perant 父控件
 * @param hiserr_node 历史故障节点
 * @return lv_obj_t* 
 */
lv_obj_t *cpo_hiserr_detail_create(lv_obj_t *perant, err_his_node_t *hiserr_node)
{
    // clang-format off
    // Init c_err_detail
    ctl.c_hiserr_detail = lv_obj_create(perant);
    lv_obj_set_pos(ctl.c_hiserr_detail, 112, 60);
    lv_obj_set_size(ctl.c_hiserr_detail, 800, 480);
    lv_obj_set_scrollbar_mode(ctl.c_hiserr_detail, LV_SCROLLBAR_MODE_OFF);

    // Set style of ctl.c_hiserr_detail
    lv_obj_set_style_bg_color(ctl.c_hiserr_detail, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ctl.c_hiserr_detail, LVGL_IMAGE_PATH(screen/s60_故障详情.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor(ctl.c_hiserr_detail, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ctl.c_hiserr_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ctl.c_hiserr_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ctl.c_hiserr_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ctl.c_hiserr_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctl.c_hiserr_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ctl.c_hiserr_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Init l_err_no
    lv_obj_t *l_err_no = lv_label_create(ctl.c_hiserr_detail);
    lv_label_set_text(l_err_no, "Label");
    lv_label_set_long_mode(l_err_no, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(l_err_no, 51, 62);
    lv_obj_set_size(l_err_no, 100, 32);

    // Set style of l_err_no
    lv_obj_set_style_text_font(l_err_no, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(l_err_no, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Init b_close
    lv_obj_t *b_close = lv_btn_create(ctl.c_hiserr_detail);
    lv_obj_t *b_close_label = lv_label_create(b_close);
    lv_label_set_text(b_close_label, aic_get_translation(he));
    lv_obj_align(b_close_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_pos(b_close, 731, 0);
    lv_obj_set_size(b_close, 69, 56);

    // Set style of b_close
    lv_obj_set_style_bg_opa(b_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(b_close, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(b_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(b_close, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // clang-format on

    /*******************************************二次开发****************************************/
    lv_obj_add_event_cb(b_close, wdg_hiserr_detail_b_return_clicked, LV_EVENT_CLICKED, NULL);
}
