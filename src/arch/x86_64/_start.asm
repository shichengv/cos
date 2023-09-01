[bits 64]

extern _term_init
extern _show_machine_info
extern _mac_if
; extern _gm_gdt_init

section .text
    global _start
_start:
    push rax
    push rbx
    lgdt [gdt_ptr]
    xchg bx, bx
    ; call _gm_gdt_init
    xor rdi, rdi
    mov di, word [0x44A]
    mov rsi, 25
    call _term_init

    mov rdi, _mac_if
    call _show_machine_info


    jmp $

section .data
; set GDTR
gdt_ptr:
    dw 0xFFFF
    dq gdt_base
; null
gdt_base:
    dq 0, 0; NULL decriptor
; kernel code segment
gdt_code:
    dw 0xFFFF ; Limit
    dw 0
    db 0
    ;Present位_必须为1
    ;DPL 00 表示内核权限
    ;S为1 表示代码段或数据段
    ;Executable 1 表示代码段
    ;DC 0表示向上增长1向下
    ;R/W 1 可以访问
    ;Accessed 最好清0
    db 0b_1_00_1_1_0_1_0
    ; Granularity 1 4KIB
    ; Size 1 32 Pmode
    ; L 0 code or data
    db 0b_1_0_1_0_0000 | (0xf)
    db 0
    dq 0
; kernel data segment
gdt_data:
    dw 0xFFFF ; Limit
    dw 0 ; Base
    db 0
    ;Present位_必须为1
    ;DPL 00 表示内核权限
    ;S为1 表示代码段或数据段
    ;Executable 0 表示数据段
    ;DC 0表示向上增长1向下
    ;R/W 1 可以访问
    ;Accessed 最好清0
    db 0b_1_00_1_0_0_1_0
    ; Granularity 1 4KIB
    ; Size 1 32 Pmode
    ; L 0 code or data
    db 0b_1_0_1_0_0000 | (0xf)
    db 0
    dq 0
