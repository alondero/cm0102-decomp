#include "cmre.h"

/* Original routine 0x005c6350 (preferred-image VA, image base 0x00400000).
   Convention: ret 8, two stack arguments (arg1 = pointer, arg2 = low byte used
   as a signed index). Locals kept as plain variables:
     result_slot  stored at frame+0x14 (u32), zeroed before the first call
     key_hi       stored at frame+0x13 (byte), computed from the object at arg1
     key_lo       stored at frame+0x12 and frame+0x16 (byte), zeroed before the first call
   Branch A runs when the first callee returns nonzero (loop over the table at
   the global 0x00ae23a8, bound 0x00ae2350); branch B runs when it returns zero
   (loop over 0x00ae23b4, bound 0x00ae235c).
   Non-stack memory reads are volatile so -O2 cannot sink or drop them: the
   original faults at the listing's own read sites (0x005c6384 et al.). */

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern int cm_fn_0090cfc0(int param_1);
extern int cm_fn_0053b3d0(int *param_1);

CM_STDCALL u32 cm_fn_005c6350(u32 arg1, u32 arg2)
{
    u32 p;
    u32 q;
    u32 r;
    u32 want;
    u32 result_slot = 0;
    u8 key_hi;
    u8 key_lo = 0;
    u16 i = 0;

    r = cm_fn_0053b900((u32 *)*(volatile u32 *)arg1);
    p = *(volatile u32 *)arg1;

    if (r != 0) {
        u32 ecx_value;
        s16 w;
        s32 t;
        s32 g;

        ecx_value = *(volatile u32 *)(p + 0x53u);
        q = CM_GLOBAL(volatile u32, 0x00ae23a8);
        want = *(volatile u32 *)(ecx_value + 0x71u);
        w = *(volatile s16 *)(p + 0x80u);
        t = (s32)(((long long)(s32)0x10624dd3 * (long long)(s32)w) >> 32);
        t = t >> 5;
        t = t + (s32)((u32)t >> 31);
        key_hi = (u8)(t + 1);
        g = CM_GLOBAL(volatile s32, 0x00ae2350);
        if (g <= 0) {
            return (u32)g;
        }
        for (;;) {
            if (*(volatile u32 *)(q + 0x71u) == want) {
                s16 cx = (s16)(s8)key_hi;
                if (*(volatile s16 *)(q + 0x8eu) <= cx) {
                    s32 c0;
                    u8 d;
                    u8 bl;

                    c0 = cm_fn_0090cfc0(5);
                    d = *(volatile u8 *)(q + 0x8eu);
                    d = (u8)(d - 2u);
                    bl = (u8)((u8)c0 + d);
                    if ((s8)bl > (s8)key_lo) {
                        result_slot = (u32)cm_fn_0053b3d0((int *)q);
                        key_lo = bl;
                    }
                    if ((s32)(s8)key_lo > (s32)(s8)key_hi - 4) {
                        goto epilogue;
                    }
                }
            }
            g = CM_GLOBAL(volatile s32, 0x00ae2350);
            q = q + 0x122u;
            i = (u16)(i + 1u);
            if ((s32)(s16)i >= g) {
                break;
            }
        }
        goto epilogue;
    } else {
        s16 w;
        s32 t;
        s32 g;

        w = *(volatile s16 *)(p + 0x80u);
        want = *(volatile u32 *)(p + 0x53u);
        t = (s32)(((long long)(s32)0x10624dd3 * (long long)(s32)w) >> 32);
        t = t >> 5;
        t = t + (s32)((u32)t >> 31);
        key_hi = (u8)(t + 1);
        g = CM_GLOBAL(volatile s32, 0x00ae235c);
        q = CM_GLOBAL(volatile u32, 0x00ae23b4);
        if (g <= 0) {
            return (u32)g;
        }
        for (;;) {
            if (*(volatile u32 *)(q + 0x53u) == want) {
                s16 w2 = *(volatile s16 *)(q + 0x80u);
                s32 t2;
                u8 bl2;
                s32 c0;
                u8 cl;
                u8 al;

                t2 = (s32)(((long long)(s32)0x10624dd3 * (long long)(s32)w2) >> 32);
                t2 = t2 >> 5;
                t2 = t2 + (s32)((u32)t2 >> 31);
                bl2 = (u8)t2;
                c0 = cm_fn_0090cfc0(5);
                cl = (u8)(bl2 - 2u);
                al = (u8)((u8)c0 + cl);
                cl = key_hi;
                if ((s8)al <= (s8)cl) {
                    if ((s8)al > (s8)key_lo) {
                        result_slot = q;
                        key_lo = bl2;
                    }
                    if ((s32)(s8)key_lo > (s32)(s8)cl - 4) {
                        goto epilogue;
                    }
                }
            }
            g = CM_GLOBAL(volatile s32, 0x00ae235c);
            q = q + 0x245u;
            i = (u16)(i + 1u);
            if ((s32)(s16)i >= g) {
                break;
            }
        }
        goto epilogue;
    }

epilogue:
    if (result_slot != 0) {
        *(volatile u32 *)(arg1 + (u32)(s32)(s8)arg2 * 4u + 0x15u) = result_slot;
    }
    return result_slot;
}
