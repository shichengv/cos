#include "../../../libk/include/stdint.h"
#include "../../../libk/include/stdio.h"
#include "../include/types.h"
#include "../include/machine_info.h"
#include "../include/io.h"
#include "./include/intr_m.h"
#include "./include/apic.h"
#include "./include/pic.h"

extern struct machine_info _mac_if;

int _apic_m_apic_init(){



    // __asm__(
    //     "movl $0x1B, %ecx\n"
    //     "rdmsr\n"
    //     "orl $0x800, %eax\n"
    //     "wrmsr\n");

}
