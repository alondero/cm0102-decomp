/* Preferred-image VA 0076d880, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 first, u32 second);
extern u32 CM_CDECL cm_fn_0053b4f0(u32 nation);
extern u32 CM_THISCALL cm_fn_00896c60(u32 self, u32 club, u32 person,
                                     u32 argument, u32 option);

u32 CM_THISCALL cm_fn_0076d880(u32 self, u32 nation)
{
    u32 records, id, offset, high, low, country, slot, left, player, manager;
    u8 flags;
    records = D(self + 4u);
    id = D(nation);
    offset = id * 110u;
    high = D(records + offset + 0x66u);
    low = D(records + offset + 0x62u);
    if (cm_fn_00456740(0x00ae2c90u, low, high) == 0)
        return 0;
    country = cm_fn_0053b4f0(nation);
    slot = country + 0xd7u;
    left = 0x32;
    do {
        player = D(slot);
        if (player != 0 && (s8)B(player + 0x18u) > 0x15) {
            manager = D(0x00de1d08u);
            (void)cm_fn_00896c60(manager, country, player, 1, 1);
            player = D(slot);
            manager = D(0x00de1d08u);
            (void)cm_fn_00896c60(manager, country, player, 0, 1);
            player = D(slot);
            D(player + 0x24u) = 0;
            player = D(slot);
            B(player + 0x28u) = 0;
            player = D(slot);
            flags = B(player + 0x5eu);
            B(player + 0x5eu) = flags & 3u;
            D(slot) = 0;
        }
        slot += 4u;
    } while (--left != 0);
    id = D(nation);
    records = D(self + 4u);
    offset = id * 55u;
    B(records + offset * 2u + 0x6bu) = 0;
    return offset;
}
