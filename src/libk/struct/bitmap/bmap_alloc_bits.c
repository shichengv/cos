#include "../../include/struct/bitmap.h"
#include "../../include/string.h"

/*  从位图中申请连续的空闲count位
    map:    目标位图
    count:  申请的位数   */
int _bmap_alloc_bits(bitmap_t *map, uint32_t count){
    int32_t start_bit = -1;                    // 目标开始的位置
    uint32_t bits_left = map->length * 8;   // 剩余的位数
    uint32_t next_bit = 0;
    uint32_t counter = 0;                   // 计数器 

    while (bits_left-- > 0)
    {
        /* 如果 为0, 增加计数器， 否则设置为0*/
        if (!_bmap_test_bit(map, map->bmap_offset + next_bit))
            counter++;
        else
            counter = 0;

        /* index++ */
        next_bit++;

        // 找到数量一致，则设置开始的位置，结束
        if (counter == count )
        {
            start_bit = next_bit - count;
            break;
        }
    }

    /*如果没找到，就返回-1*/
    if (start_bit == -1)
        return -1;
    
    // 否则将找到的位, 全部置为1
    bits_left = count;
    next_bit = start_bit;
    while (bits_left--)
    {
        _bmap_set_bit(map, map->bmap_offset + next_bit, 1);
        next_bit++;
    }

    return start_bit + map->bmap_offset;

}