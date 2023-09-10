/*  Aligned 16 bytes */

#include "./include/mem_m.h"
#include "./include/kmalloc.h"
#include "../../../libk/include/stdio.h"

#define WSIZE 4
#define DSIZE 8

#define CHUNKSIZE               ( 1 << 12 )


#define MAX(x, y)           ((x) < (y) ? (x) : (y))

#define PACK(size, allocated)   ((size) | (allocated))

#define GET(p)                  (*(uint32_t*)(p))
#define PUT(p, val)             (*(uint32_t*)(p) = (val))

#define HDRP(bp)                ((char *)(bp) - WSIZE)
#define FTRP(bp)                ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define GET_SIZE(p)             (GET(p) & ~0x7)
#define GET_ALLOC(p)            (GET(p) & 0x1)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)       ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)       ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

static char * heap_start = (char *)KM_HEAP_START;
static char * brk = (char *)KM_HEAP_START;
static char * heap_end = (char *)KM_HEAP_END;
static char * heap_listp;

static void *coalesce(void *bp){
    /* 记录上一个块的尾部的标志位 */
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    /* 记录下一个块的头部的标志位 */
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    
    if (prev_alloc && next_alloc) {             // 如果前一个块和后一个块都是已分配的
        return bp;
    } else if(prev_alloc && !next_alloc){      // 前一个块已分配，后一个块未分配
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    } else if(!prev_alloc && next_alloc){      // 前一个块未分配，后一个块已分配
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    } else {                                    // 前后的块都未分配
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
            GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    return bp;
}

/* 找到一个合适的块 */
static void *find_fit(size_t asize){

    void *bp;
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }
    return NULL;
}
/* 放置块 */
static void place(void *bp, size_t asize){
    size_t csize = GET_SIZE((HDRP(bp)));
    if ((csize - asize) >= (2 * DSIZE)){
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        /* 分割块 */
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
    }
    else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

/* 扩展堆 */
static void * extend_heap(size_t words){
    char *bp;
    size_t size;

    /* 双字对齐 */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    /* 申请新的堆空间 */
    if ((long)(bp = (char*)mem_sbrk(size)) == -1)
        return NULL;
    
    /* 设置申请的块的头 */
    PUT(HDRP(bp), PACK(size, 0));
    /* 设置申请的块的尾 */
    PUT(FTRP(bp), PACK(size, 0));
    /* New epilogue heander */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}


void * mem_sbrk(int incr){
    char * old_brk = brk;

    if (incr < 0 || (brk + incr ) > heap_end) {
        printk("mm_malloc error:\n\tmem_sbrk failed. Run out of memory...\n");
        return (void*)-1;
    }
    brk += incr;
    return (void *)old_brk;
}

int mm_init(void){
    
    if ((heap_listp = (char *)mem_sbrk(4 * WSIZE)) == (void*)-1) 
        return -1;

    // initialize heap_list
    /*  Header    */
    PUT(heap_listp, 0);
    /*  Prologue header & Prologue footer */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
    /*  Footer    */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));

    /* 设置堆指针，此时指向唯一的块（块大小为0) */
    heap_listp += (2 * WSIZE);

    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;

}


void * kmalloc(size_t size){
    size_t asize;
    size_t extendsize;
    char * bp;

    if (size == 0)
        return NULL;
    
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE)) + (DSIZE - 1) / DSIZE);
    
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;

    place(bp, asize);
    return bp;
    
}
void kfree(void *bp){
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

