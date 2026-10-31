#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x380a: table of 16-bit signed counts, one per outer index (0..15).
   0x7d6:  48-byte records addressed by (outer << 4) + inner; a dword at
           record + 0x00 is compared with the stack argument.
   0x7ee:  dword at record + 0x18, returned when the compare matches. */

u32 CM_THISCALL cm_fn_007ffdd0(u32 self, u32 key)
{
    u16 outer;
    u16 inner;
    s16 count;
    u32 row;

    outer = 0u;
    do {
        count = (s16)*(volatile u16 *)(self + 0x380au + (u32)outer * 2u);
        if (count > 0) {
            row = (u32)(s32)((s32)outer << 4);
            inner = 0u;
            do {
                u32 rec = ((u32)(s32)(s16)inner + row) * 48u;
                if (*(volatile u32 *)(self + rec + 0x7d6u) == key) {
                    return *(volatile u32 *)(self + rec + 0x7eeu);
                }
                inner = (u16)(inner + 1u);
            } while ((s16)inner < count);
        }
        outer = (u16)(outer + 1u);
    } while ((s16)outer < 0x10);
    return 1u;
}
