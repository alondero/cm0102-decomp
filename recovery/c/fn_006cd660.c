#include "cmre.h"

void CM_FASTCALL cm_fn_006cd660(void *object, u32 edx_in)
{
    volatile u8 *entry;
    volatile u32 *table;
    volatile u16 *word;
    s8 bl = 0x0c;
    s8 dl = -1;
    u32 i;
    s8 age;
    s8 index;
    u32 pointer;
    s8 value;

    (void)edx_in;
    entry = (volatile u8 *)object + 0x4796;
    for (i = 0; i != 0x14; ++i, entry += 0x1be) {
        age = (s8)entry[0x19];
        if (age < 0)
            continue;
        if (age <= 10) {
            index = (s8)entry[0x27];
            table = (volatile u32 *)((volatile u8 *)object + 0x8ebc) + index;
            pointer = *table;
            word = (volatile u16 *)pointer + age;
            if (*word == 1)
                continue;
        }
        value = (s8)entry[0x103];
        if (bl > value) {
            bl = value;
            *(volatile u32 *)((volatile u8 *)object + 0xf58e) = (u32)entry;
        }
        age = (s8)entry[0x19];
        if (age < 0)
            continue;
        if (age <= 10) {
            index = (s8)entry[0x27];
            table = (volatile u32 *)((volatile u8 *)object + 0x8ebc) + index;
            pointer = *table;
            word = (volatile u16 *)pointer + age;
            if (*word == 1)
                continue;
        }
        value = (s8)entry[0x103];
        if (dl < value) {
            dl = value;
            *(volatile u32 *)((volatile u8 *)object + 0xf596) = (u32)entry;
        }
    }

    bl = -1;
    dl = 0x0c;
    entry = (volatile u8 *)object + 0x6a6e;
    for (i = 0; i != 0x14; ++i, entry += 0x1be) {
        age = (s8)entry[0x19];
        if (age < 0)
            continue;
        if (age <= 10) {
            index = (s8)entry[0x27];
            table = (volatile u32 *)((volatile u8 *)object + 0x8ebc) + index;
            pointer = *table;
            word = (volatile u16 *)pointer + age;
            if (*word == 1)
                continue;
        }
        value = (s8)entry[0x103];
        if (bl < value) {
            bl = value;
            *(volatile u32 *)((volatile u8 *)object + 0xf592) = (u32)entry;
        }
        age = (s8)entry[0x19];
        if (age < 0)
            continue;
        if (age <= 10) {
            index = (s8)entry[0x27];
            table = (volatile u32 *)((volatile u8 *)object + 0x8ebc) + index;
            pointer = *table;
            word = (volatile u16 *)pointer + age;
            if (*word == 1)
                continue;
        }
        value = (s8)entry[0x103];
        if (dl > value) {
            dl = value;
            *(volatile u32 *)((volatile u8 *)object + 0xf59a) = (u32)entry;
        }
    }
}
