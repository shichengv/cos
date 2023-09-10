#include "../../include/struct/list.h"


/* 获得list的长度 */
uint32_t list_len(list_t * list){
    list_node_t * next_node = list->header.next;

    uint32_t length = 0;
    while(next_node != &list->footer){
        length++;
        next_node = next_node->next;
    }
    return length;
}
