/* CM0102 3.9.68, preferred-image VA 006cfd10, base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The wrapper returns the original callee's full EAX unchanged. Preserve
   its CALL with copied stack arguments: the callee writes its score formal. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_006e9630(u32, u32, u32, u32, u32, u32, u32);

__attribute__((optimize("no-optimize-sibling-calls")))
u32 CM_THISCALL cm_fn_006cfd10(u32 self, u32 arg1, u32 arg2, u32 arg3,
    u32 arg4, u32 arg5, u32 arg6)
{
    return cm_fn_006e9630(self + 0xf5b2u, arg1, arg2, arg3,
                          arg4, arg5, arg6);
}
