#include "cmre.h"

u32 CM_THISCALL cm_fn_00714d00(u32 self)
{
    if (*(volatile u8 *)(self + 0x27) == 1) {
        u32 object = *(volatile u32 *)(self + 0x19e);
        s8 value = *(volatile s8 *)(object + 0x8ea8);
        return (object & 0xffffff00u) | (value >= 6);
    } else {
        u32 object = *(volatile u32 *)(self + 0x19e);
        s8 value = *(volatile s8 *)(object + 0x8ea8);
        return value < 6;
    }
}
