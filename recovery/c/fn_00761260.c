#include "cmre.h"
/* Preferred-image VA 0x00761260, image base 0x00400000, CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/EDX/ECX positions precede the one stack word (RET4).
   EDX high bits survive the two original DX writes. */
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0055f990(
    u32 call_eax, u32 call_edx, u32 receiver,
    u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5,
    u32 a6, u32 a7, u32 a8, u32 a9, u32 a10, u32 a11,
    u32 a12, u32 a13, u32 a14, u32 a15, u32 a16, u32 a17);

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_00761260(
    u32 incoming_eax, u32 incoming_edx, u32 self, volatile u32 argument)
{
    u32 arg = argument;
    u32 first_word = CM_GLOBAL(volatile u16, 0x00aebdea);
    u32 second_word = CM_GLOBAL(volatile u16, 0x00aebdec);
    u32 third_word = CM_GLOBAL(volatile u16, 0x00ae31aa);
    u32 high_edx = incoming_edx & 0xffff0000u;
    u32 index = (u32)(s32)CM_GLOBAL(volatile s16, self + 0x382au);
    u32 offset = index * 0x300u;
    u32 receiver = CM_GLOBAL(volatile u32, self + offset + 0x7fcu);
    (void)incoming_eax;
    return cm_fn_0055f990(offset, high_edx | third_word, receiver,
        1u, 0u, 0u, 0u, 0u, 0u, 0u, 0x01000010u,
        high_edx | third_word, (arg & 0xffff0000u) | second_word,
        1u, 1u, high_edx | first_word, 0u, 0u, 0u, 0u, arg);
}
