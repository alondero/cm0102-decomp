/* Preferred-image VA 0x0091DEF0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the context pointer, read before written), no stack
   arguments, ret 0; returns EAX.
   Reads index1 = sign-extended byte ctx+0x100 and table = dword ctx+0x0c, then
   r1 = table[index1] (dword) and index2 = sign-extended byte ctx+0x101, then
   r2 = table[index2]. n1 = signed word r1+0x3e; if n1 <= 0 return 0.
   Otherwise n2 = signed word r2+0x3e and, for j = 0 .. n1-1 (16-bit signed):
   - if n2 > 0: t = dword r2+0xa7, u = dword r1+0xb1, value = dword at
     u + 59*j, then for i = 0 .. n2-1 (stop early at the first match)
     compare value with the dword at t + 6*i;
   - if the inner loop ran to i == n2 (including n2 == 0 with no loop), return
     the dword at (dword r1+0xb1) + 59*j (re-read);
   - otherwise continue with the next j.
   If no j returns, the routine returns 0. The saved-ECX spill in the listing is an
   internal register spill and is not modelled. Field meanings remain unknown;
   the 59 and 6 strides are the listing's scaled-index arithmetic. Context accesses
   are volatile so -O2 keeps the listing's read order. */
#include "cmre.h"

u32 CM_THISCALL cm_fn_0091def0(u8 *ctx)
{
    s32 index1;
    s32 index2;
    u32 table;
    u8 *r1;
    u8 *r2;
    s32 n1;
    s32 n2;
    s32 j;
    s32 i;
    u32 t_base;
    u32 value;

    index1 = (s32)(s8)*(volatile u8 *)(ctx + 0x100);
    table = *(volatile u32 *)(ctx + 0x0c);
    r1 = (u8 *)*(volatile u32 *)(table + (u32)index1 * 4u);
    index2 = (s32)(s8)*(volatile u8 *)(ctx + 0x101);
    r2 = (u8 *)*(volatile u32 *)(table + (u32)index2 * 4u);

    n1 = (s32)(s16)*(volatile u16 *)(r1 + 0x3e);
    if (n1 <= 0) {
        return 0u;
    }
    n2 = (s32)(s16)*(volatile u16 *)(r2 + 0x3e);

    j = 0;
    for (;;) {
        i = 0;
        if (n2 > 0) {
            t_base = *(volatile u32 *)(r2 + 0xa7);
            value = *(volatile u32 *)(*(volatile u32 *)(r1 + 0xb1) + 59u * (u32)j);
            for (; i < n2; i++) {
                if (value == *(volatile u32 *)(t_base + 6u * (u32)i)) {
                    break;
                }
            }
        }
        if (i == n2) {
            return *(volatile u32 *)(*(volatile u32 *)(r1 + 0xb1) + 59u * (u32)j);
        }
        j++;
        if (j >= n1) {
            break;
        }
    }
    return 0u;
}
