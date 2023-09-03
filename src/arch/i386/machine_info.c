#include "./include/types.h"
#include "./include/machine_info.h"
#include "../../libk/include/stdint.h"
#include "../../libk/include/stdio.h"

#define YES(yes) (yes ? "yes" : "no")

struct machine_info _mac_if;

static void _mi_get_cpu_vendor_id(char * str){
    reg_t eax, ebx, ecx, edx;
    char uc;

    eax = 0;
    __asm__ __volatile__ ("cpuid"
                            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                            : "a" (eax));

    int i = 3;
    do
    {
        uc = (char)(ebx >> (i * 8));
        str[i] = uc;
        
    } while (i-- >= 0);
    i = 7;
    do
    {
        uc = (char)(edx >> (i * 8));
        str[i] = uc;
        
    } while (i-- > 4);
    i = 11;
    do
    {
        uc = (char)(ecx >> (i * 8));
        str[i] = uc;
        
    } while (i-- > 8);

}

static void _mi_detect_apic(){

    reg_t eax, ebx, ecx, edx;

    eax = 1;
    __asm__ __volatile__ ("cpuid"
                            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                            : "a" (eax));
    _mac_if._support_f_apic = (edx >> 8) & 0x1;
}

void _show_machine_info(struct machine_info * info){
    info->column = *(uint16_t*)0x44A;
    info->available_ram = *(uint32_t*)0x9000;
    info->display_mode = *(uint8_t*)0x449;
    info->hd_num = *(uint8_t*)0x475;
    _mi_get_cpu_vendor_id(info->cpu_vendor_id);
    _mi_detect_apic();
    printk("Avaliable Ram Size: %dKB\n\r", (info->available_ram >> 10));
    printk("Hard Disk Count: %d\n\r", info->hd_num);
    printk("Display Mode: %d\n\r", info->display_mode);
    printk("Display Column: %d\n\r", info->column);
    printk("CPU Vendor ID: %s\n\r", info->cpu_vendor_id);
    printk("\tSupport for APIC: %s\n\r", YES(info->_support_f_apic));
}
