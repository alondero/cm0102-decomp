/* Preferred-image VA 008c5560, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 CM_CDECL cm_fn_004b02e0(u32 first, u32 second);

u32 CM_STDCALL cm_fn_008c5560(u16 mode, u32 record)
{
    u32 id, receiver, people, person, manager, clubs, previous, destination, first, second;
    id = D(record + 0xcu);
    receiver = D(0x00ae1050u);
    people = D(0x00ae23bcu);
    person = people + id * 110u;
    manager = cm_fn_004cf5a0(receiver, person);
    clubs = D(0x00ae23b4u);
    D(0x00ae1080u) = manager;
    if (manager != 0) {
        id = D(manager + 4u);
        previous = clubs + id * 0x245u;
    } else
        previous = 0;
    id = D(record + 8u);
    destination = clubs + id * 0x245u;
    if (mode == 0x2000)
        return (s8)B(person + 0x18u) < 24;
    if (mode == 0x1000) {
        if (previous == 0)
            return 1;
        first = D(previous + 0x57u);
        second = D(destination + 0x57u);
        return first == second;
    }
    if (mode != 0x800 || destination == 0 || previous == 0)
        return 1;
    first = cm_fn_00600a50(previous, 0, 1, 0, 0);
    if (first != 0 && cm_fn_00600a50(destination, 0, 1, 0, 0) != 0)
        return 1;
    if ((u8)cm_fn_004b02e0(destination, previous) != 0)
        return 1;
    if (D(destination + 0xb3u) == previous)
        return 1;
    if (D(destination + 0xb7u) == previous)
        return 1;
    if (D(destination + 0xbbu) == previous)
        return 1;
    if (D(previous + 0xb3u) == destination)
        return 1;
    if (D(previous + 0xb7u) == destination)
        return 1;
    return D(previous + 0xbbu) == destination;
}
