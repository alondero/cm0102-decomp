#include "cmre.h"

/* Index argument v (cdecl, one stack argument, not popped by callee).
   Globals: 0x009cf6b0, 0x009cf5d8 and 0x009cf548 hold three special indices
   that return 1 at once. Otherwise the table at [0x00ae23d0] has an entry at
   byte offset 0x6b*v + 0x5d that must be nonzero, and the element table at
   [0x00adadfc] gives an element pointer X (indexed by v, dword stride).
   Element X: byte at +0x43 must be 2, word at +0x3e nonzero, dword at +0xb1
   a pointer to a record Y whose dwords at +0x57 and +0x60 are tested against
   the dword at X+0x04. */

u32 CM_CDECL cm_fn_004b0700(u32 v)
{
    u32 base;
    u32 slot;
    u32 table;
    u32 element;
    u32 record;
    u32 value;

    if (v == CM_GLOBAL(volatile u32, 0x009cf6b0)) {
        return 1u;
    }
    if (v == CM_GLOBAL(volatile u32, 0x009cf5d8)) {
        return 1u;
    }
    if (v == CM_GLOBAL(volatile u32, 0x009cf548)) {
        return 1u;
    }

    base = CM_GLOBAL(volatile u32, 0x00ae23d0);
    slot = *(volatile u32 *)(base + v * 0x6bu + 0x5du);
    if (slot == 0u) {
        return 0u;
    }

    table = CM_GLOBAL(volatile u32, 0x00adadfc);
    element = *(volatile u32 *)(table + v * 4u);
    if (element == 0u) {
        return 0u;
    }
    if (*(volatile u8 *)(element + 0x43u) != 2u) {
        return 0u;
    }
    if (*(volatile u16 *)(element + 0x3eu) == 0u) {
        return 0u;
    }

    record = *(volatile u32 *)(element + 0xb1u);
    if (record == 0u) {
        return 0u;
    }
    record = *(volatile u32 *)record;
    value = *(volatile u32 *)(element + 0x04u);
    if (*(volatile u32 *)(record + 0x57u) == value) {
        return 1u;
    }
    if (*(volatile u32 *)(record + 0x60u) == value) {
        return 1u;
    }
    return 0u;
}
