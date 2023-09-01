#include "../../../libk/include/stdint.h"
#include "../../../libk/include/stdio.h"
#include "../include/types.h"
#include "../include/machine_info.h"
#include "../include/io.h"
#include "./include/intr_m.h"
#include "./include/apic.h"

extern struct machine_info _mac_if;

int _apic_m_apic_init(){

    _intr_m_interrupt_enable(false);
    /*  disable pic */
    outb(0xa1, 0xff);
    outb(0x21, 0xff);

    // __asm__(
    //     "movl $0x1B, %ecx\n"
    //     "rdmsr\n"
    //     "orl $0x800, %eax\n"
    //     "wrmsr\n");

    
    reg_t eax, ebx, ecx, edx;

    eax = 1;
    __asm__ __volatile__ ("cpuid"
                            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                            : "a" (eax));
    
    
    _mac_if._support_f_apic = (edx >> 8);
    
    

    
}