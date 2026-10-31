#include "cmre.h"

/* Row table: base pointer, a row stride of 79 bytes (0x4f). The row count is the
   signed dword at 0x00ae2364. Output rows are 110 bytes (0x6e) from the base
   pointer at 0x00ae23bc. Row fields by byte offset from the row:
   0x27 index (0xffffffff means none), 0x37 key, 0x3b index, 0x4b key. */
#define ROW_U32(row, off) (*(volatile u32 *)((row) + (off)))

u32 CM_CDECL cm_fn_00549dd0(u32 indexp, u32 key)
{
    u32 index;
    u32 row;
    u32 found;
    u32 out;

    if (indexp == 0u) {
        return 0u;
    }
    index = *(volatile u32 *)indexp;
    if ((s32)index >= CM_GLOBAL(volatile s32, 0x00ae2364)) {
        return 0u;
    }
    row = CM_GLOBAL(volatile u32, 0x00ae2d0c) + index * 79u;
    if (ROW_U32(row, 0x37) == key) {
        found = ROW_U32(row, 0x27);
        if (found != 0xffffffffu) {
            out = CM_GLOBAL(volatile u32, 0x00ae23bc);
            return out + found * 110u;
        }
    }
    if (ROW_U32(row, 0x4b) == key) {
        if (ROW_U32(row, 0x27) != 0xffffffffu) {
            found = ROW_U32(row, 0x3b);
            out = CM_GLOBAL(volatile u32, 0x00ae23bc);
            return out + found * 110u;
        }
    }
    return 0u;
}
