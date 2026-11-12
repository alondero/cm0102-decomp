/* Translation of the original listing at preferred-image VA 0x005ac920.
   The original reserves 0x14 bytes (`sub esp, 0x14`) and keeps four saved
   registers below it.  With L = E-0x14 the locals are:
     L+0x00  word  <- *(u16*)arg1, later the dword counter i
     L+0x04  word  <- (u16)*(u8*)arg2, later the dword counter j
     L+0x08  word  <- *(u16*)arg0, later the dword counter k
     L+0x0c  byte  <- al, the low byte returned by 0x0048dd70
     L+0x10  dword <- ecx, the incoming this pointer
   Only the low word of the first three slots is written at entry; the upper
   bytes stay uninitialized and are read back through dword accesses, exactly
   as the original does.  Stack arguments: arg0 at E+4, arg1 at E+8, arg2 at
   E+0xc, arg3 at E+0x10, arg4 at E+0x14, arg5 at E+0x18, arg6 at E+0x1c,
   arg7 at E+0x20; the 0x2c-byte pop also covers the three unused words.
   The 0x0048dd70 lookup index is arg4 (0x005ac949 reads E+0x14), while arg3
   is the flag that selects the ascending or descending scan. */

#include "cmre.h"

extern u8 CM_CDECL cm_fn_0048dd70(u32 param_1, u32 param_2);
extern u32 CM_THISCALL cm_fn_005abf10(u32 self, volatile u32 first,
                                      volatile u32 second, volatile u32 third,
                                      volatile s8 fourth);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

#define FR_U8(off)  (*(volatile u8 *)(frame + (off)))
#define FR_U16(off) (*(volatile u16 *)(frame + (off)))
#define FR_S16(off) (*(volatile s16 *)(frame + (off)))
#define FR_U32(off) (*(volatile u32 *)(frame + (off)))

u32 CM_THISCALL cm_fn_005ac920(u32 self, u32 arg0, u32 arg1, u32 arg2,
                                u32 arg3, u32 arg4, u32 arg5, u32 arg6,
                                u32 arg7, u32 arg8, u32 arg9, u32 arg10)
{
    u8 frame[0x14];
    u8 al_res;
    u32 ebx;
    u32 r;
    u32 esi_v;
    u32 g;
    int in_z;

    (void)arg8;
    (void)arg9;
    (void)arg10;
    in_z = 0;

    /* 005ac92b: the incoming ecx is kept at L+0x10 and read back as the
       thiscall receiver of 0x005abf10. */
    FR_U32(0x10) = self;

    /* 005ac930..005ac95a: entry loads, in listing order. */
    FR_U16(8) = *(volatile u16 *)arg0;
    FR_U16(0) = *(volatile u16 *)arg1;
    FR_U16(4) = (u16)*(volatile u8 *)arg2;

    /* 005ac94e..005ac96a: global at 0x00ae23d0 indexed by arg4, second
       argument zero. */
    g = CM_GLOBAL(volatile u32, 0x00ae23d0);
    al_res = cm_fn_0048dd70(g + (u32)arg4 * 107u, 0u);
    FR_U8(0xc) = al_res;
    ebx = FR_U32(0xc);

    /* 005ac96f..005ac988: arg7 -> edi, arg6 -> ebp, arg3 -> eax; the zero
       flag selects the descending scan. */
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
    esi_v = cm_fn_005abf10(FR_U32(0x10), FR_U32(8), FR_U32(0), FR_U32(4), ebx);
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
