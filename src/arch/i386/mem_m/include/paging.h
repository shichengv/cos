#ifndef _PAGING_H
#define _PAGING_H 1

#include "../../include/types.h"

/*
页表条目中的 PWT 和 PCD 位是保护模式下的页表条目标志位之一。其中，PWT（Page Write Through）位用于控制缓存中数据的写入方式
，当 PWT=1 时，写入缓存的数据同时也会被写入内存；当 PWT=0 时，写入缓存的数据不会立即被写入内存，而是由 CPU 缓存控制器来决定
何时将缓存中的数据映射到内存中。PCD（Page Cache Disable）位用于控制是否将页面缓存在高速缓存中，当 PCD=1 时，禁止当前页写入
缓存，直接写入内存。
*/

/*  Use of CR3 with 32-Bit Paging   */
typedef struct 
{
    uint32_t ignored0 : 3;
    uint32_t pwt : 1;
    uint32_t pcd : 1;
    uint32_t ignored1 : 7;
    uint32_t pt_referened : 20;

} __attribute__((packed)) CR3;

/*  Format of a 32-Bit Page-Directory Entry that References a  Page Table   */
typedef struct 
{
    uint32_t present : 1;               //  Present
    uint32_t wirte : 1;                 //  Read/Write
    uint32_t user : 1;                  //  User/Supervisor
    uint32_t pwt : 1;                   //  Page-level write-through
    uint32_t pcd : 1;                   //  Page-level cache disable
    uint32_t accessed : 1;              //  indicates whether this entry has been used for 
                                        //  linear-address translation
    uint32_t ignored0 : 1;
    uint32_t ps : 1;                    //  If CR4.PSE = 1, must be 0 (otherwise, this entry 
                                        //  maps a 4-MByte page; see Table 4-4); otherwise, ignored
    uint32_t ignored1 : 4;              
    uint32_t pt_referenced : 20;        //  Page-Table referenced

} __attribute__((packed)) Page_Directory;

/*  Format of a 32-Bit Page-Table Entry that Maps a 4-KByte Page    */
typedef struct 
{
    uint32_t present : 1;               //  Present
    uint32_t wirte : 1;                 //  Read/Write
    uint32_t user : 1;                  //  User/Supervisor
    uint32_t pwt : 1;                   //  Page-level write-through
    uint32_t pcd : 1;                   //  Page-level cache disable
    uint32_t accessed : 1;              //  indicates whether this entry has been used for 
    uint32_t dirty : 1;                 //  Dirty; indicates whether software has written to 
                                        //  the 4-KByte page referneced by this entry
    uint32_t pat : 1;                   //  If the PAT is supported, indirectly determines the memory type
                                        //  used to access the 4-KByte page referenced by this entry Otherwise, reserved

    uint32_t global : 1;                //  if CR4.PGE = 1, determines whether the translation is global
    uint32_t ignored0 : 3;
    uint32_t pt_referenced : 20;        //  Physical address of the 4-KByte page referneced by this entry

} __attribute__((packed)) Page_Table;



/*  Construct Page-Directory Table  */
void _mem_m_construct_pdt(Page_Directory * pdt, bool write, bool user, ptr_t pt_referneced);
/*  Construct Page Table  */
void _mem_m_construct_pt(Page_Table * pt, bool write, bool user, ptr_t pt_referneced);


#endif
