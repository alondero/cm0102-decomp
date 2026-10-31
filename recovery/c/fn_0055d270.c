/* Preferred-image VA 0055d270, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
/* Original 007e88a7 consumes the amount slot as a signed WORD. */
extern u32 CM_THISCALL cm_fn_007e85b0(u32 self, u32 person, u32 filter,
                                    u32 amount, u32 match);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip, u8 flag,
                                 u32 person, u32 force);

u32 CM_THISCALL cm_fn_0055d270(u32 self, volatile u32 person,
                              u32 club_key, u32 amount_arg, u32 match_arg)
{
    u32 input = person;
    u32 id, limit, table, selected, match, amount, receiver, club, address;
    u8 index, group;
    if (input != 0) {
        id = D(input);
        limit = D(0x00ae2364u);
        if ((s32)id >= (s32)limit) return 0;
    }
    table = D(0x00adadfcu);
    if (table == 0) return 0;
    id = D(club_key);
    selected = D(table + id * 4u);
    person = selected;
    if (selected == 0) return 0;
    match = match_arg;
    amount = amount_arg;
    table = D(self);
    receiver = D(table);
    if (cm_fn_007e85b0(receiver, input, 1, amount, match) != 0) return 1;
    selected = person;
    group = B(selected + 0x50u);
    if (group == 0xffu) return 0;
    table = D(self);
    receiver = D(table + (u32)(s32)(s8)group * 4u);
    if (receiver == 0) return 0;
    if (cm_fn_007e85b0(receiver, input, 0, amount, match) == 0) return 0;
    club = D(input + 0x39u);
    if (club != 0 && cm_fn_00600a50(club, 0, 1, 0, 0) != 0) {
        club = D(input + 0x39u);
        address = club + 0x1e0u;
        index = 0;
        while (D(address) != input) {
            ++index;
            address += 4u;
            if ((s8)index >= 0x14) return 1;
        }
        D(address) = 0;
    }
    return 1;
}
