#ifndef _LIST_H
#define _LIST_H 1

#include "../stdint.h"

typedef struct list_node_t {
    struct list_node_t * prev;
    struct list_node_t * next;
} list_node_t;

typedef struct {
    list_node_t header;
    list_node_t footer;
} list_t;


/* 初始化list链表 */
void list_init(list_t *list);

/* 将node节点插入到anchor节点之前 */
void list_insert_before(list_node_t * anchor, list_node_t * node);

/* 将node节点插入到anchor节点之后 */
void list_insert_after(list_node_t * anchor, list_node_t * node);

/* 将node节点插入到list链表头之后 */
void list_push_front(list_t * list, list_node_t * node);

/* 移除list链表头之后的节点 */
list_node_t * list_pop_front(list_t * list);

/* 将node节点插入到list链表末尾之前 */
void list_push_rear(list_t * list, list_node_t * node);

/* 移除list链表末尾之前的节点 */
list_node_t * list_pop_rear(list_t * list);

/* 查找list中node节点是否存在 */
int8_t list_search(list_t * list, list_node_t * node);

/* 从该节点所属的链表中移除该节点 */
void list_remove(list_node_t * node);

/* 判断list是否为空 */
int list_is_empty(list_t * list);

/* 获得list的长度 */
uint32_t list_len(list_t * list);

#endif


