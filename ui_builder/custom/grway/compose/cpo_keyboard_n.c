/**
 * @file cpo_keyboard_n.c
 * @brief 数字键盘控件模块
 */
#include "cpo_keyboard_n.h"

struct cpo_keyboard_n_ctl {
    lv_obj_t *c_keyboard_n;                  /**< 键盘控件 */
    lv_obj_t *l_input;                       /**< 输入框控件 */
    uint8_t   input_dot;                     /**< 小数位数  */
    void (*enter_cb)(int32_t val, bool_t x); /**< 确认和取消回调 */
};

static struct cpo_keyboard_n_ctl ctl = {
    .c_keyboard_n = NULL,
    .l_input      = NULL,
    .input_dot    = 0,
    .enter_cb     = NULL,
};

/**
 * @brief 键盘按键回调
 *
 * @param e 事件句柄
 */
static void cpo_keyboard_n_b_clicked(lv_event_t *e)
{
    static char    input_str[32];
    static uint8_t input_idx = 0;
    static int8_t  input_fx  = -1;
    static int32_t input_val;

    char *n = lv_event_get_user_data(e);
    if ((n[0] == 'j') || (n[0] == 'X')) { // 确认或取消
        if ((n[0] == 'j') && (input_idx > 0)) {
            if (input_fx == -1) {
                input_fx  = 0;
                input_val = atoi(input_str);
            } else {
                double f  = atof(input_str);
                input_val = 1;
                for (int i = 0; i < input_fx; i++) {
                    input_val *= 10;
                }
                input_val = f * input_val;
            }
            for (int i = 0; i < (ctl.input_dot - input_fx); i++) {
                input_val *= 10;
            }
            lv_log("input_val: %ld\n", input_val);

            if (ctl.enter_cb) { ctl.enter_cb(input_val, false); }
        } else {
            if (ctl.enter_cb) { ctl.enter_cb(input_val, true); }
        }
        ctl.enter_cb = NULL;
        input_fx     = -1;
        input_idx    = 0;
        memset(input_str, 0, sizeof(input_str));
        lv_obj_del(ctl.c_keyboard_n);
        ctl.c_keyboard_n = NULL;
    } else {
        if (('0' <= n[0]) && (n[0] <= '9')) { // 数字
            if ((input_str[0] == '0') && (input_fx == -1)) {
                input_idx--;
            }
            if (ctl.input_dot != input_fx) {
                if (input_fx >= 0) {
                    input_fx++;
                }
                if (input_idx < 10) {
                    input_str[input_idx++] = n[0];
                }
            }
        }
        if ((n[0] == '<') && (input_idx > 0)) { // 退回
            input_str[--input_idx] = '\0';
            if (input_fx >= 0) {
                input_fx--;
            }
        }
        if ((n[0] == '.') && (input_fx == -1) && (ctl.input_dot != 0)) { // 小数点
            input_str[input_idx++] = n[0];
            input_fx               = 0;
        }
        if (n[0] == '-') { // 负号
            // 如果第一个字符已经是负号，则移除负号
            if (input_str[0] == '-') {
                // 将所有字符向前移动一位（移除负号）
                for (int i = 0; i < input_idx; i++) {
                    input_str[i] = input_str[i + 1];
                }
                input_idx--;
            } else {
                // 如果第一个字符不是负号，则添加负号
                // 将所有字符向后移动一位
                for (int i = input_idx; i > 0; i--) {
                    input_str[i] = input_str[i - 1];
                }
                input_str[0] = '-'; // 在开头添加负号
                input_idx++;
            }
        }
        lv_label_set_text(ctl.l_input, input_str);
    }
}

typedef struct keyboard_n_ {
    char    *name; /**< 按键名称 */
    uint16_t w;    /**< 按键宽度 */
    uint16_t h;    /**< 按键高度 */
} keyboard_n_t;

/**
 * @brief 按键图表
 * w, h 按照 10000 的比例来划分
 */
const static keyboard_n_t key_map[] = {
    {"l", 8571, 2934}, // 行1
    {"-/+", 1429, 2934},
    {"1", 1429, 3534}, // 行2
    {"2", 1429, 3534},
    {"3", 1429, 3534},
    {"4", 1429, 3534},
    {"5", 1429, 3534},
    {"6", 1429, 3534},
    {"<", 1429, 3534},
    {"X", 1429, 3534}, // 行3
    {"7", 1429, 3534},
    {"8", 1429, 3534},
    {"9", 1429, 3534},
    {"0", 1429, 3534},
    {".", 1429, 3534},
    {"j", 1429, 3534},
    {NULL, 0, 0},
};

/**
 * @brief 创建键盘控件
 *
 * @param parent 父控件
 * @param fx 小数位数
 * @param cb 确认和取消回调
 * @param user_data 用户数据
 * @return lv_obj_t*
 */
lv_obj_t *cpo_keyboard_n_create(lv_obj_t *parent, uint8_t fx, void (*cb)(int32_t val, bool_t x), void *user_data)
{
    lv_coord_t w, h;
    lv_coord_t paint_x = 0;
    lv_coord_t paint_y = 0;
    lv_coord_t paint_w = 0;
    lv_coord_t paint_h = 0;

    if (ctl.c_keyboard_n != NULL) {
        return ctl.c_keyboard_n;
    }

    /*******************************************生成代码****************************************/
    // Init c_keyboard_n
    ctl.c_keyboard_n = lv_obj_create(parent);
    lv_obj_set_pos(ctl.c_keyboard_n, 112, 344);
    lv_obj_set_size(ctl.c_keyboard_n, 800, 256);
    lv_obj_set_scrollbar_mode(ctl.c_keyboard_n, LV_SCROLLBAR_MODE_OFF);

    // Set style of c_keyboard_n
    // clang-format off
    lv_obj_set_style_bg_color(ctl.c_keyboard_n, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ctl.c_keyboard_n, LVGL_IMAGE_PATH(widget/键盘.png), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor(ctl.c_keyboard_n, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ctl.c_keyboard_n, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ctl.c_keyboard_n, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ctl.c_keyboard_n, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ctl.c_keyboard_n, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctl.c_keyboard_n, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ctl.c_keyboard_n, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // clang-format on

    /*******************************************二次开发****************************************/
    lv_obj_update_layout(ctl.c_keyboard_n);
    w = lv_obj_get_width(ctl.c_keyboard_n);
    h = lv_obj_get_height(ctl.c_keyboard_n);

    // Init l_input
    ctl.l_input = lv_label_create(ctl.c_keyboard_n);
    lv_label_set_text(ctl.l_input, " ");
    lv_label_set_long_mode(ctl.l_input, LV_LABEL_LONG_WRAP);
    paint_x = 0;
    paint_y = 0;
    paint_w = w * key_map[0].w / 10000;
    paint_h = h * key_map[0].h / 10000;
    lv_obj_set_pos(ctl.l_input, paint_x, paint_y);
    lv_obj_set_size(ctl.l_input, paint_w, paint_h);

    for (int i = 1; key_map[i].name != NULL; i++) {
        lv_obj_t *b       = lv_btn_create(ctl.c_keyboard_n);
        lv_obj_t *b_label = lv_label_create(b);
        lv_label_set_text(b_label, key_map[i].name);
        lv_obj_align(b_label, LV_ALIGN_CENTER, 0, 0);
        paint_x += paint_w;
        if (paint_x > (w - 10)) {
            paint_x = 0;
            paint_y += paint_h;
        }
        paint_w = w * key_map[i].w / 10000;
        paint_h = h * key_map[i].h / 10000;
        lv_obj_set_pos(b, paint_x, paint_y);
        lv_obj_set_size(b, paint_w, paint_h);
        lv_obj_add_event_cb(b, cpo_keyboard_n_b_clicked, LV_EVENT_CLICKED, key_map[i].name);
    }

    ctl.input_dot               = fx;
    ctl.enter_cb                = cb;
    ctl.c_keyboard_n->user_data = user_data;

    return ctl.c_keyboard_n;
}
