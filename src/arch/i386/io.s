[bits 32]

section .text

global inb 
inb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp + 8]

    in al, dx

    leave; mov esp, ebp;pop ebp
    ret

global outb
outb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp + 8]
    mov eax, [ebp + 12]

    out dx, al

    leave; mov esp, ebp;pop ebp
    ret

global inw
inw:
    push ebp; 
    mov ebp, esp ; 保存帧

    xor eax, eax ; 将 eax 清空
    mov edx, [ebp + 8]; port 
    in ax, dx; 将端口号 dx 的 16 bit 输入到 ax

    leave ; 恢复栈帧
    ret

global outw
outw:
    push ebp; 
    mov ebp, esp ; 保存帧

    mov edx, [ebp + 8]; port 
    mov eax, [ebp + 12]; value
    out dx, ax; 将 ax 中的 16 bit 输入出到 端口号 dx

    leave ; 恢复栈帧
    ret