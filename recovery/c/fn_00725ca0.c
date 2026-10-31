#include "cmre.h"

/* Stack model: B is esp after the four register pushes (edi, esi, ebp, ebx).
   frame[n] is the original byte at B + 0x10 + n. The count word is at
   frame[2] (B+0x12), the int array at frame[4] (B+0x14, stride 4). The one
   stack argument is a 16-bit value at B+0x58 (above the return address).
   Globals and the object at 0x00b74450 are the original VAs. */

extern u32 cm_fn_00765480(u32 arg);
extern void CM_THISCALL cm_fn_007ffd80(void *self, u32 p2, u32 p3, u32 p4);
extern u32 CM_THISCALL cm_fn_007ffdd0(void *self, u32 key);
extern u32 CM_FASTCALL cm_fn_00803d80(void *self);
extern void CM_THISCALL cm_fn_00802f80(void *self, u32 p2, u32 p3, u32 p4, u32 p5);
extern char *cm_fn_0066f4e0(u32 dst, u32 fmt, ...);
extern void cm_fn_00717cc0(u32 p1, u32 p2);
extern void cm_fn_00718aa0(u32 p1, u32 p2);
extern u32 cm_fn_00474e00(u32 p1);
extern void cm_fn_00725f70(void);

#define OBJ_B74450 ((void *)0x00b74450u)
#define GLOBAL_WORD CM_GLOBAL(volatile s16, 0x00b77c7a)

s32 CM_CDECL cm_fn_00725ca0(s16 arg0)
{
    u8 frame[0x44];
    u32 p, ebp_v, ecx_v, esi_v, rec, e, v, r, off, eax_v;
    u32 base, ecx2;
    s32 di_g;
    u8 bl, dl;
    s8 idx;
    s16 cnt;
    u16 cx, cx_word;
    s16 dx_a;

    p = (u32)cm_fn_00765480(0u);
    if (p != 0u) {
        ebp_v = *(volatile u32 *)(p + 0xe2u);
    } else {
        ebp_v = 0u;
    }

    if (CM_GLOBAL(volatile u32, 0x00b77c84) == 0u) {
        goto x2;
    }
    ecx_v = CM_GLOBAL(volatile u32, 0x00dd6be0);
    if (ecx_v != 1u) {
        goto y_path;
    }
    {
        u32 edx_v = CM_GLOBAL(volatile u32, 0x00dd6be4);
        ecx_v = 0u;
        CM_GLOBAL(volatile u32, 0x00dd6be0) = 0u;
        CM_GLOBAL(volatile u32, 0x00dd6be4) = 0u;
        if (edx_v != 3u) {
            goto y_path;
        }
    }

    /* First table path. The low word of EAX is the global; the high word is
       carried from the 0x00765480 result still held in EAX. */
    {
        u32 eax_val = (p & 0xffff0000u) | (u16)GLOBAL_WORD;
        cm_fn_007ffd80(OBJ_B74450, (u32)(frame + 4), (u32)(frame + 2), eax_val);
    }
    bl = 0u;
    cnt = *(volatile s16 *)(frame + 2);
    if (cnt <= 0) {
        goto nofound;
    }
    for (;;) {
        idx = (s8)bl;
        e = *(volatile u32 *)(frame + 4 + 4 * (u32)(s32)idx);
        off = ((u32)(s32)GLOBAL_WORD * 3u) << 8;
        if (e == *(volatile u32 *)(0x00b74c26u + off)) {
            goto cont;
        }
        if (cm_fn_007ffdd0(OBJ_B74450, e) != 0u) {
            goto cont;
        }
        v = *(volatile u32 *)(frame + 4 + 4 * (u32)(s32)idx);
        r = (u32)cm_fn_00765480(v);
        ecx_v = *(volatile u32 *)(r + 0xe2u);
        if (ecx_v == ebp_v) {
            goto found_first;
        }
    cont:
        bl = (u8)(bl + 1u);
        if (!((s16)(s8)bl < *(volatile s16 *)(frame + 2))) {
            break;
        }
    }

nofound:
    if (cm_fn_00803d80(OBJ_B74450) == 0u) {
        return -11;
    }
    cm_fn_0066f4e0(0x00de1f64u, 0x00a16d94u);
    cx_word = (u16)GLOBAL_WORD;
    cm_fn_00802f80(OBJ_B74450, 0x00de1f64u, 0u, 0u, cx_word);
    off = ((u32)(s32)GLOBAL_WORD * 3u) << 8;
    cm_fn_00718aa0(*(volatile u32 *)(0x00b74c26u + off), ebp_v);
    return -4;

found_first:
    idx = (s8)bl;
    CM_GLOBAL(volatile u32, 0x00b77c4a) = *(volatile u32 *)(frame + 4 + 4 * (u32)(s32)idx);
    return -6;

y_path:
    if (ecx_v == 0x43u) {
        CM_GLOBAL(volatile u32, 0x00dd6be0) = 0u;
        cm_fn_00725f70();
        return -4;
    }

x2:
    dx_a = arg0;
    di_g = (s32)(s16)GLOBAL_WORD;
    if (dx_a == (s16)-1) {
        ecx2 = 0u;
    } else {
        base = *(volatile u32 *)((((u32)di_g * 3u) << 8) + 0x00b74c4cu);
        ecx2 = *(volatile u32 *)(base + (u32)(s32)dx_a * 396u + 0x000ba966u);
    }
    cx = (u16)ecx2;
    if (cx == 0u) {
        cx = (u16)CM_GLOBAL(volatile s16, 0x00dd6bde);
    }
    if (cx != 0x3e8u) {
        return 0;
    }

    if (CM_GLOBAL(volatile u32, 0x00b67a48) == 0u) {
        goto x3;
    }
    eax_v = CM_GLOBAL(volatile u32, 0x00b67a50);
    ebp_v = *(volatile u32 *)(eax_v + 0x15u);
    for (dl = 1u; dl <= 2u; dl = (u8)(dl + 1u)) {
        if ((s32)ebp_v <= 0) {
            continue;
        }
        eax_v = CM_GLOBAL(volatile u32, 0x00b67a50);
        esi_v = *(volatile u32 *)(eax_v + 0x0du);
        bl = 0u;
        for (;;) {
            idx = (s8)bl;
            rec = esi_v + 84u * (u32)(s32)idx;
            if (dl == 1u) {
                if (*(volatile u32 *)(rec + 0x10u) != 0u &&
                    *(volatile u32 *)(rec + (u32)di_g * 4u + 0x14u) == 0u) {
                    goto found_rec;
                }
            } else {
                if (*(volatile u32 *)(rec + 0x0cu) != 0u &&
                    *(volatile u32 *)(rec + (u32)di_g * 4u + 0x14u) == 0u) {
                    goto found_rec;
                }
            }
            bl = (u8)(bl + 1u);
            if (!((s32)(s8)bl < (s32)ebp_v)) {
                break;
            }
        }
    }
    return -8;

found_rec:
    cm_fn_00717cc0(*(volatile u32 *)((((u32)di_g * 3u) << 8) + 0x00b74c26u), rec);
    return -4;

x3:
    if (p == 0u) {
        return 0;
    }
    if (ebp_v == 0u) {
        return 0;
    }
    if (cm_fn_00474e00(p) != 0u) {
        return -4;
    }
    cm_fn_00725f70();
    return -4;
}
