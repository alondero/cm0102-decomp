#include "cmre.h"
/* Preferred-image VA 0x0086da20, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Retain full EAX from the last append or diagnostic boundary. The second
   record reuses fields of the first, including bytes never initialized when
   the first record is skipped. The later null dereference is original. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_0086da20(u32 self, u32 entry, u32 value, u32 club)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x244];
    u32 local = (u32)scratch;
    u32 player, id, first, second, third, field, base, row, receiver, text, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962d9b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(local + 4u);
    frame.state = 0;
    if (club != 0) {
        player = D(club + 0xcfu);
        D(local) = player;
        if (player != 0) {
            cm_fn_00417c10(local + 4u, local + 0x3cu, 0x00ae2c90);
            id = D(club);
            field = D(entry);
            first = D(entry + 4u);
            B(local + 0x10u) = 0x69;
            D(local + 0xcu) = id;
            D(local + 0x11u) = field;
            field = D(first + 0x33u);
            second = D(entry + 8u);
            D(local + 0x15u) = field;
            third = D(entry + 0xcu);
            field = D(second + 0x33u);
            D(local + 0x19u) = field;
            field = D(third + 0x33u);
            D(local + 0x1du) = field;
            field = D(value);
            D(local + 0x21u) = field;
            if (local + 4u != 0) {
                player = D(local);
                base = D(self + 0xcu);
                id = D(player);
                row = base + id * 8u;
                receiver = D(self);
                cm_fn_0058e340(receiver, row, row + 4u, local + 4u);
            } else {
                cm_fn_00944cff(0x00a83b78, 0, 0, local + 0x144u, local + 0x44u);
                text = cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                               local + 0x144u, local + 0x44u, 0x1010);
                cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
                D(0x00b67a34) = 0;
            }
        }
    }
    cm_fn_00417c10(local + 4u, local + 0x3cu, 0x00ae2c90);
    id = D(club);
    field = D(value);
    B(local + 0x10u) = 0x6a;
    D(local + 0xcu) = id;
    D(local + 0x11u) = field;
    if (entry != 0 && local + 4u != 0) {
        id = D(entry);
        base = D(self + 0xcu);
        row = base + id * 8u;
        receiver = D(self);
        result = cm_fn_0058e340(receiver, row, row + 4u, local + 4u);
    } else {
        cm_fn_00944cff(0x00a83b78, 0, 0, local + 0x44u, local + 0x144u);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x44u, local + 0x144u, 0x1010);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
