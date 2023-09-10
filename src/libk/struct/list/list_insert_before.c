#include "../../include/struct/list.h"
/* 将node节点插入到anchor节点之前 */
void list_insert_before(list_node_t * anchor, list_node_t * node){
    /* 将node->prev 指向 anchor 的前一个节点 */
    node->prev = anchor->prev;
    /* 将node->next 指向 anchor */
    node->next = anchor;

    /* 将anchor的前一个节点的下一个节点指向node */
    anchor->prev->next = node;
    /* 将anchor的前一个节点指向node */
    anchor->prev = node;

}