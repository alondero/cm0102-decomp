#include "cmre.h"

u32 CM_CDECL cm_fn_00938cc0(u32 self)
{
    u32 cursor = *(volatile u32 *)(self + 0x20);
    u32 callbacks = *(volatile u32 *)(self + 0x28);
    u32 points = *(volatile u32 *)(self + 8);
    u32 index, flag, amount = 0, target;
    cursor -= 4;
    *(volatile u32 *)(self + 0x20) = cursor;
    callbacks += 0x58;
    index = *(volatile u32 *)cursor;
    flag = *(volatile u8 *)(self + 4);
    *(volatile u32 *)(self + 0x34) = index;
    *(volatile u32 *)(self + 0x30) = index;
    if (flag & 1u) {
        u32 second_table = *(volatile u32 *)(points + 4);
        u32 first_table = *(volatile u32 *)points;
        u32 second = *(volatile u32 *)(second_table + index * 4);
        u32 first = *(volatile u32 *)(first_table + index * 4);
        u32 computed, mapped;
        target = *(volatile u32 *)(self + 0x46);
        computed = ((u32 (CM_CDECL *)(u32, u32, u32))target)(self, first, second);
        target = *(volatile u32 *)(callbacks + 0x18);
        mapped = ((u32 (CM_CDECL *)(u32, u32, u32))target)(self, computed, 0);
        amount = mapped - computed;
    }
    target = *(volatile u32 *)(self + 0x42);
    return ((u32 (CM_CDECL *)(u32, u32, u32, u32))target)(self, points, index, amount);
}
