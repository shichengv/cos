#include "../../include/struct/list.h"

/* 初始化link链表 */
void list_init(list_t *list){
    list->header.prev = 0;
    list->footer.next = &list->footer;
    list->footer.prev = &list->header;
    list->footer.next = 0;
}