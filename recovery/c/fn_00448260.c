#include "cmre.h"

/* Thiscall with no stack arguments (ret 0); ECX is the object and EAX returns it.
   The SEH record sits directly above an 8-byte scratch area. Scratch offset 0
   holds the object pointer and offset 4 holds a field pointer; nothing reads them
   back except the listing's own stores. The state byte writes update only the
   low byte of frame.state, as the original does. */

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_FASTCALL cm_fn_004485d0(u32 param_1);
extern u32 CM_FASTCALL cm_fn_00448620(u32 param_1);
extern u32 CM_FASTCALL cm_fn_00448670(u32 param_1);
extern u32 CM_FASTCALL cm_fn_00450d40(u32 param_1);
extern u32 CM_FASTCALL cm_fn_00645800(u32 param_1);

#define SEH_STATE_LOW8(v) (*(volatile u8 *)&seh.state = (u8)(v))

u32 CM_THISCALL cm_fn_00448260(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 locals[8];
    u32 edi;

    seh.state = -1;
    seh.handler = 0x00955441u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    *(volatile u32 *)(locals + 0) = self;
    cm_fn_0054a110((u16 *)(self + 0x12));
    seh.state = 0;
    cm_fn_0054a110((u16 *)(self + 0x35));
    SEH_STATE_LOW8(1);
    cm_fn_00645800(self + 0x58);
    SEH_STATE_LOW8(2);
    cm_fn_00645800(self + 0x7d);
    SEH_STATE_LOW8(3);
    cm_fn_00645800(self + 0x9c);
    SEH_STATE_LOW8(4);
    cm_fn_00645800(self + 0xbb);
    SEH_STATE_LOW8(5);
    cm_fn_00645800(self + 0xda);
    SEH_STATE_LOW8(6);
    cm_fn_00645800(self + 0xf9);
    SEH_STATE_LOW8(7);
    cm_fn_00448670(self + 0x118);
    SEH_STATE_LOW8(8);
    cm_fn_00448670(self + 0x12c);
    SEH_STATE_LOW8(9);
    cm_fn_00448670(self + 0x140);
    SEH_STATE_LOW8(0xa);
    cm_fn_00448670(self + 0x154);
    SEH_STATE_LOW8(0xb);
    cm_fn_00450d40(self + 0x169);
    SEH_STATE_LOW8(0xc);
    cm_fn_00450d40(self + 0x184);

    edi = self + 0x19e;
    SEH_STATE_LOW8(0xd);
    *(volatile u32 *)(locals + 4) = edi;
    cm_fn_00450d40(edi + 1);
    SEH_STATE_LOW8(0xe);
    cm_fn_00645800(edi + 0x19);
    SEH_STATE_LOW8(0xf);
    cm_fn_00450d40(self + 0x1d7);
    SEH_STATE_LOW8(0x10);
    cm_fn_00450d40(self + 0x1f5);
    SEH_STATE_LOW8(0x11);
    cm_fn_00450d40(self + 0x211);
    SEH_STATE_LOW8(0x12);
    cm_fn_00450d40(self + 0x22d);
    SEH_STATE_LOW8(0x13);
    cm_fn_00450d40(self + 0x249);
    SEH_STATE_LOW8(0x14);
    cm_fn_00450d40(self + 0x263);

    edi = self + 0x27b;
    SEH_STATE_LOW8(0x15);
    *(volatile u32 *)(locals + 4) = edi;
    cm_fn_0054a110((u16 *)edi);
    SEH_STATE_LOW8(0x16);
    cm_fn_00450d40(edi + 8);
    SEH_STATE_LOW8(0x17);
    cm_fn_00645800(edi + 0x20);

    edi = self + 0x2ba;
    SEH_STATE_LOW8(0x18);
    *(volatile u32 *)(locals + 4) = edi;
    cm_fn_0054a110((u16 *)edi);
    SEH_STATE_LOW8(0x19);
    cm_fn_00450d40(edi + 8);
    SEH_STATE_LOW8(0x1a);
    cm_fn_00645800(edi + 0x20);
    SEH_STATE_LOW8(0x1b);
    cm_fn_00450d40(self + 0x2fb);
    SEH_STATE_LOW8(0x1c);
    cm_fn_004485d0(self + 0x313);
    SEH_STATE_LOW8(0x1d);
    cm_fn_00448620(self + 0x33c);
    SEH_STATE_LOW8(0x1e);
    cm_fn_00450d40(self + 0x371);

    CM_FS(u32, 0) = seh.prev;
    return self;
}
