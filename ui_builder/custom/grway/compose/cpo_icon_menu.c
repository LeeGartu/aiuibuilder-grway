/**
 * @file cpo_icon_menu.c
 * @brief 图表菜单控件模块
 */
#include "cpo_icon_menu.h"

/**
 * @brief 创建图表菜单空间
 * 
 * @param parent 父控件
 * @param menu_tab 菜单描述表
 * @return lv_obj_t* 
 */
lv_obj_t *cpo_icon_menu_create(lv_obj_t *parent, cpo_icon_menu_desc_t *menu_tab)
{
#define MENU_COL_NUM    (4)
#define MENU_ROW_NUM    (2)
#define MENU_MAX_NUM    (MENU_ROW_NUM *MENU_COL_NUM)
#define C_ICON_POS_X(x) (75 + x * (C_ICON_WIDTH + 5))
#define C_ICON_POS_Y(y) (25 + y * (C_ICON_HEIGHT + 5))
#define C_ICON_WIDTH    (150)
#define C_ICON_HEIGHT   (145)

#define L_ICON_POS_X  (4)
#define L_ICON_POS_Y  (103)
#define L_ICON_WIDTH  (141)
#define L_ICON_HEIGHT (42)

#define I_ICON_POS_X (25)
#define I_ICON_POS_Y (0)

    int       i;
    int       x, y;
    lv_obj_t *tv_menu      = lv_tileview_create(parent);
    lv_obj_t *tv_menu_tile = lv_tileview_add_tile(tv_menu, 0, 0, LV_DIR_ALL);
    lv_obj_set_pos(tv_menu, 122, 114);
    lv_obj_set_size(tv_menu, 780, 323);
    lv_obj_set_scrollbar_mode(tv_menu, LV_SCROLLBAR_MODE_ON);

    // Set style of tv_menu
    lv_obj_set_style_bg_color(tv_menu, lv_color_hex(0xc5c5c5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tv_menu, 35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tv_menu, lv_color_hex(0xc5c5c5), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    for (i = 0, x = 0, y = 0; menu_tab[i].event_cb != NULL; i++, x++) {
        if (x >= MENU_COL_NUM) {
            x = 0;
            y++;
            if (y >= MENU_ROW_NUM) {
                y            = 0;
                tv_menu_tile = lv_tileview_add_tile(tv_menu, i / MENU_MAX_NUM, 0, LV_DIR_ALL);
            }
        }

        /*******************************************生成代码****************************************/
        // Init c_icon
        lv_obj_t *c_icon = lv_obj_create(tv_menu_tile);
        // lv_obj_set_pos(c_icon, 75, 25);
        // lv_obj_set_size(c_icon, 150, 145);
        lv_obj_set_scrollbar_mode(c_icon, LV_SCROLLBAR_MODE_OFF);

        // Set style of c_icon
        lv_obj_set_style_bg_color(c_icon, lv_color_hex(0xeff8f5), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(c_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(c_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(c_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(c_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(c_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(c_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(c_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Set event handler of c_icon
        // lv_obj_add_event_cb(c_icon, menu_tab[i].event_cb, LV_EVENT_CLICKED, NULL);

        // Init l_icon
        lv_obj_t *l_icon = lv_label_create(c_icon);
        // lv_label_set_text(l_icon, "Label");
        lv_label_set_long_mode(l_icon, LV_LABEL_LONG_WRAP);
        // lv_obj_set_pos(l_icon, 4, 103);
        // lv_obj_set_size(l_icon, 141, 42);

        // Set style of l_icon
        lv_obj_set_style_text_font(l_icon, aic_get_multi_lang_font(16), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(l_icon, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Init i_icon
        lv_obj_t *i_icon = lv_img_create(c_icon);
        // lv_img_set_src(i_icon, LVGL_IMAGE_PATH(menu/回路1.png));
        lv_img_set_pivot(i_icon, 50, 50);
        lv_img_set_angle(i_icon, 0);
        lv_obj_set_style_img_opa(i_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_pos(i_icon, 25, 0);

        /*******************************************二次开发****************************************/
        lv_obj_set_pos(c_icon, C_ICON_POS_X(x), C_ICON_POS_Y(y));
        lv_obj_set_size(c_icon, C_ICON_WIDTH, C_ICON_HEIGHT);
        lv_obj_add_event_cb(c_icon, menu_tab[i].event_cb, LV_EVENT_CLICKED, NULL);

        lv_label_set_text(l_icon, aic_get_translation(menu_tab[i].text));
        lv_obj_set_pos(l_icon, L_ICON_POS_X, L_ICON_POS_Y);
        lv_obj_set_size(l_icon, L_ICON_WIDTH, L_ICON_HEIGHT);

        lv_img_set_src(i_icon, menu_tab[i].image_path);
        lv_obj_set_pos(i_icon, I_ICON_POS_X, I_ICON_POS_Y);
    }

    return tv_menu;
}