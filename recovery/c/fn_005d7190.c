/* Preferred-image VA 005d7190/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00547840(u32, u32);
extern u32 CM_CDECL cm_fn_00587c60(u32, u32, u32);
extern void CM_THISCALL cm_fn_00867700(u32, u32, u32);

/* CDECL1/fullEAX=0: the final outer count decrement sets EAX to zero.
   Keep the saved outer slot pointer independent of the inner scan index,
   reread the matching slot after 547840, and use signed WORD day %14.
   The two original helpers retain their order even when the first is a no-op. */
u32 CM_CDECL cm_fn_005d7190(volatile u32 club)
{
    u32 owner = club, slot = owner + 0xd7u, remaining = 50;
    u32 player, record, receiver, other, scan, index, selected, mask, mode;
    s32 day;
    do {
        player = D(slot);
        if (player == 0) goto next;
        receiver = D(0x00ae1050u);
        record = cm_fn_004cf5f0(receiver, player, owner);
        if (record == 0) goto next;
        player = D(slot);
        if (D(player + 0x65u) == 0) goto next;
        scan = owner + 0xd7u;
        index = 0;
        for (;;) {
            other = D(scan);
            if (other != player && other != 0 && cm_fn_00547840(other, player) != 0)
                break;
            ++index;
            scan += 4u;
            if (index == 50) goto next;
        }
        selected = club;
        other = D(selected + index * 4u + 0xd7u);
        if (other == 0) goto next;
        day = (s16)W(0x00ae2c90u);
        player = D(slot);
        if ((u32)(day % 14) + D(player) != 0) goto next;
        selected = club;
        if (other == D(selected + 0xcfu)) {
            mode = 0;
            mask = 0x2000;
        } else if (other == D(selected + 0xd3u)) {
            mode = 1;
            mask = 0x1000;
        } else {
            mode = 2;
            mask = 0x800;
        }
        cm_fn_00587c60(player, other, mode);
        cm_fn_00867700(record, mask, 0);
next:
        slot += 4u;
        --remaining;
        if (remaining != 0) owner = club;
    } while (remaining != 0);
    return 0;
}
