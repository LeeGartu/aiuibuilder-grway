/**
 * @file cpo_item_list.h
 * @brief 参数控件模块
 */
#ifndef __CPO_ITEM_LIST_H__
#define __CPO_ITEM_LIST_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../screen/scr.h"
#include "../mem/mem_item.h"

#define DECLARE_PARAM(mem_range_tab, name_idx, title_idx) \
    {NULL, mem_range_tab, name_idx, title_idx, 0}
#define DECLARE_PARAM_OFS(mem_range_tab, name_idx, title_idx, ofs) \
    {NULL, mem_range_tab, name_idx, title_idx, ofs}

#define DECLARE_STATUS(mem_tab, name_idx, title_idx) \
    {mem_tab, NULL, name_idx, title_idx, 0}
#define DECLARE_STATUS_OFS(mem_tab, name_idx, title_idx, ofs) \
    {mem_tab, NULL, name_idx, title_idx, ofs}

typedef struct cpo_item_list_desc_ {
    const mem_t       *mem_tab;
    const mem_range_t *mem_range_tab;
    uint16_t           name_idx;
    uint16_t           title_idx;
    uint32_t           offset;
} cpo_item_list_desc_t;

lv_obj_t *cpo_item_list_create(lv_obj_t *parent, cpo_item_list_desc_t *item_list, int32_t dev_addr);

#ifdef __cplusplus
}
#endif
#endif /* __CPO_ITEM_LIST_H__ */
