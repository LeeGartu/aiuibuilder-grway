/**
 * @file cpo_hiserr_list.c
 * @brief 历史故障控件模块
 */
#include "cpo_hiserr_list.h"
#include "cpo_hiserr_detail.h"
#include "../mem/err.h"

struct cpo_err_list_ctl {
    lv_obj_t   *perant;
    lv_obj_t   *c_hiserr_list;      /**< 历史故障容器 */
    lv_timer_t *timer;              /**< 历史故障定时器 */
    lv_ll_t     hiserr_detail_list; /**< 历史故障详情列表 */
    int         hiserr_idx;         /**< 历史故障索引 */
};

static struct cpo_err_list_ctl ctl = {
    .c_hiserr_list      = NULL,
    .timer              = NULL,
    .hiserr_detail_list = {0, NULL, NULL},
    .hiserr_idx         = 0,
};

/**
 * @brief 历史故障详情按钮点击事件处理
 *
 * @param e 事件句柄
 */
static void cpo_hiserr_list_b_detail_clicked(lv_event_t *e)
{
    err_his_node_t *hiserr_node = lv_event_get_user_data(e);
    cpo_hiserr_detail_create(ctl.perant, hiserr_node);
}

/**
 * @brief 绘制历史故障列表
 *
 * @param c_hiserr_list 历史故障列表容器
 */
static void cpo_hiserr_list_cb(err_his_node_t node)
{
#define C_HITEM_POS_X    (9)
#define C_HITEM_POS_Y(i) (7 + i * (C_HITEM_HEIGHT + 10))
#define C_HITEM_WIDTH    (637)
#define C_HITEM_HEIGHT   (44)

#define L_HNO_POS_X  (27)
#define L_HNO_POS_Y  (9)
#define L_HNO_WIDTH  (229)
#define L_HNO_HEIGHT (LV_SIZE_CONTENT)

#define L_HERR_POS_X  (455)
#define L_HERR_POS_Y  (1)
#define L_HERR_WIDTH  (87)
#define L_HERR_HEIGHT (43)

    // Init c_hiserr_item
    lv_obj_t *c_hiserr_item = lv_obj_create(ctl.c_hiserr_list);
    // lv_obj_set_pos(c_hiserr_item, 9, 7);
    // lv_obj_set_size(c_hiserr_item, 637, 44);
    lv_obj_set_scrollbar_mode(c_hiserr_item, LV_SCROLLBAR_MODE_OFF);

    // Set style of c_hiserr_item
    lv_obj_set_style_bg_color(c_hiserr_item, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(c_hiserr_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Init l_time
    lv_obj_t *l_time = lv_label_create(c_hiserr_item);
    // lv_label_set_text(l_time, "Label");
    lv_label_set_long_mode(l_time, LV_LABEL_LONG_WRAP);
    // lv_obj_set_pos(l_time, 27, 9);
    // lv_obj_set_size(l_time, 229, 26);

    // Set style of l_time
    lv_obj_set_style_text_font(l_time, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(l_time, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Init b_detail
    lv_obj_t *b_detail       = lv_btn_create(c_hiserr_item);
    lv_obj_t *b_detail_label = lv_label_create(b_detail);
    // lv_label_set_text(b_detail_label, "Button");
    lv_obj_align(b_detail_label, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_pos(b_detail, 455, 1);
    // lv_obj_set_size(b_detail, 87, 43);

    // Set style of b_detail
    lv_obj_set_style_text_font(b_detail, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(b_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(b_detail, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*******************************************二次开发****************************************/
    err_his_node_t *hiserr_node = _lv_ll_ins_head(&(ctl.hiserr_detail_list));
    if (hiserr_node) {
        hiserr_node->no      = node.no;
        hiserr_node->unit_no = node.unit_no;
        hiserr_node->wttemp  = node.wttemp;
    }
    // Init c_hiserr_item
    lv_obj_set_pos(c_hiserr_item, C_HITEM_POS_X, C_HITEM_POS_Y(ctl.hiserr_idx));
    lv_obj_set_size(c_hiserr_item, C_HITEM_WIDTH, C_HITEM_HEIGHT);

    // Init l_time
    lv_label_set_text_fmt(l_time, "no:%d", node.no);
    lv_obj_set_pos(l_time, L_HNO_POS_X, L_HNO_POS_Y);
    lv_obj_set_size(l_time, L_HNO_WIDTH, L_HNO_HEIGHT);

    // Init b_detail
    lv_obj_clear_flag(b_detail, LV_OBJ_FLAG_SCROLL_ON_FOCUS); // 去掉“焦点滚入”行为
    lv_label_set_text(b_detail_label, "Button");
    lv_obj_set_pos(b_detail, L_HERR_POS_X, L_HERR_POS_Y);
    lv_obj_set_size(b_detail, L_HERR_WIDTH, L_HERR_HEIGHT);
    lv_obj_add_event_cb(b_detail, cpo_hiserr_list_b_detail_clicked, LV_EVENT_CLICKED, hiserr_node);
    ctl.hiserr_idx++;
}

/**
 * @brief 定时检测是否需要更新故障显示
 *
 * @param timer 历史故障定时器
 */
static void cpo_hiserr_list_timer_cb(lv_timer_t *timer)
{
    if (err_is_update()) {
        lv_obj_clean(ctl.c_hiserr_list);
        _lv_ll_clear_custom(&(ctl.hiserr_detail_list), NULL);
        err_read_his(&cpo_hiserr_list_cb);
    }
}

/**
 * @brief 创建当前故障列表
 *
 * @param perant 父控件
 * @return lv_obj_t*
 */
lv_obj_t *cpo_hiserr_list_create(lv_obj_t *perant)
{
    // Init c_hiserr_list
    ctl.c_hiserr_list = lv_obj_create(perant);
    lv_obj_set_pos(ctl.c_hiserr_list, 179, 203);
    lv_obj_set_size(ctl.c_hiserr_list, 663, 270);
    lv_obj_set_scrollbar_mode(ctl.c_hiserr_list, LV_SCROLLBAR_MODE_AUTO);

    // Set style of ctl.c_hiserr_list
    lv_obj_set_style_bg_color(ctl.c_hiserr_list, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ctl.c_hiserr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ctl.c_hiserr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ctl.c_hiserr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ctl.c_hiserr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ctl.c_hiserr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctl.c_hiserr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ctl.c_hiserr_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*******************************************二次开发****************************************/
    ctl.perant     = perant;
    ctl.hiserr_idx = 0;
    /* 初始化历史故障详情链表 */
    if (ctl.hiserr_detail_list.head == NULL) {
        _lv_ll_init(&(ctl.hiserr_detail_list), sizeof(err_his_node_t));
    } else {
        _lv_ll_clear_custom(&(ctl.hiserr_detail_list), NULL);
    }

    /* 绘制历史故障 */
    err_read_his(&cpo_hiserr_list_cb);
    
    if (ctl.timer == NULL) {
        ctl.timer = lv_timer_create(cpo_hiserr_list_timer_cb, 200, NULL);
    }

    return ctl.c_hiserr_list;
}
