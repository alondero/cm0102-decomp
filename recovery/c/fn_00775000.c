#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_THISCALL cm_fn_006a3770(u32 self, u32 entry, u32 option);

u32 CM_THISCALL cm_fn_00775000(volatile u8 *self, volatile u32 *record)
{
    u32 index = *record;
    u32 base = *(volatile u32 *)(self + 4);
    u32 value;
    *(volatile u32 *)(base + index * 110u + 4) = 0xffffffffu;
    index = *record;
    base = *(volatile u32 *)(self + 8);
    value = *(volatile u32 *)(base + index * 4u);
    if (value != 0xffffffffu) {
        base = *(volatile u32 *)(self + 4);
        *(volatile u32 *)(base + index * 110u + 4) = value;
        index = *record;
        base = *(volatile u32 *)(self + 8);
        *(volatile u32 *)(base + index * 4u) = 0xffffffffu;
        index = CM_GLOBAL(volatile u32, 0x00b67a44);
        if (index) {
            u32 entry = cm_fn_0053b3d0((u32)record);
            u32 receiver = CM_GLOBAL(volatile u32, 0x00b67a44);
            return cm_fn_006a3770(receiver, entry, 1);
        }
    }
    return index;
}
