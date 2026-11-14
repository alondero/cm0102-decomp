#include "cmre.h"

/* Callee 0x0055f990 takes its first three words in EAX/EDX/ECX and eighteen
   stack words (RET 0x48). */
extern u32 CM_STDCALL __attribute__((regparm(3)))
    cm_fn_0055f990(u32 incoming_eax, u32 incoming_edx, u32 self,
    volatile u32 a0, volatile u32 a1, volatile u32 a2, volatile u32 a3,
    volatile u32 a4, volatile u32 a5, volatile u32 a6, volatile u32 a7,
    volatile u32 a8, volatile u32 a9, volatile u32 a10, volatile u32 a11,
    volatile u32 a12, volatile u32 a13, volatile u32 a14, volatile u32 a15,
    volatile u32 a16, volatile u32 a17);

u32 CM_THISCALL cm_fn_00804030(u32 self, u32 arg1, u32 arg2, u32 arg3,
    u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10,
    u32 arg11, u32 arg12)
{
    s32 index = (s32)*(const s16 *)(self + 0x382au);
    u32 offset = ((u32)index * 3u) << 8;
    u32 receiver = *(const u32 *)(self + 0x7fcu + offset);

    return cm_fn_0055f990((u32)index, offset, receiver,
        2u, arg1, arg2, arg3, arg4, 0u, 0u, arg5, arg6, arg6, arg7, arg8,
        arg9, arg10, 0u, arg11, arg12, 0xffffffffu);
}
