/* Preferred-image VA 00767fa0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00767fa0(u32 self, u32 person)
{
    u8 scratch[0x200];
    u32 count = D(self + 4);
    u32 table = D(self);
    u32 entry = table;
    if ((s32)count > 0) {
        u32 id = D(person);
        u32 scanned = 0;
        do {
            if (D(entry) == id) goto update;
            ++scanned;
            entry += 12;
        } while ((s32)scanned < (s32)count);
    }
    u32 capacity = D(self + 8);
    if (count == capacity) {
        capacity += 300;
        D(self + 8) = capacity;
        u32 allocation;
        if (!table) allocation = (u32)cm_fn_00944e46(capacity * 12u);
        else allocation = (u32)cm_fn_00945667(table, capacity * 12u);
        table = allocation;
        D(self) = allocation;
        if (!table) {
            cm_fn_00944cff(0x00a5b234, 0, 0, scratch + 0x100, scratch);
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                           scratch + 0x100, scratch, 0x115);
            u32 result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            return result;
        }
    }
    u32 id = D(person);
    u32 people = D(0x00ae23bc);
    u32 club = D(people + id * 110u + 0x39);
    u32 competition = 0xffffffffu;
    if (club) {
        u32 pointer = D(club + 0x57);
        if (pointer) competition = D(pointer);
    }
    count = D(self + 4);
    D(table + count * 12u) = id;
    count = D(self + 4);
    table = D(self);
    D(table + count * 12u + 4) = competition;
    count = D(self + 4);
    table = D(self);
    W(table + count * 12u + 8) = 0;
    id = D(person);
    people = D(0x00ae23bc);
    count = D(self + 4);
    u8 flag = B(people + id * 110u + 0x18);
    table = D(self);
    B(table + count * 12u + 11) = flag;
    count = D(self + 4);
    table = D(self);
    B(table + count * 12u + 10) = 0;
    count = D(self + 4);
    table = D(self);
    ++count;
    D(self + 4) = count;
    entry = table + count * 12u - 12;
update:
    u16 addition = B(person + 0x35);
    W(entry + 8) += addition;
    u8 appearances = B(entry + 10);
    B(entry + 10) = (u8)(appearances + 1);
    return entry;
}
