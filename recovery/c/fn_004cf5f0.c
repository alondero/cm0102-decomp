/* Preferred-image VA 004cf5f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);

u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club)
{
    u32 id, limit, map, index, records, first, second, club_id, current, mapped;
    u32 input = person;
    if (input == 0) return 0;
    if (club == 0) return 0;
    id = D(input);
    limit = D(0x00ae2364u);
    if ((s32)id >= (s32)limit) return 0;
    map = D(0x00ae2d0cu);
    map += id * 79u;
    index = D(map);
    if ((s32)index < 0) return 0;
    records = D(self);
    first = records + index * 80u;
    if (first == 0) return 0;
    club_id = D(club);
    current = D(first + 4u);
    if (current == club_id) return first;
    index = D(map + 4u);
    if ((s32)index < 0) {
        second = 0;
    } else {
        second = records + index * 80u;
        if (second != 0 && D(second + 4u) == club_id) return second;
    }
    /* 00540a50 writes its direction into the original first-argument slot. */
    mapped = cm_fn_00540a50(club, (u32)&person, 1);
    if (mapped != 0 && person == 0) club = mapped;
    id = D(club);
    current = D(first + 4u);
    if (current == id) return first;
    if (second != 0 && D(second + 4u) == id) return second;
    return 0;
}
