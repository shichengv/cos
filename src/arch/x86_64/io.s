[bits 64]

section .text

global inb 
inb:
    push rbp
    mov rbp, rsp

    xor rax, rax
    xor rdx, rdx
    mov rdx, rdi

    in al, dx

    leave; mov esp, ebp;pop ebp
    ret

global outb
outb:
    push rbp
    mov rbp, rsp

    xor rax, rax
    xor rdx, rdx
    mov rdx, rdi 
    mov rax, rsi 

    out dx, al

    leave; mov esp, ebp;pop ebp
    ret

global inw
inw:
    push rbp; 
    mov rbp, rsp ; 保存帧

    xor rax, rax ; 将 eax 清空
    mov rdx, rdi; port 
    in ax, dx; 将端口号 dx 的 16 bit 输入到 ax

    leave ; 恢复栈帧
    ret

global outw
outw:
    push rbp; 
    mov rbp, rsp ; 保存帧

    mov rdx, rdi; port 
    mov rax, rsi; value
    out dx, ax; 将 ax 中的 16 bit 输入出到 端口号 dx

    leave ; 恢复栈帧
    ret