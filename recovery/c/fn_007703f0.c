#include "cmre.h"

/* Element index: 581 bytes per x4 (x4 + 145 * x4 * 4) inside the table at 0x00ae23b4.
   Scratch frame: fr+0x0c is the 0xc8-byte text buffer (buf1), fr+0xd4 is the
   0x12c-byte buffer (buf2); both are in one array so their adjacency matches. */

extern unsigned long long CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, void *param_2, u32 param_3, u32 param_4);
extern u32 CM_CDECL cm_fn_00542100(u32 param_1, u32 param_2, void *param_3, u32 param_4, u32 param_5, u32 param_6);
extern u32 CM_CDECL cm_fn_0053e140(u32 p1, u32 p2, u32 p3, void *p4, u32 p5, u32 p6, u32 p7);
extern u32 CM_CDECL cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern char *CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);

u32 CM_STDCALL cm_fn_007703f0(u32 a1, u32 a2, u32 a3)
{
    u8 fr[0x200];
    u32 x0;
    u32 x1;
    u32 x2;
    u32 x3;
    u32 x4;
    u32 x5;
    u32 x6;
    u32 esi;
    u32 g;
    u32 m;
    u32 r;

    x0 = (u32)cm_fn_00785830(a1, 0u);
    x1 = (u32)cm_fn_00785830(a1, 1u);
    x2 = (u32)cm_fn_00785830(a1, 2u);
    x3 = (u32)cm_fn_00785830(a1, 3u);
    x4 = (u32)cm_fn_00785830(a1, 4u);

    if (a2 == 0u) {
        /* Zero path: the listing pushes index 5 and calls with ECX = a1 (EBX unchanged). */
        x5 = (u32)cm_fn_00785830(a1, 5u);
        (void)cm_fn_0053e140(x1, x2, x3, (void *)&fr[0xd4], 0x12cu, 5u, x0);

        esi = 581u * x4;
        g = CM_GLOBAL(volatile u32, 0x00ae23b4);
        m = (u32)(s32)CM_GLOBAL(volatile s8, esi + g + 0x52u);
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa5c73cu, m, m, (void *)&fr[0xd4], esi + g + 0x38u);

        (void)cm_fn_004ae660(a3, 0xde1f64u);

        g = CM_GLOBAL(volatile u32, 0x00ae23bc);
        r = cm_fn_0053e990(110u * x0 + g, x1, x2, x3, 0u);
        if (r != 0u) {
            g = CM_GLOBAL(volatile u32, 0x00ae23bc);
            (void)cm_fn_004ae8a0(a3, (void *)&fr[0xd4], 0x7d7u, 110u * x0 + g);
        }

        /* 0x7706a3: this path passes the element's +0x38 field, not a frame buffer. */
        g = CM_GLOBAL(volatile u32, 0x00ae23b4);
        if (x5 != 0u) {
            return cm_fn_004ae8a0(a3, (void *)(esi + g + 0x38u), 0x7d6u, esi + g);
        }
        return cm_fn_004ae8a0(a3, (void *)(esi + g + 0x38u), 0x7d5u, esi + g);
    }

    x5 = (u32)cm_fn_00785830(a1, 5u);
    x6 = (u32)cm_fn_00785830(a1, 6u);
    /* mov bl, al: only the low byte of EBX (X6) is tested below; the upper EBX bits
       are never observed before EBX is reloaded. */

    esi = 581u * x4;
    g = CM_GLOBAL(volatile u32, 0x00ae23b4);
    (void)cm_fn_00542100(esi + g, x5, (void *)&fr[0x0c], 0xc8u, 2u, 1u);
    (void)cm_fn_0053e140(x1, x2, x3, (void *)&fr[0xd4], 0x12cu, 1u, x0);

    if (((u8)x6) == 0u) {
        g = CM_GLOBAL(volatile u32, 0x00ae23b4);
        m = (u32)(s32)CM_GLOBAL(volatile s8, esi + g + 0x52u);
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa5c9f0u, m, m, (void *)&fr[0xd4], (void *)&fr[0x0c]);
    } else {
        g = CM_GLOBAL(volatile u32, 0x00ae23b4);
        m = (u32)(s32)CM_GLOBAL(volatile s8, esi + g + 0x52u);
        (void)cm_fn_0066f4e0(0xde1f64u, 0xa5c944u, m, m, (void *)&fr[0xd4], (void *)&fr[0x0c]);
    }

    /* mov ebx, [a3]: EBX is reloaded with the third argument. */
    (void)cm_fn_004ae660(a3, 0xde1f64u);

    g = CM_GLOBAL(volatile u32, 0x00ae23bc);
    r = cm_fn_0053e990(110u * x0 + g, x1, x2, x3, 0u);
    if (r != 0u) {
        g = CM_GLOBAL(volatile u32, 0x00ae23bc);
        (void)cm_fn_004ae8a0(a3, (void *)&fr[0xd4], 0x7d7u, 110u * x0 + g);
    }

    g = CM_GLOBAL(volatile u32, 0x00ae23b4);
    if (x5 != 0u) {
        return cm_fn_004ae8a0(a3, (void *)&fr[0x0c], 0x7d6u, esi + g);
    }
    return cm_fn_004ae8a0(a3, (void *)&fr[0x0c], 0x7d5u, esi + g);
}
