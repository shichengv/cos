#ifndef _INTR_M_H
#define _INTR_M_H 1
#include "../../../../libk/include/stdint.h"
#include "../../include/types.h"

#define TASK_GATE 0x5 // 任务门
#define INT_GATE 0xE // 中断门
#define TRAP_GATE 0xF // 陷阱门

void _intr_m_interrupt_enable(bool state);

#endif
