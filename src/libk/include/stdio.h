#ifndef _STDIO_H
#define _STDIO_H 1

#include "./sys/cdefs.h"
#include "stdarg.h"

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *fmt, ...);
int printk(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
