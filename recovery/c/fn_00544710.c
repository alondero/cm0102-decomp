#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);
extern s32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 param_2, u32 param_3);

/* Cdecl with three stack arguments (arg_04 = record pointer P, arg_08 = signed
   byte count c, arg_0c = dword), returning u32 (0 or 1). `ret 0`.
   Q = [P+0x61] and b = byte [Q+0x45]. When c == 0 the routine returns 0 before
   reading anything. Otherwise:
   - c > 0: sum = b + c. If sum > 10 and the helper 0x90cfc0(0x15) is below
     sum, the value is (helper(c+1) low byte) + b; else b + c.
   - c < 0: sum = b + c. If sum < 10, t = (Q[+0x2f] + P[+0x5a] + P[+0x58]) / 3
     (signed magic division), r = helper(t); if r > sum the value is
     (helper(c-1) low byte) + b, else b + c. If sum >= 10, b + c.
   The value is clamped to 1..0x14 (signed byte) in a stack byte that overlays
   arg_08 (the listing writes the clamp there). If arg_0c != 0 and c > 0, then
   0x4cf5f0 (thiscall on the global at 0x00ae1050, arguments P and [P+0x39])
   may replace the clamp with b, when its result is non-null, its dword at +0x45
   is non-zero and the clamp is not below b (signed). Finally the clamp is
   stored to Q[+0x45] and the return is 1 when the clamp differs from b. */
u32 CM_CDECL cm_fn_00544710(u32 arg_04, u32 arg_08, u32 arg_0c)
{
    u32 p;
    u32 q;
    u32 q2;
    u8 cl_arg;
    s8 c;
    u8 bl;
    s32 sum;
    s32 r0;
    s32 r1;
    s32 r2;
    s32 r;
    s32 t;
    s32 ecx_t;
    long long prod;
    s32 hi;
    u8 al;
    u8 clamp;
    s8 al_s;
    u32 ecx_p;
    u32 ecx_g;
    s32 res;
    u32 result;

    cl_arg = (u8)arg_08;
    if (cl_arg == 0)
        return 0;

    p = arg_04;
    q = *(volatile u32 *)(p + 0x61);
    bl = *(volatile u8 *)(q + 0x45);
    c = (s8)cl_arg;

    if (c > 0) {
        sum = (s32)(s8)bl + (s32)c;
        if (sum > 10) {
            r0 = cm_fn_0090cfc0(0x15);
            if (r0 < sum) {
                r1 = cm_fn_0090cfc0((s32)c + 1);
                al = (u8)((u8)r1 + bl);
            } else {
                al = (u8)(bl + cl_arg);
            }
        } else {
            al = (u8)(bl + cl_arg);
        }
    } else {
        sum = (s32)(s8)bl + (s32)c;
        if (sum < 10) {
            ecx_t = (s32)*(volatile s8 *)(q + 0x2f) + (s32)*(volatile s8 *)(p + 0x5a) +
                    (s32)*(volatile s8 *)(p + 0x58);
            prod = (long long)(s32)0x55555556 * (long long)ecx_t;
            hi = (s32)(prod >> 32);
            t = hi + (s32)((u32)hi >> 31);
            r = cm_fn_0090cfc0(t);
            if (r > sum) {
                r2 = cm_fn_0090cfc0((s32)c - 1);
                al = (u8)((u8)r2 + bl);
            } else {
                al = (u8)(bl + cl_arg);
            }
        } else {
            al = (u8)(bl + cl_arg);
        }
    }

    al_s = (s8)al;
    if (al_s < 1)
        clamp = 1;
    else if (al_s > 0x14)
        clamp = 0x14;
    else
        clamp = al;

    if (arg_0c != 0 && (s8)cl_arg > 0) {
        ecx_p = *(volatile u32 *)(p + 0x39);
        ecx_g = CM_GLOBAL(u32, 0x00ae1050);
        res = cm_fn_004cf5f0(ecx_g, p, ecx_p);
        if (res != 0) {
            if (*(volatile u32 *)((u32)res + 0x45) != 0 && (s8)clamp >= (s8)bl)
                clamp = bl;
        }
    }

    al = clamp;
    q2 = *(volatile u32 *)(p + 0x61);
    result = (al != bl) ? 1u : 0u;
    *(volatile u8 *)(q2 + 0x45) = al;
    return result;
}
