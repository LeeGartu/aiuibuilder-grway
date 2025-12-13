/**
 * @file cpo_item_list.c
 * @brief 参数控件模块
 */
#include "cpo_item_list.h"
#include "cpo_keyboard_n.h"

struct cpo_item_list_ctl_ {
    lv_obj_t   *parent;            /**< 父对象 */
    lv_obj_t   *c_keyboard_n;      /**< 键盘控件 */
    lv_ll_t     b_item_label_list; /**< 列表项标签链表 */
    lv_timer_t *timer;             /**< 刷新定时器 */
    size_t      offset_u8;         /**< 列表项偏移 */
};

struct cpo_item_list_ctl_ ctl = {
    .parent            = NULL,
    .c_keyboard_n      = NULL,
    .b_item_label_list = {0, NULL, NULL},
    .timer             = NULL,
    .offset_u8         = 0,
};

/**
 * @brief 获取状态参数项内存信息
 *
 * @param list 状态参数项列表
 * @param i 状态参数项索引
 * @return gw_inline const*
 */
gw_inline const mem_t *cpo_item_list_get_mem(cpo_item_list_desc_t *list, int i)
{
    if (list->mem_range_tab != NULL) {
        return &list->mem_range_tab[i].mem;
    } else {
        return &list->mem_tab[i];
    }
}

/**
 * @brief 获取状态参数项数值
 *
 * @param list 状态参数项列表
 * @param i 状态参数项索引
 * @return gw_inline
 */
gw_inline int32_t cpo_item_list_get_value(cpo_item_list_desc_t *list, int i)
{
    if (list->mem_range_tab != NULL) {
        return param_get_value_offset(&list->mem_range_tab[i], ctl.offset_u8);
    } else {
        return status_get_value_offset(&list->mem_tab[i], ctl.offset_u8);
    }
}

/**
 * @brief 显示状态参项数值
 *
 * @param b_item_label 显示的控件
 * @param list 状态参数项列表
 * @param i 状态参数项索引
 * @return gw_inline
 */
gw_inline void cpo_item_list_b_item_label(lv_obj_t *b_item_label, cpo_item_list_desc_t *list, int i)
{
    char         text[64];
    uint8_t      attr;
    int32_t      value = cpo_item_list_get_value(list, i);
    const mem_t *mem   = cpo_item_list_get_mem(list, i);

    switch (mem->atype) {
    case ATYP_INT:
        g_ftoa(value, text, 0);
        break;
    case ATYP_FLOAT:
        if (mem->attr != 0) {
            attr = mem->attr;
        } else {
            attr = 1;
        }
        g_ftoa(value, text, attr);
        break;
    case ATYP_BOOL:
        if (value) {
            strcpy(text, "true");
        } else {
            strcpy(text, "false");
        }
        break;
    case ATYP_ENUM:
        sprintf(text, "e: %ld", value);
        break;
    default:
        sprintf(text, "error");
        break;
    }

    lv_label_set_text(b_item_label, text);
}

/**
 * @brief 状态参数项数值定时刷新
 *
 * @param timer 刷新定时器
 */
static void cpo_item_list_timer_cb(lv_timer_t *timer)
{
    cpo_item_list_desc_t *item_list = lv_timer_get_user_data(timer);
    lv_obj_t            **b_item_label;
    int                   i = 0;

    _LV_LL_READ(&(ctl.b_item_label_list), b_item_label)
    {
        cpo_item_list_b_item_label(*b_item_label, item_list, i);
        i++;
    }
}

/**
 * @brief 设置状态参数项
 *
 * @param val 物理值
 * @param x 是否执行设置（用于键盘x返回）
 */
void cpo_item_list_set_val(int32_t val, bool_t x)
{
    if (!x) {
        const mem_range_t *mem_range = lv_obj_get_user_data(ctl.c_keyboard_n);
        param_set_value_offset(mem_range, val, ctl.offset_u8);
    }
    ctl.c_keyboard_n = NULL;
}

/**
 * @brief 状态参数项点击事件处理
 *
 * @param e
 */
static void cpo_item_list_b_item_clicked(lv_event_t *e)
{
    if (ctl.c_keyboard_n) {
        return;
    }

    const mem_range_t *mem_range = lv_event_get_user_data(e);
    int32_t            var       = param_get_value(mem_range);
    int32_t            min       = param_get_min(mem_range);
    int32_t            max       = param_get_max(mem_range);
    uint8_t            fx        = 0;

    switch (mem_range->mem.atype) {
    case ATYP_FLOAT:
        fx = 1;
        if (mem_range->mem.attr > 0) {
            fx = mem_range->mem.attr;
        }
    case ATYP_INT:
        ctl.c_keyboard_n = cpo_keyboard_n_create(ctl.parent, fx, cpo_item_list_set_val, (void *)mem_range);
        break;
    case ATYP_BOOL:
        min = 0;
        max = 1;
    case ATYP_ENUM:
        var++;
        if (var > max) {
            var = min;
        }
        param_set_value_offset(mem_range, var, ctl.offset_u8);
        break;
    default:
        break;
    }
}

/**
 * @brief 销毁状态参数项列表控件
 *
 * @return void
 */
void cpo_item_list_destroy(lv_event_t * e)
{
    _lv_ll_clear_custom(&(ctl.b_item_label_list), NULL);
    lv_timer_del(ctl.timer);
    
    ctl.parent            = NULL;
    ctl.c_keyboard_n      = NULL;
    ctl.timer             = NULL;
    ctl.offset_u8         = 0;
}

/**
 * @brief 创建状态参数项列表控件
 *
 * @param parent 父控件
 * @param item_list 项描述列表
 * @param dev_addr 机组编号
 * @return lv_obj_t*
 */
lv_obj_t *cpo_item_list_create(lv_obj_t *parent, cpo_item_list_desc_t *item_list, int32_t dev_addr)
{
#define P_ITEM_POS_X    (9)
#define P_ITEM_POS_Y(i) (7 + i * (P_ITEM_HEIGHT + 10))
#define P_ITEM_WIDTH    (762)
#define P_ITEM_HEIGHT   (77)

#define PL_ITEM_POS_X  (0)
#define PL_ITEM_POS_Y  (0)
#define PL_ITEM_WIDTH  (194)
#define PL_ITEM_HEIGHT (LV_SIZE_CONTENT)

#define PB_ITEM_POS_X  (643)
#define PB_ITEM_POS_Y  (15)
#define PB_ITEM_WIDTH  (87)
#define PB_ITEM_HEIGHT (43)
    int          i   = 0;
    const mem_t *mem = cpo_item_list_get_mem(item_list, i);
    ctl.parent       = parent;
    ctl.offset_u8    = dev_addr * item_list->offset;

    /* 初始化状态值控件链表 */
    if (ctl.b_item_label_list.head == NULL) {
        _lv_ll_init(&(ctl.b_item_label_list), sizeof(lv_obj_t *));
        /* 清空链表 */
    } else {
        _lv_ll_clear_custom(&(ctl.b_item_label_list), NULL);
    }

    // Init c_param
    lv_obj_t *c_param = lv_obj_create(parent);
    lv_obj_set_pos(c_param, 123, 113);
    lv_obj_set_size(c_param, 775, 327);
    lv_obj_set_scrollbar_mode(c_param, LV_SCROLLBAR_MODE_AUTO);

    // Set style of c_param
    lv_obj_set_style_bg_color(c_param, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(c_param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(c_param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(c_param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(c_param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(c_param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(c_param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(c_param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    for (i = 0; mem->pvar != NULL;) {
        /*******************************************生成代码****************************************/
        // Init param_item
        lv_obj_t *param_item = lv_obj_create(c_param);
        // lv_obj_set_pos(param_item, 9, 7);
        // lv_obj_set_size(param_item, 762, 77);
        lv_obj_set_scrollbar_mode(param_item, LV_SCROLLBAR_MODE_OFF);

        // Set style of param_item
        lv_obj_set_style_bg_color(param_item, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(param_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(param_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(param_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(param_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(param_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(param_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Init l_item
        lv_obj_t *l_item = lv_label_create(param_item);
        // lv_label_set_text(l_item, "Label");
        lv_label_set_long_mode(l_item, LV_LABEL_LONG_WRAP);
        // lv_obj_set_pos(l_item, 0, 0);

        // Set style of l_item
        lv_obj_set_style_text_font(l_item, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(l_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Init b_item
        lv_obj_t *b_item       = lv_btn_create(param_item);
        lv_obj_t *b_item_label = lv_label_create(b_item);
        // lv_label_set_text(b_item_label, "Button");
        lv_obj_align(b_item_label, LV_ALIGN_CENTER, 0, 0);
        // lv_obj_set_pos(b_item, 643, 15);
        // lv_obj_set_size(b_item, 87, 43);

        // Set style of b_item
        lv_obj_set_style_text_font(b_item, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(b_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(b_item, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        /*******************************************二次开发****************************************/
        lv_obj_set_pos(param_item, P_ITEM_POS_X, P_ITEM_POS_Y(i));
        lv_obj_set_size(param_item, P_ITEM_WIDTH, P_ITEM_HEIGHT);

        lv_obj_set_pos(l_item, PL_ITEM_POS_X, PL_ITEM_POS_Y);
        lv_obj_set_size(l_item, PL_ITEM_WIDTH, PL_ITEM_HEIGHT);
        lv_obj_set_align(l_item, LV_ALIGN_LEFT_MID);
        lv_label_set_text(l_item, aic_get_translation(item_list->name_idx + i));

        lv_obj_clear_flag(b_item, LV_OBJ_FLAG_SCROLL_ON_FOCUS); // 去掉“焦点滚入”行为
        lv_obj_set_pos(b_item, PB_ITEM_POS_X, PB_ITEM_POS_Y);
        lv_obj_set_size(b_item, PB_ITEM_WIDTH, PB_ITEM_HEIGHT);

        lv_obj_t **new_node = _lv_ll_ins_tail(&(ctl.b_item_label_list));
        if (new_node) {
            *new_node = b_item_label;
        }

        if (item_list->mem_range_tab != NULL) {
            lv_obj_add_event_cb(b_item, cpo_item_list_b_item_clicked, LV_EVENT_CLICKED, (void *)&(item_list->mem_range_tab[i]));
        }
        i++;
        mem = cpo_item_list_get_mem(item_list, i);
    }
    if (ctl.timer == NULL) {
        ctl.timer = lv_timer_create(cpo_item_list_timer_cb, 200, item_list);
    }
    cpo_item_list_timer_cb(ctl.timer);
    // 添加销毁回调
    lv_obj_add_event_cb(c_param, cpo_item_list_destroy, LV_EVENT_DELETE, NULL);

    return c_param;
}
