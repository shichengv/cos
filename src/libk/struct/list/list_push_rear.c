#include "../../include/struct/list.h"

/* 将node节点插入到list链表末尾之前 */
void list_push_rear(list_t * list, list_node_t * node){
    list_insert_before(&list->footer, node);
}
