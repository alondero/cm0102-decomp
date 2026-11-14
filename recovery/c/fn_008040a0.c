#include "cmre.h"

/* Preferred-image VA 0x008040a0, base 0x00400000, CM0102 3.9.68.
   The callee is the accepted regparm(3) routine: EAX/EDX/ECX carry the
   first three words, eighteen words follow on the stack (RET 0x48). */

extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0055f990(
    u32 incoming_eax, u32 incoming_edx, u32 self,
    volatile u32 a0, volatile u32 a1, volatile u32 a2, volatile u32 a3,
    volatile u32 a4, volatile u32 a5, volatile u32 a6, volatile u32 a7,
    volatile u32 a8, volatile u32 a9, volatile u32 a10, volatile u32 a11,
    volatile u32 a12, volatile u32 a13, volatile u32 a14, volatile u32 a15,
    volatile u32 a16, volatile u32 a17);

u32 CM_THISCALL cm_fn_008040a0(u32 self, u32 arg1, u32 arg2, u32 arg3,
    u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10,
    u32 arg11)
{
    /* 0x008040d9: movsx eax, word ptr [ecx + 0x382a] */
    s32 index = (s32)*(const volatile s16 *)(self + 0x382au);
    /* 0x008040e3: lea eax,[eax+eax*2]; 0x008040e8: shl eax,8 */
    u32 offset = ((u32)index * 3u) << 8;
    /* 0x008040ef: mov ecx, dword ptr [eax + ecx + 0x7fc] */
    u32 receiver = *(const volatile u32 *)(self + 0x7fcu + offset);

    return cm_fn_0055f990(offset, arg2, receiver,
        2u, 0u, 0u, 0u, 0u,
        arg2, arg3, arg4, arg5, arg5, arg6, arg7, arg8, arg9,
        0u, arg10, arg11, arg1);
}
