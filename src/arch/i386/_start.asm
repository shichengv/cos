[bits 32]

extern _gdtm_gdt_init

extern _term_init

extern _mac_if
extern _show_machine_info

extern _intr_m_init

extern _mem_m_init

extern kernel_main
; equ伪指令定义符号常量
; 前两位00表示kernel级，第二位0表示GDT
; 指向GDT表的第二个项，内核级的代码段
; segment_selector_code equ ( 1 << 3 )
; ; 指向GDT表的第三个项，内核级的数据段
; segment_selector_data equ ( 2 << 3 )

section .text
    global _start

_start:
    ; 修改栈指针
    mov esp, 0xF00000
    push eax

    call _gdtm_gdt_init

    call _term_init

    mov eax, _mac_if
    push eax
    call _show_machine_info
    add esp, 4

    call _intr_m_init
    call _mem_m_init
    add esp, 4

    call kernel_main

    jmp $
    ;cli
    ;hlt
