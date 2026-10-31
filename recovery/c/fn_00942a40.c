#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
/* EDX:EAX transports the original helper's incidental registers, so that
   MOV DX below does not invent the upper half of the pushed EDX. */
extern unsigned long long CM_CDECL cm_fn_00941630(u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00942c00(u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00941760(u32, u32);

static u32 cm_be16(u32 address)
{
    u32 high = B(address);
    u32 low = B(address + 1);
    return high * 256u + low;
}

u32 CM_CDECL cm_fn_00942a40(u32 self, u32 code)
{
    u32 cache, directory, count, table, cursor, entry, index, found;
    u32 first, second, third, fourth, allocation, ax, dx, kind, offset, base;
    u32 start, end, delta, range, glyph, result;
    unsigned long long registers;
    cache = D(self + 0x28);
    if (cache == 0) {
        directory = D(self);
        count = (u32)(s32)(s16)W(directory + 8);
        found = 0;
        index = 0;
        if ((s32)count > 0) {
            table = D(directory + 0x10);
            cursor = table;
            do {
                entry = D(cursor);
                if (D(entry + 4) == 0x636d6170u) {
                    found = D(table + index * 4u);
                    break;
                }
                ++index;
                cursor += 4;
            } while ((s32)index < (s32)count);
        }
        /* No missing-table guard exists in the original. */
        first = D(found + 0x10);
        second = D(found + 0xc);
        third = D(self + 0x54);
        fourth = D(self + 0x5c);
        registers = cm_fn_00941630(fourth, third, second, first, 0, 0);
        allocation = (u32)registers;
        dx = ((u32)(registers >> 32) & 0xffff0000u) | W(self + 0x32);
        fourth = D(self + 0x5c);
        ax = (allocation & 0xffff0000u) | W(self + 0x30);
        result = cm_fn_00942c00(fourth, ax, dx, allocation);
        D(self + 0x28) = result;
        cm_fn_00941760(allocation, 0);
    }
    cache = D(self + 0x28);
    kind = W(cache + 0x16);
    if (kind == 0) {
        if ((u16)code >= 0x100)
            return 0;
        index = (u32)(s32)(s16)W(cache + 0x14);
        table = D(cache + 8);
        entry = D(table + index * 4u);
        offset = D(cache + 0xc);
        base = D(entry + 4);
        result = base + offset;
        glyph = B(result + (u16)code + 6);
        return (result & 0xffff0000u) | glyph;
    }
    if (kind != 6 && kind != 4)
        return 0;
    index = (u32)(s32)(s16)W(cache + 0x14);
    table = D(cache + 8);
    offset = D(cache + 0xc);
    entry = D(table + index * 4u);
    base = D(entry + 4);
    cursor = base + offset + 6;
    if (kind == 6) {
        start = cm_be16(cursor);
        cursor += 2;
        index = code - start;
        count = cm_be16(cursor);
        if ((u16)index >= (u16)count)
            return 0;
        result = cursor + (u32)(u16)index * 2u + 2;
        glyph = cm_be16(result);
        return (result & 0xffff0000u) | glyph;
    }
    count = cm_be16(cursor);
    cursor += 8;
    do {
        end = cm_be16(cursor);
        cursor += 2;
    } while ((u16)code > (u16)end);
    cursor += (u16)count;
    start = cm_be16(cursor);
    if ((u16)code < (u16)start)
        return 0;
    index = code - start;
    cursor += (u16)count;
    delta = cm_be16(cursor);
    cursor += (u16)count;
    range = cm_be16(cursor);
    if (range == 0)
        return code + delta;
    cursor += (u16)range;
    glyph = cm_be16(cursor + (u32)(u16)index * 2u);
    return glyph + delta;
}
