/* Preferred-image VA 0x007fffa0, image base 0x00400000.
 * Primary SHA-256:
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 *
 * Physical entry contract, not four original stack arguments:
 * GCC i386 regparm(3) takes entry_eax in EAX, ignored_edx in EDX,
 * self in ECX; key remains at entry ESP+4. stdcall pops that one
 * stack word (RET 4). Existing original thiscall callers are unchanged.
 * EDX is not an original input; its formal merely occupies its ABI slot.
 * At VA 0x007fffaa a null key bypasses XOR EAX,EAX (0x007fffac).
 * OR AX,0xffff at 0x007fffe6 preserves incoming EAX[31:16].
 * Explicit ABI formals transport that value without an undefined-return
 * helper or register-allocation assumptions.
 */
#include "cmre.h"

u32 __attribute__((regparm(3), stdcall))
cm_fn_007fffa0(u32 entry_eax, u32 ignored_edx, u32 self, u32 key)
{
    u32 outer;
    (void)ignored_edx;

    if (key == 0u)
        return entry_eax | 0xffffu;

    for (outer = 0u; outer < 16u; ++outer) {
        /* VA 0x007fffb3: signed word count. */
        s32 count = *(volatile s16 *)(self + 0x380au + outer * 2u);

        if (count > 0) {
            /* VA 0x007fffd0: DWORD keys, 48-byte rows, 16 rows/slot. */
            volatile u8 *row = (volatile u8 *)(self + 0x7d6u + outer * 768u);
            volatile u8 *end = row + (u32)count * 48u;

            do {
                if (*(volatile u32 *)row == key)
                    return outer;
                row += 48;
            } while (row != end);
        }
    }

    return 0xffffu;
}
