#include "cmre.h"

/* Cdecl, `ret 0`: one 16-bit stack argument (arg), read with `mov dx, word [esp+4]`.
   Reads the word global at 0x00b77c7a (w_b77c7a) and two tables indexed by it:
   the table base at 0x00b74c4c + w_b77c7a*768 (an entry each for the 0x0ba966 and
   0x0ba9a6 fields, 396-byte record stride). The object at 0x00b74450 is passed as
   the ECX receiver of both thiscall callees. Returns 0, or 0xfffffff5 (EAX) on the
   mismatch path. */
#define MEM_U32(addr) (*(volatile u32 *)(addr))
#define MEM_U16(addr) (*(volatile u16 *)(addr))

extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 p2);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 p2, u32 p3, u32 p4);

u32 CM_CDECL cm_fn_00498520(s16 arg)
{
    s16 w_b77c7a;
    s16 ax_v;
    u32 base;
    s32 n;
    long long prod;
    s32 hi;
    s32 q;
    u32 esi_v;
    u32 eax_v;

    w_b77c7a = (s16)MEM_U16(0x00b77c7au);
    if (arg == -1) {
        ax_v = 0;
    } else {
        base = MEM_U32(0x00b74c4cu + (u32)(s32)w_b77c7a * 768u);
        ax_v = (s16)MEM_U32(base + (u32)(s32)arg * 396u + 0x000ba966u);
    }
    if (ax_v == 0) {
        ax_v = (s16)MEM_U16(0x00dd6bdeu);
    }
    if (ax_v != 5) {
        return 0u;
    }

    if (arg == -1) {
        n = 0;
    } else {
        base = MEM_U32(0x00b74c4cu + (u32)(s32)w_b77c7a * 768u);
        n = (s32)MEM_U32(base + (u32)(s32)arg * 396u + 0x000ba9a6u);
    }
    /* Signed divide by 1000 as the compiler lowered it: imul 0x10624dd3, sar 6, then
       add the sign bit of the shifted quotient and subtract 1. */
    prod = (long long)(s32)0x10624dd3 * (long long)n;
    hi = (s32)(prod >> 32);
    q = hi >> 6;
    esi_v = (u32)q + ((u32)q >> 31) - 1u;

    eax_v = cm_fn_007ff010(0x00b74450u, 0x0000002au);
    if (esi_v != eax_v) {
        cm_fn_007ff270(0x00b74450u, 0x0000002au, esi_v, 0u);
        return 0xfffffff5u;
    }
    return 0u;
}
