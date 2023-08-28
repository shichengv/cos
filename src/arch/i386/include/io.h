#ifndef _IO_H
#define  _IO_H

#include "../../../libk/include/stdint.h"

// 从 port 号里面读取一个字节
extern uint8_t inb(uint16_t port);
// 把 value[一个字节] 的值复制到 port 里面
extern void outb(uint16_t port, uint8_t value);

// 从 port 号里面读取一个字
extern uint16_t inw(uint16_t port);
// 把 value[一个字] 的值复制到 port 里面
extern void outw(uint16_t port, uint16_t value);

#endif
