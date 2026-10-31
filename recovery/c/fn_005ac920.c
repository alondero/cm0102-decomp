/* WIP: detail mismatch, case 1 */
#include "cmre.h"

/* Stack model: E is the entry esp; the one-word-wide local area is frame[0..0x14)
   at E-0x14 (L). Locals: frame+0 counter i (word from *arg1, later dword),
   frame+4 counter j (word from the byte at arg2, later dword), frame+8 counter k
   (word from *arg0, later dword), frame+0xc byte result of 0x0048dd70. Only the
   low word of the first store is initialized; the upper bytes stay uninitialized
   as in the original and are read through volatile dword accesses.
   Stack arguments: arg0 at E+4 (short *), arg1 at E+8 (short *), arg2 at E+0xc
   (byte *), arg3 at E+0x10 (index), arg4 at E+0x14, arg5 at E+0x18, arg6 at
   E+0x1c, arg7 at E+0x20. Arguments 8-10 are part of the 0x2c-byte pop but
   unused by this routine. */

extern u8 CM_CDECL cm_fn_0048dd70(u32 param_1, u32 param_2);
extern u32 CM_THISCALL cm_fn_005abf10(void *self, u32 p2, u32 p3, u32 p4, u8 p5);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 self);

#define FR_U8(off)  (*(volatile u8 *)(frame + (off)))
#define FR_U16(off) (*(volatile u16 *)(frame + (off)))
#define FR_S16(off) (*(volatile s16 *)(frame + (off)))
#define FR_U32(off) (*(volatile u32 *)(frame + (off)))

u32 CM_THISCALL cm_fn_005ac920(u32 self, u32 arg0, u32 arg1, u32 arg2,
                               u32 arg3, u32 arg4, u32 arg5, u32 arg6,
                               u32 arg7, u32 arg8, u32 arg9, u32 arg10)
{
    u8 frame[0x14];
    u8 bres;
    u8 bval;
    u32 r;
    u32 esi_v;
    u32 g;
    int in_z;

    (void)arg8;
    (void)arg9;
    (void)arg10;
    in_z = 0;

    /* Entry loads, in listing order: *arg0 (word) -> k; *arg1 (word) -> i;
       *arg2 (byte, zero-extended) -> j. */
    FR_U16(8) = *(volatile u16 *)arg0;
    FR_U16(0) = *(volatile u16 *)arg1;
    FR_U16(4) = (u16)*(volatile u8 *)arg2;

    g = CM_GLOBAL(volatile u32, 0x00ae23d0);
    bres = cm_fn_0048dd70(g + (u32)arg3 * 107u, 0u);
    FR_U8(0xc) = bres;
    bval = bres;

    if (arg3 == 0u) {
        in_z = 1;
        goto z_top;
    }

main_top:
    if (FR_S16(0) >= 0x16e) {
        goto k_end;
    }
main_j:
    if (FR_S16(4) >= 3) {
        goto i_inc;
    }
body:
    esi_v = cm_fn_005abf10((void *)self, FR_U32(8), FR_U32(0), FR_U32(4), bval);
    if (esi_v == 0u) {
        goto body_fail;
    }
    *(volatile u32 *)(esi_v + 0x0au) = *(volatile u32 *)(esi_v + 0x02u);
    r = cm_fn_0068d1b0(esi_v);
    if (r == 0u) {
        goto body_fail;
    }
cand:
    {
        u32 c = 1u;
        if (arg4 != 0xffffffffu && arg4 != *(volatile u32 *)r) {
            c = 0u;
        }
        if (arg5 != 0xffffffffu && arg5 != (u32)(s32)*(volatile s16 *)(r + 0x34u)) {
            c = 0u;
        }
        if (arg6 != 0xffffffffu &&
            arg6 != *(volatile u32 *)(r + 0x0cu) &&
            arg6 != *(volatile u32 *)(r + 0x10u)) {
            c = 0u;
        }
        if (arg7 == 0xffffffffu ||
            arg7 == *(volatile u32 *)(r + 0x0cu) ||
            arg7 == *(volatile u32 *)(r + 0x10u)) {
            if (c == 1u) {
                goto success;
            }
        }
    }
    r = cm_fn_0068d1b0(esi_v);
    if (r != 0u) {
        goto cand;
    }
body_fail:
    if (in_z) {
        goto z_next_j;
    }
    {
        u32 jn = FR_U32(4) + 1u;
        FR_U32(4) = jn;
        if ((s16)(u16)jn < 3) {
            goto body;
        }
    }
i_inc:
    {
        u32 i_val = FR_U32(0);
        FR_U32(4) = 0u;
        i_val = i_val + 1u;
        FR_U32(0) = i_val;
        if ((s16)(u16)i_val < 0x16e) {
            goto main_j;
        }
    }
k_end:
    {
        u32 k_val = FR_U32(8) + 1u;
        u32 ecx_w = (u16)*(volatile u16 *)arg0;
        s32 d;
        FR_U32(8) = k_val;
        d = (s32)(s16)(u16)k_val - (s32)ecx_w;
        FR_U32(0) = 0u;
        if (d >= 5) {
            return 0u;
        }
    }
    goto main_top;

z_top:
    if (FR_S16(0) < 0) {
        goto z_i_neg;
    }
z_j:
    if (FR_S16(4) < 0) {
        goto z_j_neg;
    }
    goto body;

z_next_j:
    {
        u32 jn = FR_U32(4) - 1u;
        FR_U32(4) = jn;
        if ((s16)(u16)jn >= 0) {
            goto body;
        }
    }
z_j_neg:
    {
        u32 iv = FR_U32(0);
        FR_U32(4) = 2u;
        iv = iv - 1u;
        FR_U32(0) = iv;
        if ((s16)(u16)iv >= 0) {
            goto z_j;
        }
    }
z_i_neg:
    {
        u32 kv = FR_U32(8);
        s32 d;
        kv = kv - 1u;
        FR_U32(0) = 0x16du;
        d = (s32)*(volatile s16 *)arg0;
        FR_U32(8) = kv;
        d = d - (s32)(s16)(u16)kv;
        if (d >= 5) {
            return 0u;
        }
    }
    goto z_top;

success:
    *(volatile u16 *)arg1 = FR_U16(0);
    *(volatile u16 *)arg0 = FR_U16(8);
    *(volatile u8 *)arg2 = FR_U8(4);
    return r;
}
