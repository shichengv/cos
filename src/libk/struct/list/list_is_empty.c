#include "../../include/struct/list.h"

/* 判断list是否为空 */
int list_is_empty(list_t * list){
    return (list->header.next == &list->footer);
}

