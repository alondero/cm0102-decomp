/* CM0102 3.9.68, preferred-image VA0045cb60, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All229 indexed callers discard EAX high16 before observation. Keep the
   original CALL and copied18stackargs; original55f990 remains a boundary. */
#include "cmre.h"

extern u16 CM_THISCALL cm_fn_0055f990(u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9, u32 a10, u32 a11, u32 a12, u32 a13, u32 a14, u32 a15, u32 a16, u32 a17, u32 a18);

__attribute__((optimize("no-optimize-sibling-calls")))
u16 CM_THISCALL cm_fn_0045cb60(u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9, u32 a10, u32 a11, u32 a12, u32 a13, u32 a14, u32 a15, u32 a16, u32 a17, u32 a18)
{
    s16 index = *(volatile s16 *)(self + 0x382au);
    u32 receiver = *(volatile u32 *)(self + 0x7fcu + (u32)(s32)index * 0x300u);
    return cm_fn_0055f990(receiver, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
}
