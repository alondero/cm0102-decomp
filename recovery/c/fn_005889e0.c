#include "cmre.h"

/* Frame offsets are from F = entry esp - 0x344 (E = entry esp; arg1 = E+4,
   arg2 = E+8, arg3 = E+0x0c). The two 0x190-byte buffers are adjacent in the
   frame: scratch[0..0x190) = F+0x24 (passed as the buffer of the call
   `cm_fn_0053e140(..., 0x190, ...)`) and scratch[0x190..0x320) = F+0x1b4,
   which ends at E. The 0x190 length is the 5th argument of both calls.
   The zero-arg2 path differs only in the 6th argument of 0x0053e140
   (5 instead of 1) and in the variadic format string. */

#define RD_U32_CM(a) (*(volatile u32 *)(u32)(a))

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_CDECL cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 CM_CDECL cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 a, u32 b, u32 c);

void CM_CDECL cm_fn_005889e0(u32 arg1, u32 arg2, u32 arg3)
{
    u8 scratch[0x320];
    u32 obj;
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 b;
    u32 e110;
    u32 fmt;

    obj = arg1;
    r0 = (u32)cm_fn_00785830((void *)obj, 0);
    r1 = (u32)cm_fn_00785830((void *)obj, 1);
    r2 = (u32)cm_fn_00785830((void *)obj, 2);
    r3 = (u32)cm_fn_00785830((void *)obj, 3);
    r4 = (u32)cm_fn_00785830((void *)obj, 4);
    r5 = (u32)cm_fn_00785830((void *)obj, 5);
    r6 = (u32)cm_fn_00785830((void *)obj, 6);
    r7 = (u32)cm_fn_00785830((void *)obj, 7);

    if (arg2 == 0) {
        cm_fn_0053e140(r5, r6, r7, (u32)&scratch[0x190], 0x190u, 5u, r4);
        cm_fn_0053e140(r1, r2, r3, (u32)&scratch[0], 0x190u, 5u, r0);
        fmt = 0x009c5d28u;
    } else {
        cm_fn_0053e140(r5, r6, r7, (u32)&scratch[0x190], 0x190u, 1u, r4);
        cm_fn_0053e140(r1, r2, r3, (u32)&scratch[0], 0x190u, 1u, r0);
        fmt = 0x009c5d8cu;
    }

    cm_fn_0066f4e0(0x00de1f64u, fmt, (u32)&scratch[0], (u32)&scratch[0x190]);
    cm_fn_004ae660((void *)arg3, 0x00de1f64u);

    b = RD_U32_CM(0x00ae23bcu);
    e110 = 110u * r0;
    if (cm_fn_0053e990(e110 + b, r1, r2, r3, 0u) != 0) {
        b = RD_U32_CM(0x00ae23bcu);
        cm_fn_004ae8a0(arg3, (u32)&scratch[0], 0x7dau, e110 + b);
    }

    b = RD_U32_CM(0x00ae23bcu);
    e110 = 110u * r4;
    if (cm_fn_0053e990(e110 + b, r5, r6, r7, 0u) != 0) {
        b = RD_U32_CM(0x00ae23bcu);
        cm_fn_004ae8a0(arg3, (u32)&scratch[0x190], 0x7dau, e110 + b);
    }
}
