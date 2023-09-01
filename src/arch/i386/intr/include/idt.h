#ifndef _IDT_H
#define _IDT_H
#include "../../include/types.h"

#define INTEL_RESERVED 0x30
#define IDT_NR 256

struct gate_descriptor
{
    unsigned offset_low : 16;
    unsigned seg_selector : 16; // segement_selector
    unsigned reseverd : 8;
    unsigned gate_type : 4;
    unsigned null : 1;
    unsigned dpl : 2;
    unsigned present : 1; // 有效引用段
    unsigned offset_high : 16;

} __attribute__((packed));

// GDTR & IDTR 寄存器结构
struct pseudo_des_idtr
{
    /* data */
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#endif
