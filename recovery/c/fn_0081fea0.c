/* Preferred-image VA 0x0081fea0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_007fe600(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 extra);

unsigned long long CM_CDECL cm_fn_0081fea0(void)
{
    unsigned long long result = cm_fn_007fe600(0x00b74450u, 0x0081ff10u,
                                               0x00820950u, 0, 0, 0);
    if ((u32)result) {
        result = cm_fn_007ff270(0x00b74450u, 0, 0, 0);
        result = cm_fn_007ff270(0x00b74450u, 1, 0, 0);
        result = cm_fn_007ff270(0x00b74450u, 2, 0, 0);
        result = cm_fn_007ff270(0x00b74450u, 3, 0, 0);
        result = cm_fn_007ff270(0x00b74450u, 4, 0, 0);
    }
    return result;
}
