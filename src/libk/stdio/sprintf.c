#include "../include/stdio.h"
#include "../include/stdarg.h"
#include "../include/stdint.h"

// 结果按格式输出字符串到 buf
int sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int i = vsprintf(buf, fmt, args);
    va_end(args);
    return i;
}
