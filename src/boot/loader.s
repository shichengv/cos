[org 0x500]

dw 0x55aa; 加载器标识，判断错误
available_ram equ 0x9000; 有效内存存放的位置
                        ; 存放了可用的RAM大小
; hello
mov si, loading
call print

; detecting memory
call detect_memory
mov si, detecting
call print

; 开启保护模式
jmp perpare_protected_mode

perpare_protected_mode: 
    cli ; 关闭中断

    ; turn on A20
    in al, 0x92
    or al, 0b10
    out 0x92, al
    
    ; 加载GDT IDT
    lgdt [ gdt_48 ]
    lidt [ idt_48 ]
    
    ; open P-mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; 用jmp刷新缓存, 启用保护模式
    jmp dword segment_selector_code:protect_mode

; 内存检测
detect_memory:
    ; 初始化，准备第一次调用
    xor bp, bp ;计数
    ; EBX
    xor ebx, ebx;ebx必须为0才能开始
    ; ES:DI
    mov ax, 0
    mov es, ax; 将es置为0
            ; es:di 结构体的缓存位置
    mov edi, acpi_s_buffer; acpi_s_buffer
            ; 指向末尾，避免覆盖其他内存
    mov [es:di + 20], dword 1	; 强制有效的ACPI 3.X条目
    ; EAX Function Code
    mov eax, 0x0000e820; 高16位应该为0
    ; EDX
    mov edx, 0x534d4150; 将 "SMAP" 放入edx
    ; ECX
    mov ecx, 24; 请求24字节
    int 0x15
    ; 检测进位标志，如果为1,就失败
    ; short 是段内短转移，范围-128~127;
    jc short .failed
    mov edx, 0x534D4150
    cmp eax, edx;EAX将被设置为0x534d4150
    jne short .failed
    test ebx, ebx; 如果ebx为0, ZF=1
    je short .failed; je if ZF=1 go
    jmp .next

; 进行下一次调用
.continue_detect:
    mov eax, 0xe820
    mov [es:di + 20], dword 1	; 强制有效的ACPI 3.X条目
    mov ecx, 24
    int 0x15
    jc short .completed_t_time
    mov edx, 0x534D4150

; 检测当前调用的状态
.next:
    jcxz .skip
    cmp cl, 20 ; ecx <= 20
    jbe short .completed_t_time ; 低于20表示没有使用ACPI 3.x
    test byte [es:di + 20], 1; 测试是否为可用ram
    je short .skip; 如果是就继续检测

; 本次检测完成，进行验证，如果ebx!=0，继续检测
.completed_t_time:

    mov eax, [es:di] ; 保存基地址

    mov ecx, [es:di + 8] ; 小端法，获取低32位
    add [available_ram], ecx ; 由于32位系统，最大内存为4G,所以并不需要管高位
    ; 保存可用区域的长度
    or ecx, [es:di + 12] ; 获取高32位，如果 = 0，跳过

    jz .skip; 如果长度uin64_t = 0 跳过

    inc bp
    add di, 24; acpi_s_buffer + 24

; 验证ebx是否为0,如果不是就说明还有ram等待检测
.skip:
    test ebx, ebx ; 如果不相等，zf=0，继续检测
    jne short .continue_detect

; 存储状态
.store_status:
    mov [acpi_s_count], bp ; 写入
    clc ; 清楚进位标志寄存器
    ret
; 失败就设置进位标志，然后返回
.failed:
    stc ; 设置进位标志为1
    mov si, detect_error
    call print
    jmp error
    ;ret

jmp $


; print
print:
    mov ah, 0x0e
.next:
    mov al, [si]
    cmp al, 0
    jz  .done
    int 0x10
    inc si
    jmp .next
.done:
    ret


loading:
    db "Loading Cos...", 10, 13, 0; \n\r
detecting:
    db "Detecting Memory Success.", 10, 13, 0
detect_error:
    db "Detect Error !!!", 10, 13, 0

error:
    mov si, .msg
    call print
    hlt; 让cpu停止
    jmp $
    .msg db "Loading Error!!!", 10, 13, 0
    


[bits 32]
protect_mode:

    ; 初始化数据段寄存器
    mov ax, segment_selector_data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; 修改栈指针 为了使用read_disk
    mov esp, 0xFFFF;

    ; 将内核读取到 0x10000的位置
    mov edi, 0x10000
    mov ecx, 6
    mov bl, 200
    call read_disk

    ; 魔数
    mov eax, 0x10100101
    mov ebx, acpi_s_count

    ; 根据逻辑地址，找到GDT表中的第二个项，内核级代码段
    ; 由于设置为00级，所以跳到的位置被视为内核
    jmp dword segment_selector_code:0x10000
    
    ud2; error
    

jmp $; 

read_disk:
    ; 设置读写扇区的数量
    ; dx 为端口 out in 操作端口
    mov dx, 0x1F2
    mov al, bl
    ; 将200写入到 0x1f2端口中，告诉是要读取200个扇区数量
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
            mov [edi] ,ax
            add edi, 2
            loop .readw
        ret

; equ伪指令定义符号常量
; 前两位00表示kernel级，第二位0表示GDT
; 指向GDT表的第二个项，内核级的代码段
segment_selector_code equ ( 1 << 3 )
; 指向GDT表的第三个项，内核级的数据段
segment_selector_data equ ( 2 << 3 )


page_base equ 0; 内存开始的位置

; 可以访问 0 ~ (4G/4Kib) 的页
page_limit equ ((1024*1024*1024*4) / (1024 * 4)) - 1
idt_48:
    dw 0
    dd 0
; set GDTR
gdt_48:
    dw 0xFFFF
    dd gdt_base
; null
gdt_base:
    dd 0, 0; NULL decriptor
; kernel code segment
gdt_code:
    dw page_limit & 0xFFFF ; Limit
    dw page_base & 0xFFFF ; Base
    db (page_base >> 16) && 0xFF
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
    db 0b_1_1_0_0_0000 | ((page_limit >> 16) & 0xf)
    db page_base >> 24 && 0xFF
; kernel data segment
gdt_data:
    dw page_limit & 0xFFFF ; Limit
    dw page_base & 0xFFFF ; Base
    db (page_base >> 16) && 0xFF
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
    db 0b_1_1_0_0_0000 | ((page_limit >> 16) & 0xf)
    db page_base >> 24 && 0xFF


; 调用的次数
acpi_s_count:
    dd 0
; acpi结构体保存的位置
acpi_s_buffer:
