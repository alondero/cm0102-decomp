#include "cmre.h"

u32 CM_CDECL cm_fn_00939750(u32 self)
{
    u32 object = *(volatile u32 *)(self + 8);
    u32 result = *(volatile u32 *)(self + 0x30);
    u32 list = *(volatile u32 *)(self + 0xc);
    u32 first_table = *(volatile u32 *)object;
    u32 second_table = *(volatile u32 *)(object + 4);
    s16 count;
    u32 first_base, second_base;
    result *= 4;
    count = *(volatile s16 *)(self + 0x3e);
    first_base = *(volatile u32 *)(first_table + result);
    second_base = *(volatile u32 *)(second_table + result);
    if (count >= 0) {
        do {
            u32 cursor = *(volatile u32 *)(self + 0x20) - 4;
            u32 index, table, delta1, delta2, target;
            *(volatile u32 *)(self + 0x20) = cursor;
            index = *(volatile u32 *)cursor;
            table = *(volatile u32 *)(list + 4);
            delta2 = *(volatile u32 *)(table + index * 4) - second_base;
            table = *(volatile u32 *)list;
            delta1 = *(volatile u32 *)(table + index * 4) - first_base;
            target = *(volatile u32 *)(self + 0x46);
            result = ((u32 (CM_CDECL *)(u32, u32, u32))target)(self, delta1, delta2);
            target = *(volatile u32 *)(self + 0x42);
            result = ((u32 (CM_CDECL *)(u32, u32, u32, u32))target)(self, list, index, 0u - result);
            count = (s16)(*(volatile u16 *)(self + 0x3e) - 1u);
            *(volatile u16 *)(self + 0x3e) = (u16)count;
            count = *(volatile s16 *)(self + 0x3e);
        } while (count >= 0);
    }
    *(volatile u16 *)(self + 0x3e) = 0;
    return result;
}
