/* Preferred-image VA 0x008f41b0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_007fe600(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 extra);

unsigned long long CM_CDECL cm_fn_008f41b0(u32 first, u32 unused, u32 mode)
{
    unsigned long long result = cm_fn_007fe600(0x00b74450u, 0x008f4210u,
                                               0x008f44d0u, 1, 0, 0);
    (void)unused;
    if ((u32)result) {
        u32 flag = (u8)mode == 1;
        u32 value = CM_GLOBAL(volatile u32, first);
        cm_fn_007ff270(0x00b74450u, 0, value, 0);
        result = cm_fn_007ff270(0x00b74450u, 1, flag, 0);
    }
    return result;
}
