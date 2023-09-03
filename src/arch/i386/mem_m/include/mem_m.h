#ifndef _MEM_M_H
#define _MEM_M_H 1
#include "../../include/types.h"

/*  Kernel Page-Directory Table address */
#define KERNEL_PDT_ADDR 0x0
#define PTE_NR 1024

/*  内存位图存储地址    */
#define MEM_BMAP_ADDR 0x20000
/*  内核代码&数据开始处 */
#define KERNEL_CODE_START 0x100000
/*  可分配区域  */
#define AVAILABLE_MEM_START 0x1000000

#define format_2_page_addr(addr) (((addr) >> 12) << 12)

#define PAGE_SIZE 0x1000

/*  Address Range Descriptor Structure  ACPI 3.0    */
typedef struct {
    unsigned long long base;
    unsigned long long size;
    uint32_t type;
    uint32_t extend;
} __attribute__((packed)) ARDes_t;

#endif
