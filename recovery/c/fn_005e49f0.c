#include "cmre.h"

/* cdecl, five stack arguments, no callee cleanup: a0 dword (its low word is
   the mask), a1..a3 byte output pointers, a4 table pointer (0 selects the
   global table at 0x00ae3158). The table dwords read are at offsets 0x10, 0x14
   and 0x18. Each output is ((dword & mask) << 8) / (dword + 1), low byte.
   Global 0x00aebe0c nonzero makes the routine return before any access. */

void CM_CDECL cm_fn_005e49f0(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4)
{
    u32 table;
    u32 mask;
    u32 value;

    if (CM_GLOBAL(volatile s32, 0x00aebe0c) != 0) {
        return;
    }

    table = a4;
    if (table == 0u) {
        table = 0x00ae3158u;
    }
    mask = a0 & 0xffffu;

    value = *(volatile u32 *)(table + 0x10u);
    *(volatile u8 *)a1 = (u8)(((value & mask) << 8) / (value + 1u));

    value = *(volatile u32 *)(table + 0x14u);
    *(volatile u8 *)a2 = (u8)(((value & mask) << 8) / (value + 1u));

    value = *(volatile u32 *)(table + 0x18u);
    *(volatile u8 *)a3 = (u8)(((value & mask) << 8) / (value + 1u));
}
