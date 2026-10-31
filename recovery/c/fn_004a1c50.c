#include "cmre.h"

/* Preferred-image VA004a1c50, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/fullEAX1, SEH95670b, scratch26c: yearWORD L0, packedrecord L2.
   Original clone helpers cover each packed subrecord; retain all header
   stores, the original misalignment and the six no-op destructor boundaries.
   58e340 is THIS3/RET12, not the draft's missing-receiver declaration. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0049f160(u32);
extern u32 CM_THISCALL cm_fn_00448110(u32, u32);
extern u32 CM_THISCALL cm_fn_00448170(u32, u32);
extern u32 CM_THISCALL cm_fn_004ab5d0(u32, u32);
extern u32 CM_THISCALL cm_fn_004a1f40(u32, u32);
extern u32 CM_THISCALL cm_fn_00451570(u32, u32);
extern u32 CM_THISCALL cm_fn_004a2050(u32, u32);
extern u32 CM_THISCALL cm_fn_004a1f70(u32, u32);
extern u32 CM_THISCALL cm_fn_004a1fb0(u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_004ad8c0(u32);
extern u32 CM_THISCALL cm_fn_004a2090(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_THISCALL cm_fn_005a53c0(u32);

u32 CM_THISCALL cm_fn_004a1c50(u32 self, volatile u32 save_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x26c];
    u32 l = (u32)scratch, source, pointer, a, b, c, d, owner, save;
    u8 byte;
    u16 word;
    frame.state = 0xffffffffu; frame.handler = 0x0095670b;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0049f160(l + 2);
    save = save_arg;
    frame.state = 0;
    if (save == 0) goto clear;
    pointer = D(self + 0x502);
    source = self + 0x28f;
    word = W(pointer + 0x40);
    W(l) = word;
    a = D(source); D(l + 2) = a;
    a = D(source + 9);
    byte = B(source + 4);
    b = D(source + 5);
    B(l + 6) = byte;
    c = D(source + 0xd); D(l + 7) = b;
    b = D(source + 0x11); D(l + 0xf) = c;
    c = D(source + 0x19);
    d = D(source + 0x1d);
    D(l + 0x13) = b;
    D(l + 0xb) = a;
    a = D(source + 0x15);
    D(l + 0x1b) = c;
    c = D(source + 0x25);
    D(l + 0x1f) = d;
    D(l + 0x17) = a;
    a = D(source + 0x21);
    D(l + 0x27) = c;
    D(l + 0x23) = a;
    cm_fn_00448110(l + 0x2b, source + 0x29);
    a = D(source + 0x48);
    D(l + 0x4a) = a;
    cm_fn_00448110(l + 0x4e, source + 0x4c);
    a = D(source + 0x6b);
    D(l + 0x6d) = a;
    c = D(source + 0x6f);
    D(l + 0x71) = c;
    word = W(source + 0x73);
    W(l + 0x75) = word;
    cm_fn_00448110(l + 0x77, source + 0x75);
    cm_fn_00448110(l + 0x96, source + 0x94);
    cm_fn_00448170(l + 0xb5, source + 0xb3);
    d = D(source + 0xc7);
    D(l + 0xc9) = d;
    cm_fn_00448170(l + 0xcd, source + 0xcb);
    a = D(source + 0xdf);
    D(l + 0xe1) = a;
    cm_fn_00448170(l + 0xe5, source + 0xe3);
    c = D(source + 0xf7);
    D(l + 0xf9) = c;
    cm_fn_004ab5d0(l + 0xfd, source + 0xfb);
    cm_fn_004a1f40(l + 0x115, source + 0x113);
    cm_fn_00451570(l + 0x134, source + 0x132);
    cm_fn_004a1f40(l + 0x16c, source + 0x16a);
    cm_fn_004a2050(l + 0x18b, source + 0x189);
    cm_fn_004a1f40(l + 0x1ae, source + 0x1ac);
    cm_fn_004a1f70(l + 0x1cd, source + 0x1cb);
    cm_fn_004a1fb0(l + 0x1ed, source + 0x1eb);
    cm_fn_004a1fb0(l + 0x22c, source + 0x22a);
    owner = D(0x00adae14);
    cm_fn_0058e340(owner, self + 0x4f8, self + 0x4fc, l);
clear:
    cm_fn_004ad8c0(self + 0x28f);
    cm_fn_004a2090(self);
    pointer = D(self + 0xc);
    if (pointer != 0) {
        cm_fn_009452ca(pointer);
        D(self + 0xc) = 0;
        D(self + 0x10) = 0;
        D(self + 0x14) = 0;
        B(self + 0x18) = 0xfe;
    }
    pointer = D(self + 0x19);
    if (pointer != 0) {
        cm_fn_009452ca(pointer);
        D(self + 0x19) = 0;
        D(self + 0x1d) = 0;
        D(self + 0x21) = 0;
        B(self + 0x25) = 0xfe;
    }
    cm_fn_005a53c0(l + 0x1ed);
    cm_fn_005a53c0(l + 0x134);
    cm_fn_005a53c0(l + 0xfd);
    cm_fn_005a53c0(l + 0xe5);
    cm_fn_005a53c0(l + 0xcd);
    cm_fn_005a53c0(l + 0xb5);
    CM_FS(volatile u32, 0) = frame.prev;
    return 1;
}
