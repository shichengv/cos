#ifndef _PIC_H
#define _PIC_H 1
#include "../../include/types.h"

#define PIC1        0x20        /* IO base address for master PIC */
#define PIC2        0xA0        /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA    (PIC2+1)

#define PIC_EOI        0x20        /* End-of-interrupt command code */
/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4    0x01        /* Indicates that ICW4 will be present */
#define ICW1_SINGLE    0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4    0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL    0x08        /* Level triggered (edge) mode */
#define ICW1_INIT    0x10        /* Initialization - required! */

// 对于我们的目的就是 ICW1_ICW4 | ICW1_INIT

#define ICW4_8086    0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO    0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE    0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER    0x0C        /* Buffered mode/master */
#define ICW4_SFNM    0x10        /* Special fully nested (not) */

#define MASTER_OFFSET 0x20
#define PIT_OFFSET (MASTER_OFFSET)
#define SLAVE_OFFSET (MASTER_OFFSET+8)

// 外中断索引
enum IRQ_port {
    IRQ_CLOCK = 0,
    IRQ_KEYBOARD,
    // CASCADE
    IRQ_SLAVE,
    IRQ_SERIAL_2,
    IRQ_SERIAL_1,
    IRQ_PARALLEL_2,
    IRQ_FLOPPY,
    IRQ_PARALLEL_1,
    IRQ_RTC,
    IRQ_REDIRECT,
    IRQ_MOUSE = 12,
    IRQ_COPROCESSER,
    IRQ_HARDDISK_1,
    IRQ_HARDDISK_2,
};

/*
arguments:
    offset1 - vector offset for master PIC
        vectors on the master become offset1..offset1+7
    offset2 - same for slave PIC: offset2..offset2+7
*/
void _pic_m_PIC_remap(int offset1, int offset2);
void _pic_m_PIC_sendEOI(unsigned char irq);
void _pic_m_IRQ_set_mask(unsigned char IRQline);
void _pic_m_IRQ_clear_mask(unsigned char IRQline);

void _pic_m_disable_pic();
#endif
