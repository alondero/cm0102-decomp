#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);

u32 CM_THISCALL cm_fn_008c3e50(u32 self, u32 record, u32 kind)
{
    u32 eax, edx, base, value;
    if ((u8)kind == 4) {
        base = *(volatile u32 *)(self + 0x86e);
        edx = *(volatile u32 *)record;
        eax = (record & 0xffff0000u) | *(volatile u8 *)(record + 0x22);
        edx = (edx & 0xffff0000u) | *(volatile u8 *)(base + edx * 8 + 7);
        return eax - edx;
    }
    value = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), record);
    if (!value)
        return 0;
    value = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), record);
    return (value & 0xffff0000u) | *(volatile u16 *)(value + 0x3b);
}
