#include "cmre.h"

/* Frame offsets are from F, the stack pointer after the four register pushes
   (E = entry stack pointer; arg1 = E+4, arg2 = E+8, arg3 = E+12).
   F+0x13: byte result of the call with argument 0x0a (rewritten by the
   0x00786d30 branch); F+0x14, F+0x18, F+0x1c, F+0x20: dword call results;
   F+0x24..F+0xec: 0xc8-byte buffer passed to 0x0053e140 and the format calls. */

#define RD_U32(a) (*(volatile u32 *)(u32)(a))
#define RD_U8(a) (*(volatile u8 *)(u32)(a))
#define RD_S8(a) ((s8)RD_U8(a))
#define RD_S16(a) ((s16)*(volatile u16 *)(u32)(a))
#define WR_U8(a, v) (*(volatile u8 *)(u32)(a) = (u8)(v))

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786d30(u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
extern u32 CM_CDECL cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 CM_CDECL cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae770(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 a, u32 b, u32 c);

void CM_STDCALL cm_fn_008e47a0(u32 arg1, u32 arg2, u32 arg3)
{
    u8 buf[0xc8];
    u8 b13;
    u32 s14;
    u32 s18;
    u32 s1c;
    u32 s20;
    u32 obj;
    u32 r0;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 r10;
    u32 g;
    u32 cnt;
    u32 x0;
    u32 p;
    u32 cl;
    u32 d4;
    u32 d5;
    u32 d6;
    u32 mem;
    u32 self;
    u32 a3;
    u32 a4;
    u32 a;
    u32 e110;
    u32 bptr;
    u32 p4;
    u32 p5;
    u32 esi2;
    s32 v1;
    s32 v2;
    s32 t;
    u32 bal;
    u32 ptr;

    obj = arg1;
    r0 = (u32)cm_fn_00785830((void *)obj, 0);
    s18 = (u32)cm_fn_00785830((void *)obj, 1);
    s14 = (u32)cm_fn_00785830((void *)obj, 2);
    s1c = (u32)cm_fn_00785830((void *)obj, 3);
    r4 = (u32)cm_fn_00785830((void *)obj, 4);
    r5 = (u32)cm_fn_00785830((void *)obj, 5);
    r10 = (u32)cm_fn_00785830((void *)obj, 0xa);
    b13 = (u8)r10;

    if (b13 == 1) {
        r6 = (u32)cm_fn_00785830((void *)obj, 6);
        s20 = r6;
        r7 = (u32)cm_fn_00785830((void *)obj, 7);
        g = RD_U32(0x00de1f54);
        cnt = RD_U32(g + 4);
        x0 = RD_U32(g);
        if ((s32)s20 >= (s32)cnt) {
            p = RD_U32(x0);
        } else {
            p = RD_U32(x0 + s20 * 4u);
        }

        if (p != 0 && RD_U32(p + 0x24) == r7) {
            cl = RD_U8(p + 0x2c);
            if (cl == 1) {
                goto L48a3;
            }
            b13 = (u8)cl;
            a3 = 0xbu;
            a4 = (u32)(s32)(s8)cl;
            goto Lcall;
        }
        if (p != 0 && r0 == RD_U32(p + 0xc) && r4 == RD_U32(p + 8)) {
            b13 = 0;
            a3 = 0xau;
            a4 = 0u;
            goto Lcall;
        }
        b13 = 4;
        a3 = 0xau;
        a4 = 4u;

Lcall:
        d6 = RD_U32(obj + 0xd6);
        g = (u32)(s32)RD_S16(0x00b77c7a);
        mem = RD_U32(0x00b74c26u + g * 768u);
        self = RD_U32(0x00b7431cu);
        cm_fn_00786d30(self, mem, d6, a3, a4, 0u);
    }

L48a3:
    if (arg2 == 0) {
        goto L4a37;
    }

    /* 0x008e48b3 path: arg2 is nonzero. */
    cm_fn_0053e140(s18, s14, s1c, (u32)buf, 0xc8u, 1u, r0);   /* r0 pushed at 0x8e48aa */
    d4 = 581u * r4;
    d5 = 581u * r5;
    a = RD_U32(0x00ae23b4);
    v1 = RD_S8(d4 + a + 0x37);
    v2 = RD_S8(d5 + a + 0x37);
    p4 = a + d4 + 4u;
    p5 = a + d5 + 4u;
    cm_fn_0066f4e0(0xde1f64u, 0xa9dcf4u, v2, v2, v1, v1, p5, p4, (u32)buf);

    cm_fn_004ae660((void *)arg3, 0xde1f64u);

    a = RD_U32(0x00ae23b4);
    cm_fn_004ae8a0(arg3, d4 + a + 4u, 0x7d5u, d4 + a);

    a = RD_U32(0x00ae23b4);
    cm_fn_004ae8a0(arg3, a + d5 + 4u, 0x7d5u, a + d5);

    /* 0x008e4969: 110 * r0 plus the value at 0x00ae23bc. */
    g = RD_U32(0x00ae23bc);
    e110 = 110u * r0;
    if (cm_fn_0053e990(e110 + g, s18, s14, s1c, 0u) != 0) {
        g = RD_U32(0x00ae23bc);
        bptr = (u32)buf;
        esi2 = e110 + g;
        cm_fn_004ae8a0(arg3, bptr, 0x7d7u, esi2);
    }

    if (b13 == 3) {
        a = RD_U32(0x00ae23b4);
        t = RD_S8(d4 + a + 0x52);
        ptr = d4 + a + 0x38u;
        cm_fn_0066f4e0(0xde1f64u, 0xa9439cu, t, t, ptr);
        bal = RD_U8(0x00de1f64);
        if (bal == 0) {
            return;
        }
        cm_fn_004ae770(arg3, 0xde1f64u);
        return;
    }
    if (b13 == 0) {
        a = RD_U32(0x00ae23b4);
        t = RD_S8(d4 + a + 0x52);
        ptr = d4 + a + 0x38u;
        cm_fn_0066f4e0(0xde1f64u, 0xa9dcb8u, t, t, ptr);
        bal = RD_U8(0x00de1f64);
        if (bal == 0) {
            return;
        }
        cm_fn_004ae770(arg3, 0xde1f64u);
        return;
    }
    WR_U8(0x00de1f64, 0);
    return;

L4a37:
    /* Zero-argument path: the frame values come from the same slots as above. */
    cm_fn_0053e140(s18, s14, s1c, (u32)buf, 0xc8u, 5u, r0);   /* r0 pushed at 0x8e48aa */
    cm_fn_0066f4e0(0xde1f64u, 0xa9dc7cu, (u32)buf);
    cm_fn_004ae660((void *)arg3, 0xde1f64u);

    g = RD_U32(0x00ae23bc);
    e110 = 110u * r0;
    if (cm_fn_0053e990(e110 + g, s18, s14, s1c, 0u) != 0) {
        g = RD_U32(0x00ae23bc);
        bptr = (u32)buf;
        esi2 = e110 + g;
        cm_fn_004ae8a0(arg3, bptr, 0x7d7u, esi2);
    }
}
