/* ADR 0006 reconstruction. Preferred-image VA 0x006fa140, image base
   0x00400000, primary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Question: can plain C pass the unchanged semantic gate when the compiler-inserted
   __ftol call at 006fa1ac (callee 00945640) is written as the C cast MSVC lowers to it?
   __ftol truncates ST0 through a 64-bit FISTP and returns EDX:EAX; this routine uses
   only the low 16 bits (006fa1b1 mov ecx,eax ... 006fa1cf cmp ax,cx). */
#include "cmre.h"

u8 CM_THISCALL cm_fn_006fa140(u8 *self, u32 arg1, u32 arg2)
{
    /* 006fa140..006fa157: byte differences, this+0x102 read before this+0x103 */
    s8 a = (s8)((u8)arg1 - *(volatile u8 *)(self + 0x102));
    s8 b = (s8)((u8)arg2 - *(volatile u8 *)(self + 0x103));
    s16 cur;
    s16 ref;
    s16 limit;
    s32 diff;
    long double t;

    if (a == 0 && b == 0)
        return 0; /* 006fa166: AL is the zero difference */

    /* 006fa169..006fa17c: index = b + a*24 into a 16-bit table at 00b67c30 */
    cur = *(volatile s16 *)(self + 0x19a);
    ref = *(volatile s16 *)(0x00b67c30u + (u32)((s32)b + (s32)a * 24) * 2u);
    if (cur == ref)
        return 1;

    /* 006fa191..006fa1ac: FILD (signed byte widened), FMUL m64, FADD m64, CALL __ftol */
    t = (long double)(s32)*(volatile s8 *)(self + 0x10c);
    t = t * CM_GLOBAL(double, 0x00968ec8);
    t = t + CM_GLOBAL(double, 0x00968d68);
    limit = (s16)(u32)(long long)t;

    /* 006fa1b3..006fa1be: 32-bit absolute difference via CDQ/XOR/SUB */
    diff = (s32)cur - (s32)ref;
    if (diff < 0)
        diff = -diff;
    /* 006fa1c0..006fa1cd: signed 16-bit compare with 180, then 360 - diff */
    if ((s16)diff > 0xb4)
        diff = 0x168 - diff;
    /* 006fa1cf..006fa1d5: signed 16-bit AX < CX */
    return (u8)((s16)diff < limit);
}
