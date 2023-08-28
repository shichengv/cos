#include "../../include/kernel/types.h"
#include "../../libk/include/stdint.h"
#include "../../libk/include/stdio.h"

struct machine_info
{
    uint32_t available_ram;
    uint8_t display_mode;
    uint16_t column;
    uint8_t hd_num;
};

struct machine_info mac_if;

void _show_machine_info(struct machine_info * data){
    data->column = *(uint16_t*)0x44A;
    data->available_ram = *(uint32_t*)0x9000;
    data->display_mode = *(uint8_t*)0x449;
    data->hd_num = *(uint8_t*)0x475;
    printk("Avaliable Ram Size: %dKB\n\r", (data->available_ram >> 10));
    printk("Hard Disk Count: %d\n\r", data->hd_num);
    printk("Display Mode: %d\n\r", data->display_mode);
    printk("Display Column: %d\n\r", data->column);
}