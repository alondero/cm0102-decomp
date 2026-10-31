#include "cmre.h"

typedef u32 (CM_CDECL *handler)(u32, u32, u32);

u32 CM_CDECL cm_fn_00939f50(u32 self)
{
    u32 sp = *(volatile u32 *)(self + 0x20);
    u8 flags = *(volatile u8 *)(self + 4);
    u32 tables = *(volatile u32 *)(self + 0x10);
    u32 index, high_table, low_table, high, low, target, result;
    sp -= 4;
    *(volatile u32 *)(self + 0x20) = sp;
    index = *(volatile u32 *)sp;
    if (flags & 1) {
        high_table = *(volatile u32 *)(tables + 12);
        low_table = *(volatile u32 *)(tables + 8);
        high = *(volatile u32 *)(high_table + index * 4);
        low = *(volatile u32 *)(low_table + index * 4);
        target = *(volatile u32 *)(self + 0x4a);
    } else {
        high_table = *(volatile u32 *)(tables + 4);
        low_table = *(volatile u32 *)tables;
        high = *(volatile u32 *)(high_table + index * 4);
        low = *(volatile u32 *)(low_table + index * 4);
        target = *(volatile u32 *)(self + 0x46);
    }
    result = ((handler)target)(self, low, high);
    sp = *(volatile u32 *)(self + 0x20);
    *(volatile u32 *)sp = result;
    sp = *(volatile u32 *)(self + 0x20) + 4;
    *(volatile u32 *)(self + 0x20) = sp;
    return sp;
}
