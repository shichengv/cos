#include "../../include/struct/list.h"

/* 将node节点插入到list链表头之后 */
void list_push_front(list_t * list, list_node_t * node){
    list_insert_after(&list->header, node);
}