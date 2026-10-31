/* Preferred-image VA 0x009446a0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL __attribute__((regparm(1))) cm_fn_009446a0(
    u32 incoming_eax, u32 base, u32 count)
{
    u32 passes = count - 1u, cursor, remaining, changed;
    if ((s32)passes <= 0) return incoming_eax;
    do {
        changed = 0;
        cursor = base;
        remaining = passes;
        do {
            s16 next = *(volatile s16 *)(cursor + 2u);
            s16 current = *(volatile s16 *)cursor;
            if (next < current) {
                *(volatile u16 *)cursor = (u16)next;
                changed = 1;
                *(volatile u16 *)(cursor + 2u) = (u16)current;
            }
            cursor += 2u;
        } while (--remaining != 0);
    } while (changed);
    return cursor;
}
