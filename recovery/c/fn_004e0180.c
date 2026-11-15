/* Preferred-image VA004e0180, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Reviewed/amended inherited task c-004e0180-fd87ead50bc98d2c; retained
   task source SHA2561f13f332eb05a0f53acd7baa6d8fcd4413f984644217489276fb438ec90f34fd.
   EAX=index, EDX=index*768, ECX=loaded receiver at0055f990; eighteen
   stack words, then RET8 here. The three WORD loads retain argument high
   halves. Original helper remains an explicit boundary. */
#include "cmre.h"

extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0055f990(
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_004e0180(u32 self, volatile u32 first, volatile u32 second)
{
    u32 a = first;
    u32 b = second;
    u32 word1 = (a & 0xffff0000u) | CM_GLOBAL(volatile u16, 0x00aebdeau);
    u32 word2 = (b & 0xffff0000u) | CM_GLOBAL(volatile u16, 0x00aebdecu);
    u32 word3 = (word1 & 0xffff0000u) | CM_GLOBAL(volatile u16, 0x00ae31aau);
    u32 index = (u32)(s32)CM_GLOBAL(volatile s16, self + 0x382au);
    u32 scaled = index * 768u;
    u32 receiver = CM_GLOBAL(volatile u32, self + scaled + 0x7fcu);
    return cm_fn_0055f990(index, scaled, receiver,
        2, 0, 0, 0, 0, 0, 0, 0x10, word3, word2, 0x21, 1,
        word1, b, 0, 0, 0, a);
}
