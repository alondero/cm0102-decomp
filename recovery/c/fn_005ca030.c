#include "cmre.h"

u32 CM_THISCALL cm_fn_005ca030(u32 self)
{
    u32 base = CM_GLOBAL(volatile u32, 0x00adadfc);
    u32 index = CM_GLOBAL(volatile u32, 0x009cf668);
    u32 value = CM_GLOBAL(volatile u32, 0x00ae2c74);
    u32 p = self + 0x116, i;
    *(volatile u32 *)(base + index * 4) = value;
    for (i = 0; i < 0x7f; ++i) {
        value = *(volatile u32 *)(p - 0x65);
        if (value) {
            value = *(volatile u32 *)p;
            if (value) {
                index = (u32)(s32)*(volatile s16 *)(self + 0xaf);
                base = CM_GLOBAL(volatile u32, 0x00adadfc);
                *(volatile u32 *)(base + (index + i) * 4) = value;
            }
        }
        p += 0x69;
    }
    return p;
}
