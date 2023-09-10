#include "../../include/struct/list.h"

/* 查找list中node节点是否存在 */
int8_t list_search(list_t * list, list_node_t * node){
    list_node_t * next_node = &list->header;
    while (next_node != &list->footer) {
        if(next_node == node)
            return 1;
        next_node = next_node->next;
    }
    return 0;
}
