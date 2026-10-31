/* Preferred-image VA 0x0094de8d, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00951552(u32 a, u32 b, volatile u32 *out_b,
                                   volatile u32 *out_a, u32 mode);
extern u32 CM_CDECL cm_fn_00951373(u32 mode, u32 app, u32 cmd, u32 env);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

u32 CM_CDECL cm_fn_0094de8d(u32 mode, u32 app, volatile u32 a, volatile u32 b)
{
    u32 result = cm_fn_00951552(a, b, &b, &a, app);
    if (result == 0xffffffffu) return result;
    result = cm_fn_00951373(mode, app, b, a);
    cm_fn_009452ca(b);
    cm_fn_009452ca(a);
    return result;
}
