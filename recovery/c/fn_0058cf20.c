#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);

void CM_THISCALL cm_fn_0058cf20(u32 self)
{
    s32 index = 0;
    u32 offset = 0;
    if (*(volatile u32 *)self == 0) return;
    if (CM_GLOBAL(volatile s32, 0x00ae2350u) > 0) {
        do {
            u32 base = *(volatile u32 *)self;
            u32 pointer = *(volatile u32 *)(base + offset + 4u);
            if (pointer != 0) cm_fn_009452ca(pointer);
            ++index;
            offset += 0x2cu;
        } while (index < CM_GLOBAL(volatile s32, 0x00ae2350u));
    }
    cm_fn_009452ca(*(volatile u32 *)self);
}
