#include "cmre.h"

u32 CM_CDECL cm_fn_0093ad80(u32 self)
{
    u32 cursor = *(volatile u32 *)(self + 0x20);
    u32 context = *(volatile u32 *)(self + 0x28);
    u32 table, index, entry, selector, base, adjustment, limit, address;
    u16 raw;
    u32 target;
    cursor -= 4;
    *(volatile u32 *)(self + 0x20) = cursor;
    table = *(volatile u32 *)(context + 0x90);
    index = *(volatile u32 *)cursor;
    entry = table + index * 8;
    selector = *(volatile u16 *)(entry + 6);
    base = *(volatile u32 *)entry;
    adjustment = *(volatile u32 *)(context + selector * 4 + 0x9c);
    limit = *(volatile u16 *)(entry + 4);
    address = cursor - 4;
    limit += base;
    *(volatile u32 *)(self + 0x20) = address;
    raw = *(volatile u16 *)address;
    limit += adjustment;
    raw = (u16)(raw - 1u);
    base += adjustment;
    target = *(volatile u32 *)(self + 0x4e);
    if ((s16)raw >= 0) {
        u32 count = (u32)(s32)(s16)raw + 1;
        do {
            ((u32 (CM_CDECL *)(u32, u32, u32))target)(self, base, limit);
            --count;
        } while (count != 0);
        return 0;
    }
    return (address & 0xffff0000u) | raw;
}
