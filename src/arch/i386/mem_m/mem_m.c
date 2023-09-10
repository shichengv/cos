#include "./include/mem_m.h"
#include "./include/paging.h"
#include "../include/machine_info.h"
#include "../../../libk/include/stdio.h"
#include "../../../libk/include/string.h"
#include "../../../libk/include/stdlib.h"
#include "../../../libk/include/debug.h"
#include "../../../libk/include/struct/bitmap.h"

extern struct machine_info _mac_if;

static bitmap_t _mem_m_user_bitmap;


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
static ptr_t get_ppage(){
    ptr_t paddr = _bmap_alloc_bits(&_mem_m_user_bitmap, 1);
    _mem_m_page_ref_count[paddr - _mem_m_user_bitmap.bmap_offset] = 1;
    assert(_mem_m_free_pages);
    _mem_m_free_pages--;
    paddr <<= 12;
    DEBUGK("System Debug:\n\tGET A Physics Page, Address: 0x%p\n", paddr);
    return paddr;
}
/*  释放物理页  */
static int free_ppage(ptr_t paddr){
    ptr_t paddr_page_offset = paddr >> 12;
    /*  如果释放一个无效页，代码有问题，需要排查    */
    assert(_mem_m_page_ref_count[paddr_page_offset - _mem_m_user_bitmap.bmap_offset] > 0);
    _mem_m_page_ref_count[paddr_page_offset - _mem_m_user_bitmap.bmap_offset]--;
    /*  如果该页的引用为0, 释放该页 */
    if (!_mem_m_page_ref_count[paddr_page_offset - _mem_m_user_bitmap.bmap_offset]){
        _bmap_set_bit(&_mem_m_user_bitmap, paddr_page_offset, 0);
        _mem_m_free_pages++;
        DEBUGK("System Debug:\n\tPUT A Physics Page, Address: 0x%p\n", paddr);
        return 1;
    }
    return 0;
}


__attribute__((always_inline)) inline void 
_mem_m_set_cr3(ptr_t pdbr){
    __asm__ __volatile__ ("movl %0, %%cr3\n" :: "a"(pdbr));
}

static void flush_tlb(uint32_t vaddr)
{
    /*  By specifying "memory" as a clobber, you tell the compiler that 
        the inline assembly code modifies memory in some way, so it can 
        take appropriate precautions to ensure that memory is not cached 
        or optimized in a way that would interfere with the inline assembly code.  */
    __asm__ volatile("invlpg (%0)" ::"r"(vaddr)
            : "memory");
}

static Page_Directory * get_pd_addr(){
    return (Page_Directory *)0x0;
}

static Page_Table * get_pte_addr(ptr_t vaddr){
    ptr_t pd_offset = vaddr >> 22;
    Page_Directory * dst_pt = (Page_Directory *)(sizeof(Page_Directory) * pd_offset);
    ptr_t dst_pt_addr = dst_pt->pt_referenced << 12;

    
    ptr_t pt_offset = (vaddr << 10) >> 22;
    Page_Table * dst_pte = (Page_Table *)( dst_pt_addr +  (sizeof(Page_Table) * pt_offset));

    return dst_pte;
}
/*  初始化内存  
ardc_addr: 保存ard结构体数量的地址，其后紧跟ard结构体   */
void _mem_m_init(ptr_t ardc_addr){
    /*  位图缓冲区占用的字节数  */
    uint32_t user_bmap_buffer_size = 0;
    ARDes_t * ard = (ARDes_t *)(ardc_addr + 4);
    _mem_m_reserved_mem = format_2_page_addr((ard++)->size) - PAGE_SIZE;

    for (size_t i = 0; i < *(uint32_t*)ardc_addr; i++, ard++)
        if (ard->type == 1)
            _mem_m_memory_size += ard->size;

    _mem_m_memory_size -= (AVAILABLE_MEM_END);

    _mem_m_free_pages = _mem_m_memory_size >> 12;

    printk("Reserved RAM Size: 0x%p\n", _mem_m_reserved_mem);
    printk("Total Free Pages: 0x%p\n", _mem_m_free_pages);

    user_bmap_buffer_size = _mem_m_free_pages >> 3;
    /*  设置内存缓冲区存储地址  */
    _mem_m_user_bitmap.bits = (uint8_t *)MEM_BMAP_ADDR;
    /*  更新保留内存可用地址的开始处    */
    _mem_m_reserved_mem_start = (ptr_t)(MEM_BMAP_ADDR + format_2_page_addr(user_bmap_buffer_size + 0xFFF));


    /*  初始化内存位图  */
    _bmap_init(&_mem_m_user_bitmap, _mem_m_user_bitmap.bits, user_bmap_buffer_size, AVAILABLE_MEM_END >> 12);

    /*  设置内存引用计数数组存储地址    */
    _mem_m_page_ref_count = (uint8_t*)(PAGE_REF_ADDR);
    /*  每个页最多被引用255次，用一个uint8_t 类型存储   */
    memset(_mem_m_page_ref_count, 0, _mem_m_free_pages * 1);


    /*  内存映射
     *  Virtual Addr 0x0 ~ 0x4000000 be mapped Physics Addr 0x0 ~ 0x4000000*/
    Page_Directory * pde = 0x0;
    memset(pde, 0, PAGE_SIZE);
    ptr_t pt_start = KERNEL_PDT_ADDR+PAGE_SIZE;
    int loopc = AVAILABLE_MEM_END >> 22;
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
