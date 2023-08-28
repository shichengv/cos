#ifndef _GDT_H
#define _GDT_H
#include "../../../libk/include/stdint.h"

#define GDT_SIZE 256

#define KERNEL_CODE_INDEX 1
#define KERNEL_DATA_INDEX 2
#define USER_CODE_INDEX 3
#define USER_DATA_INDEX 4
#define KERNEL_TSS_INDEX 5

#define KERNEL_CODE_SELECTOR (KERNEL_CODE_INDEX << 3)
#define KERNEL_DATA_SELECTOR (KERNEL_DATA_INDEX << 3)
#define KERNEL_TSS_SELECTOR (KERNEL_TSS_INDEX << 3)

#define USER_CODE_SELECTOR (USER_CODE_INDEX << 3 | 0b11)
#define USER_DATA_SELECTOR (USER_DATA_INDEX << 3 | 0b11)


struct segment_descriptor
{
    /* data */
    unsigned limit_low : 16;        // 段限制 20 位
    unsigned base_low : 24;         // 段基地址
    unsigned accessed : 1;          // 已访问位
    unsigned r_w : 1;               // 可读或可写
    unsigned d_c : 1;               // Direction 0向上1向下/ Conforming 0只能从, privl设置的ring执行，1可以从相等的或较低的特权级别执行
    unsigned exec : 1;              // Executable 
    unsigned des_type : 1;          // 描述符类型，0为系统段，1为代码或数据
    unsigned dpl : 2;               // 特权级别字段
    unsigned present : 1;           // 有效引用段
    unsigned limit_high : 4;        //  limit high
    unsigned reserved : 1;          // reserved
    unsigned long_mode : 1;         // 长模式标志
    unsigned db : 1;                // 操作系统长度 0: 16bit p-mode 1: 32bit p-mode
    unsigned granularity : 1;       // 0 一字节 1 4kib页
    unsigned base_high : 8; 

} __attribute__((packed));


struct simply_seg_des{
    unsigned limit_low : 16;
    unsigned base_low : 24;
    unsigned access : 8;
    unsigned limit_high : 4;
    unsigned flags : 4;
    unsigned base_high : 8;
} __attribute__((packed));

// GDTR & IDTR 寄存器结构
struct pseudo_des_gdtr
{
    /* data */
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct tss_t{
    uint16_t pre_task_link;
    uint16_t reserved_p; 

    uint32_t esp0;
    uint16_t ss0;
    uint16_t reserved_0; 

    uint32_t esp1;
    uint16_t ss1;
    uint16_t reserved_1; 

    uint32_t esp2;
    uint16_t ss2;
    uint16_t reserved_2; 

    uint32_t cr3;

    uint32_t eip;

    uint32_t eflags;

    // 通用目的寄存器
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    // 段寄存器
    uint16_t es;
    uint16_t reserved_e;
    uint16_t cs;
    uint16_t reserved_c;
    uint16_t ss;
    uint16_t reserved_s;
    uint16_t ds;
    uint16_t reserved_d;
    uint16_t fs;
    uint16_t reserved_f;
    uint16_t gs;
    uint16_t reserved_g;

    // LDT段选择器
    uint16_t ldt_seg_sel;
    uint16_t reserved_sel;

    uint16_t debug_trap : 1;
    uint16_t reserved_debug : 15;

    uint16_t iomap_base;

    uint32_t ssp;

} __attribute__((packed)) tss_t;


#endif