#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00448490(volatile u8 *self, volatile u8 *source);
extern u32 CM_THISCALL cm_fn_00448110(u32 self, u32 source);

u32 CM_THISCALL cm_fn_00451570(volatile u8 *self, volatile u8 *source)
{
    self[0] = source[0];
    cm_fn_00448490(self + 1, source + 1);
    cm_fn_00448110((u32)self + 0x19, (u32)source + 0x19);
    return (u32)self;
}
