#include "./include/gdt.h"
#include "../../libk/include/string.h"
struct segment_descriptor gdt[GDT_SIZE];
struct pseudo_des_gdtr gdt_ptr;

tss_t tss;


void _gdtm_gdt_init(){

    memset(&gdt , 0, sizeof(gdt));

    gdt[KERNEL_CODE_INDEX].limit_low = 0xffff;
    gdt[KERNEL_CODE_INDEX].base_low = 0;
    gdt[KERNEL_CODE_INDEX].accessed = 0;
    gdt[KERNEL_CODE_INDEX].r_w = 1;
    gdt[KERNEL_CODE_INDEX].d_c = 0;
    gdt[KERNEL_CODE_INDEX].exec = 1;
    gdt[KERNEL_CODE_INDEX].des_type = 1;
    gdt[KERNEL_CODE_INDEX].dpl = 0;
    gdt[KERNEL_CODE_INDEX].present = 1;
    gdt[KERNEL_CODE_INDEX].limit_high = 0xf;
    gdt[KERNEL_CODE_INDEX].long_mode = 0;
    gdt[KERNEL_CODE_INDEX].db = 1;
    gdt[KERNEL_CODE_INDEX].granularity = 1;
    gdt[KERNEL_CODE_INDEX].base_high = 0;

    gdt[KERNEL_DATA_INDEX].limit_low = 0xffff;
    gdt[KERNEL_DATA_INDEX].base_low = 0;
    gdt[KERNEL_DATA_INDEX].accessed = 0;
    gdt[KERNEL_DATA_INDEX].r_w = 1;
    gdt[KERNEL_DATA_INDEX].d_c = 0;
    gdt[KERNEL_DATA_INDEX].exec = 0;
    gdt[KERNEL_DATA_INDEX].des_type = 1;
    gdt[KERNEL_DATA_INDEX].dpl = 0;
    gdt[KERNEL_DATA_INDEX].present = 1;
    gdt[KERNEL_DATA_INDEX].limit_high = 0xf;
    gdt[KERNEL_DATA_INDEX].long_mode = 0;
    gdt[KERNEL_DATA_INDEX].db = 1;
    gdt[KERNEL_DATA_INDEX].granularity = 1;
    gdt[KERNEL_DATA_INDEX].base_high = 0;


    gdt[USER_CODE_INDEX].limit_low = 0xffff;
    gdt[USER_CODE_INDEX].base_low = 0;
    gdt[USER_CODE_INDEX].accessed = 0;
    gdt[USER_CODE_INDEX].r_w = 1;
    gdt[USER_CODE_INDEX].d_c = 0;
    gdt[USER_CODE_INDEX].exec = 1;
    gdt[USER_CODE_INDEX].des_type = 1;
    gdt[USER_CODE_INDEX].dpl = 3;
    gdt[USER_CODE_INDEX].present = 1;
    gdt[USER_CODE_INDEX].limit_high = 0xf;
    gdt[USER_CODE_INDEX].long_mode = 0;
    gdt[USER_CODE_INDEX].db = 1;
    gdt[USER_CODE_INDEX].granularity = 1;
    gdt[USER_CODE_INDEX].base_high = 0;

    gdt[USER_DATA_INDEX].limit_low = 0xffff;
    gdt[USER_DATA_INDEX].base_low = 0;
    gdt[USER_DATA_INDEX].accessed = 0;
    gdt[USER_DATA_INDEX].r_w = 1;
    gdt[USER_DATA_INDEX].d_c = 0;
    gdt[USER_DATA_INDEX].exec = 0;
    gdt[USER_DATA_INDEX].des_type = 1;
    gdt[USER_DATA_INDEX].dpl = 3;
    gdt[USER_DATA_INDEX].present = 1;
    gdt[USER_DATA_INDEX].limit_high = 0xf;
    gdt[USER_DATA_INDEX].long_mode = 0;
    gdt[USER_DATA_INDEX].db = 1;
    gdt[USER_DATA_INDEX].granularity = 1;
    gdt[USER_DATA_INDEX].base_high = 0;

    uint32_t tss_limit = sizeof(tss_t) - 1;
    uint32_t tss_ptr = (uint32_t)&tss;


    gdt[KERNEL_TSS_INDEX].limit_low = tss_limit & 0xffff;
    gdt[KERNEL_TSS_INDEX].base_low = tss_ptr & 0xffffff;
    /* TYPE 1 | 0 |Busy flag | 1*/
    gdt[KERNEL_TSS_INDEX].accessed = 1;
    gdt[KERNEL_TSS_INDEX].r_w = 0;
    gdt[KERNEL_TSS_INDEX].d_c = 0;
    gdt[KERNEL_TSS_INDEX].exec = 1;
    /* 0 */
    gdt[KERNEL_TSS_INDEX].des_type = 0;
    gdt[KERNEL_TSS_INDEX].dpl = 0;
    gdt[KERNEL_TSS_INDEX].present = 1;
    gdt[KERNEL_TSS_INDEX].limit_high = (tss_limit >> 16) & 0xf;
    /* avl: available for use by system software */
    gdt[KERNEL_TSS_INDEX].reserved = 0;
    /* Granularity|0|0 */
    gdt[KERNEL_TSS_INDEX].long_mode = 0;
    gdt[KERNEL_TSS_INDEX].db = 0;
    gdt[KERNEL_TSS_INDEX].granularity = 0;

    gdt[KERNEL_TSS_INDEX].base_high = (tss_ptr >> 24) & 0xff;

    gdt_ptr.base = (uint32_t)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;

    memset(&tss, 0, sizeof(tss));

    tss.ss0 = KERNEL_DATA_SELECTOR;
    tss.esp0 = 0x700000;
    tss.iomap_base = sizeof(tss);

    __asm__ volatile ("lgdt gdt_ptr");
    __asm__ volatile ( "ltr %%ax\n" :: "a"(KERNEL_TSS_SELECTOR));
}
