[bits 32]
extern _term_init
extern mac_if
extern _show_machine_info
extern kernel_main


section .text
    global _start
_start:
    mov eax, 25
    push eax
    xor eax, eax
    mov ax, word [0x44A]
    push eax
    call _term_init
    add esp, 8
    lea eax, mac_if
    push eax
    call _show_machine_info
    add esp, 4


    call kernel_main


    jmp $