#include "cmre.h"

/* Thiscall, no stack arguments; returns the object (ECX). Each sub-object is passed
   as ECX to a fastcall callee, with the SEH state written as 0, then 1..0xf (low
   byte), before the matching call. Sub-object 0x132 (edi) is also passed as
   edi+1 and edi+0x19 (state 8 and 9). */

extern void CM_FASTCALL cm_fn_0054a110(u32 self);
extern u32 CM_FASTCALL cm_fn_00448670(u32 self);
extern u32 CM_FASTCALL cm_fn_00450d40(u32 self);
extern u32 CM_FASTCALL cm_fn_00645800(u32 self);
extern u32 CM_FASTCALL cm_fn_0049f390(u32 self);
extern u32 CM_FASTCALL cm_fn_0049f3f0(u32 self);

u32 CM_THISCALL cm_fn_0049f160(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 edi;

    seh.state = -1;
    seh.handler = 0x009565d4u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0054a110(self + 0x29u);

    seh.state = 0;
    cm_fn_0054a110(self + 0x4cu);

    *(volatile u8 *)&seh.state = 1;
    cm_fn_0054a110(self + 0x75u);

    *(volatile u8 *)&seh.state = 2;
    cm_fn_0054a110(self + 0x94u);

    *(volatile u8 *)&seh.state = 3;
    cm_fn_00448670(self + 0xb3u);

    *(volatile u8 *)&seh.state = 4;
    cm_fn_00448670(self + 0xcbu);

    *(volatile u8 *)&seh.state = 5;
    cm_fn_00448670(self + 0xe3u);

    *(volatile u8 *)&seh.state = 6;
    cm_fn_00448670(self + 0xfbu);

    *(volatile u8 *)&seh.state = 7;
    cm_fn_0054a110(self + 0x124u);

    edi = self + 0x132u;
    *(volatile u8 *)&seh.state = 8;
    cm_fn_00450d40(edi + 1u);

    *(volatile u8 *)&seh.state = 9;
    cm_fn_00645800(edi + 0x19u);

    *(volatile u8 *)&seh.state = 0xa;
    cm_fn_00450d40(self + 0x16bu);

    *(volatile u8 *)&seh.state = 0xb;
    cm_fn_00450d40(self + 0x18du);

    *(volatile u8 *)&seh.state = 0xc;
    cm_fn_00450d40(self + 0x1adu);

    *(volatile u8 *)&seh.state = 0xd;
    cm_fn_00450d40(self + 0x1cdu);

    *(volatile u8 *)&seh.state = 0xe;
    cm_fn_0049f390(self + 0x1ebu);

    *(volatile u8 *)&seh.state = 0xf;
    cm_fn_0049f3f0(self + 0x22au);

    CM_FS(u32, 0) = seh.prev;
    return self;
}
