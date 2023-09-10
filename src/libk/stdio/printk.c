#include "../include/stdio.h"
#include "../include/stdarg.h"
#include "../include/stdint.h"


extern int64_t _term_terminal_putstring(void * dev, char *str, uint32_t length);
int printk(const char *fmt, ...)
{
    va_list args;
    int i;

    char buf[1024];

    va_start(args, fmt);

    i = vsprintf(buf, fmt, args);

    va_end(args);

    _term_terminal_putstring(NULL, buf, i);

    return i;
}
