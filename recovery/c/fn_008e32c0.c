/* WIP: fails the semantic gate (fault-path cases); not accepted. */
#include "cmre.h"

/* Globals (preferred-image VAs), read as 32-bit values where the listing reads them:
   0x00ae23b4 base added to the row index (row stride 581 bytes, see ROW below);
   0x00ae23bc base added to the 110-byte scaled index. */

/* Thiscall getter: the object is ECX (arg1 of this routine), one pushed index. */
extern unsigned long long CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 p2, u32 p3, u32 p4);
extern u32 cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern unsigned long long cm_fn_006181d0(u32 a1, u32 a2, u32 a3, u32 a4);
extern u32 cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

#define GETTER(obj, idx) ((u32)cm_fn_00785830((obj), (idx)))
#define ROW(i) ((u32)(i) * 581u)

/* The routine takes three stack arguments (ret 0xc) and does not read ECX before
   writing it, so it is stdcall. Arg1 is the object used for every getter call. */
void CM_STDCALL cm_fn_008e32c0(u32 a1, u32 a2, u32 a3)
{
    /* Scratch frame: [0x1c, 0x80) is the 0x64-byte buffer for 0x006181d0 and the
       first variadic call of the non-(-1) path; [0x80, 0x1ac) is the 0x12c-byte
       buffer for 0x0053e140, 0x0066f4e0 and 0x004ae8a0. */
    u8 frame[0x1ac];
    u32 t0;
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    u32 ed;
    u32 g1;
    u32 y;
    u32 x1;
    u32 dw;
    s32 aval;
    s32 bval;
    u32 base;
    u32 g4;
    s32 r;

    t0 = GETTER(a1, 0u);

    if (a2 == 0u) {
        t1 = GETTER(a1, 1u);
        t2 = GETTER(a1, 2u);
        t3 = GETTER(a1, 3u);
        cm_fn_0053e140(t1, t2, t3, (u32)&frame[0x80], 0x12cu, 5u, t0);
        cm_fn_0066f4e0(0x00de1f64u, 0x00a9d364u, (u32)&frame[0x80], 0x64u);
        cm_fn_004ae660(a3, 0x00de1f64u);
        g4 = CM_GLOBAL(volatile u32, 0x00ae23bc);
        base = t0 * 110u;
        r = (s32)cm_fn_0053e990(base + g4, t1, t2, t3, 0u);
        if (r != 0) {
            g4 = CM_GLOBAL(volatile u32, 0x00ae23bc);
            y = base + g4;
            cm_fn_004ae8a0(a3, (u32)&frame[0x80], 0x7d7u, y);
        }
        return;
    }

    t1 = GETTER(a1, 1u);
    t2 = GETTER(a1, 2u);
    t3 = GETTER(a1, 3u);
    t4 = GETTER(a1, 4u);
    t5 = GETTER(a1, 5u);
    cm_fn_0053e140(t1, t2, t3, (u32)&frame[0x80], 0x12cu, 1u, t0);

    if (t5 != 0xffffffffu) {
        ed = ROW(t4);
        g1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
        dw = *(volatile u32 *)(ed + g1 + 0x53u);
        cm_fn_006181d0(dw, (u32)&frame[0x1c], 0x64u, 8u);
        g1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
        y = g1 + ROW(t5);
        aval = *(volatile s8 *)(ed + g1 + 0x52u);
        bval = *(volatile s8 *)(y + 0x52u);
        cm_fn_0066f4e0(0x00de1f64u, 0x00a9d444u,
                       (u32)bval, (u32)bval, 0x64u, 0x64u,
                       (u32)aval, (u32)aval, y + 0x38u,
                       (u32)&frame[0x80], (u32)&frame[0x1c],
                       ed + g1 + 0x38u);
    } else {
        ed = ROW(t4);
        g1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
        dw = *(volatile u32 *)(ed + g1 + 0x53u);
        cm_fn_006181d0(dw, (u32)&frame[0x1c], 0x64u, 8u);
        g1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
        aval = *(volatile s8 *)(ed + g1 + 0x52u);
        cm_fn_0066f4e0(0x00de1f64u, 0x00a9d39cu,
                       0x64u, 0x64u,
                       (u32)aval, (u32)aval,
                       (u32)&frame[0x80], (u32)&frame[0x1c],
                       ed + g1 + 0x38u);
    }

    cm_fn_004ae660(a3, 0x00de1f64u);
    g1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    ed = ed + g1;
    x1 = ed;
    cm_fn_004ae8a0(a3, x1 + 0x38u, 0x7d5u, x1);

    if (t5 != 0xffffffffu) {
        g1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
        y = g1 + ROW(t5);
        cm_fn_004ae8a0(a3, y + 0x38u, 0x7d5u, y);
    }

    g4 = CM_GLOBAL(volatile u32, 0x00ae23bc);
    base = t0 * 110u;
    r = (s32)cm_fn_0053e990(base + g4, t1, t2, t3, 0u);
    if (r != 0) {
        g4 = CM_GLOBAL(volatile u32, 0x00ae23bc);
        y = base + g4;
        cm_fn_004ae8a0(a3, (u32)&frame[0x80], 0x7d7u, y);
    }
}
