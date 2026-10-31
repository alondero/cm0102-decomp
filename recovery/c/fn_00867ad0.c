/* Preferred-image VA 0x00867ad0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_THISCALL *method_fn)(u32 self);
u32 CM_CDECL cm_fn_00867ad0(u32 first, u32 second, u32 kind)
{
    u32 current = 0;
    u32 value = CM_GLOBAL(volatile u32, second + 0x57u);
    u32 index;
    u32 destination;
    u32 vtable;
    if (value) {
        u32 id = CM_GLOBAL(volatile u32, value);
        u32 table = CM_GLOBAL(volatile u32, 0x00adadfcu);
        value = CM_GLOBAL(volatile u32, table + id * 4u);
        if (value)
            current = value;
    }
    index = (u32)(s32)(s8)kind;
    if (index > 4u)
        return 0;
    destination = CM_GLOBAL(volatile u32, 0x00867b3cu + index * 4u);
    if (destination == 0x00867afeu || destination == 0x00867b14u) {
        u32 slot = destination == 0x00867afeu ? 0x98u : 0x9cu;
        if (!current)
            return 0;
        vtable = CM_GLOBAL(volatile u32, current);
        return ((method_fn)CM_GLOBAL(volatile u32, vtable + slot))(current) != 0;
    }
    if (destination == 0x00867b0eu)
        return 1;
    if (destination == 0x00867b2au)
        return (s8)CM_GLOBAL(volatile u8, first + 0x18u) > 0x1f;
    return 0;
}
