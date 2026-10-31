#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *output, u16 *source);

void CM_THISCALL cm_fn_008aff00(u8 *self, u16 *first, u16 *second, u16 *third)
{
    u32 output[2];
    cm_fn_00417c10((u32 *)(self + 0x242), output, first);
    cm_fn_00417c10((u32 *)(self + 0x24a), output, second);
    cm_fn_00417c10((u32 *)(self + 0x1d), output, third);
}
