/* Preferred-image VA 0x00766f40, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern void CM_THISCALL cm_fn_00412ef0(u32 *dst);
void CM_THISCALL cm_fn_00766f40(u32 *dst)
{
    *(volatile u32 *)dst = 0x0096da00u;
    cm_fn_00412ef0(dst);
}
