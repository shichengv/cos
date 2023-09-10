#include "../../include/struct/list.h"

/* 从list中删除node */
void list_remove(list_node_t * node){
    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->next = NULL;
    node->prev = NULL;
}
