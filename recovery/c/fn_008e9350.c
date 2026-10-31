#include "cmre.h"

/* Preferred-image VA 0x008e9350, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep full EAX: two indexed callers return it unchanged. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_008ced40(u32, u32, u32, u32, u32, u32, u32);

u32 CM_CDECL cm_fn_008e9350(volatile u32 first, u32 second)
{
    u32 selected = 0xffffffffu, value, person, club, id, base, manager, context;
    u32 result, enabled, i;
    s8 category;
    if (second != 0) {
        value = D(second);
        if (value != 0) selected = D(value + 0x24);
    }
    result = cm_fn_007fe600(0x00b74450, 0x008e9760, 0x008ec420, 1, 0x0093a710, 0);
    if (result == 0) return result;
    person = first;
    value = D(person);
    cm_fn_007ff270(0x00b74450, 0, value, 0);
    cm_fn_007ff270(0x00b74450, 1, second, 0);
    cm_fn_007ff270(0x00b74450, 2, 0, 0);
    cm_fn_007ff270(0x00b74450, 3, selected, 0);
    cm_fn_007ff270(0x00b74450, 4, 0, 0);
    cm_fn_007ff270(0x00b74450, 5, 0, 0);
    cm_fn_007ff270(0x00b74450, 6, 1, 0);
    manager = D(0x00ae1050);
    value = cm_fn_004cf5a0(manager, person);
    D(0x00ae1080) = value;
    if (value != 0 && (B(value + 0x35) & 0x3fu) != 5) {
        id = D(value + 4);
        base = D(0x00ae23b4);
        club = base + id * 581u;
    } else club = 0;
    id = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
    value = D(0x00b74c26 + id * 768u);
    context = D(value + 0x39);
    cm_fn_007ff270(0x00b74450, 8, 0, 0);
    manager = D(0x00de1f54);
    result = cm_fn_008ced40(manager, context, 0x00ae2c90, person, club, 1, 0);
    cm_fn_007ff270(0x00b74450, 9, result != 0, 0);
    enabled = 0;
    if (second != 0) {
        value = D(second);
        if (value != 0) {
            category = (s8)B(value + 0x2c);
            if (category >= 5 && category != 0x10) enabled = 1;
        }
    }
    cm_fn_007ff270(0x00b74450, 7, enabled, 0);
    for (i = 0xf; i <= 0x16; ++i)
        result = cm_fn_007ff270(0x00b74450, i, 0xffffffffu, 0);
    return result;
}
