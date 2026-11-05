/* CM0102 3.9.68, preferred-image VA 006cfcd0, base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All six indexed callers overwrite full EAX before observing it. Preserve
   the original CALL with copied stack arguments: the callee writes formals. */
#include "cmre.h"
extern void CM_THISCALL cm_fn_006ea4e0(u32, u32, u32, u32, u32,
                                      u32, u32, u32, u32);

__attribute__((optimize("no-optimize-sibling-calls")))
void CM_THISCALL cm_fn_006cfcd0(u32 self, u32 arg1, u32 arg2, u32 arg3,
    u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8)
{
    cm_fn_006ea4e0(self + 0xf5b2u, arg1, arg2, arg3, arg4,
                   arg5, arg6, arg7, arg8);
}
