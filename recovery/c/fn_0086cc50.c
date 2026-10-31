#include "cmre.h"
/* Preferred-image VA 0x0086cc50, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller 0076b8c3 overwrites AL at 0076b8c8, uses only that new AL,
   and overwrites full EAX at 0076b8de before any wider read. Return is void. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

void CM_THISCALL cm_fn_0086cc50(u32 self, u32 player, u32 value)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x240];
    u32 local = (u32)scratch;
    u32 club, id, age, pointed, base, row, receiver, text;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962d3b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(local);
    frame.state = 0;
    cm_fn_00417c10(local, local + 0x38u, 0x00ae2c90);
    B(local + 0xcu) = 6;
    club = D(player + 0x39u);
    if (club != 0) id = D(club);
    else id = 0xffffffffu;
    D(local + 8u) = id;
    pointed = D(value);
    age = (u32)(s32)(s8)B(player + 0x18u);
    D(local + 0xdu) = pointed;
    D(local + 0x11u) = age;
    if (player != 0 && local != 0) {
        id = D(player);
        base = D(self + 0xcu);
        row = base + id * 8u;
        receiver = D(self);
        cm_fn_0058e340(receiver, row, row + 4u, local);
    } else {
        cm_fn_00944cff(0x00a83b78, 0, 0, local + 0x140u, local + 0x40u);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x140u, local + 0x40u, 0x1010);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    CM_FS(u32, 0) = frame.prev;
}
