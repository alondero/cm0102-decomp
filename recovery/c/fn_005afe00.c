#include "cmre.h"
/* Preferred-image VA 0x005afe00, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Its sole caller overwrites EAX immediately after this call. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern void CM_THISCALL cm_fn_005b09d0(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

void CM_STDCALL cm_fn_005afe00(u32 report, u32 packet)
{
    u8 side, i;
    u32 club, person, count, index, base, receiver, entry, player;
    side = 0;
    do {
        club = D(report + (side ? 0x1c : 0x20));
        if (!cm_fn_00600a50(club, 0, 1, 0, 0)) goto next;
        person = D(club + 0xcf);
        if (!person) goto next;
        count = D(0x00ae2364);
        index = D(person);
        base = D(0x00ae2a60);
        receiver = D(base + (count - index) * 4u - 4u);
        if (!receiver) goto next;
        entry = packet + (u32)side * 0x625u + 0x190;
        i = 0;
        do {
            index = D(entry);
            if (index == 0xffffffffu) goto advance;
            if (D(entry + 4) == 0xffffffffu) goto advance;
            if ((s8)i > 10 && B(entry + 0x32) == 0xff) goto advance;
            base = D(0x00ae23bc);
            player = base + index * 110u;
            if (player) {
                count = D(0x00ae2364);
                index = D(player);
                if ((s32)index >= (s32)count) goto advance;
            }
            cm_fn_005b09d0(receiver, player, 0);
advance:
            ++i;
            entry += 0x49;
        } while ((s8)i < 20);
next:
        ++side;
    } while ((s8)side < 2);
}
