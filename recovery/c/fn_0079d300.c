/* Preferred-image VA 0x0079d300, base 0x00400000; CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/EDX/ECX prefix preserves the genuine incoming EAX high word
   on the null argument path. EDX is positional only and unused natively. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_004b0700(u32 key);
/* Original boundary: only EAX is consumed; bound is at native ESP+4. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

__attribute__((regparm(3)))
u32 CM_STDCALL cm_fn_0079d300(u32 incoming_eax, u32 unused_edx,
                             u32 self, u32 arg1)
{
    u32 key, eax, block, row;
    u8 low;
    u8 found = 0;
    s32 count;
    (void)unused_edx;
    if (arg1 == 0)
        return incoming_eax & 0xffff0000u;
    key = *(volatile u32 *)arg1;
    eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    eax = *(volatile u32 *)(eax + key * 4u);
    if (eax == 0)
        return eax & 0xffff0000u;
    low = *(volatile u8 *)(eax + 0x42u);
    eax = (eax & 0xffffff00u) | low;
    if (low == 4 || low == 2)
        return eax & 0xffff0000u;
    eax = cm_fn_004b0700(key);
    if (eax == 0)
        return eax & 0xffff0000u;
    eax = *(volatile u32 *)(arg1 + 0x5du);
    if (eax != 0)
        eax = *(volatile u32 *)eax;
    else
        eax = cm_fn_0090cfc0(CM_GLOBAL(volatile u32, 0x00ae2350));
    block = *(volatile u32 *)(self + 0xcu);
    block += eax * 0x102u;
    count = *(volatile s16 *)(block + 0x10u);
    if (count > 0) {
        eax = arg1; /* Native stack argument reload at VA0079d36d. */
        row = block + 0x16u;
        do {
            if (found == 0) {
                u32 row_key = *(volatile u32 *)(row + 8u);
                key = *(volatile u32 *)arg1;
                if (row_key == key) {
                    eax = *(volatile u32 *)row;
                    found = 1;
                }
            }
            row += 0x10u;
            --count;
        } while (count != 0);
    }
    if (found == 0)
        eax = *(volatile u32 *)(block + 4u);
    if ((s32)eax > 100)
        eax = (eax & 0xffff0000u) | 100u;
    return eax;
}
