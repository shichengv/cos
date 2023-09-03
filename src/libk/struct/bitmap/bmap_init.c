#include "../../include/struct/bitmap.h"
#include "../../include/string.h"

/*  初始化位图  
map:     位图结构的地址 
bits:    位图缓冲区的地址 
length:  位图缓冲区的长度 
start:   位图开始的位置 */
void _bmap_init(bitmap_t *map, uint8_t *bits, uint32_t length, uint32_t start){
    memset(bits, 0, length);
    map->bits = bits;
    map->length = length;
    map->bmap_offset = start;
}