#include "../../include/struct/list.h"

/* 移除list链表头之后的节点 */
list_node_t * list_pop_front(list_t * list){
    list_node_t * removed = list->header.next;
    list_remove(removed);
    return removed;
}
