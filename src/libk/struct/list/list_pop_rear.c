#include "../../include/struct/list.h"

/* 移除list链表末尾之前的节点 */
list_node_t * list_pop_rear(list_t * list){
    list_node_t * removed = list->footer.next;
    list_remove(removed);
    return removed;
}
