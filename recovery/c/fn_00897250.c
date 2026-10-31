#include "cmre.h"

/* Object (ECX) fields by byte offset: 0x5f0 flag byte (must be nonzero).
   Record i (0..15) lives at this + 0x5c*i: dword at +0x38 must be nonzero,
   dword at +0x30 is compared with the first stack argument, dword at +0x34
   with the second. Index i = (dword at 0x00ae2364) - (dword at P) - 1, where
   P = dword at 0x00b74c26 + 0x300 * (signed word at 0x00b77c7a). */

u32 CM_THISCALL cm_fn_00897250(u32 self, u32 a2, u32 a3)
{
    u32 base;
    u32 n;
    u32 index;
    u32 record;
    s32 w;

    if (*(volatile u8 *)(self + 0x5f0u) == 0u) {
        return 0u;
    }
    w = (s32)(s16)CM_GLOBAL(volatile u16, 0x00b77c7au);
    base = *(volatile u32 *)(0x00b74c26u + (u32)w * 0x300u);
    if (base == 0u) {
        return 0u;
    }
    n = CM_GLOBAL(volatile u32, 0x00ae2364u);
    index = n - *(volatile u32 *)base - 1u;
    if ((s32)index < 0) {
        return 0u;
    }
    if (index >= 0x10u) {
        return 0u;
    }
    record = self + index * 0x5cu;
    if (*(volatile u32 *)(record + 0x38u) == 0u) {
        return 0u;
    }
    if (*(volatile u32 *)(record + 0x30u) != a2) {
        return 0u;
    }
    if (*(volatile u32 *)(record + 0x34u) != a3) {
        return 0u;
    }
    return 1u;
}
