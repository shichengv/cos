#include "../../include/struct/bitmap.h"
#include "../../include/string.h"
#include "../../include/stdlib.h"
/*  将bit_offset[位的偏移, 不是字节偏移]位的值设置位 value
    map:    目标位图
    index:  位偏移
    value:  0 or 1  */
void _bmap_set_bit(bitmap_t *map, uint32_t bit_offset, int value){

    assert(bit_offset >= map->bmap_offset);

    /* 得到位图的索引 */
    uint32_t bitmap_idx = bit_offset - map->bmap_offset;
    /* 位图数组中的字节 */
    uint32_t byte_idx = bitmap_idx / 8;
    /* 该字节中的那一位 */
    uint8_t bit_odd = bitmap_idx % 8;


    if (value)
        map->bits[byte_idx] |= (1 << bit_odd);
    else
        map->bits[byte_idx] &= ~(1 << bit_odd);
    
}