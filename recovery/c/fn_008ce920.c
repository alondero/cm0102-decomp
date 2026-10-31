#include "cmre.h"

/* Object field by byte offset from the object pointer in ECX:
   0x86e holds the base address of a table of 8-byte entries. Entry fields used:
   0x03 (u16 word, updated by a left shift, and a byte OR/AND on its low byte)
   and 0x05 (u16 word, compared and then stored). The index is read through a
   pointer argument (first stack argument) on every use. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

void CM_THISCALL cm_fn_008ce920(u32 self, u32 key_ptr, u16 value)
{
    u32 table;
    u32 index;
    s16 diff;
    s32 remaining;

    table = OBJ_U32(0x86e);
    index = *(volatile u32 *)key_ptr;
    diff = (s16)(value - *(volatile u16 *)(table + index * 8u + 5u));
    if (diff > 0) {
        remaining = diff;
        do {
            index = *(volatile u32 *)key_ptr;
            table = OBJ_U32(0x86e);
            *(volatile u16 *)(table + index * 8u + 3u) =
                (u16)(*(volatile u16 *)(table + index * 8u + 3u) << 1);
            table = OBJ_U32(0x86e);
            index = *(volatile u32 *)key_ptr;
            *(volatile u8 *)(table + index * 8u + 3u) =
                (u8)(*(volatile u8 *)(table + index * 8u + 3u) & 0xfeu);
            remaining = remaining - 1;
        } while (remaining != 0);
    }

    index = *(volatile u32 *)key_ptr;
    table = OBJ_U32(0x86e);
    *(volatile u8 *)(table + index * 8u + 3u) =
        (u8)(*(volatile u8 *)(table + index * 8u + 3u) | 1u);
    index = *(volatile u32 *)key_ptr;
    table = OBJ_U32(0x86e);
    *(volatile u16 *)(table + index * 8u + 5u) = value;
}
