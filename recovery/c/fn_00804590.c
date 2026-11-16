#include "cmre.h"

extern void CM_THISCALL cm_fn_007b7690(u32 *self, u32 *source);
extern u32 CM_THISCALL cm_fn_007af720(u32 self, u32 record);

u32 CM_THISCALL cm_fn_00804590(u8 *self, u32 argument)
{
    u32 i;
    u32 record;
    u32 value;

    for (i = 0; i < 10; ++i) {
        record = (u32)self + 1 + i * 0x2b0;
        if (*(volatile u8 *)(record + 0xa) == 1)
            break;
    }
    if (i == 10)
        return 0;

    *(volatile u8 *)(self + 0x1b0d) = *(volatile u8 *)self;
    for (u32 j = 0; j < 10; ++j) {
        cm_fn_007b7690((u32 *)(self + 0x1b0e + j * 0x2b0),
                       (u32 *)(self + 1 + j * 0x2b0));
        value = *(volatile u32 *)(self + 0x1ae1 + j * 4);
        *(volatile u32 *)(self + 0x35ee + j * 4) = value;
    }
    value = *(volatile u32 *)(self + 0x1b09);
    *(volatile u32 *)(self + 0x3616) = value;
    *(volatile u32 *)(self + 0x361a) = 1;
    cm_fn_007af720(record, argument);
    *(volatile u32 *)(self + 0x1ae1 + i * 4) = 1;
    *(volatile u32 *)(self + 0x1b09) = 1;
    *(volatile u32 *)(self + 0x3634) = 0;
    return 1;
}
