#include "../screen/scr.h"

const screen_node_t screen_info[] = {
    {screen_0_fulldisp_create, &(ui_manager.screen_0_fulldisp.obj)},
    {screen_1_main_create, &(ui_manager.screen_1_main.obj)},
    {screen_2_menu_create, &(ui_manager.screen_2_menu.obj)},
    {screen_3_status_create, &(ui_manager.screen_3_status.obj)},
    {screen_4_param_create, &(ui_manager.screen_4_param.obj)},
    {screen_5_keyboard_n_create, &(ui_manager.screen_5_keyboard_n.obj)},
    {screen_6_curerr_create, &(ui_manager.screen_6_curerr.obj)},
    {screen_7_hiserr_create, &(ui_manager.screen_7_hiserr.obj)},
    {screen_8_hiserr_detail_create, &(ui_manager.screen_8_hiserr_detail.obj)},
    {NULL, NULL}};
