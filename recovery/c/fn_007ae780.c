#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053dba0(u32 person, u32 output, u32 capacity, u32 selector);

u32 CM_THISCALL cm_fn_007ae780(u32 self, u32 person)
{
    if (!person) {
        u32 index = *(volatile u32 *)self;
        u32 base = CM_GLOBAL(volatile u32, 0x00ae23bc);
        person = base + index * 110;
    }
    cm_fn_0053dba0(person, self + 0x70, 0xc9, 1);
    return 1;
}
