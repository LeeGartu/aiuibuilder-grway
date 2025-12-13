#include "scr.h"

// 全局链表变量
static lv_ll_t screen_list;
static bool    scr_is_init = false;
static lv_timer_t *scr_refresh_timer=NULL;

// 初始化屏幕管理链表
void scr_init(void)
{
    if (scr_is_init) {
        return;
    }
    _lv_ll_init(&screen_list, sizeof(screen_node_t));
    scr_is_init = true;
}

// 添加屏幕到链表头部（最新访问的屏幕）
void scr_push_head(void (*create_func)(ui_manager_t *ui), lv_obj_t **scr_ptr)
{
    scr_init();
    // 创建新节点并添加到头部
    screen_node_t *new_node = _lv_ll_ins_head(&screen_list);
    if (new_node) {
        new_node->create_func = create_func;
        new_node->scr_ptr     = scr_ptr;
    }
}

// 获取并移除最后一个访问的屏幕（返回上一个屏幕）
void scr_pull_head(screen_node_t *result)
{
    if (!result) {
        return;
    }

    // 获取头部节点（最新添加的）
    screen_node_t *head = _lv_ll_get_head(&screen_list);
    if (head) {
        result->create_func = head->create_func;
        result->scr_ptr     = head->scr_ptr;
        _lv_ll_remove(&screen_list, head);
        lv_free(head);
    } else {
        result->create_func = NULL;
        result->scr_ptr     = NULL;
    }
}

// 获取屏幕历史记录数量
uint32_t scr_get_len(void)
{
    return _lv_ll_get_len(&screen_list);
}

// 检查是否有上一个屏幕
bool scr_is_empty(void)
{
    return _lv_ll_is_empty(&screen_list);
}

void scr_refresh_timer_set_cb(lv_timer_cb_t timer_cb)
{
    if (scr_refresh_timer==NULL) {
        scr_refresh_timer = lv_timer_create(timer_cb, 200, NULL);
    } else {
        lv_timer_set_cb(scr_refresh_timer, timer_cb);
    }
}
