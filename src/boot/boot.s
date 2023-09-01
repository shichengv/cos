[org 0x7c00]
; 设置屏幕为文本模式，清除屏幕
mov ax, 3
int 0x10

cli ; 关闭中断

; turn on A20
in al, 0x92
or al, 0b10
out 0x92, al

; 初始化每个寄存器
mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

; 0xb800 文本显示器内存区域

mov si, booting
call print

mov edi, 0x500; 读取的目标内存
mov ecx, 2; 起始扇区
mov bl, 4; 扇区数量

call read_disk

jmp 0:0x500

; 阻塞
jmp $

read_disk:
    ; 设置读写扇区的数量
    ; dx 为端口 out in 操作端口
    mov dx, 0x1F2
    mov al, bl
    out dx, al
    
    inc dx; 0x1f3
    mov al, cl; 起始扇区的前八位
    out dx, al

    inc dx; 0x1f4
    shr ecx, 8
    mov al, cl; 起始扇区的中八位
    out dx, al

    inc dx; 0x1f5
    shr ecx, 8
    mov al, cl; 起始扇区的高八位
    out dx, al
    
    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111; 将高4位 为 0
    mov al, 0b1110_0000;
    or al, cl
    out dx, al; 主盘 - LBAmode

    inc dx; 0x1f7
    mov al, 0x20; 读硬盘
    out dx, al
    
    xor ecx, ecx;
    mov cl, bl; 得到读取扇区的数量

    .read:
        push cx 
        call .waits; 等待数据准备完毕
        call .reads
        pop cx
        loop .read
    ret

    .waits:
        mov dx, 0x1F7
        .check:
            in al, dx; 将dx读到al里面
            and al, 0b1000_1000
            cmp al, 0b0000_1000;当数据准备完毕
            ;硬件会将第7位设置位0,第三位为1
            jnz .check; jump if not zero
        ret

    .reads:
        mov dx, 0x1F0
        mov cx, 256; 一个扇区 256 字
        .readw:
            in ax, dx
            jmp $+2; 跳转到下一行
            jmp $+2; 跳转到下一行
            jmp $+2; 跳转到下一行
            mov [edi] ,ax
            add edi, 2
            loop .readw
        ret

print:
    mov ah, 0x0e;BIOS中断参数：显示一个文字
    ; ah 为 ax的高八位寄存器
.next:
    mov al, [si]
    cmp al, 0
    jz  .done
    int 0x10;调用BIOS中断操作显卡。
    inc si
    jmp .next
    
.done:
    ret

booting:
    db "Start BootLoader", 10, 13, 0; \n\r

error:
    mov si, .msg
    call print
    hlt; 让cpu停止
    jmp $
    .msg db "Booting Error...", 10, 13, 0

; 填充0
times 510 - ($ - $$) db 0

; BIOS要求主引导扇区的512B的最后两个字节必须是 0x55 0xaa
db 0x55, 0xaa
