#ifndef _PAGING_H
#define _PAGING_H 1

#include "../../include/types.h"

/*
页表条目中的 PWT 和 PCD 位是保护模式下的页表条目标志位之一。其中，PWT（Page Write Through）位用于控制缓存中数据的写入方式
，当 PWT=1 时，写入缓存的数据同时也会被写入内存；当 PWT=0 时，写入缓存的数据不会立即被写入内存，而是由 CPU 缓存控制器来决定
何时将缓存中的数据映射到内存中。PCD（Page Cache Disable）位用于控制是否将页面缓存在高速缓存中，当 PCD=1 时，禁止当前页写入
缓存，直接写入内存。
*/


/*  Format of a PML4 Entry (PML4E) that References a Page-Directory-Pointer Table*/
typedef struct PML4_Entry
{
    uint32_t present : 1;               //  Present
    uint32_t wirte : 1;                 //  Read/Write
    uint32_t user : 1;                  //  User/Supervisor
    uint32_t pwt : 1;                   //  Page-level write-through
    uint32_t pcd : 1;                   //  Page-level cache disable
    uint32_t accessed : 1;              //  indicates whether this entry has been used for 
                                        //  linear-address translation
    uint32_t ignored0 : 1;
    uint32_t ps : 1;                    //  Reserved (must be 0)
    uint32_t ignored1 : 3;              
    uint32_t R : 1;                     //  For ordinary paging, ignored; for HLAT paging, 
                                        //  restart (if 1, linear-address translation is 
                                        //  restarted with ordinary paging
    uint64_t pt_referenced : 40;        //  Page-Directory-Pointer-Table referenced
    uint32_t ignored2 : 11;
    uint32_t xd : 1;

} __attribute__((packed)) PML4_Entry;

void construct_

#endif
