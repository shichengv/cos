#include "./include/pic.h"
#include "../include/io.h"

/* 向指定的IRQ号发送EOI信号 */
void _pic_m_PIC_sendEOI(unsigned char irq){
    if(irq >= 8)
        outb(PIC2_COMMAND,PIC_EOI);

    outb(PIC1_COMMAND,PIC_EOI);
}
/* 屏蔽指定IRQ的中断 */
void _pic_m_IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}
 /* 开始指定IRQ的中断 */
void _pic_m_IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}

/* 初始化PIC */
void _pic_m_PIC_remap(int offset1, int offset2)
{
    // 初始化master
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    // 初始化slave
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // 主片在IDT的偏移
    outb(PIC1_DATA, offset1); // ICW2
    // 从片在IDT的偏移
    outb(PIC2_DATA, offset2); // ICW2

    // 告诉主片从片的位置在IRQ2, 00000100
    outb(PIC1_DATA, 4); // ICW3 
    // 告诉从片使用的是级连连接,
    // 从片连接到主片的IRQ2
    outb(PIC2_DATA, 2); // ICW3

    outb(PIC1_DATA, ICW4_8086); // ICW4
    outb(PIC2_DATA, ICW4_8086); // ICW4
}


void _pic_m_disable_pic(){
    outb(0xa1, 0xff);
    outb(0x21, 0xff);
}
