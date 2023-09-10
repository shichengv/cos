#include "../../include/struct/list.h"
/* 将node节点插入到anchor节点之后 */
void list_insert_before(list_node_t * anchor, list_node_t * node){
    /* 将archor指向的下一个节点的前一个节点指向node */
    anchor->next->prev = node;
    /* 将node的下一个节点指向anchor指向的下一个节点 */
    node->next = anchor->next;

    /* 将anchor的下一个节点指向node */
    anchor->next = node;
    /* 将node的前一个节点指向anchor */
    node->prev = anchor;


}