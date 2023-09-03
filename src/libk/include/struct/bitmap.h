#ifndef _BITMAP_H
#define _BITMAP_H 1
#include "../stdint.h"
typedef struct
{
    uint8_t * bits;
    uint32_t length;
    uint32_t bmap_offset;
} bitmap_t ;

void _bmap_init(bitmap_t *map, uint8_t *bits, uint32_t length, uint32_t start);
int _bmap_test_bit(bitmap_t *map, uint32_t bit_offset);
void _bmap_set_bit(bitmap_t *map, uint32_t bit_offset, int value);
int _bmap_alloc_bits(bitmap_t *map, uint32_t count);

#endif