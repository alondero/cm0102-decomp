#include "cmre.h"

u32 CM_THISCALL cm_fn_005cc3e0(volatile u8 *self, u32 index_a, u32 index_b)
{
    s8 count;
    s8 i;
    u32 records_address;
    volatile u32 *records;
    volatile u8 *record;
    u32 x;
    u32 y;
    s32 sentinel;
    s32 other;
    u32 base;
    u32 index;
    u32 offset;
    u32 pointer_a;
    u32 pointer_b;
    u32 key;
    s16 row_value;
    s16 sentinel_value;

    count = *(volatile s8 *)(self + 0x3506);
    if (count > 0) {
        records_address = *(volatile u32 *)(self + 0x3502);
        records = (volatile u32 *)records_address;
        i = 0;
        while (i < count) {
            record = (volatile u8 *)records + (s32)i * 8;
            x = *(volatile u32 *)record;
            if (x == index_a) {
                y = *(volatile u32 *)(record + 4);
                if (y == index_b)
                    return 1;
                y = *(volatile u32 *)(record + 4);
                if (y == 0xffffffffu)
                    return 1;
            }
            if (x == 0xffffffffu) {
                y = *(volatile u32 *)(record + 4);
                if (y == index_b)
                    return 1;
            }
            i = (s8)(i + 1);
        }
    }

    sentinel = CM_GLOBAL(volatile s32, 0x009cf224);
    base = CM_GLOBAL(volatile u32, 0x00ae23a8);
    other = CM_GLOBAL(volatile s32, 0x009cf3f4);
    index = index_a;
    if (index_a == (u32)sentinel || index_b == (u32)sentinel) {
        if (sentinel != 0)
            index = index_b;
        offset = index * 0x122u;
        pointer_a = *(volatile u32 *)(base + offset + 0x71);
        if (pointer_a != 0) {
            key = CM_GLOBAL(volatile u32, 0x009cfa18);
            x = *(volatile u32 *)pointer_a;
            other = CM_GLOBAL(volatile s32, 0x009cf3f4);
            if (x == key) {
                if ((s32)index != other)
                    return 0;
                goto fallback;
            }
        }
        if ((s32)index != other) {
            row_value = *(volatile s16 *)(base + offset + 0x8e);
            offset = (u32)sentinel * 0x122u;
            sentinel_value = *(volatile s16 *)(base + offset + 0x8e);
            if (row_value < sentinel_value)
                return 0;
        }
    }

fallback:
    offset = index * 0x122u;
    pointer_a = *(volatile u32 *)(base + offset + 0x71);
    if (pointer_a != 0) {
        x = *(volatile u32 *)pointer_a;
        key = CM_GLOBAL(volatile u32, 0x009cfa18);
        if (x == key) {
            if ((s32)index != sentinel && (s32)index != other) {
                offset = index_b * 0x122u;
                pointer_b = *(volatile u32 *)(base + offset + 0x71);
                if (pointer_b != pointer_a)
                    return 0;
            }
        }
    }
    offset = index_b * 0x122u;
    pointer_b = *(volatile u32 *)(base + offset + 0x71);
    if (pointer_b != 0) {
        key = CM_GLOBAL(volatile u32, 0x009cfa18);
        y = *(volatile u32 *)pointer_b;
        (void)key;
        (void)y;
    }
    return 0;
}
