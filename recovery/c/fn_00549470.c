/* Preferred-image VA 0x00549470, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);

u8 CM_CDECL cm_fn_00549470(u32 object)
{
    u32 index, cursor;
    if (!object) return 0;
    cursor = object + 0xd7u;
    for (index = 0; (s32)index < 50; ++index, cursor += 4u) {
        u32 person = D(cursor), details;
        if (!person) continue;
        details = D(person + 0x61u);
        if (!details || B(details + 0xfu) != 0x14u) continue;
        if (cm_fn_0062c400(D(0x00b63d28u), person) == 2) continue;
        if ((s32)D(person) < (s32)D(0x00ae2364u)) return 1;
    }
    return 0;
}
