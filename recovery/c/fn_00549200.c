#include "cmre.h"

extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 param_2);

/* Cdecl with one stack argument (arg_04 = [entry+4]), `ret 0`. Returns the low
   byte only (AL). The listing pushes ECX and reuses that saved slot's top byte
   as a local counter (flag at entry-1); ECX input is never read as a value, so
   the routine takes no receiver. The Ghidra in_EAX marker is not an input:
   AL is written only at the return.
   Entry: a zero argument returns 0 (AL cleared). Otherwise limit is
   clamp((int16)(q + (q >>> 31) + 0x28), 0x28, 0x91), where q is the high dword
   of 0x7e07e07f * (int16)[arg+0x80] shifted right arithmetically by 5.
   The loop visits 50 slots at arg+0xd7 (dwords). For each non-zero slot p with
   q = [p+0x61] non-zero, it calls 0x62c400 (thiscall on the global at 0x00b63d28,
   one stack argument p) when any of the bytes q+0x14, q+0x12, q+0x13 equals
   0x14. If that result is not 2, [p] < [0x00ae2364] and (int16)[q+5] > limit,
   the flag increments. The return is 1 when the flag is greater than 2 as a
   signed byte, else 0. */
u8 CM_CDECL cm_fn_00549200(u32 arg_04)
{
    u32 slot;
    u32 p;
    u32 q;
    u32 counter;
    u8 flag;
    u8 r;
    long long prod;
    s32 hi;
    s32 quot;
    u32 ebp_v;
    s16 limit;
    s32 a;
    s32 b;
    s16 w;

    if (arg_04 == 0)
        return 0;

    flag = 0;
    prod = (long long)(s32)0x7e07e07f * (long long)(s32)*(volatile s16 *)(arg_04 + 0x80);
    hi = (s32)(prod >> 32);
    quot = hi >> 5;
    ebp_v = (u32)quot + ((u32)quot >> 31) + 0x28u;
    limit = (s16)ebp_v;
    if (limit < 0x28)
        limit = 0x28;
    else if (limit > 0x91)
        limit = 0x91;

    slot = arg_04 + 0xd7;
    counter = 0x32;
    do {
        p = *(volatile u32 *)slot;
        if (p != 0) {
            q = *(volatile u32 *)(p + 0x61);
            if (q != 0 &&
                (*(volatile u8 *)(q + 0x14) == 0x14 ||
                 *(volatile u8 *)(q + 0x12) == 0x14 ||
                 *(volatile u8 *)(q + 0x13) == 0x14)) {
                r = cm_fn_0062c400(*(volatile u32 *)0x00b63d28, p);
                if (r != 2) {
                    a = *(volatile s32 *)p;
                    b = *(volatile s32 *)0x00ae2364;
                    if (a < b) {
                        q = *(volatile u32 *)(p + 0x61);
                        w = *(volatile s16 *)(q + 5);
                        if (w > limit)
                            flag++;
                    }
                }
            }
        }
        slot += 4u;
        counter--;
    } while (counter != 0);

    return ((s8)flag > 2) ? 1u : 0u;
}
