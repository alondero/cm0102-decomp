/* Faithful C transcription of preferred-image VA 0x00862720 (image base
   0x00400000, 3.9.68 pinned binary).

   Calling convention / result transport (the held evidence concern):
   - `ret 0x18` and no incoming register read: STDCALL, six dword stack
     arguments (arg4/arg5/arg6 are byte values in dword homes; arg4 is read as
     a full dword at 0x008642e9 before being pushed to cm_fn_005968d0, so the
     parameter keeps its dword home).
   - The result is a 32-bit float in ST0: every exit path runs
     `fld dword ptr [esp+0x14]` (0x008644dc) or
     `mov [esp+0x10],imm32 ; fld dword ptr [esp+0x10]` (0x00864459,
     0x00864480, 0x008644ae) directly before `ret 0x18`. Both direct callers
     (0x0083a2c3, 0x008626f6) call __ftol at 0x00945640 immediately after, so
     the value is consumed from ST0. The body therefore accumulates the fee in
     the dword slot [esp+0x14] (here `value`) and returns it as `float`.

   x87 transport:
   - `call 0x00945640` is MSVC's __ftol lowering of an FP->int conversion; it is
     written as `(long long)t` on a `long double` t that follows the listing's
     x87 chain on the original constants.
    - UNRESOLVED: `call 0x00945ff0` at 0x008636e0 takes its operands on the x87 stack
     (ST1 = the value loaded from [esp+0x20], ST0 = the constant loaded from
      0x00969000) and leaves the result in ST0. The previous invented series
      replacement has been removed. The plain-C call below does not transport
      those operands and is NOT a faithful implementation of this boundary.
      The semantic test passes at O0/O2 (coverage 0.7111), but this does not
      resolve the held ABI, precision/control-word, or depth proof obligation.
   - Every intermediate the listing keeps in an x87 register is computed in
     `long double` (80-bit extended) and stored back to `float`/`double` only
     where the listing stores it (`fstp dword` / `fstp qword`).

   Slot aliasing reproduced from the listing:
   - [esp+0x14] is the fee accumulator (float view) and also hosts int->float
     conversions (`mov [esp+0x14],reg ; fild dword [esp+0x14]`); those are the
     `(float)(s32)expr` conversions written below.
   - [esp+0x20] holds `*param_1 / [0x00ae2364]` (float) during the first
     dispatch and the double base/power result during the second phase.
   - [esp+0x28] holds the double club-size product and later the 64-bit
     integer scratch.
   - [esp+0x44]/[esp+0x48] are the arg5/arg6 homes, reused by the original as
     dword/float scratch in paths that never read those bytes again; the byte
     reads are kept on separate variables.
   - [esp+0x40] is the arg4 home; it is written as a byte at 0x00862f96 (only
     the low byte changes), so it is modelled as a dword patched byte-wise. */

#include "cmre.h"

/* Unresolved original boundary: 0x008636e0 supplies ST0 and ST1, not
   ordinary C stack arguments. The supplied listing does not establish a
   plain-C callable ABI for this helper. Do not substitute a power algorithm.
   This declaration preserves the explicit boundary, but does NOT prove
   operand transport; this source remains ineligible for acceptance. */
extern long double cm_fn_00945ff0(void);

#define G_U8(va)   CM_GLOBAL(u8, va)
#define G_U32(va)  CM_GLOBAL(u32, va)
#define G_S32(va)  CM_GLOBAL(s32, va)
#define G_F32(va)  CM_GLOBAL(float, va)
#define G_F64(va)  CM_GLOBAL(double, va)

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00547770(u32 object);
extern u32 CM_CDECL cm_fn_00549e50(u32 arg);
extern int CM_THISCALL cm_fn_0054cdb0(u32 self, u32 param_1, u32 param_2);
extern u32 CM_THISCALL cm_fn_005968d0(u32 self, u32 person, u32 flags);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32 a_low, u32 a_high, u32 b_low, u32 b_high);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32 a_lo, u32 a_hi, u32 b_lo, u32 b_hi);
extern u32 CM_CDECL cm_fn_00946a04(void);

/* Signed strength-reduced divisions emitted by the original compiler:
   0x10624dd3 / sar 5 for /500 (0x008627a5, 0x008628d2, ...), 0x51eb851f /
   sar 4 for /50 (0x00863a2a).  Both are followed by the `shr eax,0x1f` sign
   fixup. */
static s32 cm_div500(s32 x)
{
    s32 hi = (s32)(((long long)274877907LL * (long long)x) >> 32);
    s32 q = hi >> 5;
    q = q + (s32)((u32)q >> 31);
    return q;
}

static s32 cm_div50(s32 x)
{
    s32 hi = (s32)(((long long)1374389535LL * (long long)x) >> 32);
    s32 q = hi >> 4;
    q = q + (s32)((u32)q >> 31);
    return q;
}

/* Repeated club-factor sequence of the club-class chain (0x00863bd1,
   0x00863c9f, 0x00863d66, 0x00863e98, 0x00863f75, 0x0086400c, 0x008640d6,
   0x00864203): the factor starts as the `base` double, and when
   (s16)level50 > 10*field_0x85 it becomes (level50 / divisor) * 0.1 * base,
   clamped to at most 1.0.  Kept in extended precision exactly as the x87
   sequence does; the caller multiplies `value` by it and rounds to float. */
static long double cm_clubfactor(u32 p53, s32 level50, double base)
{
    s32 e = (s32)(s8)G_U8(p53 + 0x85);
    s32 edx = (s32)(s16)level50;
    s32 esi = e * 10;
    long double f = (long double)base;

    if (edx > esi) {
        s32 dv = (e < 1) ? 1 : e;
        f = ((long double)edx / (long double)dv) * G_F64(0x00967880) * (long double)base;
        if (f > (long double)G_F64(0x00967890))
            f = (long double)G_F64(0x00967890);
    }
    return f;
}

float CM_STDCALL cm_fn_00862720(u32 param_1, u32 param_2, u32 param_3,
                                u32 param_4, u32 param_5, u32 param_6)
{
    u32 p1 = param_1;
    u32 p4 = param_4;
    u8 p5 = (u8)param_5;
    u8 p6 = (u8)param_6;
    u32 iVar6;
    u32 local_18;
    u8 c13;
    u32 first = 0;
    u32 club = 0;
    u32 slot44 = 0;
    float value = 0.0f;
    float f20 = 0.0f;
    double d20 = 0.0;
    double d28 = 0.0;
    s32 level = 0;
    s32 esi = 0;
    s32 edi = 0;
    u32 p1a = 0;
    u32 r;
    u8 c16;
    s32 i;
    s32 lvl50;
    long double t;
    long double stv = 0.0L;

    /* 0x00862720 */
    r = cm_fn_004cf5a0(G_U32(0x00ae1050), p1);
    G_U32(0x00ae1080) = r;
    if (r == 0 || (G_U8(r + 0x35) & 0x3f) == 5) {
        local_18 = 0;
    } else {
        local_18 = G_U32(0x00ae23b4) + G_U32(r + 4) * 0x245;
    }

    /* 0x0086276f */
    first = G_U32(p1);
    c13 = G_U8(p1 + 0x18);
    iVar6 = G_U32(p1 + 0x61);
    f20 = (float)((long double)(s32)first / (long double)G_S32(0x00ae2364));

    if (param_2 == 0) {
        /* 0x008627ed */
        if (iVar6 != 0) {
            i = cm_div500((s32)*(s16 *)(iVar6 + 0xd));
        } else {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 == 0) {
                level = 1;
                goto L_2838;
            }
            i = cm_div500((s32)*(s16 *)(q69 + 0xc));
        }
        if (i < 1)
            i = 1;
        level = (s32)(s8)(u8)i;
        goto L_2838;
    }

    /* 0x0086279e */
    i = cm_div500((s32)*(s16 *)(param_2 + 0x80)) + 1;
    level = (s32)(s8)(u8)i;
    if (local_18 != 0) {
        if (G_U8(param_2 + 0x64) == 2) {
            level = level - 1;
            if (level < 1)
                level = 1;
            else if (level > 4)
                level = 4;
        }
        goto L_283c;
    }
    goto L_2859;

L_2838:
L_283c:
    if (local_18 != 0) {
        c16 = G_U8(p1 + 0x3d);
        if (c16 != 0)
            goto L_287f;
    }
L_2859:
    {
        u32 v24 = G_U32(p1 + 0x24);
        if (v24 != 0) {
            c16 = G_U8(p1 + 0x28);
            if (c16 != 0)
                goto L_287f;
        }
        c16 = G_U8(p1 + 0x3d);
        if (c16 != 0)
            goto L_287f;
        if (iVar6 != 0) {
            c16 = 0x0b;
            goto L_287f;
        }
        c16 = G_U8(p1 + 0x28);
        if (c16 != 0)
            goto L_287f;
        c16 = 9;
    }
L_287f:
    i = (s32)(s8)c16 - 5;
    if ((u32)i > 0xa)
        goto L_2efe;
    switch (i) {

    case 0:                                                     /* 0x00862895 */
        if (local_18 != 0) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 1;
            if (i > level) {
                level = i;
                goto L_28f6;
            }
        }
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                u32 q24 = G_U32(p1 + 0x24);
                if (q24 != 0 && G_U8(p1 + 0x28) != 0x0b) {
                    i = cm_div500((s32)*(s16 *)(q24 + 0x80)) + 1;
                    if (i > level)
                        level = i;
                }
            }
        }
L_28f6:                                      /* primary increase skips q24 */
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                long double a = (long double)(s32)*(s16 *)(q69 + 0xa);
                t = a * a * a * G_F64(0x0096ff38);
            } else {
                t = ((long double)f20 * G_F32(0x0096d228) + (long double)G_F32(0x00968e70))
                    * (long double)G_F32(0x0096ff34) * G_F64(0x0096a5c0);
            }
            t = (long double)level * t * G_F64(0x009689e0);
            value = (float)t;
            if ((long double)G_F32(0x0096ff30) > (long double)value) {
                value = 750.0f;
                goto L_2f06;
            }
            goto L_2bd5;
        }

    case 1:                                                     /* 0x0086296b */
        if (local_18 != 0) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 1;
            if (i > level) {
                level = i;
                goto L_29d0;
            }
        }
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                u32 q24 = G_U32(p1 + 0x24);
                if (q24 != 0) {
                    i = cm_div500((s32)*(s16 *)(q24 + 0x80)) + 1;
                    if (i > level && G_U8(p1 + 0x28) != 0x0b)
                        level = i;
                }
            }
        }
L_29d0:                                      /* 0x008629d0 */
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                long double a;
                a = (long double)(s32)*(s16 *)(q69 + 0xa);
                t = a * a * a * G_F64(0x00968eb8);
            } else {
                t = ((long double)f20 * G_F32(0x0096d228) + (long double)G_F32(0x00968e70))
                    * (long double)G_F32(0x0096d1b0) * G_F64(0x0096a5c0);
            }
            t = (long double)level * t * G_F64(0x009689e0);
            value = (float)t;
            if ((long double)G_F32(0x0096d1a8) > (long double)value)
                value = 500.0f;
        }
        /* 0x00862a3c */
        if (local_18 != 0 && local_18 != param_2) {
            if (p5 == 0x0b || p5 == 6) {
                i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 4;
                if (i > level) {
                    value = 150000.0f;
                    goto L_2f06;
                }
                if ((long double)G_F32(0x00968dac) > (long double)value)
                    value = (float)((long double)value * G_F64(0x00969588));
                goto L_2f06;
            }
            value = 150000.0f;
            goto L_2f06;
        }
        /* 0x00862bc1 */
        if (p5 != 0x0b && p5 != 6)
            goto L_2bd5;
        goto L_2f06;

    case 2:                                                     /* 0x00862ab5 */
        if (local_18 != 0) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 1;
            if (i > level) {
                level = i;
                goto L_2b1a;
            }
        }
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                u32 q24 = G_U32(p1 + 0x24);
                if (q24 != 0) {
                    i = cm_div500((s32)*(s16 *)(q24 + 0x80)) + 1;
                    if (i > level && G_U8(p1 + 0x28) != 0x0b)
                        level = i;
                }
            }
        }
L_2b1a:                                      /* 0x00862b1a */
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                long double a;
                a = (long double)(s32)*(s16 *)(q69 + 0xa);
                t = a * a * a * G_F64(0x00968eb8);
            } else {
                t = ((long double)f20 * G_F32(0x0096d228) + (long double)G_F32(0x00968e70))
                    * (long double)G_F32(0x0096d1b0) * G_F64(0x0096a5c0);
            }
            t = (long double)level * t * G_F64(0x009689e0);
            value = (float)t;
            if ((long double)G_F32(0x0096d1a8) > (long double)value)
                value = 500.0f;
        }
        /* 0x00862b86 */
        if (local_18 != 0 && local_18 != param_2) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 4;
            if (i > level) {
                value = 150000.0f;
                goto L_2f06;
            }
        }
        /* 0x00862bc1 */
        if (p5 != 0x0b && p5 != 6)
            goto L_2bd5;
        goto L_2f06;

    case 3:                                                     /* 0x00862bf7 */
        if (local_18 != 0) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 1;
            if (i > level) {
                level = i;
                goto L_2c5c;
            }
        }
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                u32 q24 = G_U32(p1 + 0x24);
                if (q24 != 0) {
                    i = cm_div500((s32)*(s16 *)(q24 + 0x80)) + 1;
                    if (i > level && G_U8(p1 + 0x28) != 0x0b)
                        level = i;
                }
            }
        }
L_2c5c:                                      /* 0x00862c5c */
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                long double a;
                a = (long double)(s32)*(s16 *)(q69 + 0xa);
                t = a * a * a * G_F64(0x0096ff28);
            } else {
                t = ((long double)f20 * G_F32(0x0096d228) + (long double)G_F32(0x00968e70))
                    * (long double)G_F32(0x00968db0) * G_F64(0x0096a5c0);
            }
            t = (long double)level * t * G_F64(0x009689e0);
            value = (float)t;
            if ((long double)G_F32(0x0096ff20) > (long double)value) {
                value = 275.0f;
            } else if ((long double)G_F32(0x00968db0) < (long double)value) {
                value = 5000.0f;
            }
        }
        /* 0x00862ce3 */
        if (local_18 != 0 && local_18 != param_2
            && p5 != 6 && p5 != 0x0b) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 4;
            if (i > level) {
                value = 150000.0f;
                goto L_2f06;
            }
        }
        goto L_2f06;

    case 4:                                                     /* 0x00862d40 */
        if (local_18 != 0) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 1;
            if (i > level) {
                level = i;
                goto L_2da5;
            }
        }
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                u32 q24 = G_U32(p1 + 0x24);
                if (q24 != 0) {
                    i = cm_div500((s32)*(s16 *)(q24 + 0x80)) + 1;
                    if (i > level && G_U8(p1 + 0x28) != 0x0b)
                        level = i;
                }
            }
        }
L_2da5:                                      /* 0x00862da5 */
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                long double a;
                slot44 = (u32)(s32)*(s16 *)(q69 + 0xa);
                a = (long double)(s32)*(s16 *)(q69 + 0xa);
                t = a * a * a * G_F64(0x0096ff18);
            } else {
                t = ((long double)f20 * G_F32(0x0096d228) + (long double)G_F32(0x00968e70))
                    * (long double)G_F32(0x0096d2dc) * G_F64(0x0096a5c0);
            }
            t = (long double)level * t * G_F64(0x009689e0);
            value = (float)t;
            if ((long double)G_F32(0x0096d210) > (long double)value) {
                value = 250.0f;
                goto L_2f06;
            }
        }
        goto L_2eed;

    case 5:                                                     /* 0x00862e1a */
        if (local_18 != 0) {
            i = cm_div500((s32)*(s16 *)(local_18 + 0x80)) + 1;
            if (i > level) {
                level = i;
                goto L_2e7f;
            }
        }
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                u32 q24 = G_U32(p1 + 0x24);
                if (q24 != 0) {
                    i = cm_div500((s32)*(s16 *)(q24 + 0x80)) + 1;
                    if (i > level && G_U8(p1 + 0x28) != 0x0b)
                        level = i;
                }
            }
        }
L_2e7f:                                      /* 0x00862e7f */
        {
            u32 q69 = G_U32(p1 + 0x69);
            if (q69 != 0) {
                long double a;
                slot44 = (u32)(s32)*(s16 *)(q69 + 0xa);
                a = (long double)(s32)*(s16 *)(q69 + 0xa);
                t = a * a * a * G_F64(0x0096ff18);
            } else {
                t = ((long double)f20 * G_F32(0x0096d228) + (long double)G_F32(0x00968e70))
                    * (long double)G_F32(0x0096d2dc) * G_F64(0x0096a5c0);
            }
            t = (long double)level * t * G_F64(0x009689e0);
            value = (float)t;
            if ((long double)G_F32(0x0096d834) > (long double)value) {
                value = 200.0f;
                goto L_2f06;
            }
        }
        goto L_2eed;

    case 9:                               /* table 0x008644ec -> 0x00862efe */
        goto L_2efe;
    default:                              /* cVar16 == 11, 12, 13, 15 */
        goto L_2f4f;
    }

L_2bd5:                                       /* 0x00862bd5 */
    if ((long double)G_F32(0x0096ff34) < (long double)value)
        value = 150000.0f;
    goto L_2f06;

L_2eed:                                       /* 0x00862eed */
    if ((long double)G_F32(0x0096d2dc) < (long double)value)
        value = 1000.0f;
    goto L_2f06;

L_2efe:                                       /* 0x00862efe: cVar16 == 14 */
    value = 1000.0f;
    goto L_2f06;

    /* ---------------- staff contract branch (cVar16 11/12/13/15) --------- */
L_2f4f:
    if (iVar6 == 0)
        goto L_3660;
    if ((u8)p4 == 0xff) {
        u32 esi_p2 = param_2;
        club = iVar6;
        if (param_2 == 0)
            goto L_3039;
        if ((s32)*(s16 *)(club + 0xb) > (s32)*(s16 *)(esi_p2 + 0x80)
            && (s32)(s8)c13 >= 0x18 && (s32)(s8)c13 < 0x22) {
            p4 = (p4 & 0xffffff00u) | 1u;
            goto L_303d;
        }
        /* 0x00862f9f: club class selection */
        {
            s32 a = (s32)*(s16 *)(club + 0xb);
            s32 b = (s32)*(s16 *)(esi_p2 + 0x80);
            long double ab = (long double)a;
            long double bb = (long double)b;

            d28 = (double)(bb * G_F64(0x00969040));
            if (ab > (long double)d28 && (s32)(s8)c13 >= 0x15) {
                p4 = (p4 & 0xffffff00u) | 2u;
                goto L_303d;
            }
            if (ab > bb * (long double)G_F64(0x00968968)
                && (s32)(s8)c13 >= 0x11) {
                p4 = (p4 & 0xffffff00u) | 4u;
                goto L_303d;
            }
            if ((s32)(s8)c13 > 0x18) {
                p4 = (p4 & 0xffffff00u) | 4u;
                goto L_303d;
            }
            {
                s32 v = (s32)*(s16 *)(club + 7) * 50;
                if ((long double)v > (long double)d28)
                    p4 = (p4 & 0xffffff00u) | 5u;
                else
                    p4 = (p4 & 0xffffff00u) | 6u;
            }
            goto L_303d;
        }
    }
    /* 0x00863031 */
    club = iVar6;
L_3039:
L_303d:
    /* 0x0086303d: base value selection */
    if (((u8)p5 == 0x0d || (u8)p5 == 0x0f) && (s32)(s8)c13 > 0x23) {
        t = (long double)G_F32(0x0096d1b0);
    } else if (((p4 & 0xff) == 6 || (p4 & 0xff) == 5)
               && (s32)*(s16 *)(club + 0xb) < 0x9c4) {
        t = (long double)G_F32(0x0096d4e0);
    } else if (((p4 & 0xff) == 6 || (p4 & 0xff) == 5)
               && (s32)*(s16 *)(club + 0xb) < 0x186a) {
        t = (long double)G_F32(0x0096d1b0);
    } else if ((s32)(s8)G_U8(club + 0x15) >= 0x0f) {
        if ((s32)*(s16 *)(club + 0xb) > 0x1a5e) {
            if ((s32)(s8)G_U8(p1 + 0x59) < 0x0f)
                t = (long double)G_F32(0x0096ff0c);
            else
                t = (long double)G_F32(0x0096ff08);
        } else {
            t = (long double)G_F32(0x0096ff04);
        }
    } else if ((s32)(s8)G_U8(club + 0x12) >= 0x0f
               || (s32)(s8)G_U8(club + 0x14) >= 0x0f
               || (s32)(s8)G_U8(club + 0x13) >= 0x0f) {
        if ((s32)*(s16 *)(club + 0xb) <= 0x1c52)
            t = (long double)G_F32(0x0096fef0);
        else if ((s32)(s8)G_U8(p1 + 0x59) < 0x0e)
            t = (long double)G_F32(0x0096fef8);
        else
            t = (long double)G_F32(0x0096fef4);
    } else if (G_U8(club + 0xf) == 0x14) {
        if ((s32)*(s16 *)(club + 0xd) > 0x1e46
            && (s32)(s8)G_U8(p1 + 0x59) < 3)
            t = (long double)G_F32(0x0096ff34);
        else
            t = (long double)G_F32(0x0096ff00);
    } else if ((s32)(s8)G_U8(club + 0x10) < 0x0f
               && G_U8(club + 0x11) == 0) {
        t = (long double)G_F32(0x0096ff34);
    } else if ((s32)*(s16 *)(club + 0xd) > 0x1d4c
               && (s32)(s8)G_U8(p1 + 0x59) < 6) {
        t = (long double)G_F32(0x0096fefc);
    } else {
        t = (long double)G_F32(0x0096ff34);
    }

    /* 0x0086315d: ST0 keeps the (possibly scaled) value in extended
       precision; the dword slot only receives it at 0x008632fa. */
    stv = t;
    value = (float)t;
    if (G_U8(0x00ae233c) == 1
        && stv > (long double)G_F32(0x0096feec)
        && param_2 != 0
        && cm_fn_00549e50(param_2) != 0) {
        if ((s32)*(s16 *)(club + 0xb) > 0x1a5e) {
            if ((s32)(s8)G_U8(p1 + 0x59) == 1)
                stv = (long double)value * G_F64(0x00968980);
            else if ((s32)(s8)G_U8(p1 + 0x59) < 6)
                stv = (long double)value * G_F64(0x0096a5a8);
            else
                stv = (long double)value * G_F64(0x009689c0);
        } else if ((s32)*(s16 *)(club + 0xb) > 0x109a) {
            stv = (long double)value * G_F64(0x00968fa0);
        }
    }

    /* 0x008631d9 */
    {
        s16 vb = *(s16 *)(club + 0xb);
        s16 v9 = *(s16 *)(club + 9);
        s16 vd = *(s16 *)(club + 0xd);
        s16 cmax = vb;
        s32 e7, e5, sum, xx, hi, q, qq, ft;
        u32 n;

        if (v9 > cmax)
            cmax = v9;
        if (vd > cmax)
            cmax = vd;
        if (p6 == 1) {
            if ((s32)cmax < 0x1c52)
                cmax = 0x1c52;
        } else if (p6 == 2) {
            if ((s32)cmax < 0x1a5e)
                cmax = 0x1a5e;
        }
        /* 0x0086321d */
        esi = (s32)vb;
        t = (long double)f20 * (long double)G_F32(0x0096d784);
        e7 = (s32)*(s16 *)(club + 7);
        e5 = (s32)*(s16 *)(club + 5);
        sum = e7 + e5;
        slot44 = (u32)esi;
        xx = (s32)vd + (s32)v9 + (s32)esi + 4 * (s32)cmax + 50 * sum;
        hi = (s32)(((long long)954437177LL * (long long)xx) >> 32);
        q = hi >> 1;
        qq = q;
        qq = qq + (s32)((u32)qq >> 31);
        ft = (s32)(long long)(t - (long double)G_F32(0x0096d228));
        n = (u32)(qq + ft);

        /* 0x0086327c */
        p1a = G_U32(p1 + 0x1a);
        club = param_2; /* 0x00863274 reloads EBX before these accesses. */
        if (p1a != 0 && G_U32(club + 0x53) != 0) {
            u8 c76 = G_U8(G_U32(club + 0x53) + 0x76);
            if (G_U8(p1a + 0x76) != c76) {
                int skip = 0;
                if (local_18 != 0) {
                    u32 l53 = G_U32(local_18 + 0x53);
                    if (l53 == 0 || G_U8(l53 + 0x76) == c76)
                        skip = 1;
                }
                if (!skip) {
                    if (n >= 0x5dc) {
                        n = (u32)(s32)(long long)
                            ((long double)(unsigned long long)n * G_F64(0x009689b8));
                    } else {
                        n = 0x5dc;
                    }
                }
            }
        }
        /* 0x008632d1 */
        if (n <= 1)
            n = 1;
        /* 0x008632db */
        {
            u32 nn = n * n;
            t = (long double)(unsigned long long)nn * stv;
        }
        t = t * G_F64(0x00968eb8);
        value = (float)t;
        if (t < (long double)G_F32(0x0096fee8))
            value = 175.0f;
    }

    /* 0x00863313 */
    {
        u8 mode;
        if (local_18 != 0 && G_U32(local_18 + 0x53) != 0)
            mode = G_U8(G_U32(local_18 + 0x53) + 0x7e);
        else if (p1a != 0)
            mode = G_U8(p1a + 0x7e);
        else
            mode = 3;
        i = (s32)(s8)mode - 1;
        if (i != 0) {
            if (i - 1 == 0)
                value = (float)((long double)value * G_F32(0x0096d248));
            else
                value = (float)((long double)value * G_F32(0x00968f18));
        }
    }

    /* 0x00863351 */
    if (club != 0 && *(s16 *)(club + 0x80) != 0) {
        long double f = (long double)(s32)slot44;
        if ((s32)*(s16 *)(club + 0x80) < 0x9c4) {
            f = f * (long double)G_F32(0x0096fee4);
        } else {
            i = level * 500;
            slot44 = (u32)i;
            f = f / (long double)i;
        }
        if (f < (long double)G_F64(0x00969520))
            f = (long double)G_F32(0x0096d364);
        if (f > (long double)G_F64(0x00968980))
            f = (long double)G_F32(0x0096d390);
        value = (float)(f * (long double)value);
    }

    /* 0x008633bb: level caps */
    if (level < 10) {
        i = level * 2500;
        t = (long double)i;
    } else if (level < 15) {
        i = level * 2750;
        t = (long double)i;
    } else {
        u32 c53 = G_U32(club + 0x53);
        if (c53 != 0
            && (G_U32(c53) == G_U32(0x009cf2e4)
                || G_U32(c53) == G_U32(0x009cf488)
                || G_U32(c53) == G_U32(0x009cf360))) {
            i = G_U8(club + 0x82) != 0 ? level * 3250 : level * 5000;
            t = (long double)i;
            if ((long double)value > t)
                value = (float)(((long double)value - t)
                                * G_F64(0x00967880) + (long double)i);
            goto L_3472;
        }
        i = level * 3250;
        t = (long double)i;
        if ((long double)value > t)
            value = (float)t;
        goto L_3472;
    }
    if ((long double)value > t)
        value = (float)t;

L_3472:
    /* 0x00863472: staff-contract value selection */
    {
        u32 si = iVar6;
        float capf;

        if (G_U8(si + 0x15) == 0x14)
            capf = 85000.0f;
        else if (G_U8(si + 0x14) == 0x14 || G_U8(si + 0x13) == 0x14)
            capf = 75000.0f;
        else if (G_U8(si + 0x12) == 0x14)
            capf = 65000.0f;
        else if (G_U8(si + 0x11) == 0x14)
            capf = 50000.0f;
        else
            capf = 40000.0f;

        /* 0x00863547 */
        if (G_U8(0x00ae233c) == 1
            && (long double)value > (long double)G_F32(0x0096feec)
            && club != 0
            && cm_fn_00549e50(club) != 0
            && (s32)*(s16 *)(si + 0xb) > 0x1c52) {
            long double c = (long double)capf * G_F64(0x009689c0);
            capf = (float)c;
        }
        /* 0x00863588 */
        if ((long double)value > (long double)capf)
            value = (float)(((long double)value - (long double)capf)
                            * G_F64(0x00968918) + (long double)capf);
    }

    /* 0x008635ad */
    if (G_U32(p1 + 0x39) != 0)
        goto L_2f0a;
    if ((s32)(s8)c13 >= 0x21
        && G_U8(p1 + 0x22) != 0
        && (s32)(s8)G_U8(iVar6 + 0x44) + (s32)(s8)G_U8(p1 + 0x58) <= 0x1e
        && (s32)G_S32(p1) % 15 != 0) {
        if ((s32)*(s16 *)(club + 0x80) <= 0x1194)
            goto L_2f0a;
    }
    /* 0x008635f6 */
    {
        s32 r54;
        u32 r77;
        long double f;

        /* 0x008635f6..03: read high dword first, then push low/high. */
        u32 high = CM_GLOBAL(volatile u32, p1 + 0x42);
        u32 low = CM_GLOBAL(volatile u32, p1 + 0x3e);
        r54 = cm_fn_0054cdb0(0x00ae2c90u, low, high);
        slot44 = (u32)r54;
        f = (long double)(float)r54;
        r77 = cm_fn_00547770(club);
        if (r77 != 0)
            f = f + (long double)G_F32(0x0096d7b0);
        f = f * (long double)G_F32(0x0096fee0);
        f = (long double)G_F32(0x0096fedc) - f;
        if ((long double)G_F32(0x0096b028) > f)
            f = (long double)G_F32(0x0096b028);
        value = (float)(f * (long double)value);
    }
    goto L_2f0a;

    /* 0x00863660: iVar6 == 0 — contract string + random */
L_3660:
    cm_fn_00944c9f(0x00de1f64u, 0x00a83968u,
                   G_U32(p1 + 4), G_U32(p1 + 8), first);
    {
        s32 rnd = (s32)cm_fn_00946a04();
        unsigned long long a64 = cm_fn_00945e40((u32)rnd, (u32)(rnd >> 31), 4, 0);
        unsigned long long b64 = cm_fn_00945e80((u32)a64, (u32)(a64 >> 32), 0x8000, 0);
        slot44 = (u32)b64;
        value = (float)((long double)(s32)slot44 * (long double)G_F32(0x0096fed8));
    }
    goto L_2f06;

    /* ---------------------- second phase (loop head) --------------------- */
L_2f06:
    club = param_2;
L_2f0a:
    if (club == 0)
        goto L_644dc;
    if (iVar6 == 0)
        goto L_639bb;
    if (G_U8(club + 0x64) == 1)
        goto L_36bd;
    {
        s16 a = *(s16 *)(club + 0x80);
        esi = (s32)a;
        t = (long double)esi;
        if (a > 0x8ca)
            t = t * G_F64(0x0096ff10);
        else
            t = t * G_F64(0x0096a5c0);
    }
    goto L_36d2;

L_36bd:                                       /* 0x008636bd */
    esi = (s32)*(s16 *)(club + 0x80);
    t = (long double)esi * G_F64(0x0096fed0);
L_36d2:                                       /* 0x008636d2 */
    d20 = (double)t;
    /* 0x008636e0: unresolved ST0/ST1 operand ABI; no helper replacement. */
    d20 = (double)cm_fn_00945ff0();
    if (d20 > (double)G_F64(0x00967890))
        d20 = 1.0;
    edi = (s32)(s8)(u8)(p4 & 0xff) - 1;
    if ((u32)edi > 5)
        goto L_3957;
    switch (edi) {
    case 0:                                   /* 0x0086371e */
        if ((s32)(s8)c13 < 0x14) {
            t = (long double)(s32)(esi * 3) * (long double)d20 * G_F64(0x00968e48);
        } else if ((s32)(s8)c13 >= 0x18 && cm_fn_00549e50(club) != 0) {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3)
                * (long double)d20 * G_F64(0x00968940);
        } else {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3)
                * (long double)d20 * G_F64(0x00968928);
        }
        goto L_3914;
    case 1:                                   /* 0x00863793 */
        if ((s32)(s8)c13 < 0x14) {
            t = (long double)(s32)(esi * 3) * (long double)d20 * G_F64(0x00968e80);
        } else if ((s32)(s8)c13 >= 0x18 && cm_fn_00549e50(club) != 0) {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3) * (long double)d20;
        } else {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3)
                * (long double)d20 * G_F64(0x00968e48);
        }
        goto L_3914;
    case 2:                                   /* 0x00863802 */
        if ((s32)(s8)c13 < 0x14) {
            t = (long double)(s32)(esi * 3) * (long double)d20 * G_F64(0x009689e0);
            if ((long double)value < t)
                value = (float)(((long double)value + t) * G_F64(0x00968968));
        }
        if ((s32)(s8)c13 >= 0x18 && cm_fn_00549e50(club) != 0) {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3)
                * (long double)d20 * G_F64(0x00968e48);
        } else {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3)
                * (long double)d20 * G_F64(0x00968e80);
        }
        goto L_3914;
    case 3:                                   /* 0x00863893 */
        if ((s32)(s8)c13 < 0x14) {
            t = (long double)(s32)(esi * 3) * (long double)d20 * G_F64(0x00967880);
        } else if ((s32)(s8)c13 >= 0x18 && cm_fn_00549e50(club) != 0) {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3)
                * (long double)d20 * G_F64(0x00968e80);
        } else {
            t = (long double)(s32)(*(s16 *)(club + 0x80) * 3)
                * (long double)d20 * G_F64(0x00968f20);
        }
        goto L_3914;
    case 4:                                   /* 0x008638ff */
        t = (long double)(s32)(esi * 3) * (long double)d20 * G_F64(0x00967888);
        goto L_3914;
    default:                                  /* 0x00863923 */
        t = (long double)(s32)(esi * 3) * (long double)d20 * G_F64(0x00968a98);
        goto L_3914;
    }

L_3914:                                       /* 0x00863914 */
    if ((long double)value < t)
        value = (float)(((long double)value + t) * G_F64(0x00968968));

L_3957:                                       /* 0x00863957 */
    r = cm_fn_004cf5a0(G_U32(0x00ae1050), p1);
    if (r != 0 && (G_U8(r + 0x45) & 0x10) != 0 && (u32)edi <= 5) {
        switch (edi) {
        case 0:
        case 1:
            value = (float)((long double)value * G_F64(0x00968f80));
            break;
        case 2:
            value = (float)((long double)value * G_F64(0x00969520));
            break;
        case 3:
            value = (float)((long double)value * G_F64(0x00968e18));
            break;
        case 4:
            value = (float)((long double)value * G_F64(0x00968e38));
            break;
        default:
            value = (float)((long double)value * G_F64(0x00968eb0));
            break;
        }
    }

    /* ------------------- club-class chain (0x008639bb) ------------------- */
L_639bb:
    {
        u32 p53 = G_U32(club + 0x53);
        u32 a39;
        u32 eax_;
        u32 p1a2;

        if (p53 == 0)
            goto L_64295;
        a39 = G_U32(p1 + 0x39);
        if (a39 == 0)
            goto L_63a09;
        if (G_U32(a39 + 0x53) != p53)
            goto L_639e1;
        p1a2 = G_U32(p1 + 0x1a);
        if (p1a2 == p53)
            goto L_63a18;
    L_639e1:
        if (a39 == 0)
            goto L_63a09;
        if (G_U32(a39 + 0x53) != p53)
            goto L_63a01;
        p1a2 = G_U32(p1 + 0x1a);
        if (p1a2 == p53)
            goto L_63a01;
        if ((s32)(s8)G_U8(p1 + 0x59) > 3)
            goto L_63a18;
        if ((s32)(s8)G_U8(p1 + 0x57) < 0x0f)
            goto L_63a18;
    L_63a01:
        if (a39 != 0)
            goto L_64295;
    L_63a09:
        p1a2 = G_U32(p1 + 0x1a);
        if (p1a2 != p53)
            goto L_64295;
    L_63a18:
        lvl50 = cm_div50((s32)*(s16 *)(club + 0x80));
        eax_ = G_U32(p53);
        if (eax_ == G_U32(0x009cf36c)) {
            /* 0x00863a3c */
            u32 v10 = (u32)level * (u32)level * 10u;
            long double y = (long double)(s32)v10;
            if ((long double)value < y)
                value = (float)y;
            {
                long double c7 = G_F64(0x009690b8);
                if ((long double)value > c7)
                    value = (float)(((long double)value - c7) * G_F64(0x009689e0) + c7);
            }
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf2e4))
            goto L_64295;
        if (eax_ == G_U32(0x009cf360))
            goto L_64295;
        if (eax_ == G_U32(0x009cf488))
            goto L_64295;

        if (eax_ == G_U32(0x009cf4f8)) {
            /* 0x00863ac3 */
            value = (float)((long double)value * G_F64(0x00969040));
            if (G_U32(club + 0x57) != 0
                && G_U32(G_U32(club + 0x57)) == G_U32(0x009cf594)) {
                long double c8 = G_F64(0x00968898);
                if ((long double)value > c8)
                    value = (float)(((long double)value - c8) * G_F64(0x00968f20) + c8);
                goto L_64295;
            }
            if (p1a2 == p53) {
                /* 0x00863b1b reloads age; >31 bypasses the young-age checks. */
                s32 age = (s32)(s8)G_U8(p1 + 0x18);
                if (age > 0x1f || (age < 0x18
                    && G_U32(p1 + 0x61) != 0
                    && (s32)*(s16 *)(G_U32(p1 + 0x61) + 0xd) < 0xea6)) {
                    long double c9 = G_F64(0x0096a5e8);
                    if ((long double)value > c9)
                        value = (float)(((long double)value - c9) * G_F64(0x00968918) + c9);
                } else {
                    long double ca = G_F64(0x0096fec8);
                    if ((long double)value > ca)
                        value = (float)(((long double)value - ca) * G_F64(0x00968918) + ca);
                }
            } else {
                long double cb = G_F64(0x0096fec0);
                if ((long double)value > cb)
                    value = (float)(((long double)value - cb) * G_F64(0x009689e0) + cb);
            }
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf484)) {
            /* 0x00863bd1 */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x009689e0));
            long double c12 = G_F64(0x00968878);
            long double c13v = G_F64(0x0096feb8);
            value = (float)((long double)value * f);
            if ((long double)value > c12)
                value = (float)(((long double)value - c12) * G_F64(0x00968968) + c12);
            if ((long double)value > c13v)
                value = (float)(((long double)value - c13v) * G_F64(0x00968968) + c13v);
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf45c)) {
            /* 0x00863c9f */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x00969510));
            long double c14 = G_F64(0x00968910);
            value = (float)((long double)value * f);
            if ((long double)value > c14)
                value = (float)(((long double)value - c14) * G_F64(0x009689e0) + c14);
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf314)
            || eax_ == G_U32(0x009cf300)
            || eax_ == G_U32(0x009cf338)) {
            /* 0x00864203 */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x00969040));
            long double c15 = G_F64(0x00968888);
            value = (float)((long double)value * f);
            if ((long double)value > c15)
                value = (float)(((long double)value - c15) * G_F64(0x009689e0) + c15);
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf4a8)) {
            /* 0x00863d66 */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x009689e0));
            u32 c64 = G_U8(club + 0x64);
            value = (float)((long double)value * f);
            if (c64 == 1) {
                long double c16v = G_F64(0x00968878);
                if ((long double)value > c16v)
                    value = (float)(((long double)value - c16v) * G_F64(0x00968918) + c16v);
            } else {
                long double c17 = G_F64(0x00968f50);
                if ((long double)value > c17)
                    value = (float)(((long double)value - c17) * G_F64(0x00968918) + c17);
            }
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf510)
            || eax_ == G_U32(0x009cf2fc)
            || eax_ == G_U32(0x009cf2dc)
            || eax_ == G_U32(0x009cf3e4)
            || eax_ == G_U32(0x009cf224)) {
            /* 0x008641a0 */
            long double cv;
            if (G_U8(club + 0x64) == 1)
                cv = G_F64(0x0096a648);
            else
                cv = G_F64(0x009688a8);
            if ((long double)value > cv)
                value = (float)(((long double)value - cv) * G_F64(0x00968f20) + cv);
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf2c0)
            || eax_ == G_U32(0x009cf408)
            || eax_ == G_U32(0x009cf430)) {
            /* 0x008640d6 */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x00968968));
            u32 c64 = G_U8(club + 0x64);
            value = (float)((long double)value * f);
            if (c64 == 1) {
                long double c18 = G_F64(0x0096f870);
                if ((long double)value > c18)
                    value = (float)(((long double)value - c18) * G_F64(0x00967880) + c18);
            } else {
                long double c19 = G_F64(0x00968898);
                if ((long double)value > c19)
                    value = (float)(((long double)value - c19) * G_F64(0x00967880) + c19);
            }
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf244)) {
            /* 0x00863e98 */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x0096a6a8));
            u32 c64 = G_U8(club + 0x64);
            value = (float)((long double)value * f);
            if (c64 == 1) {
                long double c1a = G_F64(0x00968878);
                if ((long double)value > c1a)
                    value = (float)(((long double)value - c1a) * G_F64(0x00968918) + c1a);
            } else {
                long double c1b = G_F64(0x00968898);
                if ((long double)value > c1b)
                    value = (float)(((long double)value - c1b) * G_F64(0x00968918) + c1b);
            }
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf434) || eax_ == G_U32(0x009cf2ac)) {
            /* 0x0086400c */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x00968968));
            u32 c64 = G_U8(club + 0x64);
            value = (float)((long double)value * f);
            if (c64 == 1) {
                long double c1c = G_F64(0x00968878);
                if ((long double)value > c1c)
                    value = (float)(((long double)value - c1c) * G_F64(0x00968918) + c1c);
            } else {
                long double c1d = G_F64(0x00968898);
                if ((long double)value > c1d)
                    value = (float)(((long double)value - c1d) * G_F64(0x00968918) + c1d);
            }
            goto L_64295;
        }
        if (eax_ == G_U32(0x009cf260) || eax_ == G_U32(0x009cf218)) {
            /* 0x00863f75 */
            long double f = cm_clubfactor(p53, lvl50, G_F64(0x0096a6a8));
            long double c1e = G_F64(0x009688d0);
            value = (float)((long double)value * f);
            if ((long double)value > c1e)
                value = (float)(((long double)value - c1e) * G_F64(0x009689e0) + c1e);
            goto L_64295;
        }
        /* 0x00863f3b */
        {
            long double c1f = G_F64(0x0096a728);
            t = (long double)value * G_F64(0x00968e80);
            value = (float)t;
            if (t > c1f)
                value = (float)(((long double)value - c1f) * G_F64(0x009689e0) + c1f);
            goto L_64295;
        }
    }

    /* ----------------------------- finale ------------------------------- */
L_64295:
    if (iVar6 == 0)
        goto L_64436;
    if (local_18 != 0 && club != local_18) {
        r = cm_fn_004cf5f0(G_U32(0x00ae1050), p1, local_18);
        if (r != 0 && (G_U8(r + 0x4f) & 0xf0) == 0x70) {
            long double rv = (long double)(s32)G_S32(r + 0xc);
            if (rv < (long double)value)
                value = (float)rv;
        }
    }
    /* 0x008642e5 */
    if (param_3 == 0) {
        if (local_18 != 0)
            goto L_6435e;
        if ((s32)*(s16 *)(G_U32(p1 + 0x61) + 0xb) >= 0x8ca)
            goto L_64436;
    }
    /* 0x00864312 */
    r = cm_fn_005a0590(G_U32(0x00ae2a38), club);
    if (r == 0)
        goto L_644dc;
    i = (s32)cm_fn_005968d0(r, p1, p4);
    if ((long double)i < (long double)value)
        value = (float)i;
    if (local_18 == 0 || param_3 != 0)
        goto L_64436;

L_6435e:                                      /* 0x0086435e */
    if ((s32)(s8)G_U8(p1 + 0x57) <= 0xa)
        goto L_64436;
    if ((s32)(s8)G_U8(p1 + 0x59) >= 6)
        goto L_64436;
    if ((s32)*(s16 *)(iVar6 + 0xb) <= 0x1a5e)
        goto L_64436;
    if (!(G_U8(iVar6 + 0x15) == 0x14
          || G_U8(iVar6 + 0x14) == 0x14
          || G_U8(iVar6 + 0x13) == 0x14))
        goto L_64436;
    if ((s32)(s8)G_U8(p1 + 0x18) <= 0x14)
        goto L_64436;
    cm_fn_004cf5f0(G_U32(0x00ae1050), p1, local_18);
    r = cm_fn_005a0590(G_U32(0x00ae2a38), club);
    if (r == 0)
        goto L_644dc;
    i = (s32)cm_fn_005968d0(r, p1, p4);
    {
        long double a = (long double)value * G_F64(0x00968928);
        if (G_U8(iVar6 + 0x14) == 0x14) {
            if (a < (long double)i)
                value = (float)((long double)value * G_F64(0x009689c0));
        } else {
            if (a < (long double)i) {
                if (G_U8(iVar6 + 0x13) == 0x14)
                    value = (float)((long double)value * G_F64(0x009689b8));
                else
                    value = (float)((long double)value * G_F64(0x00968980));
            }
        }
    }

L_64436:                                      /* 0x00864436 */
    if (G_U8(club + 0x64) == 1) {
        if ((s32)(s8)c13 < 0x14
            && (long double)value < (long double)G_F32(0x0096d750))
            return 75.0f;
        if (!((long double)value < (long double)G_F32(0x00968960)))
            goto L_644dc;
        value = 100.0f;
        goto L_644dc;
    }
    if ((s32)*(s16 *)(club + 0x80) > 0x8ca
        && (long double)value < (long double)G_F32(0x0096d750))
        return 75.0f;
    if ((long double)value < (long double)G_F32(0x00969560))
        return 50.0f;

L_644dc:                                      /* 0x008644dc */
    return value;
}
