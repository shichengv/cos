#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "../../kernel/types.h"
#include "../../../libk/include/stdint.h"

#define CRT_INDEX_REG 0x3D4 // CRT control regiser
#define CRT_DATA_REG 0x3D5 // CRT data regiser

// CRT寄存器的 Cursor Location Register 是一个用来指定光标位置的寄存器
// 改变光标在屏幕上的位置，从而实现光标移动
#define CURSOR_LCL_H_REG_I 0xE // cursor location high register
#define CURSOR_LCL_L_REG_I 0xF // cursor location low register

// CRT寄存器的 Start Address Register 是一个
// 用来指定屏幕左上角像素或字符的显示内存地址的寄存器
#define START_ADDR_H_REG_I 0xC // start address high register
#define START_ADDR_L_REG_I 0xD // start address low register

#define MEM_BASE_ADDR 0xB8000
#define MEM_SIZE 0x4000
#define MEM_END_ADDR (MEM_BASE_ADDR + MEM_SIZE)

// ASCII
#define NUL 0x00 // null
#define ENQ 0x05 // enquiry
#define ACK 0x06 // acknowledge
#define BEL 0x07 // bell
#define BS 0x08 // backspace
#define HT 0x09 // horizontal tab
#define LF 0x0A // NL line feed, new line
#define VT 0x0B // vertical tab
#define FF 0x0C // NP form feed, new page
#define CR 0x0D // carriage return 
#define SI 0x0F // shift in

#define ERASE 0x0720 // SPACE

/* 硬件文本模式颜色常量。*/
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

void _term_clear_terminal();
int32_t _term_terminal_putstring(void * dev, char *str, uint32_t length);
void _term_init(int col, int row);


#endif
