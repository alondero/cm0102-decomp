#include "cmre.h"

u32 CM_THISCALL cm_fn_008b0dc0(u32 self, u32 key, volatile u32 value)
{
    u32 index;
    for (index = 0; index < 50u; ++index) {
        if (*(volatile u32 *)(self + index * 5u) == key) {
            u8 byte = (u8)value;
            *(volatile u8 *)(self + index * 5u + 4u) = byte;
            *(volatile u32 *)(self + 0x802u) = 1;
            return index;
        }
    }
    return 49u;
}
