#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   the object holds 16 consecutive u32 row pointers at offsets 0x00..0x3c (loop
   runs 16 times). Each non-null row with a non-zero word at row+0x04 gets the
   byte at (row+0x00)[index * 9] cleared. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

/* Global flag byte and signed index limit. */
#define FLAG_BYTE() CM_GLOBAL(volatile u8, 0x009cc270)
#define INDEX_LIMIT() CM_GLOBAL(volatile s32, 0x00ae2364)

/* One stack argument (ret 4): a pointer to a signed index value.
   Returns nothing; a null pointer, an index at or above the limit, or a clear
   flag byte leaves the object untouched. */
void CM_THISCALL cm_fn_005afd00(u32 self, u32 arg1)
{
    s32 index;
    s32 limit;
    u32 offset;
    u32 remaining;

    if (arg1 == 0) {
        return;
    }
    index = (s32)*(volatile u32 *)arg1;
    limit = INDEX_LIMIT();
    if (index >= limit) {
        return;
    }
    if (FLAG_BYTE() == 0) {
        return;
    }

    offset = 0;
    remaining = 16u;
    do {
        u32 row = OBJ_U32(offset);
        if (row != 0 && *(volatile u32 *)(row + 0x04) != 0) {
            index = (s32)*(volatile u32 *)arg1;
            if (index < INDEX_LIMIT() && FLAG_BYTE() != 0) {
                u32 base = *(volatile u32 *)row;
                *(volatile u8 *)(base + (u32)index * 9u) = 0;
            }
        }
        offset += 4u;
        remaining--;
    } while (remaining != 0);
}
