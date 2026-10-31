/* Preferred-image VA 007ff8f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   EDX high16 survives the first branch's MOV DX before the full argument push.
   FASTCALL names that register input while retaining the original RET12. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_007ff630(u32 code);

u32 CM_FASTCALL cm_fn_007ff8f0(u32 self, u32 incoming_edx,
                               u32 callback, u32 arg2, u32 replacement)
{
    u32 offset, code;
    if (replacement != 0) {
        D(self + 0x2627a0u) = 0;
        D(self + 0x2627a4u) = arg2;
        offset = (u32)(s32)(s16)W(self + 0x382au);
        offset = (offset + offset * 2u) << 8;
        code = W(self + offset + 0x7d4u);
        code |= incoming_edx & 0xffff0000u;
        (void)cm_fn_007ff630(code);
        offset = (u32)(s32)(s16)W(self + 0x382au);
        offset = (offset + offset * 2u) << 8;
        D(self + offset + 0x806u) = replacement;
        D(self + 0x2627a0u) = callback;
        return offset;
    }
    D(self + 0x2627a4u) = arg2;
    D(self + 0x2627a0u) = callback;
    offset = (u32)(s32)(s16)W(self + 0x382au);
    offset = (offset + offset * 2u) << 8;
    code = W(self + offset + 0x7d4u);
    code |= callback & 0xffff0000u;
    return cm_fn_007ff630(code);
}
