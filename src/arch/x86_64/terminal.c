#include "./include/types.h"
#include "./include/terminal.h"
#include "./include/io.h"
#include "../../libk/include/stdint.h"
#include "../../libk/include/string.h"

#define BMB __asm__ volatile("xchgw %bx, %bx") // bochs magic breakpoint

// static int WIDTH;
// static int HEIGHT;
#define WIDTH 80
#define HEIGHT 25

#define ROW_SIZE (WIDTH * 2)
#define SCREEN_SIZE (ROW_SIZE * HEIGHT) 


static uint64_t display_memory = MEM_BASE_ADDR;
static uint64_t cursor = MEM_BASE_ADDR;
static uint16_t x = 0;
static uint16_t y = 0;
static uint8_t color = (VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));


// 设置字符
static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
    return (uint16_t) uc | (uint16_t) color << 8;
}
// 设置颜色
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

static _inline void set_display_location(){
    unsigned long scr = display_memory;
    outb(CRT_INDEX_REG, START_ADDR_H_REG_I);
    outb(CRT_DATA_REG, ((scr - MEM_BASE_ADDR)>>9 ) & 0xff);
    outb(CRT_INDEX_REG, START_ADDR_L_REG_I);
    outb(CRT_DATA_REG, ((scr - MEM_BASE_ADDR)>>1) & 0xff);
}
static _inline void get_display_location(){
    outb(CRT_INDEX_REG, START_ADDR_H_REG_I);
    display_memory = (inb(CRT_DATA_REG) << 8);
    outb(CRT_INDEX_REG, START_ADDR_L_REG_I);
    display_memory |= inb(CRT_DATA_REG);

    display_memory <<= 1;
    display_memory += MEM_BASE_ADDR;
}
static _inline void set_cursor_location(){
    unsigned long csr = cursor;
    outb(CRT_INDEX_REG, CURSOR_LCL_H_REG_I);
    outb(CRT_DATA_REG, ((csr - MEM_BASE_ADDR) >> 9) & 0xff);
    outb(CRT_INDEX_REG, CURSOR_LCL_L_REG_I);
    outb(CRT_DATA_REG, ((csr - MEM_BASE_ADDR) >> 1) & 0xff);
}
static _inline void get_cursor_location(){
    outb(CRT_INDEX_REG, CURSOR_LCL_H_REG_I);
    cursor = (inb(CRT_DATA_REG) << 8);
    outb(CRT_INDEX_REG, CURSOR_LCL_L_REG_I);
    cursor |= inb(CRT_DATA_REG);
    
    x = cursor % WIDTH;
    y = cursor / WIDTH;
    
    cursor <<= 1;
    cursor += MEM_BASE_ADDR;
}

void _term_clear_terminal(){
    display_memory = MEM_BASE_ADDR;
    cursor = MEM_BASE_ADDR;
    x = y = 0;
    set_cursor_location();
    set_display_location();

    uint16_t * ptr = (uint16_t*)MEM_BASE_ADDR;
    while ((uint64_t)ptr < MEM_END_ADDR)
        *ptr++=(uint16_t)ERASE;
}
static  void erase_screen(uint16_t erase, uint16_t * start, uint32_t count){
    int nr = 0;
    while (nr++ < count)
    {
        *(start++) = erase;
    }
    
}
// 通过设置显示器的起始位置来实现滚屏
static void scrollup_display(){
    // get_display_location();
    if ((display_memory + SCREEN_SIZE + ROW_SIZE) >= MEM_END_ADDR) {
        memcpy((void*)MEM_BASE_ADDR, (void *)display_memory, SCREEN_SIZE);
        cursor -= (display_memory - MEM_BASE_ADDR);
        display_memory = MEM_BASE_ADDR;
    }
    uint16_t* ptr = (uint16_t*)(display_memory + SCREEN_SIZE);
    erase_screen(ERASE, ptr, WIDTH);
    // for (size_t i = 0; i < WIDTH; i++)
    //     *ptr++ = (uint16_t)ERASE;
    display_memory += ROW_SIZE;
    cursor += ROW_SIZE;

    set_display_location();  
}

static void ascii_bs(){
    if (x)
    {
        x--;
        cursor -= 2;
        *(uint16_t*)cursor = (uint16_t)ERASE;
    }
    
}
static void ascii_del(){
    *(uint16_t*)cursor = (uint16_t)ERASE;
}

static void ascii_cr(){
    cursor -= (x << 1);
    x = 0;
}

static void ascii_lf(){
    cursor -= (x << 1);
    x = 0;
    if (y + 1 < HEIGHT)
    {
        y++;
        cursor += ROW_SIZE;
        return;
    }
    scrollup_display();
    
}

static void ascii_ht(){
    uint16_t * pc = (uint16_t*)cursor;
    if (x == 79)
        return;
    
    uint16_t left = x % 4;
    if (!left){
        for (size_t i = 0; i < 4; i++)
            *pc++ = (uint16_t)ERASE;
        x += 4;
    }
    left = 4 - left;
    x += left;
    for (size_t i = 0; i < left; i++)
        *pc++ = (uint16_t)ERASE;
    cursor = (uint64_t)pc;
    
}

int32_t _term_terminal_putstring(void * dev, char *str, uint32_t length){
    // int _bit_if = interrupt_disable();
    int32_t counter = 0;
    while (counter++ < length)
    {
        uint16_t * ptr = (uint16_t*)cursor;
        switch (*str)
        {
        case NUL:
            return counter;
        case BS:
            ascii_bs();
            break;
        case HT:
            ascii_ht();
            break;
        case LF:
            ascii_cr();
            ascii_lf();
            break;
        case CR:
            ascii_cr();
            break;
        
        default:
            if (x >= WIDTH)
            {
                x -= WIDTH;
                cursor -= ROW_SIZE;
                ascii_lf();
            }
            
            *ptr = vga_entry(*str, color);
            ptr++;
            cursor += 2;
            x++;
            break;
        }
        str++;
    }
    // 由于最后一次输出字符 pc 值加 1,所以要减去
    set_cursor_location();
    // set_interrupt_access(_bit_if);
    return counter;
}


void _term_init(int col, int row){
    // WIDTH = col;
    // HEIGHT = row;
    uint16_t * p1 = (uint16_t *)cursor;
    uint64_t p2 = cursor;
    uint16_t * p3 = (uint16_t *)p2;
    
    _term_clear_terminal();
    _term_terminal_putstring(NULL, "Hello", 6);

    // device_install(
    //     DEV_CHAR, DEV_CONSOLE,
    //     NULL, "console", 0,
    //     NULL, NULL, terminal_putstring);
}
