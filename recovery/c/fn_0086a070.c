#include "cmre.h"
/* Preferred-image VA 0x0086a070, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller 00689b93 overwrites full EAX at 00689b98 before reading it. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

void CM_THISCALL cm_fn_0086a070(u32 self, u32 entry)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x244];
    u32 local = (u32)scratch;
    u32 pointer, attribute, id, index = 0, argument, club, player;
    u32 rank, receiver, base, row, text;
    s16 count;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962bdb;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local) = self;
    cm_fn_0054a110(local + 4u);
    frame.state = 0;
    cm_fn_00417c10(local + 4u, local + 0x3cu, 0x00ae2c90);
    B(local + 0x10u) = 10;
    pointer = D(entry + 4u);
    count = (s16)W(entry + 0x3eu);
    attribute = (u32)(s32)(s16)W(entry + 0x3au);
    id = D(pointer);
    D(local + 0x15u) = attribute;
    D(local + 0x11u) = id;
    if (count > 0) {
        do {
            argument = (u32)(s32)(s16)(u16)index;
            pointer = cm_fn_006826d0(entry, argument);
            club = D(pointer);
            rank = argument + 1u;
            id = D(club);
            D(local + 0x19u) = rank;
            D(local + 0xcu) = id;
            player = D(club + 0xcfu);
            if (player != 0) {
                if (local + 4u != 0) {
                    receiver = D(local);
                    id = D(player);
                    base = D(receiver + 0xcu);
                    receiver = D(receiver);
                    row = base + id * 8u;
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
            ++index;
            count = (s16)W(entry + 0x3eu);
        } while ((s16)(u16)index < count);
    }
    CM_FS(u32, 0) = frame.prev;
}
