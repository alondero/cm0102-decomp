#include "cmre.h"

/* Thiscall with ECX = object (read before written). Stack arguments: first
   (the value searched for) and second (the table base; 0 selects the object's
   default base at +0xb1). Object fields: +0x3e signed 16-bit element count,
   +0xb1 default base. Elements are 0x3b bytes apart; the compared dword is at
   the element's start. Returns the element address, or 0 when not found. */
u32 CM_THISCALL cm_fn_0068ab70(u32 self, u32 first, u32 second)
{
    u32 base;
    s16 count;
    s16 i;

    base = second;
    if (base == 0) {
        base = *(volatile u32 *)(self + 0xb1);
    }

    count = (s16)*(volatile u16 *)(self + 0x3e);
    i = 0;
    if (count > 0) {
        do {
            if (*(volatile u32 *)(base + (u32)(s32)i * 59u) == first) {
                return base + (u32)(s32)i * 59u;
            }
            i = (s16)(i + 1);
        } while (i < count);
    }
    return 0;
}
