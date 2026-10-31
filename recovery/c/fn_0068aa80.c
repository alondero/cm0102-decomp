#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006a3770(u32 self, u32 entry, u32 option);

u32 CM_THISCALL cm_fn_0068aa80(volatile u8 *self)
{
    u32 result = CM_GLOBAL(volatile u32, 0x00b67a44);
    u32 index = 0;
    if (result && *(volatile s16 *)(self + 0x3e) > 0) {
        do {
            u32 base = *(volatile u32 *)(self + 0xb1);
            u32 entry = *(volatile u32 *)(base + index * 59u);
            u32 receiver = CM_GLOBAL(volatile u32, 0x00b67a44);
            result = cm_fn_006a3770(receiver, entry, 1);
            ++index;
        } while ((s16)index < *(volatile s16 *)(self + 0x3e));
    }
    return result;
}
