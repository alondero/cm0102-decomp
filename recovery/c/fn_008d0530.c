#include "cmre.h"
typedef u32 (CM_THISCALL *slot_fn)(u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_008d0530(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth)
{
    u32 p = *(volatile u32 *)(second + 0x39);
    u32 value, i;
    if (!p)
        return 1;
    value = *(volatile u32 *)(p + 0x53);
    if (*(volatile u32 *)(first + 0x53) != value)
        return 1;
    for (i = 0; i < 28; ++i) {
        u32 slot = self + 0x8ac + i * 4;
        u32 item = *(volatile u32 *)slot;
        if (*(volatile u32 *)(item + 0xa) == value) {
            u32 table, target;
            item = *(volatile u32 *)slot;
            table = *(volatile u32 *)item;
            target = *(volatile u32 *)(table + 0x24);
            return ((slot_fn)target)(item, first, second, third, fourth, fifth);
        }
    }
    return 1;
}
