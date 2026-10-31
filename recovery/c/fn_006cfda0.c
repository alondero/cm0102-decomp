#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00717410(u32 self);
extern u8 CM_THISCALL cm_fn_00714d30(volatile u8 *self);

u8 CM_THISCALL cm_fn_006cfda0(volatile u8 *self, volatile u8 *record)
{
    if ((u16)cm_fn_00717410((u32)record) == 1) {
        if (cm_fn_00714d30(record) == 1)
            return self[0x8ead];
        if (cm_fn_00714d30(record) == 0)
            return self[0x8eac];
    }
    if (cm_fn_00714d30(record) == 1)
        return (u32)record == *(volatile u32 *)(self + 0xf58a);
    return (u32)record == *(volatile u32 *)(self + 0xf586);
}
