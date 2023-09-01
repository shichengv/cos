#ifndef _INTR_M_H
#define _INTR_M_H 1
#include "../../../../libk/include/stdint.h"
#include "../../include/types.h"

#define TASK_GATE 0x5 // 任务门
#define INT_GATE 0xE // 中断门
#define TRAP_GATE 0xF // 陷阱门

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
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void _intr_m_interrupt_enable(bool status);

#endif