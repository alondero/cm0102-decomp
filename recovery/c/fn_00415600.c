#include "cmre.h"
/* Preferred-image VA 0x00415600, base 0x00400000; CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The positional EAX parameter is overwritten by the original before use.
   EDX is genuine entry state: MOV DX preserves its upper word before PUSH EDX.
   ECX is the receiver; only argument is on the stack, with RET 4 cleanup.
   The original callee is an explicit boundary and returns full EAX. */
extern u32 CM_THISCALL cm_fn_0055fba0(
    u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6,
    u32 a7, u32 a8, u32 a9, u32 a10, u32 a11);

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_00415600(
    u32 incoming_eax, u32 incoming_edx, u32 self, volatile u32 argument)
{
    u32 saved_argument = argument;
    u32 color = (incoming_edx & 0xffff0000u)
        | CM_GLOBAL(volatile u16, 0x00ae31aau);
    u32 index = (u32)(s32)CM_GLOBAL(volatile s16, self + 0x382au);
    u32 receiver = CM_GLOBAL(volatile u32, self + 0x7fcu + index * 0x300u);
    (void)incoming_eax;
    return cm_fn_0055fba0(receiver, 0, 0, 0, 0, 0, 0, 0, 0,
                          0x30, color, saved_argument);
}
