#ifndef __GW_SCR_H__
#define __GW_SCR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/gw_utils.h"
#include "../config/scr_config.h"

// 屏幕节点结构体定义
typedef struct {
    void (*create_func)(ui_manager_t *ui);
    lv_obj_t **scr_ptr;
} screen_node_t;

/**
 * @brief 初始化屏幕管理链表
 */
void scr_init(void);

/**
 * @brief 添加屏幕到链表头部（最新访问的屏幕）
 * @param create_func 屏幕创建函数指针
 * @param scr_ptr 屏幕对象指针的指针
 */
void scr_push_head(void (*create_func)(ui_manager_t *ui), lv_obj_t **scr_ptr);

/**
 * @brief 获取并移除最后一个访问的屏幕（返回上一个屏幕）
 * @return screen_node_t 屏幕节点信息
 */
void scr_pull_head(screen_node_t *result);

/**
 * @brief 获取屏幕历史记录数量
 * @return uint32_t 历史记录数量
 */
uint32_t scr_get_len(void);

/**
 * @brief 检查是否有上一个屏幕
 * @return bool true表示有上一个屏幕，false表示没有
 */
bool scr_is_empty(void);

extern const screen_node_t screen_info[];

/**
 * @brief 通过LVGL对象获取屏幕索引
 *
 * 该函数用于根据给定的LVGL对象查找并返回其对应的屏幕索引值。
 * 通常用于多屏幕管理系统中，通过对象快速定位所属屏幕的位置。
 *
 * @param obj 指向LVGL对象的指针，用于查找其所属的屏幕索引
 *
 * @return 返回对象所属屏幕的索引值，类型为32位无符号整数
 *         如果对象无效或未找到对应屏幕，返回值可能为默认值或错误标识
 */
gw_inline int scr_get_idx_by_obj(lv_obj_t *obj)
{
    for (int i = 0; screen_info[i].scr_ptr != NULL; i++) {
        if (*(screen_info[i].scr_ptr) == obj) {
            return i;
        }
    }
    LV_LOG_ERROR("gw scr get idx failed!\n");
    return -1;
}

/**
 * @brief 通过创建函数指针获取屏幕索引
 *
 * 该函数遍历screen_info数组，查找与给定创建函数指针匹配的屏幕索引。
 *
 * @param create_func 指向屏幕创建函数的函数指针
 *
 * @return 成功时返回对应的屏幕索引值，失败时返回-1
 */
gw_inline int scr_get_idx_by_create_func(void (*create_func)(ui_manager_t *ui))
{
    for (int i = 0; screen_info[i].create_func != NULL; i++) {
        if (screen_info[i].create_func == create_func) {
            return i;
        }
    }
    LV_LOG_ERROR("gw scr get idx failed!\n");
    return -1;
}

/**
 * @brief 清空屏幕历史记录
 */
gw_inline void scr_clear(void)
{
    screen_node_t scr_node;
    while (!scr_is_empty()) {
        scr_pull_head(&scr_node);
        lv_obj_clean(*(scr_node.scr_ptr));
    }
}

/**
 * @brief 从指定屏幕打开新屏幕的函数
 *
 * 1. 打开新屏幕时，可选是否将旧屏幕插入链表
 * 2. 打开主界面屏幕时，先清空屏幕链表
 *
 * @param new_create_func 目标屏幕的创建函数指针，用于初始化屏幕内容
 */
gw_inline void scr_open_base(void (*new_create_func)(ui_manager_t *ui), bool insert_head)
{
    lv_obj_t *act_scr = lv_scr_act();
    if (!screen_is_loading(act_scr)) {
        if (new_create_func == GW_SCR_MAIN_CREATE) {
            scr_clear();
        } else {
            if (insert_head) {
                int idx = scr_get_idx_by_obj(act_scr);
                scr_push_head(screen_info[idx].create_func, screen_info[idx].scr_ptr);
            }
        }
        /* create screen */
        new_create_func(&ui_manager);
        /* load screen */
        int new_idx = scr_get_idx_by_create_func(new_create_func);
        lv_scr_load_anim(*(screen_info[new_idx].scr_ptr), LV_SCR_LOAD_ANIM_NONE, 0, 0, ui_manager.auto_del);
    }
}
gw_inline void scr_open(void (*new_create_func)(ui_manager_t *ui))
{
    scr_open_base(new_create_func, true);
}
gw_inline void scr_open_no_hist(void (*new_create_func)(ui_manager_t *ui))
{
    scr_open_base(new_create_func, false);
}

/**
 * @brief 屏幕返回功能
 *
 * 1. 主界面不再返回,并清空屏幕链表
 * 2. 屏幕栈中无历史记录时返回主界面
 * 3. 屏幕栈中存在历史记录时返回上一级屏幕
 */
gw_inline void scr_return(void)
{
    lv_obj_t *act_scr = lv_scr_act();

    if (!screen_is_loading(act_scr)) {
        if (act_scr == GW_SCR_MAIN_OBJ) {
            if (!scr_is_empty()) {
                scr_clear();
            } else {
                return;
            }
        }
        screen_node_t scr_node;
        if (!scr_is_empty()) {
            scr_pull_head(&scr_node);
        } else {
            scr_node.scr_ptr     = &GW_SCR_MAIN_OBJ;
            scr_node.create_func = GW_SCR_MAIN_CREATE;
        }
        /* create screen */
        scr_node.create_func(&ui_manager);
        /* load screen */
        lv_scr_load_anim(*(scr_node.scr_ptr), LV_SCR_LOAD_ANIM_NONE, 0, 0, ui_manager.auto_del);
    }
}

void scr_refresh_timer_set_cb(lv_timer_cb_t timer_cb);

#ifdef __cplusplus
}
#endif

#endif /* __GW_SCR_H__ */