/* Preferred-image VA 0x004d0b50, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))

extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 argument);
extern u32 CM_FASTCALL cm_fn_00598370(u32 self);

u32 CM_THISCALL cm_fn_004d0b50(u32 self, u32 argument)
{
    u32 index = D(argument);
    u32 table = D(0x00ae2d0c);
    u32 slot = D(table + index * 79u);
    u32 objects, base, entry, value, result;
    if (slot == 0xffffffffu)
        return slot;
    objects = D(self);
    base = D(0x00ae23b4);
    entry = objects + slot * 80u;
    value = D(entry + 4);
    D(entry) = 0xffffffffu;
    index = D(argument);
    table = D(0x00ae2d0c);
    D(table + index * 79u) = 0xffffffffu;
    result = cm_fn_005a0590(D(0x00ae2a38), base + value * 581u);
    if (result)
        result = cm_fn_00598370(result);
    return result;
}
