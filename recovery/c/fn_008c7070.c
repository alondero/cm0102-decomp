#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008c7070(u32 self, volatile u32 first,
    volatile u32 second, volatile u16 type,
    u32 unused __attribute__((unused)), volatile u32 sort)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 capacity, count, table, value, text, index, flags, first_pointer, second_pointer;
    u16 type_value;
    capacity = D(self + 0x942);
    count = D(self + 0x93e);
    if (count == capacity) {
        table = D(self + 0x93a);
        capacity += 0x1e;
        D(self + 0x942) = capacity;
        if (table != 0)
            value = (u32)cm_fn_00945667(table, capacity * 14);
        else
            value = (u32)cm_fn_00944e46(capacity * 14);
        D(self + 0x93a) = value;
        if (value == 0) {
            cm_fn_00944cff(0x00a8d514, 0, 0, local + 0x100, local);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009a8120, text,
                          local + 0x100, local, 0x1b1b, 0x00a8eb04);
            cm_fn_0066f4e0(0x00ae28bc, 0x009a8108);
            cm_fn_0066f4e0(0x00ae26c4, 0x009a7ff8, 0x00ae24d0);
            return cm_fn_005e8290(0x00ae28bc, 0x00ae26c4, 0);
        }
    }
    count = D(self + 0x93e);
    first_pointer = first;
    value = D(first_pointer);
    table = D(self + 0x93a);
    D(table + count * 14) = value;
    count = D(self + 0x93e);
    second_pointer = second;
    value = D(second_pointer);
    table = D(self + 0x93a);
    D(table + count * 14 + 0xa) = value;
    count = D(self + 0x93e);
    type_value = type;
    table = D(self + 0x93a);
    W(table + count * 14 + 8) = type_value;
    count = D(self + 0x93e);
    table = D(self + 0x93a);
    W(table + count * 14 + 6) = 0;
    count = D(self + 0x93e);
    table = D(self + 0x93a);
    W(table + count * 14 + 4) = 0;
    count = D(self + 0x93e);
    D(self + 0x93e) = count + 1;
    index = D(first_pointer);
    table = D(self + 0x86e);
    flags = B(table + index * 8);
    B(table + index * 8) = (u8)(flags | 0x20);
    value = sort;
    if (value != 0) {
        count = D(self + 0x93e);
        table = D(self + 0x93a);
        value = cm_fn_00945333(table, count, 14, 0x008e55c0);
        /* Original qsort00945342/345 returns without touching EAX for count<2;
           the parent's incoming EAX is the count load008c7234. */
        if (count < 2)
            value = count;
    }
    return value;
}
