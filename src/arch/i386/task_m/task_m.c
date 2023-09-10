#include "./include/task_m.h"
#include "../../../libk/include/struct/list.h"
#include "../../../libk/include/struct/bitmap.h"
#include "../mem_m/include/mem_m.h"
#include "../mem_m/include/kmalloc.h"

list_t _tm_ready_list;
list_t _tm_dead_list;
list_t _tm_blocked_list;
list_t _tm_sleep_list;

static task_t * create_task(){
    task_t * task;

    
    return task;

}

void _tm_init(){
    task_t * a = create_task();
    list_init(&_tm_ready_list);
    list_init(&_tm_dead_list);
    list_init(&_tm_blocked_list);
    list_init(&_tm_sleep_list);
}
