#include "./include/idt.h"
#include "../../../libk/include/stdint.h"
#include "../../../libk/include/stdio.h"
#include "../include/types.h"
#include "../include/machine_info.h"
#include "../include/io.h"
#include "./include/intr_m.h"
#include "./include/apic.h"
#include "./include/pic.h"

/*  中断描述符表    */
struct gate_descriptor idt[IDT_NR];

/*  中断描述符表指针    */
struct pseudo_des_idtr idt_ptr;

typedef void * handler_t;

// From arch/i386/machine_info.c
extern struct machine_info _mac_if;

// From arch/i386/intr/handler_table.s
//  中断发生时跳转到的地址
extern handler_t _intr_jmp_addr_table[0x30];

//  真正的中断处理函数
handler_t _intr_handler_table[0x30];


static char *exception_msg[] = {
    "#DE Divide Error\n\r\0",
    "#DB RESERVED\n\r\0",
    "--  NMI Interrupt\n\r\0",
    "#BP Breakpoint\n\r\0",
    "#OF Overflow\n\r\0",
    "#BR BOUND Range Exceeded\n\r\0",
    "#UD Invalid Opcode (Undefined Opcode)\n\r\0",
    "#NM Device Not Available (No Math Coprocessor)\n\r\0",
    "#DF Double Fault\n\r\0",
    "    Coprocessor Segment Overrun (reserved)\n\r\0",
    "#TS Invalid TSS\n\r\0",
    "#NP Segment Not Present\n\r\0",
    "#SS Stack-Segment Fault\n\r\0",
    "#GP General Protection\n\r\0",
    "#PF Page Fault\n\r\0",
    "--  (Intel reserved. Do not use.)\n\r\0",
    "#MF x87 FPU Floating-Point Error (Math Fault)\n\r\0",
    "#AC Alignment Check\n\r\0",
    "#MC Machine Check\n\r\0",
    "#XF SIMD Floating-Point Exception\n\r\0",
    "#VE Virtualization Exception\n\r\0",
    "#CP Control Protection Exception\n\r\0",
};

void _intr_exception_handler(
    int vector, uint32_t edi, uint32_t esi, uint32_t ebp,
    uint32_t esp, uint32_t ebx, uint32_t edx, uint32_t ecx, uint32_t eax,
    uint32_t gs, uint32_t fs, uint32_t es, uint32_t ds, uint32_t vector0,
    uint32_t error, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    if (vector <= 21)
        printk("Exception: %s", exception_msg[vector]); // 打印异常信息
    else
        printk("#Other Error\n\r\0"); // 未定义异常

    printk("   VECTOR : 0x%02X\n\r\0", vector);
    printk("    ERROR : 0x%08X\n\r\0", error);
    printk("   EFLAGS : 0x%08X\n\r\0", eflags);
    printk("       CS : 0x%02X\n\r\0", cs);
    printk("      EIP : 0x%08X\n\r\0", eip);
    printk("      ESP : 0x%08X\n\r\0", esp);

    while (1)
    {
        /* code */
    }
    return;
}


void _intr_m_interrupt_enable(bool state){
    if (state)
        __asm__ volatile("sti\n"); // 响应可屏蔽硬件中断
    else
        __asm__ volatile("cli\n"); // 禁止响应可屏蔽硬件中断
}

static void idt_init(){
    for (size_t i = 0; i < INTEL_RESERVED; i++)
    {
        handler_t handler = _intr_jmp_addr_table[i]; // 将标签的位置的地址赋值给handler
        idt[i].offset_low = (uint32_t)handler & 0xffff;
        idt[i].offset_high = ((uint32_t)handler >> 16) & 0xffff;
        idt[i].gate_type = INT_GATE;
        idt[i].dpl = 0;
        idt[i].present = 1;
        idt[i].seg_selector = (1 << 3);
        idt[i].reseverd = 0;
    }
    for (size_t i = 0; i < INTEL_RESERVED; i++)
        _intr_handler_table[i] = _intr_exception_handler;
    
    idt_ptr.base = (uint32_t)idt;
    idt_ptr.limit = sizeof(idt) - 1;

    __asm__ __volatile__ ("lidt idt_ptr\n");
    
}

void _intr_m_intr_init(){

    _intr_m_interrupt_enable(false);

    // Test pic

    _mac_if._support_f_apic = 0;
    
    /*  如果支持APIC就启用APIC并禁用PIC */
    if (_mac_if._support_f_apic){
        printk("Initialize APIC");
        _pic_m_disable_pic();
        _apic_m_apic_init();
    } else {
        printk("Initialize PIC");
        _pic_m_PIC_remap(MASTER_OFFSET, SLAVE_OFFSET);
    }

    idt_init();

}
