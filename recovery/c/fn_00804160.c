#include "cmre.h"

extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0055f990(u32 incoming_eax, u32 incoming_edx, u32 self,
    volatile u32 a0, volatile u32 a1, volatile u32 a2, volatile u32 a3,
    volatile u32 a4, volatile u32 a5, volatile u32 a6, volatile u32 a7,
    volatile u32 a8, volatile u32 a9, volatile u32 a10, volatile u32 a11,
    volatile u32 a12, volatile u32 a13, volatile u32 a14, volatile u32 a15,
    volatile u32 a16, volatile u32 a17);

u32 CM_THISCALL cm_fn_00804160(u32 *self, u32 p1, u32 p2, u32 *p3, u32 p4,
                               u32 p5, u32 p6, u32 p7, u32 p8, u32 p9,
                               u32 p10, u32 p11)
{
    u32 pv3;
    s32 w;
    u32 tbl;
    u32 obj;

    (void)p10;
    (void)p11;

    pv3 = *(volatile u32 *)p3;
    w = (s32)(s16)(*(volatile u16 *)((u32)self + 0x382a));
    tbl = (u32)(w * 768);
    obj = *(volatile u32 *)((u32)self + tbl + 0x7fc);

    /* 0x008041b5 call 0x55f990 ; 0x008041ba ret 0x2c - no write to EAX between
       them, so the callee's EAX is this routine's result. */
    return cm_fn_0055f990(tbl, p2, obj,
                          1, 0, 0, 0, 0, p2, pv3, p4, p5, 0,
                          p6, p7, p8, p9, 0, 0, 0, p1);
}
