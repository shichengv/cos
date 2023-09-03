#include "./include/mem_m.h"
#include "./include/paging.h"
#include "../include/machine_info.h"
#include "../../../libk/include/stdio.h"
#include "../../../libk/include/string.h"
#include "../../../libk/include/stdlib.h"
#include "../../../libk/include/debug.h"
#include "../../../libk/include/struct/bitmap.h"

extern struct machine_info _mac_if;

static bitmap_t _mem_m_kernel_bitmap;

/*  真正可分配的物理内存    */
ptr_t _mem_m_memory_size = 0;

/*  保留内存大小   */
ptr_t _mem_m_reserved_mem = 0;
/*  保留内存可用地址的开始处 4Kib 对齐   */
ptr_t _mem_m_reserved_mem_start = 0;


/*  可用内存页数量  */
uint32_t _mem_m_free_pages = 0;
/*  内存页引用计数  */
uint8_t* _mem_m_page_ref_count;

/*  申请新的物理页  */
static ptr_t get_page(){
    ptr_t paddr = _bmap_alloc_bits(&_mem_m_kernel_bitmap, 1);
    _mem_m_page_ref_count[paddr - _mem_m_kernel_bitmap.bmap_offset]++;
    return paddr << 12;
}
/*  释放物理页  */
static void put_page(ptr_t paddr){
    /*  如果释放一个无效页，代码有问题，需要排查    */
    assert(_mem_m_page_ref_count[paddr - _mem_m_kernel_bitmap.bmap_offset] > 0);
    _mem_m_page_ref_count[paddr - _mem_m_kernel_bitmap.bmap_offset]--;
    /*  如果该页的引用为0, 释放该页 */
    if (!_mem_m_page_ref_count[paddr - _mem_m_kernel_bitmap.bmap_offset])
        _bmap_set_bit(&_mem_m_kernel_bitmap, paddr, 0);
    else
        return;
}

__attribute__((always_inline)) inline void 
_mem_m_set_cr3(ptr_t pdbr){
    __asm__ __volatile__ ("movl %0, %%cr3\n" :: "a"(pdbr));
}

/*  初始化内存  
    ardc_addr: 保存ard结构体数量的地址，其后紧跟ard结构体   */
void _mem_m_init(ptr_t ardc_addr){
    /*  位图缓冲区占用的字节数  */
    uint32_t bmap_buffer_size = 0;
    ARDes_t * ard = (ARDes_t *)(ardc_addr + 4);
    _mem_m_reserved_mem = (ard++)->size - PAGE_SIZE;

    for (size_t i = 0; i < *(uint32_t*)ardc_addr; i++, ard++)
        if (ard->type == 1)
            _mem_m_memory_size += ard->size;
    
    _mem_m_memory_size -= (AVAILABLE_MEM_START - KERNEL_CODE_START);
    _mem_m_free_pages = _mem_m_memory_size >> 12;
    printk("Reserved RAM Size: 0x%p\n", _mem_m_reserved_mem);
    printk("Free Pages: 0x%p\n", _mem_m_free_pages);

    bmap_buffer_size = _mem_m_free_pages >> 3;
    /*  设置内存缓冲区存储地址  */
    _mem_m_kernel_bitmap.bits = (uint8_t*)MEM_BMAP_ADDR;
    /*  设置内存引用计数数组存储地址    */
    _mem_m_page_ref_count = (uint8_t*)(MEM_BMAP_ADDR + format_2_page_addr(bmap_buffer_size + 0xFFF));
    /*  更新保留内存可用地址的开始处    */
    _mem_m_reserved_mem_start = (ptr_t)_mem_m_page_ref_count + format_2_page_addr(_mem_m_free_pages + 0xFFF);
    /*  每个页最多被引用255次，用一个uint8_t 类型存储   */
    memset(_mem_m_page_ref_count, 0, _mem_m_free_pages * 1);
    /*  初始化内存位图  */
    _bmap_init(&_mem_m_kernel_bitmap, _mem_m_kernel_bitmap.bits,  bmap_buffer_size, AVAILABLE_MEM_START >> 12);

    /*  内存映射    */

    Page_Directory * pde = 0x0;
    memset(pde, 0, PAGE_SIZE);
    ptr_t pt_start = KERNEL_PDT_ADDR+PAGE_SIZE;
    int loopc = AVAILABLE_MEM_START >> 22;
    int mapped_page = 0;

    for (int i = 0; i < loopc; pde++, i++) {
        _mem_m_construct_pdt(pde, 1, 0, pt_start >> 12);

        memset((void*)pt_start, 0, PAGE_SIZE);
        Page_Table * pt = (Page_Table*)pt_start;
        for (int j = 0; j < PTE_NR; j++, mapped_page++, pt++) 
            _mem_m_construct_pt(pt, 1, 0, mapped_page);
        pt_start += PAGE_SIZE;
    }

    _mem_m_set_cr3((ptr_t)pde);

    __asm__ __volatile__(
        "movl %cr0, %eax\n"
        "orl $0x80000000, %eax\n"
        "movl %eax, %cr0\n");

    return;
    
}
