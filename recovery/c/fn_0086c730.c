#include "cmre.h"
/* Preferred-image VA 0x0086c730, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller 008b0048 overwrites full EAX at 008b004d before reading it. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

void CM_THISCALL cm_fn_0086c730(u32 self, u32 player, u32 club,
                              u32 other, u32 value, u32 attribute)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x440];
    u32 local = (u32)scratch;
    u32 id, base, row, receiver, first, second, line, text;
    u32 predecessor = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00962cfb;
    frame.prev = predecessor;
    CM_FS(u32, 0) = (u32)&frame;
    if (player == 0 || other == 0) {
        first = local + 0x340u;
        second = local + 0x140u;
        line = 0x7ee;
        goto error;
    }
    cm_fn_0054a110(local);
    frame.state = 0;
    cm_fn_00417c10(local, local + 0x38u, 0x00ae2c90);
    B(local + 0xcu) = 0x32;
    if (club != 0) id = D(club);
    else id = 0xffffffffu;
    D(local + 8u) = id;
    id = D(other);
    D(local + 0xdu) = id;
    value &= 0xffffu;
    D(local + 0x15u) = (u32)(s32)(s8)(u8)attribute;
    D(local + 0x11u) = value;
    if (local != 0) {
        id = D(player);
        base = D(self + 0xcu);
        row = base + id * 8u;
        receiver = D(self);
        cm_fn_0058e340(receiver, row, row + 4u, local);
    } else {
        first = local + 0x240u;
        second = local + 0x40u;
        line = 0x1010;
error:
        cm_fn_00944cff(0x00a83b78, 0, 0, first, second);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, first, second, line);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    CM_FS(u32, 0) = frame.prev;
}
