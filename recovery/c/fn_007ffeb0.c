#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x380a: table of 16-bit signed counts, one per outer index (0..15).
   0x7d6:  48-byte records addressed by (outer << 4) + inner; a dword at
           record + 0x00 is compared with the first stack argument.
   0x7f2:  dword at record + 0x1c, written with the second stack argument
           when the compare matches.

   The original has no explicit return value. The match path returns the
   record offset left in EAX, and the not-found path returns EAX as the
   last outer iteration left it. reg_eax models that register. */

u32 CM_THISCALL cm_fn_007ffeb0(u32 self, u32 key, u32 value)
{
    u32 reg_eax;
    u16 outer;
    u16 inner;
    s16 count;
    u32 row;

    outer = 0u;
    do {
        reg_eax = (u32)(s32)(s16)outer;
        count = (s16)*(volatile u16 *)(self + 0x380au + reg_eax * 2u);
        if (count > 0) {
            reg_eax = reg_eax << 4;
            row = reg_eax;
            inner = 0u;
            do {
                reg_eax = ((u32)(s32)(s16)inner + row) * 48u;
                if (*(volatile u32 *)(self + reg_eax + 0x7d6u) == key) {
                    *(volatile u32 *)(self + reg_eax + 0x7f2u) = value;
                    return reg_eax;
                }
                inner = (u16)(inner + 1u);
            } while ((s16)inner < count);
        }
        outer = (u16)(outer + 1u);
    } while ((s16)outer < 0x10);
    return reg_eax;
}
