#include "../../include/stdlib.h"
#include "../../include/struct/bitmap.h"
#include "../../include/string.h"

/*  检测map中bit_offset位是否有效   
    map:        目标位图
    bit_offset: 位偏移*/
int _bmap_test_bit(bitmap_t *map, uint32_t bit_offset){
    assert(bit_offset >= map->bmap_offset);

    /* 得到位图的索引 */
    uint32_t bitmap_idx = bit_offset - map->bmap_offset;
    /* 位图数组中的字节 */
    uint32_t byte_idx = bitmap_idx / 8;
    /* 该字节中的那一位 */
    uint8_t bit_odd = bitmap_idx % 8;
    
    assert(byte_idx < map->length);
    /* */
    return (map->bits[byte_idx] & (1 << bit_odd));
}