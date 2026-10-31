#include "cmre.h"
/* Preferred-image VA 0x0086d800, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full EAX from the final append/diagnostic. The second record
   retains fields of the first, or their uninitialized bytes if skipped. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_0086d800(u32 self, u32 entry)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x240];
    u32 local = (u32)scratch;
    u32 found, club, player, receiver, index, base, id;
    u32 first, second, third, field, row, text, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962d7b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(local);
    receiver = D(0x00ae1050);
    frame.state = 0;
    found = cm_fn_004cf5a0(receiver, entry);
    D(0x00ae1080) = found;
    if (found != 0) {
        index = D(found + 4u);
        base = D(0x00ae23b4);
        club = base + index * 581u;
        if (club != 0) {
            player = D(club + 0xcfu);
            if (player != 0) {
                cm_fn_00417c10(local, local + 0x38u, 0x00ae2c90);
                field = D(entry);
                first = D(entry + 4u);
                B(local + 0xcu) = 0x68;
                id = D(club);
                D(local + 8u) = id;
                D(local + 0xdu) = field;
                field = D(first + 0x33u);
                second = D(entry + 8u);
                D(local + 0x11u) = field;
                third = D(entry + 0xcu);
                field = D(second + 0x33u);
                D(local + 0x15u) = field;
                field = D(third + 0x33u);
                D(local + 0x19u) = field;
                if (local != 0) {
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
            }
        }
    } else club = 0;
    cm_fn_00417c10(local, local + 0x38u, 0x00ae2c90);
    B(local + 0xcu) = 0x67;
    if (club != 0) id = D(club);
    else id = 0xffffffffu;
    D(local + 8u) = id;
    if (entry != 0 && local != 0) {
        id = D(entry);
        base = D(self + 0xcu);
        row = base + id * 8u;
        receiver = D(self);
        result = cm_fn_0058e340(receiver, row, row + 4u, local);
    } else {
        cm_fn_00944cff(0x00a83b78, 0, 0, local + 0x40u, local + 0x140u);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x40u, local + 0x140u, 0x1010);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
