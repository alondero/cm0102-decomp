/* WIP: original instruction coverage 0.4783; successful callee return path not exercised. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_005af6e0(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_005afd70(u32 self, u32 input);

u32 CM_THISCALL cm_fn_005af630(u32 self, volatile u32 input)
{
    u32 address = self;
    u32 count = 16;
    u32 index;
    do {
        *(volatile u32 *)address = 0;
        address += 4;
        --count;
    } while (count != 0);
    if (CM_GLOBAL(volatile u8, 0x009cc270) == 0)
        return self;
    if (input == 0)
        return self;
    if (cm_fn_005af6e0(self, input) != 0)
        return self;
    for (index = 0; index < 16; ++index) {
        u32 row = (CM_GLOBAL(volatile u32, 0x00ae2364) - index - 1u) * 110u;
        u32 base = CM_GLOBAL(volatile u32, 0x00ae23bc);
        if (*(volatile u8 *)(base + row + 0x5f) == 1)
            cm_fn_005afd70(self, base + row);
    }
    return self;
}
