#include "../include/stdio.h"
#include "../include/stdarg.h"
#include "../include/stdint.h"

int printf(const char *fmt, ...)
{
    va_list args;
    int i;

    char buf[1024];

    va_start(args, fmt);

    i = vsprintf(buf, fmt, args);

    va_end(args);

    // write(stdout, buf, i);


    return i;
}
