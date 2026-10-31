/* Preferred-image VA 0x00934960, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009342c0(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00934380(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);
unsigned long long CM_CDECL cm_fn_00934960(u32 a, u32 b, u32 c, u32 d, u32 e, u32 flag)
{
    if ((u16)flag)
        return cm_fn_009342c0(a, b, c, 0, 1, 1, 0x47u, 0x72u, d, e);
    return cm_fn_00934380(a, b, c, 0, 1, 1, 0x47u, 0x72u, d, e);
}
