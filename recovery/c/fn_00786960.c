#include "cmre.h"

/* Preferred-image VA 0x00786960, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_00785670(u32);
extern unsigned long long CM_THISCALL cm_fn_00786010(u32, u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static u32 news_error(u32 name, u32 ext, u32 line)
{
    u32 text;
    cm_fn_00944cff(0x00a693ec, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}

u32 CM_THISCALL cm_fn_00786960(u32 self, u32 person, u32 count_ptr, u32 filter)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch, id, total, slot, table, start, index;
    u32 offset, base, row, value, count, destination, result = 0;
    if (person == 0) return news_error(local + 0x108, local + 8, 0x21fa);
    id = D(person);
    total = D(0x00ae2364);
    id = id - total + 16u;
    if ((s32)id < 0) return news_error(local + 8, local + 0x108, 0x2205);
    table = D(self + 8);
    slot = table + id * 12u;
    start = D(slot);
    if (start == 0xffffffffu) return 0;
    index = D(slot + 4) - 1u;
    for (;;) {
        start = D(slot);
        if ((s32)index < (s32)start) index = start + 99u;
        offset = index * 223u;
        base = D(self);
        row = offset + base;
        value = D(row + 0xd6);
        if (value == 0) break;
        if ((u8)filter != 0) {
            value = (u32)cm_fn_00785670(row);
            if ((u8)value != (u8)filter) goto next;
        }
        if (result == 0) {
            result = cm_fn_00944e46(0x571c);
            if (result == 0) return news_error(local + 8, local + 0x108, 0x222a);
            D(count_ptr) = 0;
        }
        count = D(count_ptr);
        destination = result + count * 223u;
        D(count_ptr) = count + 1u;
        base = D(self);
        cm_fn_00786010(destination, offset + base);
next:
        --index;
        value = D(slot + 4) - 1u;
        if (index == value) break;
    }
    return result;
}
