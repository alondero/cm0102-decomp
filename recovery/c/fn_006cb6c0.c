/* Preferred-image VA 0x006cb6c0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/unused EDX/ECX prefix carries genuine entry EAX and self.
   Four native stack slots supply low bytes; both exits pop 16 bytes.
   Equal deltas replace AX only, retaining incoming EAX bits 16..31.
   Otherwise MOVSX/LEA defines EAX = 3*s8(a) before the table replaces AX. */
#include "cmre.h"

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006cb6c0(
    u32 incoming_eax, u32 incoming_edx, u32 self,
    u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u8 a = (u8)((u8)arg3 - (u8)arg1);
    u8 b = (u8)((u8)arg4 - (u8)arg2);
    s32 three_a;
    s32 index;
    (void)incoming_edx;
    if (a == 0 && b == 0)
        return (incoming_eax & 0xffff0000u)
             | *(volatile u16 *)(self + 0x8ed8u);
    three_a = 3 * (s32)(s8)a;
    index = (s32)(s8)b + 8 * three_a;
    return ((u32)three_a & 0xffff0000u)
         | *(volatile u16 *)(0x00b67c30u + (u32)index * 2u);
}
