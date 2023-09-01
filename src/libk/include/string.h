#ifndef _STRING_H
#define _STRING_H 1

#include "sys/cdefs.h"

#ifndef _SIZE_T
#define _SIZE_T 1
typedef unsigned long size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);

#ifdef __cplusplus
}
#endif

#endif
