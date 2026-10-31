#include "cmre.h"

/* Preferred-image VA 0x00787140, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_00782b70(u32, u32, u32, u32, u32);
extern u16 CM_THISCALL cm_fn_007fffa0(u32, u32);
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

u32 CM_THISCALL cm_fn_00787140(u32 self, u32 person, u32 item, u32 propagate)
{
    u8 scratch[0x204];
    u32 local = (u32)scratch, id, total, table, slot, value, index = 0;
    u32 row, base, reply, person_row, group_arg;
    u16 group, other_group;
    u8 flag;
    if (person == 0) return news_error(local + 0x104, local + 4, 0x2358);
    if (item == 0) return news_error(local + 4, local + 0x104, 0x2360);
    id = D(person);
    total = D(0x00ae2364);
    id = id - total + 16u;
    if ((s32)id < 0) return news_error(local + 4, local + 0x104, 0x236b);
    table = D(self + 8);
    slot = table + id * 12u;
    value = D(slot);
    if (value == 0xffffffffu) return 0;
    do {
        value = D(slot);
        base = D(self);
        row = base + (value + index) * 223u;
        value = D(row + 0xd6);
        if (value == item) {
            value = D(row + 0xda);
            if (value == 0) {
                reply = (u32)cm_fn_00782b70(row, person, 0x00de1f64, 2000, local);
                if ((u8)reply != 2) {
                    D(row + 0xda) = 1;
                    value = D(slot + 8);
                    D(slot + 8) = value - 1u;
                    break;
                }
            }
        }
        ++index;
    } while ((s32)index < 100);
    if (row == 0) return 0;
    if (propagate != 0) {
        value = (u32)(s32)(s16)W(0x00b77c7a);
        group_arg = D(0x00b74c26 + value * 768u);
        group = cm_fn_007fffa0(0x00b74450, group_arg);
        index = 0;
        do {
            total = D(0x00ae2364);
            base = D(0x00ae23bc);
            value = (total - index - 1u) * 110u;
            flag = B(base + value + 0x5f);
            person_row = base + value;
            if (flag == 1 && person_row != person) {
                other_group = cm_fn_007fffa0(0x00b74450, person_row);
                if (other_group == group) cm_fn_00787140(self, person_row, item, 0);
            }
            ++index;
        } while ((s32)index < 16);
    }
    return 1;
}
