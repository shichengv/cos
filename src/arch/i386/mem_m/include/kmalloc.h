#ifndef _KMALLOC_H
#define _KMALLOC_H 1
#include "../../include/types.h"
void mem_brk_init(void);

void * mem_sbrk(int incr);

int mm_init(void);
void * kmalloc(size_t size);
void kfree(void *bp);

#endif 
