#include "cmre.h"

/* Row table: base pointer at 0x00ae2d0c, row stride 79 bytes (0x4f), row count
   the signed dword at 0x00ae2364. Row fields by byte offset: 0x27 index
   (0xffffffff means none), 0x37 value, 0x3b index, 0x4b value.
   Arguments: p1 and p2 point to dword indexes (both must be non-null and below
   the count); val is stored in the row selected by *p1. The function returns no
   value in any path that this recovery models. */
void CM_CDECL cm_fn_00549cf0(u32 p1, u32 p2, u32 val)
{
    u32 idx1;
    u32 idx2;
    u32 count;
    u32 row;
    u32 base;

    if (p1 == 0u) {
        return;
    }
    idx1 = *(volatile u32 *)p1;
    count = CM_GLOBAL(volatile u32, 0x00ae2364);
    if ((s32)idx1 >= (s32)count) {
        return;
    }
    if (p2 == 0u) {
        return;
    }
    idx2 = *(volatile u32 *)p2;
    if ((s32)idx2 >= (s32)count) {
        return;
    }
    row = CM_GLOBAL(volatile u32, 0x00ae2d0c) + idx1 * 79u;
    if (*(volatile u32 *)(row + 0x27u) == 0xffffffffu) {
        *(volatile u32 *)(row + 0x27u) = idx2;
        idx1 = *(volatile u32 *)p1;
        base = CM_GLOBAL(volatile u32, 0x00ae2d0c);
        *(volatile u32 *)(base + idx1 * 79u + 0x37u) = val;
        return;
    }
    *(volatile u32 *)(row + 0x3bu) = idx2;
    idx1 = *(volatile u32 *)p1;
    base = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    *(volatile u32 *)(base + idx1 * 79u + 0x4bu) = val;
}
