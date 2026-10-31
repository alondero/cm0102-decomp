#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007f6f20(u32 self, u32 value, u32 record);
extern u32 CM_THISCALL cm_fn_007f6cd0(u32 self, s8 kind, volatile u32 record);
extern void CM_THISCALL cm_fn_007b7690(u32 self, u32 source);

u32 CM_THISCALL cm_fn_00804450(volatile u8 *input)
{
    u32 receiver = CM_GLOBAL(volatile u32, 0x00b74448);
    u8 value = *input;
    u32 source = (u32)input + 1;
    u32 i;
    cm_fn_007f6f20(receiver, value, 0);
    for (i = 0; i < 10; ++i) {
        u32 target = cm_fn_007f6cd0(CM_GLOBAL(volatile u32, 0x00b74448), (s8)i, 0);
        cm_fn_007b7690(target, source);
        source += 0x2b0;
    }
    return 1;
}
