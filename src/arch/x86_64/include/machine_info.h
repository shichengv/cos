
#ifndef _MACHINE_INFO
#define _MACHINE_INFO 1

#include "../../../libk/include/stdint.h"

#define VENDOR_LEN 12

struct machine_info
{
    uint32_t available_ram;
    uint8_t display_mode;
    uint16_t column;
    uint8_t hd_num;
    char cpu_vendor_id[VENDOR_LEN];
    bool _support_f_apic;
};
#endif