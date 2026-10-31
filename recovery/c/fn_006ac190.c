#include "cmre.h"

/* Element pointer: 0x00ae23a8 base, 290 bytes per index (145 * index * 2).
   Element field used here: 0x37 s8. Frame scratch: text buffer 1 at offset 0x8c
   (0x1f4 bytes, ends at the saved return address) and text buffer 2 at offset
   0x14 (0x78 bytes), in one array so their adjacency matches the original frame. */

extern unsigned long long CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, void *param_2, u32 param_3, u32 param_4);
extern u32 CM_CDECL cm_fn_0053dba0(u32 param_1, void *param_2, u32 param_3, u32 param_4);
extern u32 CM_CDECL cm_fn_0053e990(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32 arg1, void *arg2, u32 arg3, u32 arg4);
extern char *CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);

u32 CM_STDCALL cm_fn_006ac190(u32 a1, u32 a2, u32 a3)
{
    u8 fr[0x280];
    u32 x0;
    u32 x1;
    u32 x2;
    u32 x3;
    u8 x4;
    u32 x5;
    u32 x6;
    u32 x7;
    u32 e;
    u32 g2;
    u32 r;
    u32 m;

    x0 = (u32)cm_fn_00785830(a1, 0u);
    x1 = (u32)cm_fn_00785830(a1, 1u);
    x2 = (u32)cm_fn_00785830(a1, 2u);
    x3 = (u32)cm_fn_00785830(a1, 3u);
    x4 = (u8)cm_fn_00785830(a1, 4u);
    x5 = (u32)cm_fn_00785830(a1, 5u);
    x6 = (u32)cm_fn_00785830(a1, 6u);
    x7 = (u32)cm_fn_00785830(a1, 7u);
    (void)cm_fn_00785830(a1, 8u);

    e = CM_GLOBAL(volatile u32, 0x00ae23a8) + 290u * x5;
    g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
    (void)cm_fn_0053dba0(g2 + 110u * x0, (void *)&fr[0x8c], 0x1f4u, 1u);

    if (a2 == 0u) goto p2;

    (void)cm_fn_006181d0(e, (void *)&fr[0x14], 0x78u, 9u);
    if (x4 == 0u) goto p1_al0;

    if (x6 == 0xffffffffu) {
        if (x7 == 0xffffffffu) {
            m = (u32)(s32)CM_GLOBAL(volatile s8, e + 0x37u);
            (void)cm_fn_0066f4e0(0xde1f64u, 0xa128c0u, m, m, 0x64u, (void *)&fr[0x14], (void *)&fr[0x8c]);
        } else {
            m = (u32)(s32)CM_GLOBAL(volatile s8, e + 0x37u);
            if (x7 == 1u) {
                (void)cm_fn_0066f4e0(0xde1f64u, 0xa12ab4u, m, m, 0x64u, (void *)&fr[0x14], (void *)&fr[0x8c]);
            } else {
                (void)cm_fn_0066f4e0(0xde1f64u, 0xa12994u, m, m, 0x64u, (void *)&fr[0x14], (void *)&fr[0x8c], 0xffffffffu);
            }
        }
    } else if (x6 == 1u) {
        m = (u32)(s32)CM_GLOBAL(volatile s8, e + 0x37u);
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa12c84u, m, m, 0x64u, (void *)&fr[0x14], (void *)&fr[0x8c]);
    } else {
        m = (u32)(s32)CM_GLOBAL(volatile s8, e + 0x37u);
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa12b88u, m, m, 0x64u, (void *)&fr[0x14], (void *)&fr[0x8c], x6);
    }
    goto tail;

p1_al0:
    m = (u32)(s32)CM_GLOBAL(volatile s8, e + 0x37u);
    (void)cm_fn_0066f4e0(0xde1f64u, 0xa127f0u, m, m, 0x64u, (void *)&fr[0x14], (void *)&fr[0x8c]);
    goto tail;

p2:
    if (x4 == 0u) {
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa12778u);
        goto tail;
    }
    if (x6 == 0xffffffffu && x7 == 0xffffffffu) {
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa127c0u, 0x64u, (void *)&fr[0x8c]);
    } else {
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa12790u, 0x64u, (void *)&fr[0x8c]);
    }

tail:
    (void)cm_fn_004ae660(a3, 0xde1f64u);
    g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
    r = cm_fn_0053e990(g2 + 110u * x0, x1, x2, x3, 0u);
    if (r == 0u) return r;
    g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
    return cm_fn_004ae8a0(a3, (void *)&fr[0x8c], 0x7dau, g2 + 110u * x0);
}
