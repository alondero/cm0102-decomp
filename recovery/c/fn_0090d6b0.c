#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))

extern u32 CM_STDCALL cm_fn_0090e760(u32, u32, u32, u32, u32);
extern u32 CM_STDCALL cm_fn_0090e9a0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00896c60(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32, u32);

u32 CM_THISCALL cm_fn_0090d6b0(u32 self, u32 record)
{
    /* The second output at L+0x10 is the original argument slot. EBX
       retains its incoming value. 0090e760 overwrites only the two
       output DWORDs at L+0 and L+0x10, leaving the intervening gap alone. */
    u8 scratch[0x14];
    u32 index, cursor, value, club, person, detail, which, limit, id, context, list, result;
    D((u32)scratch + 4) = 0;
    D((u32)scratch + 8) = 0;
    D((u32)scratch + 0x10) = record;
    cursor = self + 0x18;
    for (index = 0; index != 17; ++index, cursor += 4) {
        if (D((u32)scratch + 4) == 0) {
            value = D(cursor);
            club = D(record + 0x1c);
            if (value == club) {
                cm_fn_0090e760(index, 0, 0, (u32)scratch, (u32)scratch + 0x10);
                detail = D((u32)scratch + 0x10);
                person = D((u32)scratch);
                club = D(cursor);
                cm_fn_0090e9a0(club, person, detail);
                D((u32)scratch + 4) = 1;
                continue;
            }
        }
        if (D((u32)scratch + 8) == 0) {
            value = D(cursor);
            club = D(record + 0x20);
            if (value == club) {
                cm_fn_0090e760(index, 0, 0, (u32)scratch, (u32)scratch + 0x10);
                detail = D((u32)scratch + 0x10);
                person = D((u32)scratch);
                club = D(cursor);
                cm_fn_0090e9a0(club, person, detail);
                D((u32)scratch + 8) = 1;
            }
        }
    }
    for (which = 0; which != 2; ++which) {
        if (D((u32)scratch + 4 + which * 4u) != 0)
            continue;
        cursor = D(record + 0x1c + which * 4u) + 0xd7;
        for (index = 0; index != 50; ++index, cursor += 4) {
            person = D(cursor);
            if (person != 0) {
                limit = D(0x00ae2364);
                id = D(person);
                if ((s32)id >= (s32)limit) {
                    if (which == 0) {
                        club = D(record + 0x1c);
                        context = D(0x00de1d08);
                    } else {
                        context = D(0x00de1d08);
                        club = D(record + 0x20);
                    }
                    cm_fn_00896c60(context, club, person, 1, 1);
                    person = D(cursor);
                    club = D(record + 0x1c + which * 4u);
                    context = D(0x00de1d08);
                    cm_fn_00896c60(context, club, person, 0, 1);
                    D(cursor) = 0;
                }
            }
        }
    }
    value = D(self + 0x5e);
    list = self + 0x5c;
    D(list + 0xa) = value;
    result = cm_fn_0068d1b0(list);
    while (result != 0) {
        if (D(result) == record)
            return cm_fn_0068d1f0(list, result);
        result = cm_fn_0068d1b0(list);
    }
    return result;
}
