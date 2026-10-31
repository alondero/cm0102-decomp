#include "cmre.h"

/* cdecl, two stack arguments: p1 is a pointer to a dword index, value is the
   dword to match. Global 0x00ae2364 is a signed count; the table base is the
   dword at 0x00ae2d0c and each record is 0x4f bytes. The index is re-read and
   the table base re-read before each final store, as in the listing. A record
   dword at +0x37 matching the value stores -1 at +0x27 and at the table
   record's +0x37; otherwise a dword at +0x4b matching stores -1 at +0x3b and
   at the table record's +0x4b. */

void CM_CDECL cm_fn_00549d60(u32 p1, u32 value)
{
    u32 index;
    u32 base;
    u32 record;

    if (p1 == 0u) {
        return;
    }
    index = *(volatile u32 *)p1;
    if ((s32)index >= CM_GLOBAL(volatile s32, 0x00ae2364)) {
        return;
    }

    base = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    record = base + index * 0x4fu;
    if (*(volatile u32 *)(record + 0x37u) == value) {
        *(volatile u32 *)(record + 0x27u) = 0xffffffffu;
        index = *(volatile u32 *)p1;
        base = CM_GLOBAL(volatile u32, 0x00ae2d0c);
        *(volatile u32 *)(base + index * 0x4fu + 0x37u) = 0xffffffffu;
        return;
    }
    if (*(volatile u32 *)(record + 0x4bu) == value) {
        *(volatile u32 *)(record + 0x3bu) = 0xffffffffu;
        index = *(volatile u32 *)p1;
        base = CM_GLOBAL(volatile u32, 0x00ae2d0c);
        *(volatile u32 *)(base + index * 0x4fu + 0x4bu) = 0xffffffffu;
    }
}
